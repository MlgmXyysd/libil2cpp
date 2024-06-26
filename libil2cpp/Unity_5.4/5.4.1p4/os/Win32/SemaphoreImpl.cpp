#include "il2cpp-config.h"

#if IL2CPP_THREADS_WIN32

#include "SemaphoreImpl.h"
#include "WindowsHelpers.h"
#include <cassert>

namespace il2cpp
{
namespace os
{

SemaphoreImpl::SemaphoreImpl(int32_t initialValue, int32_t maximumValue)
{
	m_Handle = ::CreateSemaphore (NULL, initialValue, maximumValue, NULL);

	assert (m_Handle);
}

SemaphoreImpl::~SemaphoreImpl ()
{
	assert (m_Handle);

	::CloseHandle (m_Handle);
}

bool SemaphoreImpl::Post (int32_t releaseCount, int32_t* previousCount)
{
	return ::ReleaseSemaphore (m_Handle, releaseCount, reinterpret_cast<LPLONG> (previousCount)) != 0;
}

WaitStatus SemaphoreImpl::Wait (bool interruptible)
{
	return Wait (INFINITE, interruptible);
}

WaitStatus SemaphoreImpl::Wait (uint32_t ms, bool interruptible)
{
	return il2cpp::os::win::WaitForSingleObjectAndAccountForAPCs (m_Handle, ms, interruptible);
}

}
}

#endif 
