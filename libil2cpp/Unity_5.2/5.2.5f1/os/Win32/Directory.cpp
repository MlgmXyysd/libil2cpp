#include "il2cpp-config.h"

#if IL2CPP_PLATFORM_WIN32

#include "WindowsHeaders.h"

#include "os/Directory.h"
#include "os/ErrorCodes.h"
#include "utils/StringUtils.h"
#include "utils/PathUtils.h"

static inline int Win32ErrorToErrorCode (DWORD win32ErrorCode)
{
	return win32ErrorCode;
}

using namespace il2cpp::utils::PathUtils;

namespace il2cpp
{
namespace os
{

std::string Directory::GetCurrent (int *error)
{
	UTF16String buf;
	int len, res_len;

	len = MAX_PATH + 1;
	buf.resize (len, 0);

	*error = ERROR_SUCCESS;

	res_len = ::GetCurrentDirectory (len, (LPWSTR)buf.c_str());
	if (res_len > len) { /*buf is too small.*/
		int old_res_len = res_len;
		buf.resize (res_len, 0);
		res_len = ::GetCurrentDirectory (res_len, (LPWSTR)buf.c_str()) == old_res_len;
	}

	std::string directory;

	if (res_len) {
		len = 0;
		while (buf [len])
			++ len;

		directory = il2cpp::utils::StringUtils::Utf16ToUtf8 (buf.c_str());
	} else {
		*error = Win32ErrorToErrorCode (::GetLastError ());
	}

	return directory;
}

bool Directory::SetCurrent (const std::string& path, int* error)
{
	*error = kErrorCodeSuccess;

	const UTF16String utf16Path(il2cpp::utils::StringUtils::Utf8ToUtf16 (path.c_str ()));
	if (::SetCurrentDirectory ((LPWSTR)utf16Path.c_str ()))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());
	return false;
}

bool Directory::Create (const std::string& path, int *error)
{
	*error = kErrorCodeSuccess;

	const UTF16String utf16Path (il2cpp::utils::StringUtils::Utf8ToUtf16 (path.c_str ()));
	if (::CreateDirectory ((LPWSTR)utf16Path.c_str (), NULL))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());
	return false;
}

bool Directory::Remove (const std::string& path, int *error)
{
	*error = kErrorCodeSuccess;

	const UTF16String utf16Path (il2cpp::utils::StringUtils::Utf8ToUtf16 (path.c_str ()));
	if (::RemoveDirectory ((LPWSTR)utf16Path.c_str ()))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());
	return false;
}

std::set<std::string> Directory::GetFileSystemEntries (const std::string& path, const std::string& pathWithPattern, int32_t attrs, int32_t mask, int* error)
{
	*error = kErrorCodeSuccess;
	std::set<std::string> files;
	WIN32_FIND_DATA ffd;
	const UTF16String utf16Path (il2cpp::utils::StringUtils::Utf8ToUtf16 (pathWithPattern.c_str ()));

	HANDLE handle = ::FindFirstFileExW ((LPCWSTR)utf16Path.c_str (), FindExInfoStandard, &ffd, FindExSearchNameMatch, NULL, 0);
	if (INVALID_HANDLE_VALUE == handle)
	{
		// Following the Mono implementation, do not treat a directory with no files as an error.
		int errorCode = Win32ErrorToErrorCode(::GetLastError());
		if (errorCode != ERROR_FILE_NOT_FOUND)
			*error = errorCode;
		return files;
	}

	do
	{
		const std::string fileName (il2cpp::utils::StringUtils::Utf16ToUtf8 ((uint16_t*)ffd.cFileName));

		if ((fileName.length () == 1 && fileName.at (0) == '.') ||
			(fileName.length () == 2 && fileName.at (0) == '.' && fileName.at (1) == '.'))
			continue;

		if ((ffd.dwFileAttributes & mask) == attrs)
		{
			files.insert (Combine (path, fileName));
		}
	} while (::FindNextFile (handle, &ffd) != 0);

	::FindClose (handle);

	return files;
}

}
}

#endif
