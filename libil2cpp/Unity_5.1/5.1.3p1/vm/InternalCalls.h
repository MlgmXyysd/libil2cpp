#pragma once

namespace il2cpp
{
namespace vm
{

class InternalCalls
{
public:
	static void Init ();
	static void Add (const char* name, methodPointerType method);
	static methodPointerType Resolve (const char* name);
};

} /* namespace vm */
} /* namespace il2cpp */
