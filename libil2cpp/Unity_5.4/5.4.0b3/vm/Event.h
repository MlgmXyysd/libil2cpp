#pragma once

#include <stdint.h>

struct EventInfo;

namespace il2cpp
{
namespace vm
{

class Event
{
public:
	// exported
	static uint32_t GetToken (const EventInfo *eventInfo);
};

} /* namespace vm */
} /* namespace il2cpp */
