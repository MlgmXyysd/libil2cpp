#pragma once

#include "os/ErrorCodes.h"
#include "os/Handle.h"
#include "os/WaitStatus.h"
#include "utils/NonCopyable.h"

namespace il2cpp
{
namespace os
{

class MutexImpl;

class Mutex : public il2cpp::utils::NonCopyable
{
public:
	Mutex (bool initiallyOwned = false);
	~Mutex ();

	void Lock (bool interruptible = false);
	bool TryLock (uint32_t milliseconds = 0, bool interruptible = false);
	void Unlock ();

private:
	MutexImpl* m_Mutex;
};

struct AutoLock : public il2cpp::utils::NonCopyable
{
	AutoLock (Mutex* mutex) : m_Mutex (mutex) { m_Mutex->Lock (); }
	~AutoLock () { m_Mutex->Unlock (); }
private:
	Mutex* m_Mutex;
};

class MutexHandle : public Handle
{
public:
	MutexHandle (Mutex* mutex) : m_Mutex (mutex) {}
	virtual ~MutexHandle () { delete m_Mutex; };
	virtual bool Wait () { m_Mutex->Lock (true); return true; }
	virtual bool Wait (uint32_t ms) { return m_Mutex->TryLock (ms, true); }
	virtual void Signal () { m_Mutex->Unlock (); }
	Mutex* Get() { return m_Mutex; }

private:
	Mutex* m_Mutex;
};

}
}
