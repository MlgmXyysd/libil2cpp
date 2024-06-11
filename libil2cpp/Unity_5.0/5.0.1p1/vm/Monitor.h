#pragma once

struct Il2CppObject;

namespace il2cpp
{
namespace vm
{

class Monitor
{
public:
	static void Enter (Il2CppObject* object);
	static bool TryEnter (Il2CppObject* object, uint32_t timeout);
	static void Exit (Il2CppObject* object);
	static void Pulse (Il2CppObject* object);
	static void PulseAll (Il2CppObject* object);
	static void Wait (Il2CppObject* object);
	static bool TryWait (Il2CppObject* object, uint32_t timeout);
	static bool IsAcquired (Il2CppObject* object);
};

} /* namespace vm */
} /* namespace il2cpp */
