#pragma once

#include <stdint.h>
#include <vector>
#include "il2cpp-api-types.h"

struct Il2CppArray;
struct Il2CppDelegate;
struct Il2CppObject;
struct MethodInfo;
struct TypeInfo;

struct VirtualInvokeData
{
	void* target;
	MethodInfo* methodInfo;
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
	static const char *GetConfigDir ();
	static const char *GetFrameworkVersion ();
	static Il2CppObject* DelegateInvoke (Il2CppDelegate *obj, void **params, Il2CppObject **exc);
	static Il2CppObject* Invoke (MethodInfo *method, void *obj, void **params, Il2CppObject **exc);
	static Il2CppObject* InvokeConvertArgs (MethodInfo *method, void *obj, Il2CppObject **params, int paramCount, Il2CppObject **exc);
	static Il2CppObject* InvokeArray (MethodInfo *method, void *obj, Il2CppArray *params, Il2CppObject **exc);
	static void ObjectInit (Il2CppObject* object);
	static void ObjectInitException (Il2CppObject* object, Il2CppObject **exc);
	static void SetUnhandledExceptionPolicy (Il2CppRuntimeUnhandledExceptionPolicy value);

	static VirtualInvokeData GetVirtualInvokeData (MethodInfo* method, void* obj);
	static VirtualInvokeData GetInterfaceInvokeData (MethodInfo* method, void* obj);
	static VirtualInvokeData GetGenericVirtualInvokeData (MethodInfo* method, void* obj);
	static VirtualInvokeData GetGenericInterfaceInvokeData (MethodInfo* method, void* obj);

	static void RaiseExecutionEngineExceptionIfMethodIsNotFound(const MethodInfo* method);

public:
	// internal
	static Il2CppRuntimeUnhandledExceptionPolicy GetUnhandledExceptionPolicy ();
	static void UnhandledException (Il2CppObject* exc);
	static void ClassInit (TypeInfo *klass);

#if IL2CPP_SUPPORT_NATIVE_STACKTRACES
	static void RegisterMethods(const std::vector<MethodInfo*>& managedMethods);
	static MethodInfo* GetMethodFromNativeSymbol(const methodPointerType nativeMethod);

	static void RegisterRuntimeMethod (MethodInfo* method);
#endif
	
	static const char *GetBundledMachineConfig ();
	static void RegisterBundledMachineConfig (const char *config_xml);

private:
	static void CallUnhandledExceptionDelegate (Il2CppDomain* domain, Il2CppDelegate* delegate, Il2CppObject* exc);
	static Il2CppObject* CreateUnhandledExceptionEventArgs (Il2CppObject* exc);

	static Il2CppRuntimeUnhandledExceptionPolicy s_UnhandledExceptionPolicy;
	static const char *s_BundledMachineConfig;
	static char *s_ConfigDir;
	static char *s_RuntimeVersion;
	static const char *s_FrameworkVersion;
};

} /* namespace vm */
} /* namespace il2cpp */
