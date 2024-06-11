#pragma once

#include "os/ErrorCodes.h"

namespace il2cpp
{
namespace os
{
    class ThreadLocalValueImpl;

    class ThreadLocalValue
    {
    public:
        ThreadLocalValue();
        ~ThreadLocalValue();
        ErrorCode SetValue(void* value);
        ErrorCode GetValue(void** value);
    private:
#if IL2CPP_SUPPORT_THREADS
        ThreadLocalValueImpl * m_ThreadLocalValue;
#else
        void* m_ThreadLocalValue;
#endif
    };
}
}
