#pragma once

#include "pool.h"
#include "cotask.h"

typedef struct CoSched CoSched;

struct CoSched {
    CoTaskList tasks, pending_tasks;
    Pool *pool;
};

