#pragma once

#include "coroutine/tasks.h"

DECLARE_EXTERN_TASK(phase_timer, { CoEvent *event; int duration; });

DECLARE_EXTERN_TASK(obj_GoTo, {Pool *pool; Entity objId; float x; float y; float speed;});