#pragma once

#include "object-internals.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Runtime
{
namespace CompilerServices
{

class RuntimeHelpers
{
public:
	static Il2CppObject* GetObjectValue (Il2CppObject* obj);
	static void RunClassConstructor (Il2CppIntPtr type);
	static void RunModuleConstructor (Il2CppIntPtr module);
	static int get_OffsetToStringData (void);
	static void InitializeArray (Il2CppArray* arr,Il2CppIntPtr ptr);
};

} /* namespace CompilerServices */
} /* namespace Runtime */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
