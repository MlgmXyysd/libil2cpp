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
struct Il2CppType;

namespace il2cpp
{
namespace vm
{

class MetadataCache
{
public:

	static void Register (const Il2CppCodeRegistration * const codeRegistration, const Il2CppMetadataRegistration * const metadataRegistration);

	static void Initialize();

	static TypeInfo* GetGenericInstanceType (TypeInfo* genericTypeDefinition, const il2cpp::metadata::Il2CppTypeVector& genericArgumentTypes);
	static const MethodInfo* GetGenericInstanceMethod (const MethodInfo* genericMethodDefinition, Il2CppGenericContext* context);
	static const MethodInfo* GetGenericInstanceMethod (const MethodInfo* genericMethodDefinition, const il2cpp::metadata::Il2CppTypeVector& genericArgumentTypes);
	static const Il2CppGenericContext* GetMethodGenericContext(const MethodInfo* method);
	static Il2CppGenericContainer* GetMethodGenericContainer(const MethodInfo* method);
	static const MethodInfo* GetGenericMethodDefinition(const MethodInfo* method);

	static const MethodInfo* GetNativeDelegate (methodPointerType nativeFunctionPointer);
	static void AddNativeDelegate (methodPointerType nativeFunctionPointer, const MethodInfo* managedMethodInfo);

	static TypeInfo* GetPointerType(TypeInfo* type);
	static void AddPointerType(TypeInfo* type, TypeInfo* pointerType);

	static Il2CppGenericInst* GetGenericInst (const il2cpp::metadata::Il2CppTypeVector& types);
	static Il2CppGenericMethod* GetGenericMethod (const MethodInfo* methodDefinition, Il2CppGenericInst* classInst, Il2CppGenericInst* methodInst);

	static InvokerMethod GetInvokerMethodPointer (const MethodInfo* methodDefinition, Il2CppGenericContext* context);
	static methodPointerType GetMethodPointer (const MethodInfo* methodDefinition, Il2CppGenericContext* context);

	static TypeInfo* GetTypeInfoFromIndex (TypeIndex index);
	static const Il2CppType* GetIl2CppTypeFromIndex (TypeIndex index);
	static const MethodInfo* GetMethodInfoFromIndex (MethodIndex index);
};

} // namespace vm
} // namespace il2cpp
