#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX

#include <sys/types.h>
#include <unistd.h>

#include "os/Process.h"

struct ProcessHandle
{
	pid_t pid;
};

namespace il2cpp
{
namespace os
{

int Process::GetCurrentProcessId()
{
	return getpid();
}

ProcessHandle* Process::GetProcess(int processId)
{
	// If/when we implement the CreateProcess_internal icall we will likely
	// need to so something smarter here to find the process if we did
	// not create it and return a known pseudo-handle. For now this
	// is sufficient though.
	return (ProcessHandle*)(intptr_t)processId;
}

void Process::FreeProcess(ProcessHandle* handle)
{
	// We have nothing to do here.
}

}
}

#endif
