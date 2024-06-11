#pragma once
#include "il2cpp-string-types.h"
#include <string>
#include <vector>
#include <stdint.h>
#include "il2cpp-config.h"

namespace il2cpp
{
namespace utils
{

class LIBIL2CPP_CODEGEN_API StringUtils
{
public:
	static std::string Printf (const char* format, ...);
	static std::string NPrintf (const char* format, size_t max_n, ...);
	static std::string Utf16ToUtf8 (const Il2CppChar* utf16String);
	static std::string Utf16ToUtf8(const Il2CppChar* utf16String, int maximumSize);
	static UTF16String Utf8ToUtf16(const char* utf8String);
	static UTF16String Utf8ToUtf16 (const char* utf8String, size_t length);
	static char* StringDuplicate (const char *strSource);
	static Il2CppChar* StringDuplicate (const Il2CppChar* strSource, size_t length);
	static bool EndsWith(const std::string& string, const std::string& suffix);
	static Il2CppChar Utf16ToLower(Il2CppChar c);
	static bool CaseSensitiveEquals(Il2CppString* left, const char* right);
	static bool CaseSensitiveEquals(const char* left, const char* right);
	static bool CaseInsensitiveEquals(Il2CppString* left, const char* right);
	static bool CaseInsensitiveEquals(const char* left, const char* right);

#if IL2CPP_PLATFORM_WIN32
	static inline std::string NativeStringToUtf8(const Il2CppNativeString& nativeStr)
	{
		assert(nativeStr.length() < static_cast<size_t>(std::numeric_limits<int>::max()));
		return Utf16ToUtf8(nativeStr.c_str(), static_cast<int>(nativeStr.length()));
	}
#else
	static inline std::string NativeStringToUtf8(Il2CppNativeString nativeStr)
	{
		return nativeStr;
	}
#endif

	template <typename CharType, size_t N>
	static inline size_t LiteralLength(const CharType (&str)[N])
	{
		return N - 1;
	}

	template <typename CharType>
	static size_t StrLen(const CharType* str)
	{
		size_t length = 0;
		while (*str)
		{
			str++;
			length++;
		}

		return length;
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

	// Taken from github.com/Microsoft/referencesource/blob/master/mscorlib/system/string.cs
	template <typename CharType>
	static inline size_t Hash (const CharType *str, size_t length)
	{
		assert(length <= static_cast<size_t>(std::numeric_limits<int>::max()));

		size_t hash1 = 5381;
		size_t hash2 = hash1;
		size_t i = 0;

		CharType c;
		const CharType* s = str;
		while (true)
		{
			if (i++ >= length)
				break;
			c = s[0];
			hash1 = ((hash1 << 5) + hash1) ^ c;
			if (i++ >= length)
				break;
			c = s[1];
			hash2 = ((hash2 << 5) + hash2) ^ c;
			s += 2;
		}

		return hash1 + (hash2 * 1566083941);
	}	
	
	template <typename CharType>
	static inline size_t Hash (const CharType *str)
	{
		size_t hash1 = 5381;
		size_t hash2 = hash1;

		CharType c;
		const CharType* s = str;
		while ((c = s[0]) != 0)
		{
			hash1 = ((hash1 << 5) + hash1) ^ c;
			c = s[1];
			if (c == 0)
				break;
			hash2 = ((hash2 << 5) + hash2) ^ c;
			s += 2;
		}

		return hash1 + (hash2 * 1566083941);
	}
	
	template <typename StringType>
	struct StringHasher
	{
		typedef typename StringType::value_type CharType;

		size_t operator()(const StringType& value) const
		{
			return Hash(value.c_str(), value.length());
		}
	};
};


} /* utils */
} /* il2cpp */
