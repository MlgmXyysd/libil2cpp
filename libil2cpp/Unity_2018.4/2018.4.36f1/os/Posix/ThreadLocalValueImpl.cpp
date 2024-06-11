#include "os/c-api/il2cpp-config-platforms.h"

#if IL2CPP_THREADS_PTHREAD

#include "ThreadLocalValueImpl.h"

#include <pthread.h>

namespace il2cpp
{
namespace os
{
    ThreadLocalValueImpl::ThreadLocalValueImpl()
    {
        pthread_key_t key;
        int result = pthread_key_create(&key, NULL);
        IL2CPP_ASSERT(!result);
        NO_UNUSED_WARNING(result);

        m_Key = key;
    }

    ThreadLocalValueImpl::~ThreadLocalValueImpl()
    {
        int result = pthread_key_delete(m_Key);
        IL2CPP_ASSERT(result == 0);
        NO_UNUSED_WARNING(result);
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
