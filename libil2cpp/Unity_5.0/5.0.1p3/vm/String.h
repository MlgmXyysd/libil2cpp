#pragma once

#include <stdint.h>

struct Il2CppString;

namespace il2cpp
{
namespace vm
{

class String
{
public:
	//exported
	static int32_t GetLength (Il2CppString* str);
	static uint16_t* GetChars (Il2CppString* str);
	static int32_t GetHash (Il2CppString* str);
	static Il2CppString* New (const char* str);
	static Il2CppString* NewWrapper (const char* str);
	static Il2CppString* NewLen (const char* str, uint32_t length);
	static Il2CppString* NewSize (int32_t len);
	static Il2CppString* NewUtf16 (const uint16_t *text, int32_t len);
public:
	// internal
	static Il2CppString* Load (const uint16_t* chars, uint32_t length);
	static Il2CppString* Intern(Il2CppString* str);
	static Il2CppString* IsInterned(Il2CppString* str);
};

} /* namespace vm */
} /* namespace il2cpp */