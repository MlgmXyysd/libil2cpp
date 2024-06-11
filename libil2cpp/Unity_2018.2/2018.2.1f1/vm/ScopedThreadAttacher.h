#pragma once

#include "vm/Domain.h"
#include "vm/Thread.h"

namespace il2cpp
{
namespace vm
{
    class ScopedThreadAttacher
    {
    public:
        ScopedThreadAttacher()
#if !IL2CPP_HAS_NATIVE_THREAD_CLEANUP
            : m_AttachedThread(NULL)
#endif
        {
#if !IL2CPP_HAS_NATIVE_THREAD_CLEANUP
            if (il2cpp::vm::Thread::Current() == NULL)
                m_AttachedThread = il2cpp::vm::Thread::Attach(il2cpp::vm::Domain::GetRoot());
#else
            il2cpp::vm::Thread::Attach(il2cpp::vm::Domain::GetRoot());
#endif
        }

        ~ScopedThreadAttacher()
        {
#if !IL2CPP_HAS_NATIVE_THREAD_CLEANUP
            if (m_AttachedThread != NULL)
                il2cpp::vm::Thread::Detach(m_AttachedThread);
#endif
        }

    private:
#if !IL2CPP_HAS_NATIVE_THREAD_CLEANUP
        Il2CppThread * m_AttachedThread;
#endif
    };
}
}
