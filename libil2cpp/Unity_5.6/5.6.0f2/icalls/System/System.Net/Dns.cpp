#include "il2cpp-config.h"

#include "icalls/System/System.Net/Dns.h"

#include "class-internals.h"
#include "object-internals.h"
#include "vm/Array.h"
#include "vm/String.h"
#include "os/Socket.h"
#include "os/WaitStatus.h"
#include "utils/StringUtils.h"

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace Net
{

bool Dns::GetHostByAddr (Il2CppString* addr, Il2CppString** h_name, Il2CppArray** h_aliases, Il2CppArray** h_addr_list)
{
	std::string name;
	std::vector<std::string> aliases;
	std::vector<std::string> addr_list;
	
	const std::string address = il2cpp::utils::StringUtils::Utf16ToUtf8 (addr->chars);
	const os::WaitStatus result = os::Socket::GetHostByAddr (address, name, aliases, addr_list);
	
	*h_name = vm::String::New (name.c_str ());
	*h_aliases = vm::Array::New (il2cpp_defaults.string_class, (il2cpp_array_size_t)aliases.size ());
	*h_addr_list = vm::Array::New (il2cpp_defaults.string_class, (il2cpp_array_size_t)addr_list.size ());
	
	int32_t index = 0;
	
	std::vector<std::string>::const_iterator it = aliases.begin ();
	while (it != aliases.end ())
	{
		il2cpp_array_setref(*h_aliases, index++, vm::String::New ((*it).c_str ()));
		++it;
	}
	
	index = 0;
	
	it = addr_list.begin ();
	while (it != addr_list.end ())
	{
		il2cpp_array_setref(*h_addr_list, index++, vm::String::New ((*it).c_str ()));
		++it;
	}
	
	return (result != os::kWaitStatusFailure);
}

bool Dns::GetHostByName (Il2CppString* host, Il2CppString** h_name, Il2CppArray** h_aliases, Il2CppArray** h_addr_list)
{
	std::string name;
	std::vector<std::string> aliases;
	std::vector<std::string> addr_list;
	
	const std::string host_name = il2cpp::utils::StringUtils::Utf16ToUtf8 (host->chars);
	const os::WaitStatus result = os::Socket::GetHostByName (host_name, name, aliases, addr_list);
	
	*h_name = vm::String::New (name.c_str ());
	*h_aliases = vm::Array::New (il2cpp_defaults.string_class, (il2cpp_array_size_t)aliases.size ());
	*h_addr_list = vm::Array::New (il2cpp_defaults.string_class, (il2cpp_array_size_t)addr_list.size ());
	
	int32_t index = 0;
	
	std::vector<std::string>::const_iterator it = aliases.begin ();
	while (it != aliases.end ())
	{
		il2cpp_array_setref(*h_aliases, index++, vm::String::New ((*it).c_str ()));
		++it;
	}
	
	index = 0;
	
	it = addr_list.begin ();
	while (it != addr_list.end ())
	{
		il2cpp_array_setref(*h_addr_list, index++, vm::String::New ((*it).c_str ()));
		++it;
	}
	
	return (result != os::kWaitStatusFailure);
}

bool Dns::GetHostName (Il2CppString** name)
{
	std::string host_name;
	
	*name = (os::Socket::GetHostName (host_name) != os::kWaitStatusFailure)
		? vm::String::New(host_name.c_str())
		: NULL;
	
	return *name != NULL;
}

} /* namespace Net */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
