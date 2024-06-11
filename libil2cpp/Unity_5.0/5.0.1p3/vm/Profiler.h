#pragma once

#include <stdint.h>
#include "il2cpp-api-types.h"

namespace il2cpp
{
namespace vm
{

class Profiler
{
// exported 
public:
	static void Install (Il2CppProfiler *prof, Il2CppProfileFunc shutdownCallback);
	static void SetEvents (Il2CppProfileFlags events);

	static void InstallEnterLeave (Il2CppProfileMethodFunc enter, Il2CppProfileMethodFunc fleave);
	static void InstallAllocation (Il2CppProfileAllocFunc callback);
	static void InstallGC (Il2CppProfileGCFunc callback, Il2CppProfileGCResizeFunc heap_resize_callback);

// internal
public:

#if IL2CPP_ENABLE_PROFILER
	static void Allocation (Il2CppObject *obj, TypeInfo *klass);
	static void MethodEnter (MethodInfo *method);
	static void MethodExit (MethodInfo *method);
#endif

	static Il2CppProfileFlags s_profilerEvents;

	static inline bool ProfileAllocations ()
	{
		return (s_profilerEvents & IL2CPP_PROFILE_ALLOCATIONS) != 0;
	}

private:
};

} /* namespace vm */
} /* namespace il2cpp */
