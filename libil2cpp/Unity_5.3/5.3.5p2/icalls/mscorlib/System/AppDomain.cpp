#include "il2cpp-config.h"

#include "icalls/mscorlib/System/AppDomain.h"

#include "utils/StringUtils.h"

#include "vm/Array.h"
#include "vm/Assembly.h"
#include "vm/Class.h"
#include "vm/Domain.h"
#include "vm/Exception.h"
#include "vm/Image.h"
#include "vm/Object.h"
#include "vm/Reflection.h"
#include "vm/Thread.h"
#include "vm/Type.h"

#include "tabledefs.h"
#include "class-internals.h"
#include "object-internals.h"
#include "il2cpp-api.h"

#include <cassert>
#include <map>
#include <string>

using namespace il2cpp::vm;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

Il2CppAppDomain* AppDomain::getCurDomain ()
{
	Il2CppDomain *add = Domain::GetCurrent ();

	if (add->domain)
		return add->domain;

	add->domain = (Il2CppAppDomain*)il2cpp::vm::Object::New (il2cpp_defaults.appdomain_class);
	return add->domain;
}

Il2CppAppDomain* AppDomain::getRootDomain ()
{
	return Domain::GetRoot ()->domain;
}

Il2CppAppDomain* AppDomain::createDomain(Il2CppString*, mscorlib_System_AppDomainSetup*)
{
	NOT_SUPPORTED_IL2CPP(AppDomain::createDomain, "IL2CPP only supports one app domain, others cannot be created.");
	return NULL;
}

Il2CppArray* AppDomain::GetAssemblies(Il2CppAppDomain* ad, bool refonly)
{
	static Il2CppClass *System_Reflection_Assembly;

	if (!System_Reflection_Assembly)
		System_Reflection_Assembly = Class::FromName (
			il2cpp_defaults.corlib, "System.Reflection", "Assembly");

	vm::AssemblyVector* assemblies = Assembly::GetAllAssemblies();
	
	int c = 0;
	Il2CppArray *res = Array::New (System_Reflection_Assembly, (il2cpp_array_size_t)assemblies->size());
	for (vm::AssemblyVector::const_iterator assembly = assemblies->begin(); assembly != assemblies->end(); ++assembly)
		il2cpp_array_setref(res, c++, Reflection::GetAssemblyObject(*assembly));
	
	return res;
}

Il2CppString *  AppDomain::getFriendlyName(Il2CppAppDomain* ad)
{
	return il2cpp_string_new(ad->data->friendly_name);
}

Il2CppObject* AppDomain::getSetup (Il2CppAppDomain* domain)
{
	assert (domain != NULL);
	assert (domain->data != NULL);

	return domain->data->setup;
}

Il2CppAppContext* AppDomain::InternalGetDefaultContext (void)
{
	return vm::Domain::ContextGet();
}

Il2CppAppContext* AppDomain::InternalGetContext (void)
{
	return vm::Domain::ContextGet();
}

Il2CppString* AppDomain::InternalGetProcessGuid (Il2CppString* newguid)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::InternalGetProcessGuid, "This icall is only used in System.Runtime.Remoting.RemotingConfiguraiton.ProcessId.");

	return NULL;
}

void AppDomain::InternalPopDomainRef ()
{
	NOT_SUPPORTED_IL2CPP (AppDomain::InternalPopDomainRef, "This icall is only used in the System.Runtime.Remoting namespace.");
}

void AppDomain::InternalPushDomainRef (mscorlib_System_AppDomain * domain)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::InternalPushDomainRef, "This icall is only used in the System.Runtime.Remoting namespace.");
}

void AppDomain::InternalPushDomainRefByID (int32_t domain_id)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::InternalPushDomainRefByID, "This icall is only used in the System.Runtime.Remoting namespace.");
}

mscorlib_System_Runtime_Remoting_Contexts_Context * AppDomain::InternalSetContext (mscorlib_System_Runtime_Remoting_Contexts_Context * context)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::InternalSetContext, "This icall is only used in the System.Runtime.Remoting namespace.");

	return 0;
}
mscorlib_System_AppDomain * AppDomain::InternalSetDomain (mscorlib_System_AppDomain * context)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::InternalSetDomain, "This icall is only used in the System.Runtime.Remoting namespace.");

	return 0;
}

mscorlib_System_AppDomain * AppDomain::InternalSetDomainByID (int32_t domain_id)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::InternalSetDomainByID, "This icall is only used in the System.Runtime.Remoting namespace.");

	return 0;
}

Il2CppReflectionAssembly* AppDomain::LoadAssembly(Il2CppAppDomain* ad, Il2CppString* assemblyRef, struct mscorlib_System_Security_Policy_Evidence* evidence, bool refOnly)
{
	il2cpp::vm::TypeNameParseInfo info;
	std::string name = il2cpp::utils::StringUtils::Utf16ToUtf8(assemblyRef->chars);
	il2cpp::vm::TypeNameParser parser(name, info, false);

	if (!parser.ParseAssembly())
		return NULL;
	
	//assemblyRef might have been a fullname like System, CultureInfo=bla, Version=4.0, PublicKeyToken=23423423423423
	//we ignore all that info except the name. (mono ignores the keytoken, and I'm quite sure it also ignores the version).
	//il2cpp does not pack multiple assemblies with the same name, and even if that one is not the exact one that is asked for,
	//it's more useful to return it than not to. (like cases where you want to Deserialize a BinaryFormatter blob that was serialized
	//on 4.0)
	const Il2CppAssembly* assembly = vm::Assembly::GetLoadedAssembly(info.assembly_name().name.c_str());
	
	if (assembly != NULL)
		return  Reflection::GetAssemblyObject(assembly);

	return NULL;
}

int32_t AppDomain::ExecuteAssembly (Il2CppAppDomain* self, Il2CppAssembly* a, Il2CppArray* args)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::ExecuteAssembly, "This icall is not supported by il2cpp.");
	
	return 0;
}

Il2CppObject* AppDomain::GetData (Il2CppAppDomain* self, Il2CppString* name)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::GetData, "This icall is not supported by il2cpp.");
	
	return 0;
}

Il2CppAssembly* AppDomain::LoadAssemblyRaw (Il2CppAppDomain* self, Il2CppArray* rawAssembly, Il2CppArray* rawSymbolStore, void* /* System.Security.Policy.Evidence */ securityEvidence, bool refonly)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::LoadAssemblyRaw, "This icall is not supported by il2cpp.");
	
	return 0;
}

bool AppDomain::InternalIsFinalizingForUnload (int32_t domain_id)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::InternalIsFinalizingForUnload, "This icall is not supported by il2cpp.");
	
	return false;
}

void AppDomain::InternalUnload (int32_t domain_id)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::InternalUnload, "This icall is not supported by il2cpp.");
}

void AppDomain::SetData (Il2CppAppDomain* self, Il2CppString* name, Il2CppObject* data)
{
	NOT_SUPPORTED_IL2CPP (AppDomain::SetData, "This icall is not supported by il2cpp.");
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
