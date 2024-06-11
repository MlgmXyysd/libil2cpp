#include "il2cpp-config.h"

#if IL2CPP_TARGET_ANDROID

#include "os/StackTrace.h"

#include <unwind.h>

namespace il2cpp
{
namespace os
{

const int kMaxStackFrames = 128;

namespace
{
struct AndroidStackTrace
{
	size_t size;
	Il2CppMethodPointer addrs[kMaxStackFrames];

	bool PushStackFrameAddress(const uintptr_t addr)
	{
		if (size >= kMaxStackFrames)
			return false;

		addrs[size++] = reinterpret_cast<Il2CppMethodPointer>(addr);
		return true;
	}

	static _Unwind_Reason_Code Callback(struct _Unwind_Context* context, void* self)
	{
		if (static_cast<AndroidStackTrace*>(self)->PushStackFrameAddress(_Unwind_GetIP(context)))
			return _URC_NO_REASON;
		else
			return _URC_END_OF_STACK;
	}
};
}

void StackTrace::WalkStack(WalkStackCallback callback, void* context, WalkOrder walkOrder)
{
	AndroidStackTrace callstack = {};
	_Unwind_Backtrace(AndroidStackTrace::Callback, &callstack);
	for (size_t i = 0; i < callstack.size; ++i)
	{
		const size_t index = (walkOrder == kFirstCalledToLastCalled) ? (callstack.size - i - 1) : i;
		if (!callback(callstack.addrs[index], context))
			break;
	}
}

}
}

#endif
