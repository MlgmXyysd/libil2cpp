#pragma once

#include <stdint.h>
#include "il2cpp-metadata.h"

struct Il2CppGenericContainer;
struct Il2CppGenericParameter;

namespace il2cpp
{
namespace vm
{

class GenericContainer
{
public:
	// exported

public:
	//internal
	static const Il2CppGenericParameter* GetGenericParameter (const Il2CppGenericContainer* genericContainer, uint16_t index);
};

} /* namespace vm */
} /* namespace il2cpp */
