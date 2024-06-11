#pragma once

#include <stdint.h>

struct Il2CppImage;

namespace il2cpp
{
namespace vm
{

class Module
{
public:
	// exported
	static uint32_t GetToken (const Il2CppImage *image);
};

} /* namespace vm */
} /* namespace il2cpp */
