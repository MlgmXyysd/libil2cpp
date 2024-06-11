#pragma once
#if NET_4_0

#include "il2cpp-config.h"

struct Il2CppObject;
struct Il2CppDomain;
struct Il2CppArray;
struct Il2CppAsyncResult;
struct MethodInfo;
struct Il2CppNativeOverlapped;

#define SMALL_STACK (sizeof (void*) * 32 * 1024)

void threadpool_ms_cleanup (void);

Il2CppAsyncResult * threadpool_ms_begin_invoke (Il2CppDomain *domain, Il2CppObject *target, MethodInfo *method, void* *params);
Il2CppObject * threadpool_ms_end_invoke (Il2CppAsyncResult *ares, Il2CppArray **out_args, Il2CppObject **exc);

void threadpool_ms_suspend (void);
void threadpool_ms_resume (void);

void ves_icall_System_Threading_ThreadPool_GetAvailableThreadsNative (int32_t *worker_threads, int32_t *completion_port_threads);
void ves_icall_System_Threading_ThreadPool_GetMinThreadsNative (int32_t *worker_threads, int32_t *completion_port_threads);
void ves_icall_System_Threading_ThreadPool_GetMaxThreadsNative (int32_t *worker_threads, int32_t *completion_port_threads);
bool ves_icall_System_Threading_ThreadPool_SetMinThreadsNative (int32_t worker_threads, int32_t completion_port_threads);
bool ves_icall_System_Threading_ThreadPool_SetMaxThreadsNative (int32_t worker_threads, int32_t completion_port_threads);
void ves_icall_System_Threading_ThreadPool_InitializeVMTp (bool *enable_worker_tracking);
bool ves_icall_System_Threading_ThreadPool_NotifyWorkItemComplete (void);
void ves_icall_System_Threading_ThreadPool_NotifyWorkItemProgressNative (void);
void ves_icall_System_Threading_ThreadPool_ReportThreadStatus (bool is_working);
bool ves_icall_System_Threading_ThreadPool_RequestWorkerThread (void);
bool ves_icall_System_Threading_ThreadPool_PostQueuedCompletionStatus (Il2CppNativeOverlapped *native_overlapped);
bool ves_icall_System_Threading_ThreadPool_BindIOCompletionCallbackNative (void* file_handle);
bool ves_icall_System_Threading_ThreadPool_IsThreadPoolHosted (void);

/* Internals */
bool threadpool_ms_enqueue_work_item (Il2CppDomain *domain, Il2CppObject *work_item);

#endif
