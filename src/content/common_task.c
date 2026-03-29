#include "common_task.h"
#include "coevent.h"
#include "tasks.h"

DEFINE_EXTERN_TASK(phase_timer) {
    WAIT(ARGS.duration);

    coevent_signal(ARGS.event);
}
