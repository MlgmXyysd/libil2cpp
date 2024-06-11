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
	static void* Begin (TypeInfo* filter, int max_object_count, register_object_callback callback, void* userdata);
	static void End (void* state);
	static void FromRoot (Il2CppObject* root, void* state);
	static void FromStatics (void* state);
	static void StopWorld ();
	static void StartWorld ();
};

} /* namespace vm */
} /* namespace il2cpp */