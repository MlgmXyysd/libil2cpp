#pragma once
#include "types.h"
#include <string>
#include <vector>
#include <stdint.h>

namespace il2cpp
{
namespace utils
{

class StringUtils
{
public:
	static size_t Hash (const char *str);
	static size_t StrLenUtf16 (const uint16_t* str);
	static std::string Printf (const char* format, ...);
	static std::string NPrintf (const char* format, size_t max_n, ...);
	static std::string Utf16ToUtf8 (const uint16_t* utf16String);
	static std::string Utf16ToUtf8(const uint16_t* utf16String, int maximumSize);
	static UTF16String Utf8ToUtf16(const char* utf8String);
	static UTF16String Utf8ToUtf16 (const char* utf8String, size_t length);
	static char* StringDuplicate (const char *strSource);
	static Il2CppChar* StringDuplicate (const Il2CppChar* strSource, size_t length);
	static bool EndsWith(const std::string& string, const std::string& suffix);
	static uint16_t Utf16ToLower(uint16_t c);

	template <size_t N>
	static inline size_t LiteralLength(const char(&str)[N])
	{
		return N;
	}

	struct CaseSensitiveComparer
	{
		bool operator()(const std::string& left, const std::string& right) const;
		bool operator()(const std::string& left, const char* right) const;
		bool operator()(const char* left, const std::string& right) const;
		bool operator()(const char* left, const char* right) const;
	};

	struct CaseInsensitiveComparer
	{
		bool operator()(const std::string& left, const std::string& right) const;
		bool operator()(const std::string& left, const char* right) const;
		bool operator()(const char* left, const std::string& right) const;
		bool operator()(const char* left, const char* right) const;
	};
};


} /* utils */
} /* il2cpp */
