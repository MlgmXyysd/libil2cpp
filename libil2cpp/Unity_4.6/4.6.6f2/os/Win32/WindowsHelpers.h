#pragma once

#if IL2CPP_TARGET_WINDOWS

#include "WindowsHeaders.h"
#include "os/WaitStatus.h"

namespace il2cpp
{
namespace os
{
namespace win
{

// Wait for a release of the given handle in way that can be interrupted by APCs.
WaitStatus WaitForSingleObjectAndAccountForAPCs (HANDLE handle, uint32_t ms, bool interruptible);

}
}
}

#endif // IL2CPP_TARGET_WINDOWS
