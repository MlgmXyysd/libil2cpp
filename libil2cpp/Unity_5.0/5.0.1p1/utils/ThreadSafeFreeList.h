#pragma once

#include "os/Atomic.h"

namespace il2cpp
{
namespace utils
{

struct ThreadSafeFreeListNode
{
	ThreadSafeFreeListNode* nextFreeListNode;

	ThreadSafeFreeListNode ()
		: nextFreeListNode (NULL) {}
};

/// Lockless allocator that keeps instances of T on a free list.
///
/// If ALLOW_DELETION is true, an instance may be deleted instead of put on the free list if
/// there is a lot of contention on the list.  If ALLOW_DELETION is false, instances will not
/// be deleted by the list which may be very important if deletion may delete to race conditions.
///
/// T can be either derived from ThreadSafeFreeListNode or can be an arbitrary structure.  In
/// the latter case, sizeof(T) must be >= sizeof(void*).  Also, in that case releasing an instance
/// back to the pool will destroy the first 4 or 8 bytes (depending on the pointer size) of
/// instance data.
template<typename T, bool ALLOW_DELETION = true>
struct ThreadSafeFreeList
{
	ThreadSafeFreeList ()
		: m_FreeList (NULL)
	{
	}
	
	T* Allocate()
	{
		// Try to grab an existing instance from the free list.  
		for (int i = 0; i < MaxRaceIterationCount; ++i)
		{
			ThreadSafeFreeListNode* firstFree = m_FreeList;
			if (!firstFree)
			{
				// No unused instances.
				break;
			}

			// Try to grab this instance.
			ThreadSafeFreeListNode* nextFree = firstFree->nextFreeListNode;
			if (os::Atomic::CompareExchangePointer (&m_FreeList, nextFree, firstFree) == firstFree)
				return reinterpret_cast<T*> (firstFree);
		}

		return new T ();
	}

	void Release (T* instance)
	{
		for (int i = 0; i < MaxRaceIterationCount; ALLOW_DELETION && ++i)
		{
			ThreadSafeFreeListNode* nextNode = m_FreeList;
			ThreadSafeFreeListNode* thisNode = reinterpret_cast<ThreadSafeFreeListNode*> (instance);
			thisNode->nextFreeListNode = nextNode;

			if (il2cpp::os::Atomic::CompareExchangePointer (&m_FreeList, thisNode, nextNode) == nextNode)
				return;
		}

		if (ALLOW_DELETION)
			delete instance;
	}

private:

	// If possible, race for only a fixed number of iterations since while(true) loops are scary.
	static const int MaxRaceIterationCount = 5;

	ThreadSafeFreeListNode* volatile m_FreeList;
};

} /* utils */
} /* il2cpp */