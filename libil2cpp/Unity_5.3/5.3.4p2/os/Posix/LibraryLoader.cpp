#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX

//#define VERBOSE_OUTPUT

#ifdef VERBOSE_OUTPUT
#include <stdio.h>
#endif

#include <dlfcn.h>
#include <string>
#include <set>
#include <cassert>

#include "metadata.h"
#include "os/LibraryLoader.h"
#include "vm/Exception.h"

namespace il2cpp
{
namespace os
{

static std::set<void*> s_NativeHandlesOpen;
typedef std::set<void*>::const_iterator OpenHandleIterator;

struct LibraryNamePrefixAndSuffix
{
	LibraryNamePrefixAndSuffix(const char* prefix_, const char* suffix_)
	{
		prefix = std::string(prefix_);
		suffix = std::string(suffix_);
	}

	std::string prefix;
	std::string suffix;
};

static LibraryNamePrefixAndSuffix LibraryNamePrefixAndSuffixVariations[8] = {
	LibraryNamePrefixAndSuffix("", ".so"),
	LibraryNamePrefixAndSuffix("", ".dll"),
	LibraryNamePrefixAndSuffix("", ".dylib"),
	LibraryNamePrefixAndSuffix("", ".bundle"),
	LibraryNamePrefixAndSuffix("lib", ".so"),
	LibraryNamePrefixAndSuffix("lib", ".dll"),
	LibraryNamePrefixAndSuffix("lib", ".dylib"),
	LibraryNamePrefixAndSuffix("lib", ".bundle")
};

// Note that testing this code can be a bit difficult, since dlopen will cache
// the values it returns, and we don't call dlcose from the C# level. See the
// comments in the integration test code for more details.

static void* LoadLibraryWithName(const std::string& name)
{
#ifdef VERBOSE_OUTPUT
	printf("Trying name: %s\n", name.c_str());
#endif
	void* handle = dlopen(name.c_str(), RTLD_LAZY);
	if (handle != NULL)
		return handle;

#ifdef VERBOSE_OUTPUT
	printf("Error: %s\n", dlerror());
#endif

	return NULL;
}

static void* CheckLibraryVariations(const std::string& name)
{
	int numberOfVariations = sizeof(LibraryNamePrefixAndSuffixVariations) / sizeof(LibraryNamePrefixAndSuffixVariations[0]);
	for (int i = 0; i < numberOfVariations; ++i)
	{
		std::string libraryName = LibraryNamePrefixAndSuffixVariations[i].prefix + name + LibraryNamePrefixAndSuffixVariations[i].suffix;
		void* handle = LoadLibraryWithName(libraryName);
		if (handle != NULL)
			return handle;
	}

	return NULL;
}

void* LibraryLoader::LoadDynamicLibrary(const std::string& nativeDynamicLibrary)
{
#ifdef VERBOSE_OUTPUT
	printf("Attempting to load dynamic library: %s\n", nativeDynamicLibrary.c_str());
#endif

	void* handle = LoadLibraryWithName(nativeDynamicLibrary);

	if (handle == NULL)
		handle = CheckLibraryVariations(nativeDynamicLibrary);

	if (handle == NULL)
	{
		size_t lengthWithoutDotDll = nativeDynamicLibrary.length() - 4;
		if (nativeDynamicLibrary.compare(lengthWithoutDotDll, 4, ".dll") == 0)
			handle = CheckLibraryVariations(nativeDynamicLibrary.substr(0, lengthWithoutDotDll));
	}

	if (handle != NULL)
		s_NativeHandlesOpen.insert(handle);

	return handle;
}

methodPointerType LibraryLoader::GetFunctionPointer(void* dynamicLibrary, const PInvokeArguments& pinvokeArgs)
{
#ifdef VERBOSE_OUTPUT
	printf("Attempting to load method at entry point: %s\n", pinvokeArgs.entryPoint);
#endif
	void* method = dlsym(dynamicLibrary, pinvokeArgs.entryPoint);
	
#ifdef VERBOSE_OUTPUT
	if (method == NULL)
		printf("Error: %s\n", dlerror());
#endif

	return (methodPointerType)method;
}

void LibraryLoader::CleanupLoadedLibraries()
{
	for (OpenHandleIterator it = s_NativeHandlesOpen.begin(); it != s_NativeHandlesOpen.end(); it++)
	{
		dlclose(*it);
	}
}

}
}

#endif
