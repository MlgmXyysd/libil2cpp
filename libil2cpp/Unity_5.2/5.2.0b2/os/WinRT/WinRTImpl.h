#pragma once

#define MAX_COMPUTERNAME_LENGTH 31

namespace il2cpp
{
	namespace winrt
	{
		inline DWORD WIN32_FROM_HRESULT(HRESULT hr)
		{
			if ((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0))
				return HRESULT_CODE(hr);
			if (hr == S_OK)
				return HRESULT_CODE(hr);
			return ERROR_SUCCESS;
		}
	}
}

extern "C"
{

#define CreateEvent CreateEventW
#define GetComputerName GetComputerNameW
#define GetEnvironmentVariable GetEnvironmentVariableW
#define GetUserName GetUserNameW
#define GetVersionEx GetVersionExW
#define SetEnvironmentVariable SetEnvironmentVariableW

inline BOOL WINAPI CopyFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists)
{
	COPYFILE2_EXTENDED_PARAMETERS params;
	
	params.dwSize = sizeof(params);
	params.dwCopyFlags = bFailIfExists ? COPY_FILE_FAIL_IF_EXISTS : 0;
	params.pfCancel = FALSE;
	params.pProgressRoutine = nullptr;
	params.pvCallbackContext = nullptr;

	auto hr = CopyFile2(lpExistingFileName, lpNewFileName, &params);
	if (FAILED(hr))
	{
		SetLastError(il2cpp::winrt::WIN32_FROM_HRESULT(hr));
		return FALSE;
	}

	return TRUE;
}

inline HANDLE WINAPI CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCWSTR lpName)
{
	DWORD flags = 0;
	if (bManualReset)
		flags |= CREATE_EVENT_MANUAL_RESET;
	if (bInitialState)
		flags |= CREATE_EVENT_INITIAL_SET;
	return CreateEventExW(lpEventAttributes, lpName, flags, EVENT_ALL_ACCESS);
}

inline HANDLE WINAPI CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	const DWORD kFileAttributeMask = 0x0000FFFF;
	const DWORD kFileFlagMask = 0xFFFF0000;

	CREATEFILE2_EXTENDED_PARAMETERS extendedParameters;
	extendedParameters.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParameters.dwFileAttributes = dwFlagsAndAttributes & kFileAttributeMask;
	extendedParameters.dwFileFlags = dwFlagsAndAttributes & kFileFlagMask;
	extendedParameters.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParameters.lpSecurityAttributes = lpSecurityAttributes;
	extendedParameters.hTemplateFile = hTemplateFile;

	return CreateFile2(lpFileName, dwDesiredAccess, dwShareMode, dwCreationDisposition, &extendedParameters);
}

inline UINT WINAPI GetACP()
{
	return CP_ACP;
}

BOOL WINAPI GetComputerNameW(LPWSTR lpBuffer, LPDWORD nSize);

inline DWORD WINAPI GetEnvironmentVariableW(LPCWSTR lpName, LPWSTR lpBuffer, DWORD nSize)
{
	return 0;
}

HANDLE WINAPI GetStdHandle(DWORD nStdHandle);

BOOL WINAPI GetThreadContextWinRT(HANDLE hThread, LPCONTEXT lpContext);

BOOL WINAPI GetUserNameW(LPWSTR lpBuffer, LPDWORD pcbBuffer);

inline BOOL WINAPI GetVersionExW(LPOSVERSIONINFOW lpVersionInformation)
{
	Assert(lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOW));

	lpVersionInformation->dwMajorVersion = 10;
	lpVersionInformation->dwMinorVersion = 0;
	lpVersionInformation->dwBuildNumber = 0;
	lpVersionInformation->dwPlatformId = 0;
	ZeroMemory(lpVersionInformation->szCSDVersion, sizeof(lpVersionInformation->szCSDVersion));

	return TRUE;
}

inline HMODULE WINAPI LoadLibraryW(LPCWSTR lpLibFileName)
{
	return LoadPackagedLibrary(lpLibFileName, 0);
}

inline BOOL WINAPI SetEnvironmentVariableW(LPCWSTR lpName, LPCWSTR lpValue)
{
	return FALSE;
}

typedef BOOL(WINAPI* GetThreadContextFunc)(HANDLE hThread, LPCONTEXT lpContext);
extern GetThreadContextFunc GetThreadContext;

typedef DWORD(WINAPI *QueueUserAPCFunc)(PAPCFUNC pfnAPC, HANDLE hThread, ULONG_PTR dwData);
extern QueueUserAPCFunc QueueUserAPC;

} // extern "C"
