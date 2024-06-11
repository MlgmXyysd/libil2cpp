#pragma once

#include <stdint.h>

struct TypeInfo;
struct Il2CppObject;

namespace il2cpp
{
namespace vm
{

class Liveness
{
public:
	typedef void (*register_object_callback)(void** arr, int size, void* userdata);
	typedef void (*WorldChangedCallback)();
	static void* Begin (TypeInfo* filter, int max_object_count, register_object_callback callback, void* userdata, WorldChangedCallback onWorldStarted, WorldChangedCallback onWorldStopped);
	static void End (void* state);
	static void FromRoot (Il2CppObject* root, void* state);
	static void FromStatics (void* state);
	static void StopWorld (WorldChangedCallback onWorldStopped);
	static void StartWorld (WorldChangedCallback onWorldStarted);
};

} /* namespace vm */
} /* namespace il2cpp */