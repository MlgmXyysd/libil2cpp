#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX
#include "os/Environment.h"
#include "os/Path.h"
#include <string>

#if defined(__APPLE__)
#include "mach-o/dyld.h"
#endif

namespace il2cpp
{
namespace os
{

std::string Path::GetExecutablePath()
{
#if defined(__APPLE__)
	char path[1024];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) == 0)
		return path;

	std::string result;
	result.resize(size + 1);
	_NSGetExecutablePath(&result[0], &size);
	return result;
#else
	return std::string();
#endif
}

std::string Path::GetTempPath()
{
	static const char* tmpdirs[] = { "TMPDIR", "TMP", "TEMP", NULL};

	for(size_t i = 0; tmpdirs[i] != NULL; ++i)
	{
		std::string tmpdir = Environment::GetEnvironmentVariable(tmpdirs[i]);

		if(!tmpdir.empty())
			return tmpdir;
	}

	return std::string("/tmp");
}

}
}

#endif