#pragma once

#include "il2cpp-config.h"

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API InternalCalls
{
public:
	static void Init ();
	static void Add (const char* name, methodPointerType method);
	static methodPointerType Resolve (const char* name);
};

} /* namespace vm */
} /* namespace il2cpp */
