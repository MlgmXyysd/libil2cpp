#include "il2cpp-config.h"
#include "vm/InternalCalls.h"
#include "vm/Runtime.h"

#include <cassert>
#include <map>
#include <string>

typedef std::map<std::string, methodPointerType> ICallMap;
static ICallMap s_InternalCalls;

namespace il2cpp
{
namespace vm
{

void InternalCalls::Add (const char* name, methodPointerType method)
{
	//ICallMap::iterator res = s_InternalCalls.find(name);

	// TODO: Don't assert right now because Unity adds some icalls multiple times.
	//if (res != icalls.end())
	//	assert(0 && "Adding internal call twice!");

	assert(method);

	s_InternalCalls[name] = method;
}

methodPointerType InternalCalls::Resolve (const char* name)
{
	// Try to find the whole name first, then search using just type::method
	// if parameters were passed
	// ex: First, System.Foo::Bar(System.Int32)
	// Then, System.Foo::Bar
	ICallMap::iterator res = s_InternalCalls.find(name);

	if (res != s_InternalCalls.end())
		return res->second;

	std::string shortName(name);
	size_t index = shortName.find('(');

	if (index != std::string::npos)
	{
		shortName = shortName.substr(0, index);
		res = s_InternalCalls.find(shortName);

		if (res != s_InternalCalls.end())
			return res->second;
	}
	
	assert(0);

	return NULL;
}

} /* namespace vm */
} /* namespace il2cpp */
