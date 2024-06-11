#pragma once
#if NET_4_0

#include "il2cpp-config.h"

struct Il2CppIOSelectorJob;
struct Il2CppIntPtr;

void threadpool_ms_io_remove_socket (int fd);
//void mono_threadpool_ms_io_remove_domain_jobs (MonoDomain *domain);
void threadpool_ms_io_cleanup (void);

void ves_icall_System_IOSelector_Add(Il2CppIntPtr handle, Il2CppIOSelectorJob *job);
void ves_icall_System_IOSelector_Remove(Il2CppIntPtr handle);

#endif
