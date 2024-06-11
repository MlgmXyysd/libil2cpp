#pragma once

#include <stdint.h>
#include <vector>
#include "utils/NonCopyable.h"

#include "../il2cpp-api-types.h"

struct MethodInfo;

struct Il2CppArray;
struct Il2CppDomain;
struct Il2CppObject;
struct Il2CppThread;

namespace il2cpp
{
namespace vm
{

typedef std::vector<Il2CppStackFrameInfo> MethodStack;

// System.Threading.ThreadState
enum ThreadState
{
	kThreadStateRunning = 0x00000000,
	kThreadStateStopRequested = 0x00000001,
	kThreadStateSuspendRequested = 0x00000002,
	kThreadStateBackground = 0x00000004,
	kThreadStateUnstarted = 0x00000008,
	kThreadStateStopped = 0x00000010,
	kThreadStateWaitSleepJoin = 0x00000020,
	kThreadStateSuspended = 0x00000040,
	kThreadStateAbortRequested = 0x00000080,
	kThreadStateAborted = 0x00000100
}; 


class Thread
{
public:
	static char *GetName (uint32_t *len);
	static Il2CppThread* Current ();
	static Il2CppThread* Attach (Il2CppDomain *domain);
	static void Detach (Il2CppThread *thread);
	static void WalkFrameStack (Il2CppThread *thread, Il2CppFrameWalkFunc func, void *user_data);
	static Il2CppThread** GetAllAttachedThreads(size_t &size);
	static void KillAllBackgroundThreadsAndWaitForForegroundThreads ();
	static Il2CppThread* Main ();
	static bool IsVmThread (Il2CppThread *thread);
	static int32_t GetStackDepth (Il2CppThread *thread);

	static void RequestInterrupt (Il2CppThread* thread);
	static void CheckCurrentThreadForInterruptAndThrowIfNecessary ();

	////WORKAROUND: A dummy exception we throw when we abort background thread. This needs to be
	////	replaced with proper thread abortion support.
	struct TempAbortWorkaroundException {};

public:
	// internal
	static void Initialize ();
	static void UnInitialize ();

	static void AdjustStaticData ();
	static int32_t AllocThreadStaticData (int32_t size);
	static void FreeThreadStaticData (Il2CppThread *thread);
	static void* GetThreadStaticData (int32_t offset);
	
	static void Register (Il2CppThread *thread);
	static void Unregister (Il2CppThread *thread);

	static void PushFrame (Il2CppStackFrameInfo &info);
	static void PopFrame ();

	static void Setup (Il2CppThread* thread);

	/// Initialize and register thread.
	/// NOTE: Must be called on thread!
	static void Initialize (Il2CppThread *thread);

	static void Uninitialize (Il2CppThread *thread);
	static const MethodStack GetStackFrames ();
	static const MethodStack GetStackFrames (Il2CppThread *thread);
	static Il2CppStackFrameInfo *GetTopStackFrame (Il2CppThread *thread);
	static Il2CppStackFrameInfo *GetStackFrameAt (Il2CppThread *thread, int32_t offset);
	static bool GetStackFrameAtCurrentThread(Il2CppStackFrameInfo& frame, int32_t offset);

	static void SetMain (Il2CppThread* thread);

	static void SetState (Il2CppThread *thread, ThreadState value);
	static ThreadState GetState (Il2CppThread *thread);
	static void ClrState (Il2CppThread* thread, ThreadState clr);

private:
	static Il2CppThread* s_MainThread;
};

class ThreadAttacher : il2cpp::utils::NonCopyable
{
public:
	ThreadAttacher (Il2CppDomain *domain)
	{
		m_Thread = Thread::Attach (domain);
	}

	~ThreadAttacher ()
	{
		Thread::Detach (m_Thread);
	}
private:
	Il2CppThread* m_Thread;
};

class ThreadStateSetter : il2cpp::utils::NonCopyable
{
public:
	ThreadStateSetter(ThreadState state) : m_State(state)
	{
		m_Thread = il2cpp::vm::Thread::Current ();
		Thread::SetState(m_Thread, m_State);
	}

	~ThreadStateSetter ()
	{
		Thread::ClrState(m_Thread, m_State);
	}

private:
	ThreadState m_State;
	Il2CppThread* m_Thread;
};

} /* namespace vm */
} /* namespace il2cpp */