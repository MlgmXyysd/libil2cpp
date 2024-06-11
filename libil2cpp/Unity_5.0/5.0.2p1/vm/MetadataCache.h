#pragma once

#include <stdint.h>
#include <vector>
#include "Assembly.h"
#include "metadata/Il2CppTypeVector.h"
#include "class-internals.h"

struct MethodInfo;
struct TypeInfo;
struct Il2CppGenericContainer;
struct Il2CppGenericContext;
struct Il2CppGenericInst;
struct Il2CppGenericMethod;
struct Il2CppMethodGenericContainerData;
struct Il2CppType;

namespace il2cpp
{
namespace vm
{

class MetadataCache
{
public:

	static void RegisterGenericTypes (Il2CppGenericClass** types, size_t typeCount);
	static void RegisterSZArrays(TypeInfo** types, size_t typeCount);
	static void RegisterArrays(TypeInfo** types, size_t typeCount);
	static void RegisterGenericMethods (Il2CppGenericMethod** methods, size_t methodCount);
	static void RegisterGenericInsts (Il2CppGenericInst** insts, size_t instCount);
	static void RegisterMethodTable (Il2CppGenericMethodFunctions* methods, size_t methodCount);
	static void RegisterMethodPointers (methodPointerType* methods, size_t methodCount);
	static void RegisterInvokerPointers (InvokerMethod* methods, size_t methodCount);

	static void Initialize(const AssemblyVector& assemblies);

	static TypeInfo* GetGenericInstanceType (TypeInfo* genericTypeDefinition, const il2cpp::metadata::Il2CppTypeVector& genericArgumentTypes);
	static MethodInfo* GetGenericInstanceMethod (MethodInfo* genericMethodDefinition, Il2CppGenericContext* context);
	static MethodInfo* GetGenericInstanceMethod (MethodInfo* genericMethodDefinition, const il2cpp::metadata::Il2CppTypeVector& genericArgumentTypes);
	static const Il2CppGenericContext* GetMethodGenericContext(const MethodInfo* method);
	static Il2CppGenericContainer* GetMethodGenericContainer(const MethodInfo* method);
	static const MethodInfo* GetGenericMethodDefinition(const MethodInfo* method);

	static MethodInfo* GetNativeDelegate(methodPointerType nativeFunctionPointer);
	static void AddNativeDelegate(methodPointerType nativeFunctionPointer, MethodInfo* managedMethodInfo);

	static TypeInfo* GetPointerType(TypeInfo* type);
	static void AddPointerType(TypeInfo* type, TypeInfo* pointerType);

	static Il2CppGenericInst* GetGenericInst (const il2cpp::metadata::Il2CppTypeVector& types);
	static Il2CppGenericMethod* GetGenericMethod (MethodInfo* methodDefinition, Il2CppGenericInst* classInst, Il2CppGenericInst* methodInst);

	static InvokerMethod GetInvokerMethodPointer (const MethodInfo* methodDefinition, Il2CppGenericContext* context);
	static methodPointerType GetMethodPointer (const MethodInfo* methodDefinition, Il2CppGenericContext* context);
};

} // namespace vm
} // namespace il2cpp
