#pragma once

#include "coroutine/tasks.h"

DECLARE_EXTERN_TASK(phase_timer, { CoEvent *event; int duration; });
