#include "il2cpp-config.h"

#include "os/Thread.h"
#include "os/ThreadLocalValue.h"
#if IL2CPP_THREADS_STD
#include "os/Std/ThreadImpl.h"
#elif IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE
#include "os/Win32/ThreadImpl.h"
#elif IL2CPP_THREADS_PTHREAD
#include "os/Posix/ThreadImpl.h"
#else
#include "os/ThreadImpl.h"
#endif
#include <cassert>
#include <limits>

namespace il2cpp
{
namespace os
{

/// TLS variable referring to current thread.
static ThreadLocalValue s_CurrentThread;

Thread::Thread()
	: m_Thread (new ThreadImpl ())
	, m_State (kThreadCreated)
	, m_ThreadExitedEvent (true) // Manual reset event
	, m_CleanupFunc (NULL)
	, m_CleanupFuncArg (NULL)
{
}

Thread::~Thread()
{
	delete m_Thread;
}

void Thread::Init ()
{
	GetOrCreateCurrentThread ();
}

Thread::ThreadId Thread::Id ()
{
	return m_Thread->Id ();
}

void Thread::SetName (const std::string& name)
{
	m_Thread->SetName (name);
}

void Thread::SetPriority (ThreadPriority priority)
{
	m_Thread->SetPriority (priority);
}

void Thread::SetStackSize (size_t stackSize)
{
	m_Thread->SetStackSize (stackSize);
}

struct StartData
{
	Thread* thread;
	Thread::StartFunc startFunction;
	void* startFunctionArgument;
};

/// Wrapper for the user's thread start function. Sets s_CurrentThread.
void Thread::RunWrapper (void* arg)
{
	StartData* data = reinterpret_cast<StartData*> (arg);

	// Store thread reference.
	Thread* thread = data->thread;
	s_CurrentThread.SetValue (thread);

	// Get rid of StartData.
	StartFunc startFunction = data->startFunction;
	void* startFunctionArgument = data->startFunctionArgument;
	delete data;

	// Make sure thread exit event is not signaled.
	thread->m_ThreadExitedEvent.Reset ();

	// Run user thread start function.
	thread->m_State = kThreadRunning;
	startFunction (startFunctionArgument);
	thread->m_State = kThreadExited;

	// Signal that we've finished execution.
	thread->m_ThreadExitedEvent.Set ();

	if (thread->m_CleanupFunc)
		thread->m_CleanupFunc (thread->m_CleanupFuncArg);
}

ErrorCode Thread::Run (StartFunc func, void* arg)
{
	assert (m_State == kThreadCreated || m_State == kThreadExited);

	StartData* startData = new StartData;
	startData->startFunction = func;
	startData->startFunctionArgument = arg;
	startData->thread = this;

	return m_Thread->Run (RunWrapper, startData);
}

WaitStatus Thread::Join ()
{
	assert (this != GetCurrentThread () && "Trying to join the current thread will deadlock");
	return Join (std::numeric_limits<uint32_t>::max ());
}

WaitStatus Thread::Join (uint32_t ms)
{
	// Wait for thread exit event.
	if (m_ThreadExitedEvent.Wait (ms, true) != kWaitStatusSuccess)
		return kWaitStatusFailure;

	return kWaitStatusSuccess;
}

void Thread::QueueUserAPC (APCFunc func, void* context)
{
	m_Thread->QueueUserAPC (func, context);
}

void Thread::Sleep (uint32_t milliseconds, bool interruptible)
{
	ThreadImpl::Sleep (milliseconds, interruptible);
}

uint64_t Thread::CurrentThreadId ()
{
	return ThreadImpl::CurrentThreadId ();
}

Thread* Thread::GetCurrentThread ()
{
	void* value;
	s_CurrentThread.GetValue (&value);
	assert (value != NULL);
	return reinterpret_cast<Thread*> (value);
}

Thread* Thread::GetOrCreateCurrentThread ()
{
	Thread* thread = NULL;
	s_CurrentThread.GetValue (reinterpret_cast<void**> (&thread));
	if (thread)
		return thread;

	thread = new Thread (ThreadImpl::CreateForCurrentThread ());
	s_CurrentThread.SetValue (thread);

	return thread;
}

void Thread::DetachCurrentThread ()
{
#if IL2CPP_DEBUG
	void* value;
	s_CurrentThread.GetValue (&value);
	assert (value != NULL);
#endif

	s_CurrentThread.SetValue (NULL);
}

}
}
