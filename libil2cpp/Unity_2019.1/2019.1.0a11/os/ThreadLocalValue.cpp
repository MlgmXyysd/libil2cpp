#include "os/c-api/il2cpp-config-platforms.h"
#include "os/ThreadLocalValue.h"

#if IL2CPP_SUPPORT_THREADS

#if IL2CPP_THREADS_WIN32
#include "os/Win32/ThreadLocalValueImpl.h"
#elif IL2CPP_THREADS_PSP2
#include "os/PSP2/ThreadLocalValueImpl.h"
#elif IL2CPP_THREADS_PTHREAD
#include "os/Posix/ThreadLocalValueImpl.h"
#else
#include "os/ThreadLocalValueImpl.h"
#endif

namespace il2cpp
{
namespace os
{
    ThreadLocalValue::ThreadLocalValue()
        : m_ThreadLocalValue(new ThreadLocalValueImpl())
    {
    }

    ThreadLocalValue::~ThreadLocalValue()
    {
        delete m_ThreadLocalValue;
    }

    ErrorCode ThreadLocalValue::SetValue(void* value)
    {
        return m_ThreadLocalValue->SetValue(value);
    }

    ErrorCode ThreadLocalValue::GetValue(void** value)
    {
        return m_ThreadLocalValue->GetValue(value);
    }
}
}

#else

namespace il2cpp
{
namespace os
{
    ThreadLocalValue::ThreadLocalValue()
    {
    }

    ThreadLocalValue::~ThreadLocalValue()
    {
    }

    ErrorCode ThreadLocalValue::SetValue(void* value)
    {
        m_ThreadLocalValue = value;
        return kErrorCodeSuccess;
    }

    ErrorCode ThreadLocalValue::GetValue(void** value)
    {
        *value = m_ThreadLocalValue;
        return kErrorCodeSuccess;
    }
}
}

#endif
