#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINDOWS && !IL2CPP_TARGET_WINRT

#include "os/Locale.h"
#include <vector>

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

namespace il2cpp
{
namespace os
{

std::string Locale::GetLocale()
{
	LCID lcid = GetThreadLocale();

	int number_of_characters = GetLocaleInfo(lcid, LOCALE_SNAME, NULL, 0);
	std::vector<WCHAR> locale_name(number_of_characters);
	if (GetLocaleInfo(lcid, LOCALE_SNAME, &locale_name[0], number_of_characters) == 0)
		return std::string();

	std::vector<char> locale_name_char(number_of_characters);
	if (WideCharToMultiByte(CP_ACP, 0, &locale_name[0], number_of_characters, &locale_name_char[0], number_of_characters, NULL, NULL) == 0)
		return std::string();

	return std::string(locale_name_char.begin(), locale_name_char.end());
}

} /* namespace os */
} /* namespace il2cpp */

#endif