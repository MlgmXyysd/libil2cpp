#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include "il2cpp-api-types.h"
#include "il2cpp-metadata.h"

struct Il2CppArray;
struct Il2CppDelegate;
struct Il2CppObject;
struct MethodInfo;
struct TypeInfo;

struct VirtualInvokeData
{
	void* target;
	const MethodInfo* methodInfo;
};

namespace il2cpp
{
namespace vm
{

class Runtime
{
public:
	static void Init (const char* filename, const char *runtime_version);
	static void Shutdown ();
	static void SetConfigDir (const char *path);
	static void SetDataDir(const char *path);
	static std::string GetConfigDir();
	static std::string GetDataDir();
	static const char *GetFrameworkVersion();
	static Il2CppObject* DelegateInvoke (Il2CppDelegate *obj, void **params, Il2CppObject **exc);
	static Il2CppObject* Invoke (const MethodInfo *method, void *obj, void **params, Il2CppObject **exc);
	static Il2CppObject* InvokeConvertArgs (const MethodInfo *method, void *obj, Il2CppObject **params, int paramCount, Il2CppObject **exc);
	static Il2CppObject* InvokeArray (const MethodInfo *method, void *obj, Il2CppArray *params, Il2CppObject **exc);
	static void ObjectInit (Il2CppObject* object);
	static void ObjectInitException (Il2CppObject* object, Il2CppObject **exc);
	static void SetUnhandledExceptionPolicy (Il2CppRuntimeUnhandledExceptionPolicy value);

	static VirtualInvokeData GetVirtualInvokeData (Il2CppMethodSlot slot, void* obj);
	static VirtualInvokeData GetInterfaceInvokeData (Il2CppMethodSlot slot, TypeInfo* declaringInterface, void* obj);
	static VirtualInvokeData GetComInterfaceInvokeData (Il2CppMethodSlot slot, TypeInfo* declaringInterface, void* obj);
	static VirtualInvokeData GetGenericVirtualInvokeData (const MethodInfo* method, void* obj);
	static VirtualInvokeData GetGenericInterfaceInvokeData (const MethodInfo* method, void* obj);

	static void RaiseExecutionEngineExceptionIfMethodIsNotFound(const MethodInfo* method);

public:
	// internal
	static Il2CppRuntimeUnhandledExceptionPolicy GetUnhandledExceptionPolicy ();
	static void UnhandledException (Il2CppObject* exc);
	static void ClassInit (TypeInfo *klass);

#if IL2CPP_ENABLE_NATIVE_STACKTRACES
	struct MethodDefinitionKey
	{
		methodPointerType method;
		MethodIndex methodIndex;
	};

	static void RegisterMethods (const std::vector<MethodDefinitionKey>& managedMethods);
	static const MethodInfo* GetMethodFromNativeSymbol (methodPointerType nativeMethod);
#endif
	
	static const char *GetBundledMachineConfig ();
	static void RegisterBundledMachineConfig (const char *config_xml);

private:
	static void CallUnhandledExceptionDelegate (Il2CppDomain* domain, Il2CppDelegate* delegate, Il2CppObject* exc);
	static Il2CppObject* CreateUnhandledExceptionEventArgs (Il2CppObject* exc);

	static Il2CppRuntimeUnhandledExceptionPolicy s_UnhandledExceptionPolicy;
	static const char *s_BundledMachineConfig;
	static std::string s_ConfigDir;
	static std::string s_DataDir;
	static const char *s_FrameworkVersion;
};

} /* namespace vm */
} /* namespace il2cpp */
