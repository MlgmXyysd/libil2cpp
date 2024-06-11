#include "il2cpp-config.h"
#include "Finally.h"
#include "vm/Exception.h"

void il2cpp::utils::ThrowException(Il2CppException* exception)
{
#if !RUNTIME_TINY
    vm::Exception::Raise(exception, nullptr);
#else
    tiny::vm::Exception::Raise(exception);
#endif
}
