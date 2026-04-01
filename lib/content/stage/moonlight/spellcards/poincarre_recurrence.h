#pragma once

#include "coroutine/tasks.h"

DECLARE_EXTERN_TASK(poincarre_recurrence, {Pool *pool; Entity boss; int nb_rings; float speed; float ring_radius;});