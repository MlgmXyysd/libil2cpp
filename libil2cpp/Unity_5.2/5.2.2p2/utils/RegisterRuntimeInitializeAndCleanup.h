#pragma once

namespace il2cpp
{
namespace utils
{

class RegisterRuntimeInitializeAndCleanup
{
public:
	typedef void (*CallbackFunction) ();
	RegisterRuntimeInitializeAndCleanup(CallbackFunction Initialize, CallbackFunction Cleanup, int order = 0);

	static void ExecuteInitializations();
	static void ExecuteCleanup();
};

} /* namespace vm */
} /* namespace utils */
