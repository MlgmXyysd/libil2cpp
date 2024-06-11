#pragma once

#if IL2CPP_THREADS_WIN32

#include "os/ErrorCodes.h"
#include "os/WaitStatus.h"
#include "utils/NonCopyable.h"

#include "WindowsHeaders.h"

namespace il2cpp
{
namespace os
{

class MutexImpl : public il2cpp::utils::NonCopyable
{
public:
	MutexImpl ();
	~MutexImpl ();

	void Lock (bool interruptible);
	bool TryLock (uint32_t milliseconds, bool interruptible);
	void Unlock ();

private:
	////TODO: we should have an "allows interruption" parameter on the ctor and use critical sections
	////	for mutexes that don't need interruption support
	HANDLE m_MutexHandle;
};

}
}

#endif
