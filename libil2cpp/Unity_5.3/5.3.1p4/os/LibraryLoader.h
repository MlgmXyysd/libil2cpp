#pragma once

#include "il2cpp-config.h"
#include <string>

struct PInvokeArguments;

namespace il2cpp
{
namespace os
{

class LibraryLoader
{
public:
	static void* LoadDynamicLibrary(const std::string& nativeDynamicLibrary);
	static methodPointerType GetFunctionPointer(void* dynamicLibrary, const PInvokeArguments& pinvokeArgs);
	static void CleanupLoadedLibraries();
};

} /* namespace os */
} /* namespace il2cpp*/
