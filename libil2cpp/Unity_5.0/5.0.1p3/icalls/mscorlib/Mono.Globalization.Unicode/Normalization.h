#pragma once

#include <stdint.h>
#include "object-internals.h"

struct Il2CppObject;
struct Il2CppDelegate;
struct Il2CppReflectionType;
struct Il2CppReflectionMethod;
struct Il2CppReflectionField;
struct Il2CppArray;
struct Il2CppException;
struct Il2CppReflectionModule;
struct Il2CppAssembly;
struct Il2CppAssemblyName;
struct Il2CppAppDomain;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace Mono
{
namespace Globalization
{
namespace Unicode
{

class Normalization
{
public:
	static void load_normalization_resource (Il2CppIntPtr* props, Il2CppIntPtr* mappedChars, Il2CppIntPtr* charMapIndex, Il2CppIntPtr* helperIndex, Il2CppIntPtr* mapIdxToComposite, Il2CppIntPtr* combiningClass);
};

} /* namespace Unicode */
} /* namespace Globalization */
} /* namespace Mono */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
