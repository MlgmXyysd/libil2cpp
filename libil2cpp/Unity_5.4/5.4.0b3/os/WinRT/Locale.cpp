#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINRT

#include "os/Locale.h"
#include <windows.h>
#include <vector>

namespace il2cpp
{
namespace os
{

std::string Locale::GetLocale()
{
	WCHAR wideLocaleName[LOCALE_NAME_MAX_LENGTH];
	if (GetUserDefaultLocaleName(wideLocaleName, LOCALE_NAME_MAX_LENGTH) == 0)
		return std::string();

	int length = wcslen(wideLocaleName);
	std::string multiLocaleName;
	multiLocaleName.resize(2 * length);
	int narrowLength = WideCharToMultiByte(CP_ACP, 0, &wideLocaleName[0], length, &multiLocaleName[0], 2 * length, NULL, NULL);
	multiLocaleName.resize(narrowLength);
	return multiLocaleName;;
}

} /* namespace os */
} /* namespace il2cpp */

#endif