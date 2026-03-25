#include "cosched.h"

#include "coevent.h"
#include "coroutine/cotask.h"
#include "coroutine/cotask_data.h"
#include "dynarray.h"
#include "list.h"
#include <stdint.h>

void cosched_init(CoSched *sched, Pool *pool) {
    *sched = (CoSched){};
    sched->pool = pool;
}

CoTask *_cosched_new_task(CoSched *sched, CoTaskFunc func, void *arg, size_t arg_size, bool is_subtask) {
    if (sched == NULL) return NULL;

    CoTask *task = cotask_new_internal(cotask_entry);

    CoTaskInitData init_data = {};
    init_data.task = task;
    init_data.func = func;
    init_data.sched = sched;
    init_data.func_arg = arg;
    init_data.func_arg_size = arg_size;

    if (is_subtask) {
        init_data.master_task_data = cotask_get_data(cotask_active());
    }

    alist_append(&sched->pending_tasks, task);
    cotask_resume_internal(task, &init_data);

    return task;
}

unsigned int cosched_run_tasks(CoSched *sched) {
    alist_merge_tail(&sched->tasks, &sched->pending_tasks);

    unsigned int ran = 0;

    for (CoTask *t = sched->tasks.first, *next; t; t = next) {
        next = t->next;

        if (cotask_status(t) == CO_STATUS_DEAD) {
            alist_unlink(&sched->tasks, t);
            cotask_free(t);
        } else {
            cotask_resume(t, NULL);
            ++ran;
        }
    }

    return ran;
}

typedef struct {
    CoEvent *p_event;
    uint32_t unique_id;
} CoEventSnapshotRef;

typedef DYNAMIC_ARRAY(CoEventSnapshotRef) events_array;

static
void add_unique_event(events_array *arr, CoEvent *e, uint32_t uid) {
    dynarray_foreach_elem(arr, CoEventSnapshotRef *elem, {
        if (elem->p_event == e) {
            return;
        }
    });

    CoEventSnapshotRef new_ref = { .p_event = e, .unique_id = uid, };
    dynarray_append(arr, new_ref);
}

static
unsigned int gather_blocking_events(CoTaskList *tasks, events_array *events_array) {
    unsigned int n = 0;

    for (CoTask *t = tasks->first; t; t = t->next) {
        if (!t->data) continue;

        CoTaskData *tdata = t->data;
        if(tdata->wait.wait_type != COTASK_WAIT_EVENT) continue;

        CoEvent *e = tdata->wait.event.p_event;

        if (e->unique_id != tdata->wait.event.snapshot.unique_id) continue;

        add_unique_event(events_array, e, e->unique_id);
        ++n;
    }

    return n;
}

static
void cancel_blocking_events(CoSched *sched) {
    events_array events_array = {};

    gather_blocking_events(&sched->tasks, &events_array);
    gather_blocking_events(&sched->pending_tasks, &events_array);

    dynarray_foreach_elem(&events_array, CoEventSnapshotRef *snap, {
        CoEvent *e = snap->p_event;

        if (e->unique_id == snap->unique_id) coevent_cancel(e);
    });

    dynarray_free_data(&events_array);
}

static
void finish_task_list(CoTaskList *tasks) {
    for(CoTask *t; (t = alist_pop(tasks));) {
        cotask_force_finish(t);
    }
}

void cosched_finish(CoSched *sched) {
    cancel_blocking_events(sched);
    finish_task_list(&sched->tasks);
    finish_task_list(&sched->pending_tasks);
    *sched = (CoSched){};
}