#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include "il2cpp-config.h"
#include "il2cpp-api-types.h"
#include "il2cpp-metadata.h"

struct Il2CppArray;
struct Il2CppDelegate;
struct Il2CppObject;
struct MethodInfo;
struct Il2CppClass;

struct VirtualInvokeData
{
	void* target;
	const MethodInfo* methodInfo;
};

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API Runtime
{
public:
	static void Init (const char* filename, const char *runtime_version);
	static void Shutdown ();
	static void SetConfigDir (const char *path);
	static void SetDataDir(const char *path);
	static std::string GetConfigDir();
	static std::string GetDataDir();
	static const char *GetFrameworkVersion();
	static Il2CppObject* DelegateInvoke (Il2CppDelegate *obj, void **params, Il2CppException **exc);
	static Il2CppObject* Invoke (const MethodInfo *method, void *obj, void **params, Il2CppException **exc);
	static Il2CppObject* InvokeConvertArgs (const MethodInfo *method, void *obj, Il2CppObject **params, int paramCount, Il2CppException **exc);
	static Il2CppObject* InvokeArray (const MethodInfo *method, void *obj, Il2CppArray *params, Il2CppException **exc);
	static void ObjectInit (Il2CppObject* object);
	static void ObjectInitException (Il2CppObject* object, Il2CppException **exc);
	static void SetUnhandledExceptionPolicy (Il2CppRuntimeUnhandledExceptionPolicy value);

	static VirtualInvokeData GetVirtualInvokeData (Il2CppMethodSlot slot, void* obj);
	static VirtualInvokeData GetInterfaceInvokeData (Il2CppMethodSlot slot, Il2CppClass* declaringInterface, void* obj);
	static VirtualInvokeData GetComInterfaceInvokeData (Il2CppMethodSlot slot, Il2CppClass* declaringInterface, void* obj);
	static VirtualInvokeData GetGenericVirtualInvokeData (const MethodInfo* method, void* obj);
	static VirtualInvokeData GetGenericInterfaceInvokeData (const MethodInfo* method, void* obj);

	static void RaiseExecutionEngineExceptionIfMethodIsNotFound(const MethodInfo* method);

public:
	// internal
	static Il2CppRuntimeUnhandledExceptionPolicy GetUnhandledExceptionPolicy ();
	static void UnhandledException (Il2CppException* exc);
	static void ClassInit (Il2CppClass *klass);

#if IL2CPP_ENABLE_NATIVE_STACKTRACES
	struct MethodDefinitionKey
	{
		Il2CppMethodPointer method;
		MethodIndex methodIndex;
	};

	static void RegisterMethods (const std::vector<MethodDefinitionKey>& managedMethods);
	static const MethodInfo* GetMethodFromNativeSymbol (Il2CppMethodPointer nativeMethod);
#endif
	
	static const char *GetBundledMachineConfig ();
	static void RegisterBundledMachineConfig (const char *config_xml);

private:
	static void CallUnhandledExceptionDelegate (Il2CppDomain* domain, Il2CppDelegate* delegate, Il2CppException* exc);
	static Il2CppObject* CreateUnhandledExceptionEventArgs (Il2CppException* exc);
};

} /* namespace vm */
} /* namespace il2cpp */
