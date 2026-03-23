#pragma once

#include "coroutine/cotask.h"
#include "coroutine/coevent.h"
#include "coroutine/cosched.h"

#include "macro.h"

#define TASK_ARG_TYPE(name) COARGS_##name

#define TASK_ARGSDELAY_NAME(name) COARGSDELAY_##name
#define TASK_ARGSDELAY(name) struct TASK_ARGSDELAY_NAME(name)

#define TASK_ARGSCOND_NAME(name) COARGSCOND_##name
#define TASK_ARGSCOND(name) struct TASK_ARGSCOND_NAME(name)

#define TASK_IFACE_NAME(iface, suffix) COTASKIFACE_##iface##_##suffix
#define TASK_IFACE_ARGS_TYPE(iface) TASK_IFACE_NAME(iface, ARGS)
#define TASK_IFACE_ARGS_SIZED_PTR_TYPE(iface) TASK_IFACE_NAME(iface, ARGS_SPTR)
#define TASK_INDIRECT_TYPE(iface) TASK_IFACE_NAME(iface)

#define TASK_IFACE_SARGS(iface, ...) \
    ((TASK_IFACE_ARGS_SIZED_PTR_TYPE(iface)) { \
        .size = sizeof(TASK_IFACE_ARGS_TYPE(iface)), \
        .ptr (&(TASK_IFACE_ARGS_TYPE(iface)) { __VA_ARGS__ }) \
    })

#define DEFINE_TASK_INTERFACE(iface, argstruct) \
    typedef TASK_ARGS_STRUCT(argstruct) TASK_IFACE_ARGS_TYPE(iface); \
    typedef struct { \
        TASK_IFACE_ARGS_TYPE(iface) *ptr; \
        size_t size; \
    } TASK_IFACE_ARGS_SIZED_PTR_TYPE(iface); \
    typedef struct { \
        CoTaskFunc _cotask_##iface##_thunk; \
    } TASk_INDIRECT_TYPE(iface)

#define DEFINE_TASK_INTERFACE_WITH_BASE(iface, ibase, argstruct) \
	typedef struct { \
		TASK_IFACE_ARGS_TYPE(ibase) base; \
		TASK_ARGS_STRUCT(argstruct); \
	} TASK_IFACE_ARGS_TYPE(iface); \
	typedef struct { \
		union { \
			TASK_IFACE_ARGS_SIZED_PTR_TYPE(ibase) base; \
			struct { \
				TASK_IFACE_ARGS_TYPE(iface) *ptr; \
				size_t size; \
			}; \
		}; \
	} TASK_IFACE_ARGS_SIZED_PTR_TYPE(iface); \
	typedef struct { \
		union { \
			TASK_INDIRECT_TYPE(ibase) base; \
			CoTaskFunc _cotask_##iface##_thunk; \
			CoTaskFunc _cotask_##ibase##_thunk; \
		}; \
	} TASK_INDIRECT_TYPE(iface) \

#define TASK_INDIRECT_TYPE_ALIAS(task) TASK_IFACE_NAME(task, HANDLEALIAS)

#define ARGS (*_cotask_args)

#define TASK_ARGS_STRUCT(argstruct) struct {struct argstruct ; }

#define TASK_COMMON_PRIVATE_DECLARATIONS(name) \
    static void COTASK_##name(TASK_ARGS_TYPE(name) *_cotask_args)

#define TASK_COMMON_DECLARATIONS(name, argstype, handletype, linkage) \
    linkage char COTASK_UNUSED_TASKS_##name; \
    typedef handletype TASK_INDIRECT_TYPE_ALIAS(name); \
    typedef argstype TASK_ARGS_TYPE(name); \
    struct TASK_ARGSDELAY_NAME(name) { \
        int delay; \
        TASK_ARGS_TYPE(name) real_args; \
    }; \
    struct TASK_ARGSCOND_NAME(name) { \
        CoEvent *event; \
        bool unconditional; \
        TASK_ARGS_TYPEt(name) real_args; \
    }; \
    linkage void *COTASKTHUNK_##name(void *arg, size_t arg_size); \
    linkage void *COTASKTHUNK_DELAYED_##name(void *arg, size_t arg_size); \
    linkage void *COTASKTHUNK_COND_##name(void *arg, size_t arg_size) \

#define TASK_COMMON_THUNK_DECLARATIONS(name, linkage) \
    linkage void *COTASKTHUNK_##name(void *arg, size_t arg_size) { \
        TASK_ARGS_TYPE(name) args_copy = { }; \
        memcpy(&args_copy, arg, arg_size); \
        COTASK_##name(&args_copy); \
        return NULL; \
    } \
    linkage void *COTASKTHUNKDELAY_##name(void *arg, size_t arg_size) { \
        TASK_ARGSDELAY(name) args_copy = { }; \
        memcpy(&args_copy, arg, arg_size); \
        if (args_copy.delay < 0) return NULL; \
        WAIT(args_copy.delay); \
        COTASK_##name(&args_copy.real_args); \
        return NULL; \
    } \
    linkage void *COTASKTHUNKCOND_##name(void *arg, size_t arg_size) { \
        TASK_ARGSCOND(name) args_copy = { }; \
        memcpy(&args_copy, arg, arg_size); \
        if (WAIT_EVENT(args_copy.event).event_status == COEVENT_SIGNALED || args_copy.unconditional) { \
            COTASK_##name(&args_copy.real_args); \
        } \
        return NULL; \
    }

#define TASK_COMMON_BEGIN_BODY_DEFINITION(name, linkage) \
    linkage void COTASK_##name(TASK_ARGS_TYPE(name) *_cotask_args)

#define DECLARE_TASK_EXPLICIT(name, argstype, handletype, linkage) \
    TASK_COMMON_DECLARATIONS(name, argstype, handletype, linkage)

#define DEFINE_TASK_EXPLICIT(name, linkage) \
    TASK_COMMON_PRIVATE_DECLARATIONS(name); \
    TASK_COMMON_THUNK_DEFINITIONS(name, linkage) \
    TASK_COMMON_BEGIN_BODY_DEFINITION(name, linkage)

#define DECLARE_TASK(name, ...) \
    MACRO_OVERLOAD_HASARGS(DECLARE_TASK_, __VA_ARGS__)(name, ##__VA_ARGS__)
#define DECLARE_TASK_1(name, ...) \
    DECLARE_TASK_EXPLICIT(name, TASK_ARGS_STRUCT(__VA_ARGS__), void, static)
#define DECLARE_TASk_0(name) DECLARE_TASK_1(name, { })

#define DECLARE_TASK_WITH_INTERFACE(name, iface) \
    DECLARE_TASK_EXPLICIT(name, TASK_IFACE_ARGS_TYPE(iface), TASK_INDIRECT_TYPE(iface), static)

#define DEFINE_TASK(name) \
    DEFINE_TASK_EXPLICIT(name, static)

#define TASK(name, ...) \
    DECLARE_TASK(name, ##__VA_ARGS__); \
    DEFINE_TASK(name)

#define TASK_WITH_INTERFACE(name, iface) \
    DECLARE_TASK_WITH_INTERFACE(name, iface); \
    DEFINE_TASK(name)

#define DECLARE_EXTERN_TASK(name, ...) \
    MACRO_OVERLOAD_HASARGS(DECLARE_EXTERN_TASK_, __VA_ARGS__ )(name, ##__VA_ARGS__)
#define DECLARE_EXTERN_TASK_1(name, ...) \
    DECLARE_TASK_EXPLICIT(name, TASK_ARGS_STRUCT(__VA_ARGS__), void, extern)
#define DECLARE_EXTERN_TASK_0(name) \
    DECLARE_EXTERN_TASK_1(name, { })

#define DECLARE_EXTERN_TASK_WITH_INTERFACE(name, iface) \
    DECLARE_TASK_EXPLICIT(name, TASK_IFACE_ARGS_TYPE(iface), TASK_INDIRECT_TYPE(iface), extern)

#define DEFINE_EXTERN_TASK(name) \
    char COTASK_UNUSED_CHECK_##name; \
    DEFINE_TASK_EXPLICIT(name, extern)

#define INVOKE_TASK(_task, ...) \
	_internal_INVOKE_TASK(THIS_SCHED, cosched_new_task, _task, ##__VA_ARGS__)
#define INVOKE_SUBTASK(_task, ...) \
	_internal_INVOKE_TASK(THIS_SCHED, cosched_new_subtask, _task, ##__VA_ARGS__)
#define SCHED_INVOKE_TASK(_sched, _task, ...) \
	_internal_INVOKE_TASK(_sched, cosched_new_task, _task, ##__VA_ARGS__)

#define _internal_INVOKE_TASK(sched, task_constructor, name, ...) ( \
	(void)COTASK_UNUSED_CHECK_##name, \
	task_constructor( \
		sched, \
		COTASKTHUNK_##name, \
		(&(TASK_ARGS_TYPE(name)) { __VA_ARGS__ }), \
		sizeof(TASK_ARGS_TYPE(name)), \
		#name \
	) \
)

#define INVOKE_TASK_DELAYED(_delay, _task, ...) \
	_internal_INVOKE_TASK_DELAYED(THIS_SCHED, cosched_new_task, _delay, _task, ##__VA_ARGS__)
#define INVOKE_SUBTASK_DELAYED(_delay, _task, ...) \
	_internal_INVOKE_TASK_DELAYED(THIS_SCHED, cosched_new_subtask, _delay, _task, ##__VA_ARGS__)
#define SCHED_INVOKE_TASK_DELAYED(_sched, _delay, _task, ...) \
	_internal_INVOKE_TASK_DELAYED(_sched, cosched_new_task, _delay, _task, ##__VA_ARGS__)

#define _internal_INVOKE_TASK_DELAYED(sched, task_constructor, _delay, name, ...) ( \
	(void)COTASK_UNUSED_CHECK_##name, \
	task_constructor( \
		sched, \
		COTASKTHUNKDELAY_##name, \
		(&(TASK_ARGSDELAY(name)) { \
			.real_args = { __VA_ARGS__ }, \
			.delay = (_delay) \
		}), \
		sizeof(TASK_ARGSDELAY(name)), \
		#name \
	) \
)

#define INVOKE_TASK_WHEN(_event, _task, ...) \
	_internal_INVOKE_TASK_ON_EVENT(THIS_SCHED, cosched_new_task, false, _event, _task, ##__VA_ARGS__)
#define INVOKE_SUBTASK_WHEN(_event, _task, ...) \
	_internal_INVOKE_TASK_ON_EVENT(THIS_SCHED, cosched_new_subtask, false, _event, _task, ##__VA_ARGS__)
#define SCHED_INVOKE_TASK_WHEN(_sched, _event, _task, ...) \
	_internal_INVOKE_TASK_ON_EVENT(_sched, cosched_new_task, false, _event, _task, ##__VA_ARGS__)

#define INVOKE_TASK_AFTER(_event, _task, ...) \
	_internal_INVOKE_TASK_ON_EVENT(THIS_SCHED, cosched_new_task, true, _event, _task, ## __VA_ARGS__)
#define INVOKE_SUBTASK_AFTER(_event, _task, ...) \
	_internal_INVOKE_TASK_ON_EVENT(THIS_SCHED, cosched_new_subtask, true, _event, _task, ## __VA_ARGS__)
#define SCHED_INVOKE_TASK_AFTER(_sched, _event, _task, ...) \
	_internal_INVOKE_TASK_ON_EVENT(_sched, cosched_new_task, true, _event, _task, ##__VA_ARGS__)

#define _internal_INVOKE_TASK_ON_EVENT(sched, task_constructor, is_unconditional, _event, name, ...) ( \
	(void)COTASK_UNUSED_CHECK_##name, \
	task_constructor( \
		sched, \
		COTASKTHUNKCOND_##name, \
		(&(TASK_ARGSCOND(name)) { \
			.real_args = { __VA_ARGS__ }, \
			.event = (_event), \
			.unconditional = is_unconditional \
		}), \
		sizeof(TASK_ARGSCOND(name)), \
		#name \
	) \
)

#define CANCEL_TASK_WHEN(_event, _task) INVOKE_TASK_WHEN(_event, _cancel_task_helper, _task)
#define CANCEL_TASK_AFTER(_event, _task) INVOKE_TASK_AFTER(_event, _cancel_task_helper, _task)

DECLARE_EXTERN_TASK(_cancel_task_helper, { BoxedTask task; });

#define CANCEL_TASK(boxed_task) cotask_cancel(cotask_unbox(boxed_task))

#define TASK_INDIRECT(iface, task) ( \
	(void)COTASK_UNUSED_CHECK_##task, \
	(TASK_INDIRECT_TYPE_ALIAS(task)) { ._cotask_##iface##_thunk = COTASKTHUNK_##task } \
)

#define TASK_INDIRECT_INIT(iface, task) \
	{ ._cotask_##iface##_thunk = COTASKTHUNK_##task } \

#define INVOKE_TASK_INDIRECT_(sched, task_constructor, iface, taskhandle, ...) ( \
	task_constructor( \
		sched, \
		taskhandle._cotask_##iface##_thunk, \
		(&(TASK_IFACE_ARGS_TYPE(iface)) { __VA_ARGS__ }), \
		sizeof(TASK_IFACE_ARGS_TYPE(iface)), \
		"<indirect:"#iface">" \
	) \
)

#define SCHED_INVOKE_TASK_INDIRECT(_sched, _iface, _handle, ...) \
	INVOKE_TASK_INDIRECT_(_sched, cosched_new_task, _iface, _handle, ##__VA_ARGS__)
#define INVOKE_TASK_INDIRECT(_iface, _handle, ...) \
	INVOKE_TASK_INDIRECT_(THIS_SCHED, cosched_new_task, _iface, _handle, ##__VA_ARGS__)
#define INVOKE_SUBTASK_INDIRECT(_iface, _handle, ...) \
	INVOKE_TASK_INDIRECT_(THIS_SCHED, cosched_new_subtask, iface, _handle, ##__VA_ARGS__)

#define THIS_TASK         cotask_box(cotask_active_unsafe())
#define TASK_EVENTS(task) cotask_get_events(cotask_unbox(task))
#define TASK_MALLOC(size) cotask_malloc(cotask_active_unsafe(), size)

#define THIS_SCHED        cotask_get_sched(cotask_active_unsafe())

#define TASK_HOST_ENT() \
    cotask_host_entity(cotask_active())