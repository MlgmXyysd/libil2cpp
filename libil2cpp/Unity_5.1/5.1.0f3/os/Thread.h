#pragma once

#include "os/ErrorCodes.h"
#include "os/Handle.h"
#include "os/Event.h"
#include "os/WaitStatus.h"
#include "utils/NonCopyable.h"

namespace il2cpp
{
namespace os
{

class ThreadImpl;

enum ThreadPriority
{
	kThreadPriorityNormal,
	kThreadPriorityLow,
	kThreadPriorityHigh
};

class Thread : public il2cpp::utils::NonCopyable
{
public:
	Thread();
	~Thread();

	typedef void (*StartFunc) (void* arg);
	typedef void (*APCFunc) ();
	typedef uint64_t ThreadId;

	/// Initialize thread subsystem. Must be called on main thread.
	static void Init ();

	ErrorCode Run (StartFunc func, void* arg);
	ThreadId Id ();

	/// Set thread name for debugging purposes. Won't do anything if not supported
	/// by platform.
	void SetName (const std::string& name);

	void SetPriority (ThreadPriority priority);

	/// Interruptible, infinite wait join.
	WaitStatus Join ();

	/// Interruptible, timed wait join.
	WaitStatus Join (uint32_t ms);

	/// Execute the given function on the thread the next time the thread executes
	/// an interruptible blocking operation.
	/// NOTE: The APC is allowed to raise exceptions!
	void QueueUserAPC (APCFunc func);

	/// Interruptible, timed sleep.
	static void Sleep (uint32_t ms, bool interruptible = false);

	static ThreadId CurrentThreadId ();
	static Thread* GetCurrentThread ();
	static Thread* GetOrCreateCurrentThread ();
	static void DetachCurrentThread ();

	static const uint64_t kInvalidThreadId = 0;

private:
	friend class ThreadImpl; // m_Thread

	ThreadImpl* m_Thread;

	/// Event that the thread signals when it finishes execution. Used for joins.
	/// Supports interruption.
	Event m_ThreadExitedEvent;

	Thread (ThreadImpl* thread)
		: m_Thread (thread) {}

	static void RunWrapper (void* arg);
};

}
}
