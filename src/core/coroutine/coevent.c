#include "coroutine/coevent.h"
#include "cotask.h"
#include "dynarray.h"
#include "macro.h"

#include <stdint.h>
#include <string.h>

void coevent_init(CoEvent *evt) {
    static uint32_t g_uid;
    g_uid++;
    uint32_t uid = g_uid; // Chaque evenement aura un id unique a son init
    *evt = (CoEvent) { .unique_id = uid };
}

CoEventSnapshot coevent_snapshot(const CoEvent *evt) {
    return (CoEventSnapshot) {
        .unique_id = evt->unique_id,
        .num_signaled = evt->num_signaled,
    };
}

CoEventStatus coevent_poll(const CoEvent *evt, const CoEventSnapshot *snap) {
    if (evt->unique_id != snap->unique_id ||
        evt->num_signaled < snap->num_signaled || evt->unique_id == 0) {
            return CO_EVENT_CANCELED;
        }

    if (evt->num_signaled > snap->num_signaled) {
        return CO_EVENT_SIGNALED;
    }

    return CO_EVENT_PENDING;
}

// Renvoie vrai si la boxedtask référence la bonne tache
static bool subscribers_array_predicate(const void * p_elem, void * userdata) {
    return cotask_unbox(*(const BoxedTask *)p_elem);
}

void coevent_cleanup_subscribers(CoEvent *evt) {
    if (evt->subscribers.num_elements == 0) {
        return;
    }

    // on garde alors que les boxedtask encore correctes
    dynarray_filter(&evt->subscribers, subscribers_array_predicate, NULL);
}

void coevent_add_subscriber(CoEvent *evt, CoTask *task) {
    dynarray_append(&evt->subscribers, cotask_box(task));
}

static
void coevent_wake_subscribers(CoEvent *evt, uint num_subs, BoxedTask subs[num_subs]) {
    for(uint i=0; i < num_subs; i++) {
        CoTask *task = cotask_unbox(subs[i]);

        // TODO: A finir quand cotask_resume sera fait
    }
}

void coevent_signal(CoEvent *evt) {
    if (evt->unique_id == 0) return;

    evt->num_signaled++;
    // On reveille les bebes
    if(evt->subscribers.num_elements) {
        BoxedTask subs_snapshot[evt->subscribers.num_elements];
        memcpy(subs_snapshot, evt->subscribers.data, sizeof(subs_snapshot));
        evt->subscribers.num_elements = 0;
        coevent_wake_subscribers(evt, MACRO_SIZEOF_ARRAY(subs_snapshot), subs_snapshot);
    }
}

void coevent_signal_once(CoEvent *evt) {
    if (evt->num_signaled == 0) {
        coevent_signal(evt);
    }
}

void coevent_cancel(CoEvent *evt) {
    // Deja mort
    if (evt->unique_id == 0) return;

    evt->unique_id = 0;
    if (evt->subscribers.num_elements) {
        BoxedTask subs_snapshot[evt->subscribers.num_elements];
        memcpy(subs_snapshot, evt->subscribers.data, sizeof(subs_snapshot));
        dynarray_free_data(&evt->subscribers);
        coevent_wake_subscribers(evt, MACRO_SIZEOF_ARRAY(subs_snapshot), subs_snapshot);
    } else {
        dynarray_free_data(&evt->subscribers);
    }
}

void _coevent_array_action(uint num, CoEvent *events, void (*func)(CoEvent *)) {
    for (uint i = 0; i < num; i++) {
        func(events + i);
    }
}

