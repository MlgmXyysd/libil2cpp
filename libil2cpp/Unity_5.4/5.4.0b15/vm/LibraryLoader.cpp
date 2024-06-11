#include "LibraryLoader.h"
#include "os/LibraryLoader.h"
#include <cassert>

namespace il2cpp
{
namespace vm
{

static Il2CppSetFindPlugInCallback s_FindPluginCallback = NULL;

void* LibraryLoader::LoadLibrary(const std::string& nativeDynamicLibrary)
{
	if (s_FindPluginCallback)
	{
		const char* modifiedNativeDynamicLibrary = s_FindPluginCallback(nativeDynamicLibrary.c_str());
		return os::LibraryLoader::LoadDynamicLibrary(std::string(modifiedNativeDynamicLibrary));
	}

	return os::LibraryLoader::LoadDynamicLibrary(nativeDynamicLibrary);
}

void LibraryLoader::SetFindPluginCallback(Il2CppSetFindPlugInCallback method)
{
	assert(method == NULL || s_FindPluginCallback == NULL);
	s_FindPluginCallback = method;
}

} /* namespace vm */
} /* namespace il2cpp */
