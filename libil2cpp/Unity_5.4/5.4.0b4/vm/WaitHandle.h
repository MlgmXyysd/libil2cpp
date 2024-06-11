#pragma once

struct Il2CppWaitHandle;
namespace il2cpp { namespace os { class Handle; } }

namespace il2cpp
{
namespace vm
{

class WaitHandle
{
public:
	static Il2CppWaitHandle* NewManualResetEvent (bool initialState);
	static os::Handle* GetPlatformHandle (Il2CppWaitHandle* waitHandle);
};

} /* namespace vm */
} /* namespace il2cpp */
