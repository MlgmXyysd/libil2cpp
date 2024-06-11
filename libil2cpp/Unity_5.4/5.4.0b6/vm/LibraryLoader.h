#pragma once

#include "il2cpp-config.h"
#include "il2cpp-api-types.h"
#include <string>

struct PInvokeArguments;

namespace il2cpp
{
namespace vm
{

class LibraryLoader
{
public:
	static void* LoadLibrary(const std::string& nativeDynamicLibrary);
	static void SetFindPluginCallback(Il2CppSetFindPlugInCallback method);
};

} /* namespace vm */
} /* namespace il2cpp*/

