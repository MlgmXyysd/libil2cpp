#pragma once

#include <stdint.h>

struct Il2CppArray;
struct Il2CppString;

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace Net
{

class Dns
{
public:
	static bool GetHostByAddr (Il2CppString*, Il2CppString**, Il2CppArray**, Il2CppArray**);
	static bool GetHostByName (Il2CppString*, Il2CppString**, Il2CppArray**, Il2CppArray**);
	static bool GetHostName (Il2CppString**);
};

} /* namespace Net */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
