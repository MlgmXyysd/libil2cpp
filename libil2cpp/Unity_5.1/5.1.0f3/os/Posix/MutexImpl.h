#pragma once

#if IL2CPP_THREADS_PTHREAD

#include "os/ErrorCodes.h"
#include "os/WaitStatus.h"
#include "PosixWaitObject.h"

#include <pthread.h>

namespace il2cpp
{
namespace os
{

class Thread;

class MutexImpl : public posix::PosixWaitObject
{
public:
	MutexImpl ();

	void Lock (bool interruptible);
	bool TryLock (uint32_t milliseconds, bool interruptible);
	void Unlock ();

private:
	/// Thread that currently owns the object. Used for recursion checks.
	Thread* m_OwningThread;

	/// Number of recursive locks on the owning thread.
	uint32_t m_RecursionCount;
};

}
}

#endif
