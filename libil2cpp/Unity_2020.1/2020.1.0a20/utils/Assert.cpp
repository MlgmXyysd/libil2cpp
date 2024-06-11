#include "Assert.h"

#if IL2CPP_DEBUG

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE || IL2CPP_TARGET_WINRT
#include <crtdbg.h>
#include<string>
#else
#include <cstdio>
#include <cstdlib>
#endif

void il2cpp_assert(const char* assertion, const char* file, unsigned int line)
{
#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE || IL2CPP_TARGET_WINRT
    std::string message = "Assertion failed: ";
    message += assertion;
    message += " file ";
    message += file;
    message += " line ";
    message += std::to_string(line);
    _ASSERT_EXPR(0, message.c_str());
#else
    printf("Assertion failed: %s, file %s, line %u", assertion, file, line);
    abort();
#endif
}

#endif
