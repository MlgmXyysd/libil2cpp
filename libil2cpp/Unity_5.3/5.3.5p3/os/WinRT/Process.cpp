#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINRT || IL2CPP_TARGET_XBOXONE
#include "os/Win32/WindowsHeaders.h"

#include "os/Process.h"
#include "vm/Exception.h"

struct ProcessHandle
{
	HANDLE handle;
};


namespace il2cpp
{
namespace os
{

int Process::GetCurrentProcessId()
{
	return ::GetCurrentProcessId();
}

ProcessHandle* Process::GetProcess(int processId)
{
	if (processId == GetCurrentProcessId())
		return (ProcessHandle*)::GetCurrentProcess();

	NOT_SUPPORTED_IL2CPP(Process::GetProcess, "It is not possible to interact with other system processes on WinRT.");
}

void Process::FreeProcess(ProcessHandle* handle)
{
	// We have nothing to do here.
}

}
}
#endif
