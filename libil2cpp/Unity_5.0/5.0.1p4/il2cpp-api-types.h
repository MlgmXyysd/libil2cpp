#pragma once

#include "il2cpp-config.h"
#include <stdint.h>

struct TypeInfo;
struct Il2CppType;
struct MethodInfo;
struct FieldInfo;
struct PropertyInfo;

struct Il2CppAssembly;
struct Il2CppArray;
struct Il2CppDelegate;
struct Il2CppDomain;
struct Il2CppImage;
struct Il2CppException;
struct Il2CppProfiler;
struct Il2CppObject;
struct Il2CppReflectionMethod;
struct Il2CppReflectionType;
struct Il2CppString;
struct Il2CppThread;
struct Il2CppAsyncResult;

#if IL2CPP_DEBUGGER_ENABLED
struct Il2CppDebugDocument;
struct Il2CppDebugTypeInfo;
struct Il2CppDebugMethodInfo;
struct Il2CppDebugLocalsInfo;
#endif

enum Il2CppProfileFlags {
	IL2CPP_PROFILE_NONE = 0,
	IL2CPP_PROFILE_APPDOMAIN_EVENTS = 1 << 0,
	IL2CPP_PROFILE_ASSEMBLY_EVENTS  = 1 << 1,
	IL2CPP_PROFILE_MODULE_EVENTS    = 1 << 2,
	IL2CPP_PROFILE_CLASS_EVENTS     = 1 << 3,
	IL2CPP_PROFILE_JIT_COMPILATION  = 1 << 4,
	IL2CPP_PROFILE_INLINING         = 1 << 5,
	IL2CPP_PROFILE_EXCEPTIONS       = 1 << 6,
	IL2CPP_PROFILE_ALLOCATIONS      = 1 << 7,
	IL2CPP_PROFILE_GC               = 1 << 8,
	IL2CPP_PROFILE_THREADS          = 1 << 9,
	IL2CPP_PROFILE_REMOTING         = 1 << 10,
	IL2CPP_PROFILE_TRANSITIONS      = 1 << 11,
	IL2CPP_PROFILE_ENTER_LEAVE      = 1 << 12,
	IL2CPP_PROFILE_COVERAGE         = 1 << 13,
	IL2CPP_PROFILE_INS_COVERAGE     = 1 << 14,
	IL2CPP_PROFILE_STATISTICAL      = 1 << 15,
	IL2CPP_PROFILE_METHOD_EVENTS    = 1 << 16,
	IL2CPP_PROFILE_MONITOR_EVENTS   = 1 << 17,
	IL2CPP_PROFILE_IOMAP_EVENTS = 1 << 18, /* this should likely be removed, too */
	IL2CPP_PROFILE_GC_MOVES = 1 << 19
};

enum Il2CppGCEvent {
	IL2CPP_GC_EVENT_START,
	IL2CPP_GC_EVENT_MARK_START,
	IL2CPP_GC_EVENT_MARK_END,
	IL2CPP_GC_EVENT_RECLAIM_START,
	IL2CPP_GC_EVENT_RECLAIM_END,
	IL2CPP_GC_EVENT_END,
	IL2CPP_GC_EVENT_PRE_STOP_WORLD,
	IL2CPP_GC_EVENT_POST_STOP_WORLD,
	IL2CPP_GC_EVENT_PRE_START_WORLD,
	IL2CPP_GC_EVENT_POST_START_WORLD
};

enum StackFrameType
{
	FRAME_TYPE_MANAGED = 0,
	FRAME_TYPE_DEBUGGER_INVOKE = 1,
	FRAME_TYPE_MANAGED_TO_NATIVE = 2,
	FRAME_TYPE_SENTINEL = 3
};

enum Il2CppRuntimeUnhandledExceptionPolicy {
	IL2CPP_UNHANDLED_POLICY_LEGACY,
	IL2CPP_UNHANDLED_POLICY_CURRENT
};

struct Il2CppStackFrameInfo
{
	MethodInfo *method;

#if IL2CPP_DEBUGGER_ENABLED
	int32_t id;
	StackFrameType type;
	uint32_t il_offset;
	void *this_ptr;
	void **params;
	uint32_t params_count;
	void **locals;
	uint32_t locals_count;
#endif
};

typedef void (*register_object_callback)(void** arr, int size, void* userdata);

typedef void (*Il2CppFrameWalkFunc) (Il2CppThread* thread, const Il2CppStackFrameInfo *info, void *user_data);
typedef void (*Il2CppProfileFunc) (Il2CppProfiler* prof);
typedef void (*Il2CppProfileMethodFunc) (Il2CppProfiler* prof, MethodInfo *method);
typedef void (*Il2CppProfileAllocFunc) (Il2CppProfiler* prof, Il2CppObject *obj, TypeInfo *klass);
typedef void (*Il2CppProfileGCFunc) (Il2CppProfiler* prof, Il2CppGCEvent event, int generation);
typedef void (*Il2CppProfileGCResizeFunc) (Il2CppProfiler* prof, int64_t new_size);
