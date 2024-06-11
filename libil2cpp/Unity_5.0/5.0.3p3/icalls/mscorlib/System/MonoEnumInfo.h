#pragma once

struct Il2CppReflectionType;
struct Il2CppEnumInfo;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

class MonoEnumInfo
{
public:
	static void get_enum_info (Il2CppReflectionType* type, Il2CppEnumInfo* info);
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
