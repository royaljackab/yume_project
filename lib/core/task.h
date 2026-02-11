#pragma once

#include <stdlib.h>
#include <stdarg.h>

#define Task_contParam void **Task_param

#define Task_beginContext struct Task_contextTag { int Task_line; int Task_wait
#define Task_endContext(x) } *x = (struct Task_contextTag *)*Task_param

#define Task_begin(x) if(!x) {x = *Task_param = calloc(1, sizeof (*x)); x->Task_line = 0; x->Task_wait = 0;} \
    if (x) switch(x->Task_line) {case 0:;
#define Task_finish(z) } free(*Task_param); *Task_param = 0; return (z)
#define Task_finishV } free(*Task_param); *Task_param = 0; return

#define Task_return(z) \
    do { \
        ((struct Task_contextTag *)*Task_param)->Task_line = __LINE__; \
        return (z); case __LINE__:; \
    } while(0)

#define Task_returnV \
    do { \
        ((struct Task_contextTag *)*Task_param)->Task_line = __LINE__; \
        return ; case __LINE__:; \
    } while(0)

#define Task_stop(z) do { free(*Task_param); *Task_param = 0; return (z); } while(0)
#define Task_stopV do { free(*Task_param); *Task_param = 0; return; } while(0)

#define Task_context void *
#define Task_abort(ctx) do {free(ctx); ctx = 0; return} while(0)

#define yield Task_returnV
#define wait(n) do { \
    ctx->Task_wait = (n); \
    ((struct Task_contextTag *)*Task_param)->Task_line = __LINE__; \
    return; case __LINE__: \
    if (--ctx->Task_wait > 0) return; \
} while (0)

#define PARAMS(...) , __VA_ARGS__
#define NO_PARAMS

#define Define_Static_Task(name, params, ...) \
    static void name(Task_contParam params) { \
        Task_beginContext; \
            __VA_ARGS__ \
        Task_endContext(ctx); \
        Task_begin(ctx);

#define Define_Task(name, params, ...) \
    void name(Task_contParam params) { \
        Task_beginContext; \
            __VA_ARGS__ \
        Task_endContext(ctx); \
        Task_begin(ctx);

#define End_Task \
        Task_finishV; \
    }
