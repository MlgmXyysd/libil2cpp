#include "il2cpp-config.h"

#if IL2CPP_USE_GENERIC_ENVIRONMENT
#include "os/Environment.h"

#include <cstdlib>

namespace il2cpp
{
namespace os
{

std::string Environment::GetMachineName()
{
	return "il2cpp";
}

int32_t Environment::GetProcessorCount()
{
	return 1;
}

std::string Environment::GetOsVersionString()
{
	return "0.0.0.0";
}

std::string Environment::GetOsUserName()
{
	return "Unknown";
}

std::string Environment::GetEnvironmentVariable(const std::string& name)
{
	return std::string("<NotImplemented>");
}

void Environment::SetEnvironmentVariable(const std::string& name, const std::string& value)
{
}

std::vector<std::string> Environment::GetEnvironmentVariableNames ()
{
	NOT_IMPLEMENTED_ICALL (Environment::GetEnvironmentVariableNames);
	return std::vector<std::string>();
}

std::string Environment::GetHomeDirectory ()
{
	NOT_IMPLEMENTED_ICALL (Environment::GetHomeDirectory);
	return std::string();
}

std::vector<std::string> Environment::GetLogicalDrives ()
{
	NOT_IMPLEMENTED_ICALL (Environment::GetLogicalDrives);
	return std::vector<std::string>();
}

void Environment::Exit (int result)
{
	NOT_IMPLEMENTED_ICALL (Environment::Exit);
}

NORETURN void Environment::Abort()
{
	abort();
}

#if NET_4_0

bool Environment::Is64BitOs()
{
	return false;
}

#endif

}
}
#endif
