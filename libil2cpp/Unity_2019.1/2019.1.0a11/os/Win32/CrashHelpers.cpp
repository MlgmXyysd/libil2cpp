#include "il2cpp-config.h"
#include "os/CrashHelpers.h"

#if IL2CPP_TARGET_WINDOWS

#include "WindowsHeaders.h"

namespace il2cpp
{
namespace os
{
    void CrashHelpers::Crash()
    {
        // __fastfail() is available since VS2012
#if _MSC_VER >= 1700
        __fastfail(FAST_FAIL_FATAL_APP_EXIT);
#else
        abort();
#endif
    }
}
}

#endif
