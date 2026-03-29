#include "coroutine/cotask.h"
#include "coroutine/cosched.h"
#include "coevent.h"
#include "common.h"
#include "coroutine/cotask_data.h"
#include "ecs.h"
#include "list.h"
#include "pool.h"

#include <koishi.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

static CoTaskList task_pool;
static koishi_coroutine_t *co_main;

void cotask_global_init() {
    co_main = koishi_active();
}

void cotask_global_shutdown() {
    for (CoTask *task; (task = alist_pop(&task_pool)); ) {
        koishi_deinit(&task->ko);
        free(task);
    }
}

// Oui ce truc existe, c'est completement dingue....
// Je pense que ca peut rentrer dans les techniques illégales
static inline
CoTask *cotask_from_koishi_coroutine(koishi_coroutine_t *co) {
    return (CoTask *)((char *)co - offsetof(CoTask, ko));
}

BoxedTask cotask_box(CoTask *task) {
    return (BoxedTask){
        .ptr = (uintptr_t)task,
        .unique_id = task->unique_id,
    };
}

CoTask *cotask_unbox(BoxedTask box) {
    CoTask *task = (void*)box.ptr;

    if (task && task->unique_id == box.unique_id) {
        return task;
    }

    return NULL;
}

CoTask *cotask_new_internal(koishi_entrypoint_t entry_point) {
    CoTask *task;

    if ( (task = alist_pop(&task_pool))) {
        // Y a de la place dans la task_pool, on recycle
        koishi_recycle(&task->ko, entry_point);
    } else {
        // Si y a plus de place on fait un malloc
        // Peut etre causera des erreurs hehe
        task = malloc(sizeof(CoTask));
        koishi_init(&task->ko, CO_STACK_SIZE, entry_point);
    }

    static uint32_t unique_counter = 0;
    unique_counter++;
    task->unique_id = unique_counter;
    task->data = NULL;

    return task;
}

void *cotask_resume_internal(CoTask *task, void *arg) {
    arg = koishi_resume(&task->ko, arg);
    return arg;
}

static
void cancel_task_events(CoTaskData *task_data) {
    task_data->bound_ent.id = NULL_INDEX;
    task_data->bound_ent.unique_id = 0;
    COEVENT_CANCEL_ARRAY(task_data->events);
}

static
bool cotask_finalize(CoTask *task) {
    CoTaskData *task_data = cotask_get_data(task);

    // Deja en train de finaliser
    if(task_data->finalizing) return false;

    task_data->finalizing = true;
    cancel_task_events(task_data);

    if (task_data->hosted.events) {
        _coevent_array_action(task_data->hosted.num_events, task_data->hosted.events, coevent_cancel);
        task_data->hosted.events = NULL;
        task_data->hosted.num_events = 0;
    }
    if (task_data->hosted.ent != (Entity)ID_INVALID) {
        pool_kill_entity(task_data->sched->pool, task_data->hosted.ent);
        task_data->hosted.ent = ID_INVALID;
    }
    if (task_data->master) {
        alist_unlink(&task_data->master->slaves, task_data);
        task_data->master = NULL;
    }

    if (task_data->wait.wait_type == COTASK_WAIT_EVENT) {
        CoEvent *evt = task_data->wait.event.p_event;
        if (evt->unique_id ==  task_data->wait.event.snapshot.unique_id) {
            coevent_cleanup_subscribers(task_data->wait.event.p_event);
        }
    }
    task_data->wait.wait_type = COTASK_WAIT_NONE;

    for (CoTaskData *slave_data; (slave_data = alist_pop(&task_data->slaves));) {
        slave_data->master = NULL;
        cotask_cancel(slave_data->task);
    }

    task->data = NULL;
    return true;
}

static
void cotask_enslave(CoTaskData *master_data, CoTaskData *slave_data) {
    slave_data->master = master_data;
    alist_append(&master_data->slaves, slave_data);
}

static
void cotask_entry_setup(CoTask *task, CoTaskData *data, CoTaskInitData *init_data) {
    task->data = data;
    data->task = task;
    data->sched = init_data->sched;

    data->hosted.ent = NULL_INDEX;
    data->bound_ent.id = NULL_INDEX;
    data->bound_ent.unique_id = 0;

    CoTaskData *master_data = init_data->master_task_data;
    if (master_data) {
        cotask_enslave(master_data, data);
    }

    COEVENT_INIT_ARRAY(data->events);
}

void *cotask_entry(void *varg) {
    CoTaskData data = {};
    CoTaskInitData *init_data = varg;
    CoTask *task = init_data->task;
    cotask_entry_setup(task, &data, init_data);

    varg = init_data->func(init_data->func_arg, init_data->func_arg_size);

    coevent_signal(&data.events.finished);
    cotask_finalize(task);

    return varg;
}

void cotask_free(CoTask *task) {
    task->unique_id = 0;
    alist_push(&task_pool, task);
}

CoTask *cotask_active(void) {
    if (koishi_active() == co_main) {
        return NULL;
    }

    koishi_coroutine_t *co = koishi_active();
    return cotask_from_koishi_coroutine(co);
}

static
void *cotask_force_resume(CoTask *task, void *arg) {
    CoTaskData *task_data = cotask_get_data(task);
    if (task_data->wait.wait_type != COTASK_WAIT_NONE) return NULL;
    return cotask_resume_internal(task, arg);
}

static
void *cotask_wake_and_resume(CoTask *task, void *arg) {
    CoTaskData *task_data = cotask_get_data(task);
    task_data->wait.wait_type = COTASK_WAIT_NONE;
    return cotask_force_resume(task, arg);
}

static
bool cotask_do_wait(CoTaskData *task_data) {
    switch (task_data->wait.wait_type) {
        case COTASK_WAIT_NONE: return false;

        case COTASK_WAIT_DELAY: {
            if (--task_data->wait.delay.remaining < 0) {
                return false;
            }

            break;
        }

        case COTASK_WAIT_EVENT: {

            CoEventStatus stat = coevent_poll(task_data->wait.event.p_event, &task_data->wait.event.snapshot);
            if (stat != CO_EVENT_PENDING) {
                task_data->wait.result.event_status = stat;
                return false;
            }

            break;
        }

        case COTASK_WAIT_SUBTASKS: {
            if (task_data->slaves.first == NULL) {
                return false;
            }

            break;
        }
    }

    task_data->wait.result.frames++;
    return true;
}

void *cotask_resume(CoTask *task, void *arg) {
    CoTaskData *task_data = cotask_get_data(task);

    if (task_data->bound_ent.id != NULL_INDEX ) {
        Entity ent = entity_unbox(task_data->sched->pool, task_data->bound_ent);
        if (ent == NULL_INDEX) {
            cotask_cancel(task);
            return NULL;
        }
    }

    if (!cotask_do_wait(task_data)) {
        return cotask_wake_and_resume(task, arg);
    }

    return NULL;
}

void *cotask_yield(void *arg) {
    arg = koishi_yield(arg);
    return arg;
}

static inline
CoWaitResult cotask_wait_init(CoTaskData *task_data, char wait_type) {
    CoWaitResult wr = task_data->wait.result;
    task_data->wait = (typeof(task_data->wait)) {
        .wait_type = wait_type,
    };
    return wr;
}

int cotask_wait(int delay) {
    CoTask *task = cotask_active();
    CoTaskData *task_data = cotask_get_data(task);
    if (task_data->wait.wait_type != COTASK_WAIT_NONE) return -1;

    if (delay == 1) {
        cotask_yield(NULL);
        return 1;
    }

    cotask_wait_init(task_data, COTASK_WAIT_DELAY);
    task_data->wait.delay.remaining = delay;

    if (cotask_do_wait(task_data)) {
        cotask_yield(NULL);
    }

    return cotask_wait_init(task_data, COTASK_WAIT_NONE).frames;
}

int cotask_wait_subtasks(void) {
    CoTask *task = cotask_active();
    CoTaskData *task_data = cotask_get_data(task);
    if (task_data->wait.wait_type != COTASK_WAIT_NONE) return -1;

    cotask_wait_init(task_data, COTASK_WAIT_SUBTASKS);

    if (cotask_do_wait(task_data)) {
        cotask_yield(NULL);
    }

    return cotask_wait_init(task_data, COTASK_WAIT_NONE).frames;
}

Entity cotask_host_entity(CoTask *task, Tag ent_type) {
    CoTaskData *task_data = cotask_get_data(task);
    Entity ent = pool_create_entity(task_data->sched->pool);
    Tag_add(&task_data->sched->pool->tag, ent, ent_type);
    task_data->hosted.ent = ent;
    return ent;
}

void cotask_host_events(CoTask *task, unsigned int num_events, CoEvent events[num_events]) {
    CoTaskData *task_data = cotask_get_data(task);
    if (task_data->hosted.num_events != 0) return;
    if (task_data->hosted.events != NULL) return;
    task_data->hosted.events = events;
    task_data->hosted.num_events = num_events;
    _coevent_array_action(num_events, events, coevent_init);
}

static
CoWaitResult cotask_wait_event_internal(CoEvent *evt, bool once) {
    CoTask *task = cotask_active();
    CoTaskData *task_data = cotask_get_data(task);

    if (task_data->finalizing) {
        cotask_yield(NULL);
    }

    if (evt->unique_id == 0) {
        return (CoWaitResult) {.event_status = CO_EVENT_CANCELED };
    }

    if (once && evt->num_signaled > 0) {
        return (CoWaitResult) { .event_status = CO_EVENT_SIGNALED };
    }

    coevent_add_subscriber(evt, task);

    cotask_wait_init(task_data, COTASK_WAIT_EVENT);
    task_data->wait.event.p_event = evt;
    task_data->wait.event.snapshot = coevent_snapshot(evt);

    if (cotask_do_wait(task_data)) {
        cotask_yield(NULL);
    }

    return cotask_wait_init(task_data, COTASK_WAIT_NONE);
}

CoWaitResult cotask_wait_event(CoEvent *evt) {
    return cotask_wait_event_internal(evt, false);
}

CoWaitResult cotask_wait_event_once(CoEvent *evt) {
    return cotask_wait_event_internal(evt, true);
}

CoWaitResult cotask_wait_event_or_die(CoEvent *evt) {
    CoWaitResult wr = cotask_wait_event(evt);

    if (wr.event_status == CO_EVENT_CANCELED) {
        cotask_cancel(cotask_active());
        cotask_yield(NULL);
    }

    return wr;
}

// NOTE: INDEFINI si une tache essaie de tuer une autre tache pendant son exec qui se tue elle meme
void cotask_cancel(CoTask *task) {
    if (!task || cotask_status(task) == CO_STATUS_DEAD) return;

    if (!cotask_finalize(task)) return;

    if (cotask_status(task) != CO_STATUS_DEAD) {
        koishi_kill(&task->ko, NULL);
    }
}

CoStatus cotask_status(CoTask *task) {
    return koishi_state(&task->ko);
}

CoSched *cotask_get_sched(CoTask *task) {
    return cotask_get_data(task)->sched;
}

Entity cotask_bind_to_entity(CoTask *task, Entity ent) {
    CoTaskData *task_data = cotask_get_data(task);

    if (ent == NULL_INDEX) {
        cotask_cancel(task);
        return NULL_INDEX;
    }

    task_data->bound_ent = entity_box(task_data->sched->pool, ent);
    return ent;
}

CoTaskEvents *cotask_get_events(CoTask *task) {
    CoTaskData *task_data = cotask_get_data(task);
    return &task_data->events;
}

void cotask_force_finish(CoTask *task) {
    if(task->data) {
        cotask_finalize(task);
    }

    cotask_free(task);
}