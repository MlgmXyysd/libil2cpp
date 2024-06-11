#pragma once

#include <stdint.h>

struct Il2CppObject;

namespace il2cpp
{
namespace gc
{

enum GCHandleType
{
	HANDLE_WEAK,
	HANDLE_WEAK_TRACK,
	HANDLE_NORMAL,
	HANDLE_PINNED
};

class GCHandle
{
public:
	// external
	static uint32_t New (Il2CppObject *obj, bool pinned);
	static uint32_t NewWeakref (Il2CppObject *obj, bool track_resurrection);
	static Il2CppObject* GetTarget(uint32_t gchandle);
	static GCHandleType GetHandleType(uint32_t gcHandle);
	static void Free (uint32_t gchandle);
public:
	//internal
	static int32_t GetTargetHandle (Il2CppObject * obj, int32_t handle, int32_t type);
};

} /* gc */
} /* il2cpp */
