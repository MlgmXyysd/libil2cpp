#include "il2cpp-config.h"

#if !IL2CPP_USE_GENERIC_ENVIRONMENT && (IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE)
#include "WindowsHelpers.h"
#if !IL2CPP_TARGET_XBOXONE
#include <Shlobj.h>
#endif
// Windows.h defines GetEnvironmentVariable as GetEnvironmentVariableW for unicode and this will
// change the string "Environment::GetEnvironmentVariable" below to "Environment::GetEnvironmentVariableW"
// in the preprocessor. So we undef to avoid this issue and use GetEnvironmentVariableW directly.
// Same for SetEnvironmentVariable
#undef GetEnvironmentVariable
#undef SetEnvironmentVariable
#include "os/Environment.h"
#include "utils/StringUtils.h"
#include <string>
#include <assert.h>

#define BUFFER_SIZE	1024

namespace il2cpp
{
namespace os
{

std::string Environment::GetMachineName()
{
	WCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(computerName) / sizeof(computerName[0]);
	if(!GetComputerNameW(computerName, &size))
		return NULL;

	return utils::StringUtils::Utf16ToUtf8((const uint16_t*)computerName);
}

int32_t Environment::GetProcessorCount()
{
	SYSTEM_INFO info;
	GetSystemInfo (&info);
	return info.dwNumberOfProcessors;
}

std::string Environment::GetOsVersionString()
{
	OSVERSIONINFO verinfo;

	verinfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	if (GetVersionEx (&verinfo)) {
		char version [64];
		/* maximum string length is 35 bytes
		   3 x 10 bytes per number, 1 byte for 0, 3 x 1 byte for dots, 1 for NULL */
		sprintf (version, "%ld.%ld.%ld.0",
			verinfo.dwMajorVersion,
			verinfo.dwMinorVersion,
			verinfo.dwBuildNumber);
		return version;
	}

	return "0.0.0.0";
}

std::string Environment::GetOsUserName()
{
	WCHAR user_name[256+1];
	DWORD user_name_size = ARRAYSIZE(user_name);
	if (GetUserNameW(user_name, &user_name_size))
		return utils::StringUtils::Utf16ToUtf8((const uint16_t*)user_name);

	return "Unknown";
}

std::string Environment::GetEnvironmentVariable(const std::string& name)
{
	WCHAR buffer[BUFFER_SIZE];

	const UTF16String varName = utils::StringUtils::Utf8ToUtf16(name.c_str());

	DWORD ret = GetEnvironmentVariableW((LPWSTR)varName.c_str(), buffer, BUFFER_SIZE);

	if(ret == 0) // Not found
		return std::string();

	if(ret < BUFFER_SIZE) // Found and fits into buffer
		return utils::StringUtils::Utf16ToUtf8((uint16_t*)buffer);

	// Requires bigger buffer
	assert(ret >= BUFFER_SIZE);

	WCHAR *bigbuffer = new WCHAR[ret+1];

	ret = GetEnvironmentVariableW((LPWSTR)varName.c_str(), bigbuffer, ret+1);
	assert(ret != 0);

	std::string variableValue(utils::StringUtils::Utf16ToUtf8((uint16_t*)bigbuffer));

	delete bigbuffer;

	return variableValue;
}

void Environment::SetEnvironmentVariable(const std::string& name, const std::string& value)
{
	const UTF16String varName = utils::StringUtils::Utf8ToUtf16(name.c_str());
	const UTF16String varValue = utils::StringUtils::Utf8ToUtf16(value.c_str());

	SetEnvironmentVariableW((LPWSTR)varName.c_str(), (LPWSTR)varValue.c_str());
}

std::vector<std::string> Environment::GetEnvironmentVariableNames ()
{
	NOT_IMPLEMENTED_ICALL (Environment::GetEnvironmentVariableNames);
	return std::vector<std::string>();
}

std::string Environment::GetHomeDirectory ()
{
	NOT_IMPLEMENTED_ICALL (Environment::GetHomeDirectory);
	return std::string();
}

std::vector<std::string> Environment::GetLogicalDrives ()
{
	NOT_IMPLEMENTED_ICALL (Environment::GetLogicalDrives);
	return std::vector<std::string>();
}

void Environment::Exit (int result)
{
	NOT_IMPLEMENTED_ICALL (Environment::Exit);
}

#if !IL2CPP_TARGET_WINRT && !IL2CPP_TARGET_XBOXONE

std::string Environment::GetWindowsFolderPath(int32_t folder)
{
	WCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathW(NULL, folder | CSIDL_FLAG_CREATE, NULL, 0, path)))
		return utils::StringUtils::Utf16ToUtf8((uint16_t*)path);

	return std::string();
}

#endif

}

}
#endif
