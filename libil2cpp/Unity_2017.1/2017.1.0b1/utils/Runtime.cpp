#include "il2cpp-config.h"
#include "os/Environment.h"
#include "utils/Runtime.h"

namespace il2cpp
{
namespace utils
{
    NORETURN void Runtime::Abort()
    {
        os::Environment::Abort();
    }
} // utils
} // il2cpp
