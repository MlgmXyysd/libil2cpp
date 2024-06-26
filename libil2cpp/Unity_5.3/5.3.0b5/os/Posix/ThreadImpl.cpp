#include "il2cpp-config.h"

#if !IL2CPP_THREADS_STD && IL2CPP_THREADS_PTHREAD

#include <cassert>
#include <limits>
#include <unistd.h>
#include <map>
#include <pthread.h>

#if IL2CPP_TARGET_LINUX
#include <sys/prctl.h>
#endif

#include "ThreadImpl.h"
#include "PosixHelpers.h"
#include "os/Mutex.h"


namespace il2cpp
{
namespace os
{

/// An Event that we never signal. This is used for sleeping threads in an alertable state. They
/// simply wait on this object with the sleep timer as the timeout. This way we don't need a separate
/// codepath for implementing sleep logic.
static Event s_ThreadSleepObject;


#define ASSERT_CALLED_ON_CURRENT_THREAD \
	assert (pthread_equal (pthread_self (), m_Handle) && "Must be called on current thread!");


ThreadImpl::ThreadImpl ()
	: m_Handle (0)
	, m_State (kThreadCreated)
	, m_StartFunc (NULL)
	, m_StartArg (NULL)
	, m_CurrentWaitObject (NULL)
{
	pthread_mutex_init (&m_PendingAPCsMutex, NULL);
}

ThreadImpl::~ThreadImpl ()
{
	pthread_mutex_destroy (&m_PendingAPCsMutex);
}

ErrorCode ThreadImpl::Run (Thread::StartFunc func, void* arg)
{
	assert (m_State == kThreadCreated || m_State == kThreadExited);

	// Store state for run wrapper.
	m_StartFunc = func;
	m_StartArg = arg;

	// Initialize thread attributes.
	pthread_attr_t attr;
	int s = pthread_attr_init (&attr);
	if (s)
		return kErrorCodeGenFailure;

#if defined(IL2CPP_ENABLE_PLATFORM_THREAD_AFFINTY)
	// set create default core affinity
	pthread_attr_setaffinity_np(&attr, 0, NULL);
#endif
	
	// Create thread.
	pthread_t threadId;
	s = pthread_create (&threadId, &attr, &ThreadStartWrapper, this);
	if (s)
		return kErrorCodeGenFailure;

	// Destroy thread attributes.
	s = pthread_attr_destroy (&attr);
	if (s)
		return kErrorCodeGenFailure;

	// We're up and running.
	m_Handle = threadId;

	return kErrorCodeSuccess;
}

void* ThreadImpl::ThreadStartWrapper (void* arg)
{
	ThreadImpl* thread = reinterpret_cast<ThreadImpl*> (arg);

	// Also set handle here. No matter which thread proceeds first,
	// we need to make sure the handle is set.
	thread->m_Handle = pthread_self ();

	// Detach this thread since we will manage calling Join at the VM level
	// if necessary. Detaching it also prevents use from running out of thread
	// handles for background threads that are never joined.
	int returnValue = pthread_detach (thread->m_Handle);
	assert (returnValue == 0);
	(void)returnValue;

	// Run user code.
	thread->m_State = kThreadRunning;
	thread->m_StartFunc (thread->m_StartArg);
	thread->m_State = kThreadExited;

	return 0;
}

uint64_t ThreadImpl::Id ()
{
	return posix::PosixThreadIdToThreadId (m_Handle);
}

void ThreadImpl::SetName (const std::string& name)
{
	// Can only be set on current thread on OSX and Linux.
	if (pthread_self () != m_Handle)
		return;
	
#if IL2CPP_TARGET_DARWIN
	pthread_setname_np (name.c_str ());
#elif IL2CPP_TARGET_LINUX || IL2CPP_ENABLE_PLATFORM_THREAD_RENAME
	pthread_setname_np (m_Handle, name.c_str ());
#endif

}

void ThreadImpl::SetPriority (ThreadPriority priority)
{
	////TODO
}

void ThreadImpl::QueueUserAPC (Thread::APCFunc function, void* context)
{
	assert (function != NULL);

	// Put on queue.
	{
		pthread_mutex_lock (&m_PendingAPCsMutex);
		m_PendingAPCs.push_back (APCRequest(function, context));
		pthread_mutex_unlock (&m_PendingAPCsMutex);
	}

	// Interrupt an ongoing wait.
	posix::AutoLockWaitObjectDeletion lock;
	posix::PosixWaitObject* waitObject = m_CurrentWaitObject;
	if (waitObject)
		waitObject->InterruptWait ();
}

void ThreadImpl::CheckForUserAPCAndHandle ()
{
	ASSERT_CALLED_ON_CURRENT_THREAD;
	pthread_mutex_lock (&m_PendingAPCsMutex);

	while (!m_PendingAPCs.empty ())
	{
		APCRequest apcRequest = m_PendingAPCs.front ();

		// Remove from list. Do before calling the function to make sure the list
		// is up to date in case the function throws.
		m_PendingAPCs.erase (m_PendingAPCs.begin ());

		// Release mutex while we call the function so that we don't deadlock
		// if the function starts waiting on a thread that tries queuing an APC
		// on us.
		pthread_mutex_unlock (&m_PendingAPCsMutex);

		// Call function.
		apcRequest.callback (apcRequest.context);

		// Re-acquire mutex.
		pthread_mutex_lock (&m_PendingAPCsMutex);
	}

	pthread_mutex_unlock (&m_PendingAPCsMutex);
}

void ThreadImpl::SetWaitObject (posix::PosixWaitObject* waitObject)
{
	// Cannot set wait objects on threads other than the current thread.
	ASSERT_CALLED_ON_CURRENT_THREAD;

	// This is an unprotected write as write acccess is restricted to the
	// current thread.
	m_CurrentWaitObject = waitObject;
}

void ThreadImpl::Sleep (uint32_t milliseconds, bool interruptible)
{
	s_ThreadSleepObject.Wait (milliseconds, interruptible);
}

uint64_t ThreadImpl::CurrentThreadId ()
{
	return posix::PosixThreadIdToThreadId (pthread_self ());
}

ThreadImpl* ThreadImpl::GetCurrentThread ()
{
	return Thread::GetCurrentThread ()->m_Thread;
}

ThreadImpl* ThreadImpl::CreateForCurrentThread ()
{
	ThreadImpl* thread = new ThreadImpl ();
	thread->m_Handle = pthread_self ();
	return thread;
}

}
}

#endif
