#pragma once
#include "utils/NonCopyable.h"

namespace il2cpp
{
namespace os
{
    class FastReaderReaderWriterLockImpl;

    // Use this class if the work done in the shared lock is "fast" - on the order of taking a lock
    class FastReaderReaderWriterLock
    {
    public:
        FastReaderReaderWriterLock();
        ~FastReaderReaderWriterLock();

        void LockExclusive();
        void LockShared();
        void ReleaseExclusive();
        void ReleaseShared();

        FastReaderReaderWriterLockImpl* GetImpl();

    private:
        FastReaderReaderWriterLockImpl* m_Impl;
    };

    struct FastReaderReaderWriterAutoLock : public il2cpp::utils::NonCopyable
    {
        FastReaderReaderWriterAutoLock(FastReaderReaderWriterLock* lock, bool exclusive = false)
            : m_Lock(lock), m_Exclusive(exclusive)
        {
            if (m_Exclusive)
                m_Lock->LockExclusive();
            else
                m_Lock->LockShared();
        }

        ~FastReaderReaderWriterAutoLock()
        {
            if (m_Exclusive)
                m_Lock->ReleaseExclusive();
            else
                m_Lock->ReleaseShared();
        }

    private:
        FastReaderReaderWriterLock* m_Lock;
        bool m_Exclusive;
    };
}
}
