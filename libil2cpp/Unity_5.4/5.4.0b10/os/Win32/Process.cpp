#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINDOWS && !IL2CPP_TARGET_WINRT

#include "WindowsHelpers.h"

#include "os/Process.h"

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
	return (ProcessHandle*)OpenProcess(PROCESS_ALL_ACCESS, TRUE, processId);
}

void Process::FreeProcess(ProcessHandle* handle)
{
	::CloseHandle((HANDLE)handle);
}

}
}

#endif

