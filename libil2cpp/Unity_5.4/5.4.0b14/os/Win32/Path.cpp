#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE

#include "WindowsHeaders.h"
#undef GetTempPath

#include "os/Environment.h"
#include "os/Path.h"
#include "utils/StringUtils.h"
#include "WindowsHelpers.h"
#include <string>

namespace il2cpp
{
namespace os
{

std::string Path::GetExecutablePath()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileNameW(NULL, buffer, MAX_PATH);
	return utils::StringUtils::Utf16ToUtf8(reinterpret_cast<const uint16_t*>(buffer));
}

std::string Path::GetTempPath()
{
	WCHAR tempPath[MAX_PATH + 1];
	::GetTempPathW(sizeof(tempPath) / sizeof(tempPath[0]), tempPath);
	::GetLongPathNameW(tempPath, tempPath, sizeof(tempPath) / sizeof(tempPath[0]));

	return utils::StringUtils::Utf16ToUtf8((uint16_t*)tempPath);
}

}
}
#endif
