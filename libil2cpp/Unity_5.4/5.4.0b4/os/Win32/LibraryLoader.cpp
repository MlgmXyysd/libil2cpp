#include "il2cpp-config.h"

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE

#include "metadata.h"
#include "os/LibraryLoader.h"
#include "utils/StringUtils.h"

#include <sstream>
#include <unordered_map>

#include "WindowsHelpers.h"

namespace il2cpp
{
namespace os
{

static std::unordered_map<std::string, HMODULE> s_NativeDllCache;
typedef std::unordered_map<std::string, HMODULE>::const_iterator DllCacheIterator;

void* LibraryLoader::LoadDynamicLibrary(const std::string& nativeDynamicLibrary)
{
	DllCacheIterator nativeDll = s_NativeDllCache.find(nativeDynamicLibrary);

	if (nativeDll != s_NativeDllCache.end())
		return nativeDll->second;

	const UTF16String dynamicLibraryWide = il2cpp::utils::StringUtils::Utf8ToUtf16(nativeDynamicLibrary.c_str(), nativeDynamicLibrary.length());
	HMODULE module = ::LoadLibraryW((LPWSTR)dynamicLibraryWide.c_str());
	
	if (module != NULL)
		s_NativeDllCache.emplace(make_pair(nativeDynamicLibrary, module));

	return module;
}

methodPointerType LibraryLoader::GetFunctionPointer(void* dynamicLibrary, const PInvokeArguments& pinvokeArgs)
{
	if (dynamicLibrary == NULL)
		return NULL;

	HMODULE module = (HMODULE)dynamicLibrary;

	// If there's 'no mangle' flag set, just return directly what GetProcAddress returns
	if (pinvokeArgs.isNoMangle)
		return reinterpret_cast<methodPointerType>(GetProcAddress(module, pinvokeArgs.entryPoint));

	const size_t kBufferOverhead = 10;
	FARPROC functionPtr = NULL;
	size_t originalFuncNameLength = strlen(pinvokeArgs.entryPoint) + 1;
	std::string functionName;

	functionName.resize(originalFuncNameLength + kBufferOverhead + 1);	// Let's index the string from '1', because we might have to prepend an underscore in case of stdcall mangling
	memcpy(&functionName[1], pinvokeArgs.entryPoint, originalFuncNameLength);
	ZeroMemory(&functionName[1] + originalFuncNameLength, kBufferOverhead);

	// If there's no 'dont mangle' flag set, 'W' function takes priority over original name, but 'A' function does not (yes, really)
	if (pinvokeArgs.charSet == CHARSET_UNICODE)
	{
		functionName[originalFuncNameLength] = 'W';
		functionPtr = GetProcAddress(module, functionName.c_str() + 1);
		if (functionPtr != NULL)
			return reinterpret_cast<methodPointerType>(functionPtr);

		// If charset specific function lookup failed, try with original name
		functionPtr = GetProcAddress(module, pinvokeArgs.entryPoint);
	}
	else
	{
		functionPtr = GetProcAddress(module, pinvokeArgs.entryPoint);
		if (functionPtr != NULL)
			return reinterpret_cast<methodPointerType>(functionPtr);

		// If original name function lookup failed, try with mangled name
		functionName[originalFuncNameLength] = 'A';
		functionPtr = GetProcAddress(module, functionName.c_str() + 1);
	}

	if (functionPtr != NULL)
		return reinterpret_cast<methodPointerType>(functionPtr);

	// If it's not cdecl, try mangling the name
	// THIS ONLY APPLIES TO 32-bit x86!
#if !defined(__arm__)
	if (sizeof(void*) == 4 && pinvokeArgs.callingConvention != IL2CPP_CALL_C)
	{
		functionName[0] = '_';
		sprintf(&functionName[0] + originalFuncNameLength, "@%ld", pinvokeArgs.parameterSize);

		functionPtr = GetProcAddress(module, functionName.c_str());
	}
#endif

	return reinterpret_cast<methodPointerType>(functionPtr);
}

void LibraryLoader::CleanupLoadedLibraries()
{
	for (DllCacheIterator it = s_NativeDllCache.begin(); it != s_NativeDllCache.end(); it++)
	{
		FreeLibrary(it->second);
	}
}

}
}

#endif
