#pragma once

#include <stdint.h>

struct Il2CppArray;
struct Il2CppString;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

class Convert
{
public:
	static Il2CppArray* InternalFromBase64CharArray (Il2CppArray* arr, int32_t offset, int32_t length);
	static Il2CppArray* InternalFromBase64String (Il2CppString* str, bool allowWhitespaceOnly);
	static Il2CppArray* Base64ToByteArray (uint16_t* start, int length, bool allowWhitespaceOnly);
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
