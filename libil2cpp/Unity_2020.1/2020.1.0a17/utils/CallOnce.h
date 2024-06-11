#pragma once

#include "NonCopyable.h"

#include "Baselib.h"
#include "Cpp/Atomic.h"
#include "Cpp/Lock.h"

namespace il2cpp
{
namespace utils
{
    typedef void (*CallOnceFunc) (void* arg);

    struct OnceFlag : NonCopyable
    {
        OnceFlag() : m_IsSet(false)
        {
        }

        friend void CallOnce(OnceFlag& flag, CallOnceFunc func, void* arg);

        bool IsSet()
        {
            return m_IsSet;
        }

    private:
        baselib::atomic<bool> m_IsSet;
        baselib::Lock m_Lock;
    };

    inline void CallOnce(OnceFlag& flag, CallOnceFunc func, void* arg)
    {
        if (!flag.m_IsSet)
        {
            flag.m_Lock.AcquireScoped([&flag, &func, &arg] {
                if (!flag.m_IsSet)
                {
                    func(arg);
                    flag.m_IsSet = true;
                }
            });
        }
    }
}
}
