#include "il2cpp-config.h"

#include "class-internals.h"
#include "il2cpp-api.h"
#include "object-internals.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Image.h"
#include "vm/Object.h"
#include "vm/Thread.h"
#include "vm/ThreadPool.h"
#include "vm/WaitHandle.h"
#include "os/Atomic.h"
#include "os/Environment.h"
#include "os/Event.h"
#include "os/Mutex.h"
#include "os/Semaphore.h"
#include "os/Socket.h"
#include "os/Thread.h"
#include "gc/Allocator.h"
#include "gc/GCHandle.h"
#include "utils/Memory.h"
#include "icalls/System/System.Net.Sockets/Socket.h"

#include <queue>
#include <vector>
#include <cassert>
#include <list>
#include <limits>
#include <algorithm>

#if IL2CPP_TARGET_POSIX
#include <unistd.h>
#include "os/Posix/PosixHelpers.h"
#endif

#if IL2CPP_USE_SOCKET_MULTIPLEX_IO
#include "MultiplexIO.h"
#endif

////TODO: Currently the pool uses a single global lock for each compartment; doesn't scale well and provides room for optimization.

namespace il2cpp
{
namespace vm
{

enum
{
	THREADS_PER_CORE = 10
};

typedef gc::Allocator<Il2CppAsyncResult*> AsyncResultAllocator;
typedef std::list<Il2CppAsyncResult*, AsyncResultAllocator> AsyncResultList;
typedef std::vector<Il2CppAsyncResult*, AsyncResultAllocator> AsyncResultVector;
typedef std::queue<Il2CppAsyncResult*, AsyncResultList> AsyncResultQueue;


static const TypeInfo* g_SocketAsyncCallClass;
static const TypeInfo* g_ProcessAsyncCallClass;
static const TypeInfo* g_WriteDelegateClass;
static const TypeInfo* g_ReadDelegateClass;
////TODO: add System.Net.Sockets.Socket.SendFileHandler?

static bool IsInstanceOfDelegateClass (Il2CppDelegate* delegate, const char* delegateClassName, const char* outerClassName, const TypeInfo*& cachePtr)
{
	TypeInfo* klass = delegate->object.klass;

	TypeInfo* declaringType = Class::GetDeclaringType(klass);

	if (cachePtr == 0 &&
		strcmp (klass->name, delegateClassName) == 0 &&
		(strcmp (vm::Image::GetName (klass->image), "System") == 0 ||
		 strcmp (vm::Image::GetName (klass->image), "System.dll") == 0) &&
		 declaringType && strcmp (declaringType->name, outerClassName) == 0)
	{
		cachePtr = klass;
	}

	return (klass == cachePtr);
}

static bool IsSocketAsyncCall (Il2CppDelegate* delegate)
{
	return IsInstanceOfDelegateClass (delegate, "SocketAsyncCall", "Socket", g_SocketAsyncCallClass);
}

static bool IsProcessAsyncCall (Il2CppDelegate* delegate)
{
	return IsInstanceOfDelegateClass (delegate, "AsyncReadHandler", "Process", g_ProcessAsyncCallClass);
}

static bool IsFileStreamAsyncCall (Il2CppDelegate* delegate)
{
	return IsInstanceOfDelegateClass (delegate, "WriteDelegate", "FileStream", g_WriteDelegateClass) ||
	       IsInstanceOfDelegateClass (delegate, "ReadDelegate", "FileStream", g_ReadDelegateClass);
}


/// Socket operations enumeration taken from Mono. (Mostly) corresponds
/// to System.Net.Sockets.Socket.SocketOperation.
enum
{
	AIO_OP_FIRST,
	AIO_OP_ACCEPT = 0,
	AIO_OP_CONNECT,
	AIO_OP_RECEIVE,
	AIO_OP_RECEIVEFROM,
	AIO_OP_SEND,
	AIO_OP_SENDTO,
	AIO_OP_RECV_JUST_CALLBACK,
	AIO_OP_SEND_JUST_CALLBACK,
	AIO_OP_READPIPE,
	AIO_OP_LAST
};

/// We use a dedicated thread to pre-screen sockets for activity and only then handing them
/// on to the pool. This avoids having async I/O threads being hogged by single long-running
/// network requests. It's basically a separate staging step for socket AsyncResults.
struct SocketPollingThread
{
	os::FastMutex mutex;
	AsyncResultQueue queue;
	os::Thread* thread;
	os::Event threadStartupAcknowledged;
    
#if	IL2CPP_USE_SOCKET_MULTIPLEX_IO
	Sockets::MultiplexIO multiplexIO;	// container class to allow access to multiplex io socket functions
#elif IL2CPP_TARGET_POSIX
	/// On POSIX, we have no way to interrupt polls() with user APCs in a way that isn't prone
	/// to race conditions so what we do instead is create a pipe that we include in the poll()
	/// call and then write to that in order to interrupt an ongoing poll(). We cannot include pipes
	/// in polls on Windows so we'd have to use a full-blown socket there (including the taking of
	/// a local port) to do the same thing.

	enum
	{
		kMessageTerminate,
		kMessageNewAsyncResult
	};

	int readPipe;
	int writePipe;
#endif

	SocketPollingThread ()
		: threadStartupAcknowledged (true) {}

	void QueueRequest (Il2CppAsyncResult* asyncResult)
	{
		// Put in queue.
		{
			os::FastAutoLock lock (&mutex);
			queue.push (asyncResult);
		}

		// Interrupt polling thread to pick up new request.
#if	IL2CPP_USE_SOCKET_MULTIPLEX_IO
		multiplexIO.InterruptPoll(); // causes the current blocking poll to abort and recheck the queue
#elif IL2CPP_TARGET_POSIX
		char message = kMessageNewAsyncResult;
		write (writePipe, &message, 1);
#else
		thread->QueueUserAPC (ThrowPollingInterruptedException, NULL);
#endif
	}

	Il2CppAsyncResult* DequeueRequest ()
	{
		os::FastAutoLock lock (&mutex);
		if (queue.empty ())
			return NULL;
		Il2CppAsyncResult* asyncResult = queue.front ();
		queue.pop ();
		return asyncResult;
	}

	bool ResultReady()
	{
		os::FastAutoLock lock (&mutex);
		return !queue.empty();
	}

	void RunLoop ();
	void Terminate ();

	/// Dummy exception we throw to interrupt the polling thread to
	/// pick up new requests.
	struct PollingInterruptedException {};

private:

#if! IL2CPP_TARGET_POSIX
	static void STDCALL ThrowPollingInterruptedException (void* context)
	{
		throw PollingInterruptedException ();
	}
#endif
};

/// Data for a single pool of threads. We compartmentalize the pool to deal with async I/O and "normal" work
/// items separately.
struct ThreadPoolCompartment
{
	/// Human readable name of the compartment (mostly for debugging).
	const char* compartmentName;

	/// Minimum number of threads to be kept around. This is the number that the pool will
	/// actively try to maintain. Actual thread count can be less during startup of pool.
	/// NOTE: Can be changed without locking.
	int minThreads;

	/// Maximum number of threads the pool will ever have running at the same time.
	/// NOTE: Can be changed without locking.
	int maxThreads;

	/// Number of threads currently waiting for new work.
	/// NOTE: Changed atomically.
	volatile int32_t numIdleThreads;

	/// Semaphore that worker threads listen on.
	os::Semaphore signalThreads;

	/// Mutex for queue and threads vector.
	os::FastMutex mutex;

	/// Queue of pending items.
	/// NOTE: Requires lock on mutex.
	AsyncResultQueue queue;

	/// List of threads in the pool. Worker threads register and unregister themselves here.
	/// NOTE: Requires lock on mutex.
	std::vector<Il2CppThread*> threads;

	ThreadPoolCompartment ()
		: minThreads (0)
		, maxThreads (4)
		, signalThreads (0, std::numeric_limits<int32_t>::max())
		, numIdleThreads (0)
	{
	}

	void QueueWorkItem (Il2CppAsyncResult* asyncResult);
	Il2CppAsyncResult* DequeueNextWorkItem ();

	int AttachThread (Il2CppThread* thread)
	{
		os::FastAutoLock lock (&mutex);
		threads.push_back (thread);
		return (int)threads.size ();
	}

	void DetachThread (Il2CppThread* thread)
	{
		os::FastAutoLock lock (&mutex);
		threads.erase (std::remove (threads.begin (), threads.end (), thread),
			threads.end ());
	}

	void SignalAllThreads ()
	{
		signalThreads.Post ((int32_t)threads.size ());
	}

	void SpawnNewWorkerThread ();
	void WorkerThreadRunLoop ();
	
	enum
	{
		/// Time (in milliseconds) that a worker thread will wait before terminating after finding
		/// that the pool already has enough threads.
		kGracePeriodBeforeExtranenousWorkerThreadTerminates = 5000
	};
};

enum
{
	kWorkerThreadPool,
	kAsyncIOPool,
	kNumThreadPoolCompartments
};

static ThreadPoolCompartment g_ThreadPoolCompartments[kNumThreadPoolCompartments];
static SocketPollingThread g_SocketPollingThread;


static Il2CppSocketAsyncResult* GetSocketAsyncResult (Il2CppAsyncResult* asyncResult)
{
	////TODO: assert
	return reinterpret_cast<Il2CppSocketAsyncResult*> (asyncResult->async_state);
}

static bool IsSocketAsyncOperation (Il2CppAsyncResult* asyncResult)
{
	int32_t operation = GetSocketAsyncResult (asyncResult)->operation;
	return (operation >= AIO_OP_FIRST && operation <= AIO_OP_LAST);
}

static void InitPollRequest (os::PollRequest& request, Il2CppSocketAsyncResult* socketAsyncResult, os::SocketHandleWrapper& socketHandle)
{
	int32_t events = 0;

	switch (socketAsyncResult->operation)
	{
		case AIO_OP_ACCEPT:
		case AIO_OP_RECEIVE:
		case AIO_OP_RECV_JUST_CALLBACK:
		case AIO_OP_RECEIVEFROM:
		case AIO_OP_READPIPE:
			events |= os::kPollFlagsIn;
			break;

		case AIO_OP_SEND:
		case AIO_OP_SEND_JUST_CALLBACK:
		case AIO_OP_SENDTO:
		case AIO_OP_CONNECT:
			events |= os::kPollFlagsOut;
			break;

		default: // Should never happen
			assert (false && "Unrecognized socket async I/O operation");
			break;
	}

	// Acquire socket.
	socketHandle.Acquire (os::PointerToSocketHandle (socketAsyncResult->handle.m_value));

	request.socket = socketHandle.GetSocket ();
	request.revents = os::kPollFlagsNone;
	request.events = (os::PollFlags) events;
}

void SocketPollingThread::RunLoop ()
{
	// List of poll requests that we pass to os::Socket::Poll().
	std::vector<os::PollRequest> pollRequests;

	// List of AsyncResults corresponding to pollRequests. Needs to be its own list as
	// this is memory that we need the GC to scan.
	AsyncResultVector asyncResults;

	// List of socket handles we're currently using. If destructed, will automatically
	// release all sockets.
	std::vector<os::SocketHandleWrapper> socketHandles;

#if IL2CPP_TARGET_POSIX && !IL2CPP_USE_SOCKET_MULTIPLEX_IO
	std::vector<pollfd> posixPollRequests;
	{
		pollfd pollRequest;
		pollRequest.fd = readPipe;
		pollRequest.events = POLLIN;
		pollRequest.revents = 0;
		posixPollRequests.push_back (pollRequest);
	}
#else
	// A dummy semaphore that we go into interruptible sleep on while there are no sockets
	// to poll. Other threads interrupt us by throwing PollingInterruptedException
	// from a user APC. The semaphore itself is never signaled.
	os::Semaphore blockUntilInterrupted;
#endif

	bool firstIteration = true;
	while (true)
	{
		try
		{
			// Let other threads know we're ready to take requests. We have to do this
			// inside this try/catch block where we deal with PollingInterruptedException.
			if (firstIteration)
			{
				threadStartupAcknowledged.Set();
				firstIteration = false;
			}

#if !IL2CPP_TARGET_POSIX
			// If we don't have any sockets to poll, just go to sleep.
			if (pollRequests.empty () && queue.empty ())
				blockUntilInterrupted.Wait (true);
#endif

			// See if there's anything new in the queue.
			while (ResultReady())
			{
				// Grab next request.
				Il2CppAsyncResult* asyncResult = DequeueRequest ();
				if (!asyncResult)
					break;

				Il2CppSocketAsyncResult* socketAsyncResult = GetSocketAsyncResult (asyncResult);

				// Add socket handle.
				socketHandles.push_back (os::SocketHandleWrapper ());
				os::SocketHandleWrapper& socketHandle = socketHandles.back ();

				// Add the request to the list.
				os::PollRequest pollRequest;
				InitPollRequest (pollRequest, socketAsyncResult, socketHandle);

				asyncResults.push_back (asyncResult);
				pollRequests.push_back (pollRequest);
				
#if IL2CPP_TARGET_POSIX && !IL2CPP_USE_SOCKET_MULTIPLEX_IO
				pollfd posixPollRequest;
				posixPollRequest.fd = socketHandle.IsValid () ? socketHandle.GetSocket ()->GetDescriptor () : -1;
				posixPollRequest.events = 0xffff;
				posixPollRequest.revents = os::kPollFlagsNone;
				posixPollRequests.push_back (posixPollRequest);
#endif
			}

#if !IL2CPP_TARGET_POSIX
			// If we don't have any requests by now, just go back waiting.
			if (pollRequests.empty ())
				continue;
#endif

			// Poll the list.
#if IL2CPP_USE_SOCKET_MULTIPLEX_IO
			int32_t errorCode = 0;
			int32_t results = 0;
			multiplexIO.Poll(pollRequests, -1, &results, &errorCode);
#elif IL2CPP_TARGET_POSIX
			os::posix::Poll (posixPollRequests.data (), posixPollRequests.size (), -1);
			for (int i = 0; i < posixPollRequests.size (); ++i)
			{
				if (i == 0)
				{
					if (posixPollRequests[0].revents == os::kPollFlagsNone)
						continue;
					
					char message;
					if (read (readPipe, &message, 1) == 1 &&
						message == kMessageTerminate)
					{
						throw vm::Thread::NativeThreadAbortException ();
					}
					
					continue;
				}

				pollRequests[i - 1].revents = os::posix::PollEventsToPollFlags (posixPollRequests[i].revents);
			}
#else
			int32_t errorCode = 0;
			int32_t results = 0;
			os::Socket::Poll (pollRequests, -1, &results, &errorCode);
#endif

			// Go through our requests and see which ones we can forward, which ones are
			// obsolete, and which ones still need to be waited on.
			for (size_t i = 0; i < pollRequests.size ();)
			{
				os::PollRequest& pollRequest = pollRequests[i];

				// See if there's been some activity that allows us to forward the request
				// to the thread pool. We don't care what event(s) exactly happened on the
				// socket and the socket may even have been closed already. All we want is
				// to forward a socket to the pool as soon as there is some activity and then
				// have the normal processing chain sort out what kind of activity that was.
				if (pollRequest.revents)
				{
					// Yes.
					g_ThreadPoolCompartments[kAsyncIOPool].QueueWorkItem (asyncResults[i]);

					pollRequests.erase (pollRequests.begin () + i);
					asyncResults.erase (asyncResults.begin () + i);
					socketHandles.erase (socketHandles.begin () + i);
					
#if IL2CPP_TARGET_POSIX && !IL2CPP_USE_SOCKET_MULTIPLEX_IO
					posixPollRequests.erase (posixPollRequests.begin () + i + 1);
#endif
				}
				else
				{
					++i;
				}
			}
		}
		catch (PollingInterruptedException)
		{
		}
	}
}


static void FreeThreadHandle (void* data)
{
	uint32_t handle = (uint32_t)(uintptr_t)data;
	gc::GCHandle::Free (handle);
}

static void SocketPollingThreadEntryPoint (void* data)
{
	SocketPollingThread* pollingThread = reinterpret_cast<SocketPollingThread*> (data);

	// Properly attach us to the VM and mark us as a background thread.
	Il2CppThread* managedThread = vm::Thread::Attach (il2cpp_domain_get());
	uint32_t handle = gc::GCHandle::New ((Il2CppObject*) managedThread, true);
	vm::Thread::SetState (managedThread, kThreadStateBackground);
	managedThread->handle->SetName ("Socket I/O Polling Thread");
	managedThread->handle->SetPriority (os::kThreadPriorityLow);
	managedThread->handle->SetCleanupFunction (&FreeThreadHandle, (void*) (uintptr_t)handle);

	// The socket polling thread is not technically a worker pool thread but for all
	// intents and purposes it is part of the async I/O thread pool. It is important to
	// mark it as a thread pool thread so that the queueing logic correctly detects
	// when it is necessary to spin up a new thread to avoid deadlocks.
	managedThread->threadpool_thread = true;
	
#if IL2CPP_TARGET_POSIX && !IL2CPP_USE_SOCKET_MULTIPLEX_IO
	int pipeHandles[2];
	if (::pipe (pipeHandles) != 0)
	{
		vm::Exception::Raise (vm::Exception::GetExecutionEngineException ("Initialization socket polling thread for thread pool failed!"));
	}
	pollingThread->readPipe = pipeHandles[0];
	pollingThread->writePipe = pipeHandles[1];
#endif

	// Do work.
	try
	{
		pollingThread->RunLoop ();
	}
	catch (Thread::NativeThreadAbortException)
	{
		// Nothing to do. Runtime cleanup asked us to exit.
	}

	// Clean up.
	vm::Thread::Detach (managedThread);
}

static void SpawnSocketPollingThreadIfNeeded ()
{
	if (g_SocketPollingThread.thread)
		return;

	// Spawn thread.
	{
		os::FastAutoLock lock (&g_SocketPollingThread.mutex);
		// Double-check after lock to avoid race condition.
		if (!g_SocketPollingThread.thread)
		{
			g_SocketPollingThread.thread = new os::Thread ();
			g_SocketPollingThread.thread->Run (SocketPollingThreadEntryPoint, &g_SocketPollingThread);
		}
	}

	// Wait for thread to have started up so we can queue requests on it. As we are using
	// user APCs when queuing requests, we may end up interrupting the thread when it is not
	// ready yet if we don't wait here.
	g_SocketPollingThread.threadStartupAcknowledged.Wait ();
}

void SocketPollingThread::Terminate ()
{
	// Workaround on POSIX while we don't have proper thread abortion.
#if IL2CPP_TARGET_POSIX
	if (!g_SocketPollingThread.thread)
		return;
	
#if !IL2CPP_USE_SOCKET_MULTIPLEX_IO
	char message = kMessageTerminate;
	write (writePipe, &message, 1);
#endif	

	g_SocketPollingThread.thread->Join ();
#endif
}


static bool IsCurrentThreadAWorkerThread ()
{
	Il2CppThread* thread = vm::Thread::Current ();
	return thread->threadpool_thread;
}

void ThreadPoolCompartment::QueueWorkItem (Il2CppAsyncResult* asyncResult)
{
	bool forceNewThread = false;
	// Put the item in the queue.
	{
		os::FastAutoLock lock (&mutex);
		queue.push (asyncResult);
		assert(numIdleThreads >= 0);
		if (queue.size() > static_cast<uint32_t>(numIdleThreads))
			forceNewThread = true;
	}

	// If all our worker threads are tied up and we have room to grow, spawn a
	// new worker thread. Also, if an item is queued from within a work item that
	// is currently being processed and we don't have idle threads, force a new
	// thread to be spawned even if we are at max capacity. This prevents deadlocks
	// if the code queuing the item then goes and waits on the item it just queued.
	assert(maxThreads >= 0);
	if (forceNewThread &&
	    (threads.size () < static_cast<uint32_t>(maxThreads) || IsCurrentThreadAWorkerThread ()))
	{
		SpawnNewWorkerThread ();
	}
	else
	{
		// Signal existing thread.
		signalThreads.Post ();
	}
}

Il2CppAsyncResult* ThreadPoolCompartment::DequeueNextWorkItem ()
{
	os::FastAutoLock lock (&mutex);

	if (queue.empty ())
		return NULL;

	Il2CppAsyncResult* result = queue.front ();
	queue.pop ();

	return result;
}

static void WorkerThreadEntryPoint (void* data);
void ThreadPoolCompartment::SpawnNewWorkerThread ()
{
	os::Thread* thread = new os::Thread ();
	thread->Run (WorkerThreadEntryPoint, this);
}

static void HandleSocketAsyncOperation (Il2CppAsyncResult* asyncResult)
{
	Il2CppSocketAsyncResult* socketAsyncResult = GetSocketAsyncResult (asyncResult);

	const icalls::System::System::Net::Sockets::SocketFlags flags =
		static_cast<icalls::System::System::Net::Sockets::SocketFlags> (socketAsyncResult->socket_flags);
	Il2CppArray* buffer = socketAsyncResult->buffer;
	int32_t offset = socketAsyncResult->offset;
	int32_t count = socketAsyncResult->size;

	// Perform send/receive.
	switch (socketAsyncResult->operation)
	{
		case AIO_OP_RECEIVE:
			socketAsyncResult->total = icalls::System::System::Net::Sockets::Socket::Receive
				(socketAsyncResult->handle, buffer, offset, count, flags, &socketAsyncResult->error);
			break;

		case AIO_OP_SEND:
			socketAsyncResult->total = icalls::System::System::Net::Sockets::Socket::Send
				(socketAsyncResult->handle, buffer, offset, count, flags, &socketAsyncResult->error);
			break;
	}
}

void ThreadPoolCompartment::WorkerThreadRunLoop ()
{
	bool waitingToTerminate = false;

	// Pump AsyncResults until we're killed.
	while (true)
	{
		// Grab next work item.
		Il2CppAsyncResult* asyncResult = DequeueNextWorkItem ();
		if (!asyncResult)
		{
			// There's no work for us to do so decide what to do.

			// If we've exceeded the normal number of threads for the pool (minThreads),
			// wait around for a bit and then, if there is no work to do,
			// terminate.
			assert(minThreads >= 0);
			if (threads.size () > static_cast<uint32_t>(minThreads))
			{
				if (waitingToTerminate)
				{
					// We've already waited so now is the time to go.
					break;
				}

				waitingToTerminate = true;
			}

			// No item so wait for signal. We need to allow interruptions here as we don't yet
			// have proper abort support and the runtime currently uses interruptions to get
			// background threads to exit.
			os::Atomic::Increment (&numIdleThreads);
			if (waitingToTerminate)
				signalThreads.Wait (kGracePeriodBeforeExtranenousWorkerThreadTerminates, true);
			else
				signalThreads.Wait (true);
			os::Atomic::Decrement (&numIdleThreads);

			// Try again.
			continue;
		}
		waitingToTerminate = false;

		// See if it's a socket async call and if so, do whatever I/O we need to
		// do before invoking the delegate.
		Il2CppDelegate* delegate = asyncResult->async_delegate;
		const bool isSocketAsyncCall = IsSocketAsyncCall (delegate);
		if (isSocketAsyncCall)
			HandleSocketAsyncOperation (asyncResult);

		// Invoke delegate.
		Il2CppAsyncCall* asyncCall = asyncResult->object_data;
		Il2CppObject* exception = NULL;
		uint32_t argsGCHandle = (uint32_t) ((uintptr_t) asyncResult->data);
		Il2CppArray* args = (Il2CppArray*) gc::GCHandle::GetTarget (argsGCHandle);
		
		const uint8_t paramsCount = delegate->method->parameters_count;

		uint8_t byRefArgsCount = 0;
		for (uint8_t i = 0; i < paramsCount; ++i)
		{
			const Il2CppType* paramType = (Il2CppType*) delegate->method->parameters[i].parameter_type;
			if (paramType->byref)
				++byRefArgsCount;
		}

		void** byRefArgs = 0;
		if (byRefArgsCount > 0)
		{
			asyncCall->out_args = vm::Array::New (il2cpp_defaults.object_class, byRefArgsCount);
			byRefArgs = (void**) il2cpp_array_addr (asyncCall->out_args, Il2CppObject*, 0);
		}

		void** argsPtr = (void**) il2cpp_array_addr (args, Il2CppObject*, 0);
		void** params = (void**)IL2CPP_MALLOC (paramsCount * sizeof (void*));

		int byRefIndex = 0;
		for (uint8_t i = 0; i < paramsCount; ++i)
		{
			Il2CppType* paramType = (Il2CppType*)delegate->method->parameters[i].parameter_type;
			const TypeInfo* paramClass = il2cpp_class_from_type (paramType);
			const bool isValueType = il2cpp_class_is_valuetype (paramClass);

			if (paramType->byref)
			{
				if (isValueType)
				{
					// Value types are always boxed
					byRefArgs[byRefIndex] = il2cpp_object_unbox ((Il2CppObject*) argsPtr[i]);
					params[i] = byRefArgs[byRefIndex++];
				}
				else
				{
					byRefArgs[byRefIndex] = argsPtr[i];
					params[i] = &byRefArgs[byRefIndex++];
				}
			}
			else
			{
				params[i] = isValueType
					? il2cpp_object_unbox ((Il2CppObject*) argsPtr[i]) // Value types are always boxed
					: argsPtr[i];
			}
		}

		Il2CppObject* result = il2cpp_runtime_invoke (delegate->method, delegate->target, params, &exception);

		IL2CPP_FREE (params);
		gc::GCHandle::Free (argsGCHandle);

		// Store result.
		asyncCall->res = result;
		asyncCall->msg = exception;

		os::Atomic::MemoryBarrier ();
		asyncResult->completed = true;

		// Invoke callback, if we have one.
		Il2CppDelegate* asyncCallback = asyncCall->cb_target;
		if (asyncCallback)
		{
			void* args[1] = { asyncResult };
			il2cpp_runtime_invoke (asyncCallback->method, asyncCallback->target, args, &exception);
			asyncCall->msg = exception;
		}

		// Signal wait handle, if there's one.
		il2cpp_monitor_enter (&asyncResult->base);
		if (asyncResult->handle)
		{
			os::Handle* osHandle = vm::WaitHandle::GetPlatformHandle (asyncResult->handle);
			osHandle->Signal ();
		}

		il2cpp_monitor_exit (&asyncResult->base);
	}
}

static void WorkerThreadEntryPoint (void* data)
{
	ThreadPoolCompartment* compartment = reinterpret_cast<ThreadPoolCompartment*> (data);

	// Properly attach us to the VM and mark us as a background
	// worker thread.
	Il2CppThread* managedThread = vm::Thread::Attach (il2cpp_domain_get());
	uint32_t handle = gc::GCHandle::New ((Il2CppObject*) managedThread, true);
	vm::Thread::SetState (managedThread, kThreadStateBackground);
	managedThread->threadpool_thread = true;
	int threadCount = compartment->AttachThread (managedThread);

	// Configure OS thread.
	char name[2048];
	sprintf (name, "%s Thread #%i", compartment->compartmentName, threadCount - 1);
	managedThread->handle->SetName (name);
	managedThread->handle->SetPriority (os::kThreadPriorityLow);
	managedThread->handle->SetCleanupFunction (&FreeThreadHandle, (void*) (uintptr_t)handle);

	// Do work.
	try
	{
		compartment->WorkerThreadRunLoop ();
	}
	catch (Thread::NativeThreadAbortException)
	{
		// Nothing to do. Runtime cleanup asked us to exit.
	}
	catch (Il2CppExceptionWrapper e)
	{
		// Only eat a ThreadAbortException, as it may have been thrown by the runtime
		// when there was managed code on the stack, but that managed code exited already.
		if (strcmp(e.ex->object.klass->name, "ThreadAbortException") != 0)
			throw;
	}

	// Clean up.
	compartment->DetachThread (managedThread);
	vm::Thread::Detach (managedThread);
}

void ThreadPool::Initialize ()
{
	g_ThreadPoolCompartments[kWorkerThreadPool].compartmentName = "Worker Pool";
	g_ThreadPoolCompartments[kAsyncIOPool].compartmentName = "Async I/O Pool";

	int numCores = os::Environment::GetProcessorCount ();
	g_ThreadPoolCompartments[kWorkerThreadPool].minThreads = numCores;
	g_ThreadPoolCompartments[kWorkerThreadPool].maxThreads = 20 + THREADS_PER_CORE * numCores;
	g_ThreadPoolCompartments[kAsyncIOPool].minThreads = g_ThreadPoolCompartments[kWorkerThreadPool].minThreads;
	g_ThreadPoolCompartments[kAsyncIOPool].maxThreads = g_ThreadPoolCompartments[kWorkerThreadPool].maxThreads;
}

void ThreadPool::Shutdown ()
{
	g_SocketPollingThread.Terminate ();
}

ThreadPool::Configuration ThreadPool::GetConfiguration ()
{
	Configuration configuration;

	configuration.availableThreads = g_ThreadPoolCompartments[kWorkerThreadPool].numIdleThreads;
	configuration.availableAsyncIOThreads = g_ThreadPoolCompartments[kAsyncIOPool].numIdleThreads;
	configuration.minThreads = g_ThreadPoolCompartments[kWorkerThreadPool].minThreads;
	configuration.maxThreads = g_ThreadPoolCompartments[kWorkerThreadPool].maxThreads;
	configuration.minAsyncIOThreads = g_ThreadPoolCompartments[kAsyncIOPool].minThreads;
	configuration.maxAsyncIOThreads = g_ThreadPoolCompartments[kAsyncIOPool].maxThreads;

	return configuration;
}

void ThreadPool::SetConfiguration (const Configuration& configuration)
{
	assert (configuration.maxThreads >= configuration.minThreads && "Invalid configuration");
	assert (configuration.maxAsyncIOThreads >= configuration.minAsyncIOThreads && "Invalid configuration");
	assert (configuration.minThreads > 0 && "Invalid configuration");
	assert (configuration.minAsyncIOThreads > 0 && "Invalid configuration");
	assert (configuration.maxThreads > 0 && "Invalid configuration");
	assert (configuration.maxAsyncIOThreads > 0 && "Invalid configuration");

	g_ThreadPoolCompartments[kWorkerThreadPool].minThreads = configuration.minThreads;
	g_ThreadPoolCompartments[kWorkerThreadPool].maxThreads = configuration.maxThreads;
	g_ThreadPoolCompartments[kAsyncIOPool].minThreads = configuration.minAsyncIOThreads;
	g_ThreadPoolCompartments[kAsyncIOPool].maxThreads = configuration.maxAsyncIOThreads;

	// Get our worker threads to respond and exit, if necessary.
	// The method here isn't very smart and in fact won't even work reliably as idle worker
	// threads will steal the signal from threads that are currently busy.
	g_ThreadPoolCompartments[kWorkerThreadPool].SignalAllThreads ();
	g_ThreadPoolCompartments[kAsyncIOPool].SignalAllThreads ();
}

Il2CppAsyncResult* ThreadPool::Queue (Il2CppDelegate* delegate, void** params, Il2CppDelegate* asyncCallback, Il2CppObject* state)
{
	// Create AsyncCall.
	Il2CppAsyncCall* asyncCall = (Il2CppAsyncCall*) il2cpp::vm::Object::New (il2cpp_defaults.async_call_class);
	asyncCall->cb_target = asyncCallback;
	asyncCall->state = state;

	// Copy arguments.
	const uint8_t parametersCount = delegate->method->parameters_count;
	assert (!params[parametersCount] && "Expecting NULL as last element of the params array!");

	Il2CppArray* args = vm::Array::New (il2cpp_defaults.object_class, parametersCount);
	for (uint8_t i = 0; i < parametersCount; ++i)
		il2cpp_array_setref (args, i, params[i]);

	// Create AsyncResult.
	Il2CppAsyncResult* asyncResult = (Il2CppAsyncResult*) il2cpp::vm::Object::New (il2cpp_defaults.asyncresult_class);
	asyncResult->async_delegate = delegate;

	// NOTE: we store a GC handle here because .data is an IntPtr on the managed side and it won't be scanned by the GC.
	asyncResult->data = (void*) (uintptr_t) gc::GCHandle::New ((Il2CppObject*) args, true);
	asyncResult->object_data = asyncCall;
	asyncResult->async_state = state;

	// See which compartment we should process this request with and whether we
	// need to first pipe it through the socket polling stage.
	if (IsProcessAsyncCall (delegate))
	{
		NOT_SUPPORTED_IL2CPP (AsyncReadHandler, "Async Process operations are not supported by Il2Cpp");
	}
	else if (IsSocketAsyncCall (delegate))
	{
		Il2CppSocketAsyncResult* socketAsyncResult = GetSocketAsyncResult (asyncResult);
		socketAsyncResult->ares = asyncResult;

		// Apparently, using poll/WSAPoll to listen for connect() isn't reliable, so
		// we bypass the polling stage in that case.
		// Also, Mono has some AIO_OP_xxx operations that are only defined in C# and are not
		// meant to go through the polling stage.
		if ((socketAsyncResult->operation == AIO_OP_CONNECT && socketAsyncResult->blocking)
			|| !IsSocketAsyncOperation (asyncResult))
		{
			g_ThreadPoolCompartments[kAsyncIOPool].QueueWorkItem (asyncResult);
		}
		else
		{
			// Give it to polling thread.
			SpawnSocketPollingThreadIfNeeded ();
			g_SocketPollingThread.QueueRequest (asyncResult);
		}
	}
	else if (IsFileStreamAsyncCall (delegate))
	{
		g_ThreadPoolCompartments[kAsyncIOPool].QueueWorkItem (asyncResult);
	}
	else
	{
		g_ThreadPoolCompartments[kWorkerThreadPool].QueueWorkItem (asyncResult);
	}

	return asyncResult;
}

Il2CppObject* ThreadPool::Wait (Il2CppAsyncResult* asyncResult, void** outArgs)
{
	// Need lock already here to ensure only a single call to EndInvoke() gets to be the first one.
	il2cpp_monitor_enter (&asyncResult->base);

	// Throw if this result has already been waited on.
	if (asyncResult->endinvoke_called)
	{
		il2cpp_monitor_exit (&asyncResult->base);
		Exception::Raise (Exception::GetInvalidOperationException
			("Cannot call EndInvoke() repeatedly or concurrently on the same AsyncResult!" ) );
	}

	asyncResult->endinvoke_called = 1;

	// Wait if the AsyncResult hasn't yet been processed.
	if (!asyncResult->completed)
	{
		if (!asyncResult->handle)
			asyncResult->handle = WaitHandle::NewManualResetEvent (false);
		os::Handle* osHandle = WaitHandle::GetPlatformHandle (asyncResult->handle);

		il2cpp_monitor_exit (&asyncResult->base);

		osHandle->Wait ();
	}
	else
		il2cpp_monitor_exit (&asyncResult->base);

	// Raise exception now if the delegate threw while we were running it on
	// the worker thread.
	Il2CppAsyncCall* asyncCall = asyncResult->object_data;
	if (asyncCall->msg != NULL)
		il2cpp_raise_exception ((Il2CppException*)asyncCall->msg);

	// Copy out arguments.
	if (asyncCall->out_args != NULL)
	{
		void** outArgsPtr = (void**) il2cpp_array_addr (asyncCall->out_args, Il2CppObject*, 0);
		Il2CppDelegate* delegate = asyncResult->async_delegate;
		const uint8_t paramsCount = delegate->method->parameters_count;

		uint8_t index = 0;
		for(uint8_t i = 0; i < paramsCount; ++i)
		{
			Il2CppType* paramType = (Il2CppType*) delegate->method->parameters[i].parameter_type;
			const TypeInfo* paramClass = il2cpp_class_from_type (paramType);

			if (!paramType->byref)
				continue;
			
			const bool isValueType = il2cpp_class_is_valuetype (paramClass);
			if (isValueType)
			{
				assert (paramClass->native_size > 0 && "EndInvoke: Invalid native_size found when trying to copy a value type in the out_args.");

				// NOTE(gab): in case of value types, we need to copy the data over.
				memcpy (outArgs[index], outArgsPtr[index], paramClass->native_size);
			}
			else
			{
				*((void**) outArgs[index]) = outArgsPtr[index];
			}

			++index;
		}
	}

	return asyncResult->object_data->res;
}

} /* namespace vm */
} /* namespace il2cpp */
