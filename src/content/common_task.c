#include "common_task.h"
#include "coevent.h"
#include "obj.h"
#include "tasks.h"
#include <raymath.h>

DEFINE_EXTERN_TASK(phase_timer) {
    WAIT(ARGS.duration);

    coevent_signal(ARGS.event);
}

DEFINE_EXTERN_TASK(obj_GoTo) {
    TASK_BIND(ARGS.objId);

    while(true) {
        Vector2 pos = obj_GetPosition(ARGS.pool, ARGS.objId);
        Vector2 dest = (Vector2){ARGS.x, ARGS.y};

        if (Vector2Equals(pos, dest)) break;

        Vector2 intermid = Vector2MoveTowards(pos, dest, ARGS.speed);
        obj_SetPosition(ARGS.pool, ARGS.objId, intermid.x, intermid.y);

        YIELD;
    }
}