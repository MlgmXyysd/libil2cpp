#pragma once

#if !IL2CPP_THREADS_STD && IL2CPP_THREADS_WIN32

#include "os/ErrorCodes.h"
#include "os/Thread.h"
#include "os/WaitStatus.h"
#include "utils/NonCopyable.h"

#include "WindowsHeaders.h"

namespace il2cpp
{
namespace os
{

class ThreadImpl : public il2cpp::utils::NonCopyable
{
public:
	ThreadImpl();

	uint64_t Id ();
	ErrorCode Run (Thread::StartFunc func, void* arg);
	void SetName (const std::string& name);
	void SetPriority (ThreadPriority priority);
	void QueueUserAPC (Thread::APCFunc func);

	static void Sleep (uint32_t ms, bool interruptible);
	static uint64_t CurrentThreadId ();
	static ThreadImpl* CreateForCurrentThread ();

private:
	HANDLE m_ThreadHandle;
};

}
}

#endif
