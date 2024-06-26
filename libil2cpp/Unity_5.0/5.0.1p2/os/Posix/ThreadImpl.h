#pragma once

#if !IL2CPP_THREADS_STD && IL2CPP_THREADS_PTHREAD

#include <pthread.h>
#include <vector>

#include "PosixWaitObject.h"
#include "os/ErrorCodes.h"
#include "os/Mutex.h"
#include "os/Event.h"
#include "os/Thread.h"
#include "os/WaitStatus.h"
#include "utils/NonCopyable.h"

namespace il2cpp
{
namespace os
{

/// POSIX threads implementation. Supports APCs and interruptible waits.
class ThreadImpl : public il2cpp::utils::NonCopyable
{
public:

	ThreadImpl ();
	~ThreadImpl ();

	uint64_t Id ();
	ErrorCode Run (Thread::StartFunc func, void* arg);
	void QueueUserAPC (Thread::APCFunc func);
	void SetName (const std::string& name);
	void SetPriority (ThreadPriority priority);

	/// Handle any pending APCs.
	/// NOTE: Can only be called on current thread.
	void CheckForUserAPCAndHandle ();

	static void Sleep (uint32_t milliseconds, bool interruptible);
	static uint64_t CurrentThreadId ();
	static ThreadImpl* GetCurrentThread ();
	static ThreadImpl* CreateForCurrentThread ();

private:

	friend class posix::PosixWaitObject; // SetWaitObject(), CheckForAPCAndHandle()

	enum ThreadState
	{
		kThreadCreated,
		kThreadRunning,
		kThreadWaiting,
		kThreadExited
	};

	pthread_t m_Handle;

	ThreadState m_State;

	/// The synchronization primitive that this thread is currently blocked on.
	///
	/// NOTE: This field effectively turns these wait object into shared resources -- which makes deletion
	///       a tricky affair. To avoid one thread trying to interrupt a wait while the other thread already
	///       is in progress of deleting the wait object, we use a global mutex in PosixWaitObject.cpp that
	///       must be locked by any thread trying to trigger an interrupt.
	posix::PosixWaitObject* m_CurrentWaitObject;

	/// Start data.
	Thread::StartFunc m_StartFunc;
	void* m_StartArg;

	/// List of APC requests for this thread.
	pthread_mutex_t m_PendingAPCsMutex;
	std::vector<Thread::APCFunc> m_PendingAPCs;

	/// Set the synchronization object the thread is about to wait on.
	/// NOTE: This can only be called on the current thread.
	void SetWaitObject (posix::PosixWaitObject* waitObject);

	static void* ThreadStartWrapper (void* arg);
};


}
}

#endif
