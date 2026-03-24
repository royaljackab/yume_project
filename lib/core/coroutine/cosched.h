#pragma once

#include "pool.h"
#include "cotask.h"

typedef struct CoSched CoSched;

struct CoSched {
    CoTaskList tasks, pending_tasks;
    Pool *pool;
};

void cosched_init(CoSched *sched, Pool *pool);
CoTask *_cosched_new_task(CoSched *sched, CoTaskFunc func, void *arg, size_t arg_size, bool is_subtask);
#define cosched_new_task(sched, func, arg, arg_size) \
    _cosched_new_task(sched, func, arg, arg_size, false)
#define cosched_new_subtask(sched, func, arg, arg_size) \
    _cosched_new_task(sched, func, arg, arg_size, true)
uint cosched_run_tasks(CoSched *sched);
void cosched_finish(CoSched *sched);
