#pragma once

#include "il2cpp-config.h"
#include <stdint.h>

struct TypeInfo;
struct Il2CppType;
struct EventInfo;
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

enum Il2CppStat {
	IL2CPP_STAT_NEW_OBJECT_COUNT,
	IL2CPP_STAT_INITIALIZED_CLASS_COUNT,
	//IL2CPP_STAT_GENERIC_VTABLE_COUNT,
	//IL2CPP_STAT_USED_CLASS_COUNT,
	IL2CPP_STAT_METHOD_COUNT,
	//IL2CPP_STAT_CLASS_VTABLE_SIZE,
	IL2CPP_STAT_CLASS_STATIC_DATA_SIZE,
	IL2CPP_STAT_GENERIC_INSTANCE_COUNT,
	IL2CPP_STAT_GENERIC_CLASS_COUNT,
	IL2CPP_STAT_INFLATED_METHOD_COUNT,
	IL2CPP_STAT_INFLATED_TYPE_COUNT,
	//IL2CPP_STAT_DELEGATE_CREATIONS,
	//IL2CPP_STAT_MINOR_GC_COUNT,
	//IL2CPP_STAT_MAJOR_GC_COUNT,
	//IL2CPP_STAT_MINOR_GC_TIME_USECS,
	//IL2CPP_STAT_MAJOR_GC_TIME_USECS
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
	const MethodInfo *method;

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

typedef struct {
	void* (*malloc_func)(size_t size);
	void* (*aligned_malloc_func)(size_t size, size_t alignment);
	void (*free_func)(void *ptr);
	void (*aligned_free_func)(void *ptr);
	void* (*calloc_func)(size_t nmemb, size_t size);
	void* (*realloc_func)(void *ptr, size_t size);
	void* (*aligned_realloc_func)(void *ptr, size_t size, size_t alignment);
} Il2CppMemoryCallbacks;

typedef void (*register_object_callback)(void** arr, int size, void* userdata);
typedef void (*WorldChangedCallback)();

typedef void (*Il2CppFrameWalkFunc) (const Il2CppStackFrameInfo *info, void *user_data);
typedef void (*Il2CppProfileFunc) (Il2CppProfiler* prof);
typedef void (*Il2CppProfileMethodFunc) (Il2CppProfiler* prof, const MethodInfo *method);
typedef void (*Il2CppProfileAllocFunc) (Il2CppProfiler* prof, Il2CppObject *obj, TypeInfo *klass);
typedef void (*Il2CppProfileGCFunc) (Il2CppProfiler* prof, Il2CppGCEvent event, int generation);
typedef void (*Il2CppProfileGCResizeFunc) (Il2CppProfiler* prof, int64_t new_size);

typedef const char* (*Il2CppSetFindPlugInCallback)(const char*);

struct Il2CppMetadataField
{
	uint32_t offset;
	uint32_t typeIndex;
	const char* name;
	bool isStatic;
};

enum Il2CppMetadataTypeFlags
{
	kNone = 0,
	kValueType = 1 << 0,
	kArray = 1 << 1,
	kArrayRankMask = 0xFFFF0000
};

struct Il2CppMetadataType
{
	Il2CppMetadataTypeFlags flags;	// If it's an array, rank is encoded in the upper 2 bytes
	Il2CppMetadataField* fields;
	uint32_t fieldCount;
	uint32_t staticsSize;
	uint8_t* statics;
	uint32_t baseOrElementTypeIndex;
	char* name;
	const char* assemblyName;
	uint64_t typeInfoAddress;
	uint32_t size;
};

struct Il2CppMetadataSnapshot
{
	uint32_t typeCount;
	Il2CppMetadataType* types;
};

struct Il2CppManagedMemorySection
{
	uint64_t sectionStartAddress;
	uint32_t sectionSize;
	uint8_t* sectionBytes;
};

struct Il2CppManagedHeap
{
	uint32_t sectionCount;
	Il2CppManagedMemorySection* sections;
};

struct Il2CppStacks
{
	uint32_t stackCount;
	Il2CppManagedMemorySection* stacks;
};

struct NativeObject
{
	uint32_t gcHandleIndex;
	uint32_t size;
	uint32_t instanceId;
	uint32_t classId;
	uint32_t referencedNativeObjectIndicesCount;
	uint32_t* referencedNativeObjectIndices;
};

struct Il2CppGCHandles
{
	uint32_t trackedObjectCount;
	uint64_t* pointersToObjects;
};

struct Il2CppRuntimeInformation
{
	uint32_t pointerSize;
	uint32_t objectHeaderSize;
	uint32_t arrayHeaderSize;
	uint32_t arrayBoundsOffsetInHeader;
	uint32_t arraySizeOffsetInHeader;
	uint32_t allocationGranularity;
};

struct Il2CppManagedMemorySnapshot
{
	Il2CppManagedHeap heap;
	Il2CppStacks stacks;
	Il2CppMetadataSnapshot metadata;
	Il2CppGCHandles gcHandles;
	Il2CppRuntimeInformation runtimeInformation;
	void* additionalUserInformation;
};
