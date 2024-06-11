#include "il2cpp-config.h"
#include "os/CrashHelpers.h"

#if IL2CPP_TARGET_POSIX

namespace il2cpp
{
namespace os
{
    void CrashHelpers::Crash()
    {
        __builtin_trap();
    }
}
}

#endif
