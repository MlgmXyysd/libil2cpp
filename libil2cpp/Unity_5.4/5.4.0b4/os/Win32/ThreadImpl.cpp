#include "il2cpp-config.h"

#if !IL2CPP_THREADS_STD && IL2CPP_THREADS_WIN32

#include "ThreadImpl.h"
#include "os/Time.h"
#include "WindowsHelpers.h"
#include <cassert>

namespace il2cpp
{
namespace os
{

struct StartData
{
	Thread::StartFunc m_StartFunc;
	void* m_StartArg;
};

static DWORD WINAPI ThreadStartWrapper(LPVOID arg)
{
	StartData* startData = (StartData*)arg;
	startData->m_StartFunc (startData->m_StartArg);

	free (startData);

	return 0;
}

ThreadImpl::ThreadImpl ()
 : m_ThreadHandle (0), m_ThreadId(0), m_StackSize(IL2CPP_DEFAULT_STACK_SIZE)
{
}

ThreadImpl::~ThreadImpl()
{
	if (m_ThreadHandle != NULL)
		CloseHandle(m_ThreadHandle);
}

uint64_t ThreadImpl::Id ()
{
	return m_ThreadId;
}

void ThreadImpl::SetName (const std::string& name)
{
	// http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx

	const DWORD MS_VC_EXCEPTION=0x406D1388;

	#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
	   DWORD dwType; // Must be 0x1000.
	   LPCSTR szName; // Pointer to name (in user addr space).
	   DWORD dwThreadID; // Thread ID (-1=caller thread).
	   DWORD dwFlags; // Reserved for future use, must be zero.
	} THREADNAME_INFO;
	#pragma pack(pop)

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name.c_str ();
	info.dwThreadID = static_cast<DWORD>(Id());
	info.dwFlags = 0;

	__try
	{
		RaiseException (MS_VC_EXCEPTION, 0, sizeof (info) / sizeof (ULONG_PTR), (ULONG_PTR*) &info);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

void ThreadImpl::SetPriority (ThreadPriority priority)
{
	int threadPriority;
	switch (priority)
	{
		case kThreadPriorityNormal:
			threadPriority = THREAD_PRIORITY_NORMAL;
			break;

		case kThreadPriorityLow:
			threadPriority = THREAD_PRIORITY_BELOW_NORMAL;
			break;

		case kThreadPriorityHigh:
			threadPriority = THREAD_PRIORITY_ABOVE_NORMAL;
			break;
	}

	::SetThreadPriority (m_ThreadHandle, threadPriority);
}

ErrorCode ThreadImpl::Run (Thread::StartFunc func, void* arg)
{
	StartData* startData = (StartData*)malloc (sizeof(StartData));
	startData->m_StartFunc = func;
	startData->m_StartArg = arg;

	// Create thread.
	HANDLE threadHandle = ::CreateThread (NULL, m_StackSize, &ThreadStartWrapper, startData, 0, &m_ThreadId);

	if (!threadHandle)
		return kErrorCodeGenFailure;

	m_ThreadHandle = threadHandle;

	return kErrorCodeSuccess;
}

void ThreadImpl::Sleep (uint32_t ms, bool interruptible)
{
	uint32_t remainingWaitTime = ms;
	while (true)
	{
		uint32_t startWaitTime = os::Time::GetTicksMillisecondsMonotonic ();
		DWORD sleepResult = ::SleepEx (remainingWaitTime, interruptible);

		if (sleepResult == WAIT_IO_COMPLETION)
		{
			uint32_t waitedTime = os::Time::GetTicksMillisecondsMonotonic () - startWaitTime;
			if (waitedTime >= remainingWaitTime)
				return;

			remainingWaitTime -= waitedTime;
			continue;
		}

		break;
	}
}

void ThreadImpl::QueueUserAPC (Thread::APCFunc func, void* context)
{
	::QueueUserAPC(reinterpret_cast<PAPCFUNC>(func), m_ThreadHandle, reinterpret_cast<ULONG_PTR>(context));
}

uint64_t ThreadImpl::CurrentThreadId ()
{
	return GetCurrentThreadId ();
}

ThreadImpl* ThreadImpl::CreateForCurrentThread ()
{
	ThreadImpl* thread = new ThreadImpl ();
	BOOL duplicateResult = DuplicateHandle(::GetCurrentProcess(), ::GetCurrentThread(), ::GetCurrentProcess(), &thread->m_ThreadHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
	Assert(duplicateResult && "DuplicateHandle failed.");
	return thread;
}

}
}

#endif
