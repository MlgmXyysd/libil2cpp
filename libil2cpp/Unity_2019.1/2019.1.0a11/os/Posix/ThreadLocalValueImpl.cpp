#include "os/c-api/il2cpp-config-platforms.h"

#if IL2CPP_THREADS_PTHREAD && !UNITY_TINY_WITHOUT_DEBUGGER

#include "ThreadLocalValueImpl.h"

#include <pthread.h>

#define _unused(x) ((void)x)

namespace il2cpp
{
namespace os
{
    ThreadLocalValueImpl::ThreadLocalValueImpl()
    {
        pthread_key_t key;
        int result = pthread_key_create(&key, NULL);
        IL2CPP_ASSERT(!result);
        _unused(result);

        m_Key = key;
    }

    ThreadLocalValueImpl::~ThreadLocalValueImpl()
    {
        IL2CPP_ASSERT(!pthread_key_delete(m_Key));
    }

    ErrorCode ThreadLocalValueImpl::SetValue(void* value)
    {
        if (pthread_setspecific(m_Key, value))
            return kErrorCodeGenFailure;

        return kErrorCodeSuccess;
    }

    ErrorCode ThreadLocalValueImpl::GetValue(void** value)
    {
        *value = pthread_getspecific(m_Key);

        return kErrorCodeSuccess;
    }
}
}

#endif
