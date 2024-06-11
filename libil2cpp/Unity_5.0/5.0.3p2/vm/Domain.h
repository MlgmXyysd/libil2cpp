#pragma once

#include <stdint.h>

struct Il2CppDomain;

namespace il2cpp
{
namespace vm
{

class Domain
{
public:
	static Il2CppDomain* GetCurrent ();
	static Il2CppDomain* GetRoot ();

private:
	static Il2CppDomain* S_domain;
};

} /* namespace vm */
} /* namespace il2cpp */
