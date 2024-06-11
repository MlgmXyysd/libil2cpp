#pragma once

#include <stdint.h>
#include "blob.h"
#include "metadata/Il2CppTypeVector.h"
#include "utils/dynamic_array.h"
#include "class-internals.h"

struct TypeInfo;
struct EventInfo;
struct FieldInfo;
struct PropertyInfo;
struct MethodInfo;

struct Il2CppImage;
struct Il2CppReflectionType;
struct Il2CppType;
struct Il2CppDebugTypeInfo;
struct Il2CppGenericContainer;
struct Il2CppGenericContext;
struct Il2CppGenericParameter;

namespace il2cpp
{
namespace vm
{

class TypeNameParseInfo;

class Class
{
public:
	static TypeInfo* FromIl2CppType (const Il2CppType* type);
	static TypeInfo* FromName (Il2CppImage* image, const char* namespaze, const char *name);
	static TypeInfo* FromSystemType (Il2CppReflectionType *type);
	static TypeInfo* FromGenericParameter (const Il2CppGenericParameter *param);
	static TypeInfo* GetElementClass (TypeInfo *klass);
	static const Il2CppType* GetEnumBaseType (TypeInfo *klass);
	static const EventInfo* GetEvents (TypeInfo *klass, void* *iter);
	static FieldInfo* GetFields (TypeInfo *klass, void* *iter);
	static FieldInfo* GetFieldFromName (TypeInfo *klass, const char* name);
	static const MethodInfo* GetFinalizer (TypeInfo *klass);
	static int32_t GetInstanceSize (const TypeInfo *klass);
	static TypeInfo* GetInterfaces (TypeInfo *klass, void* *iter);
	static const MethodInfo* GetMethods (TypeInfo *klass, void* *iter);
	static const MethodInfo* GetMethodFromName (TypeInfo *klass, const char* name, int argsCount);
	static const MethodInfo* GetMethodFromNameFlags (TypeInfo *klass, const char* name, int argsCount, int32_t flags);
	static const char* GetName (TypeInfo *klass);
	static const char* GetNamespace (TypeInfo *klass);
	static TypeInfo* GetNestedTypes (TypeInfo *klass, void* *iter);
	static size_t GetNumMethods(const TypeInfo* klass);
	static size_t GetNumProperties(const TypeInfo* klass);
	static size_t GetNumFields(const TypeInfo* klass);
	static TypeInfo* GetParent(TypeInfo *klass);
	static const PropertyInfo* GetProperties (TypeInfo *klass, void* *iter);
	static const PropertyInfo* GetPropertyFromName (TypeInfo *klass, const char* name);
	static int32_t GetValueSize (TypeInfo *klass, uint32_t *align);
	static bool HasParent (TypeInfo *klass, TypeInfo *parent);
	// we assume that the TypeInfo's have already been initialized in this case, like in code generation
	static bool HasParentUnsafe (const TypeInfo* klass, const TypeInfo* parent) { return klass->typeHierarchyDepth >= parent->typeHierarchyDepth && klass->typeHierarchy[parent->typeHierarchyDepth - 1] == parent; }
	static bool IsAssignableFrom (TypeInfo *klass, TypeInfo *oklass);
	static bool IsGeneric (const TypeInfo *klass);
	static bool IsInflated (const TypeInfo *klass);
	static bool IsSubclassOf (TypeInfo *klass, TypeInfo *klassc, bool check_interfaces);
	static bool IsValuetype (const TypeInfo *klass);
	static bool HasDefaultConstructor (TypeInfo* klass);
	static int GetFlags (const TypeInfo *klass);
	static bool IsAbstract (const TypeInfo *klass);
	static bool IsInterface (const TypeInfo *klass);
	static bool IsNullable (const TypeInfo *klass);
	static TypeInfo* GetNullableArgument(const TypeInfo* klass);
	static int GetArrayElementSize (const TypeInfo *klass);
	static const Il2CppType* GetType (TypeInfo *klass);
	static const Il2CppType* GetType (TypeInfo *klass, const TypeNameParseInfo &info);
	static bool HasAttribute (TypeInfo *klass, TypeInfo *attr_class);
	static bool IsEnum (const TypeInfo *klass);
	static const Il2CppImage* GetImage (TypeInfo* klass);
	static const Il2CppDebugTypeInfo *GetDebugInfo (const TypeInfo *klass);
	static const char *GetAssemblyName (const TypeInfo *klass);

public:
	//internal
	static int32_t GetInterfaceOffset (TypeInfo *klass, TypeInfo *itf);
	static bool Init (TypeInfo *klass);

	static TypeInfo* GetArrayClass (TypeInfo *element_class, uint32_t rank);
	static TypeInfo* GetBoundedArrayClass (TypeInfo *element_class, uint32_t rank, bool bounded);
	static TypeInfo* GetInflatedGenericInstanceClass (TypeInfo* klass, const metadata::Il2CppTypeVector& types);
	static TypeInfo* InflateGenericClass (TypeInfo* klass, Il2CppGenericContext *context);
	static const Il2CppType* InflateGenericType (const Il2CppType* type, Il2CppGenericContext *context);

	static TypeInfo* GetArrayClassCached (TypeInfo *element_class, uint32_t rank)
	{
		return GetArrayClass (element_class, rank);
	}

	static const Il2CppGenericContainer* GetGenericContainer (TypeInfo *klass);
	static const MethodInfo* GetCCtor (TypeInfo *klass);
	static const char* GetFieldDefaultValue (const FieldInfo *field, const Il2CppType** type);
	static int GetFieldMarshaledSize(const FieldInfo *field);
	static TypeInfo* GetPtrClass (const Il2CppType* type);
	static TypeInfo* GetPtrClass (TypeInfo* elementClass);
	static bool HasReferences (TypeInfo *klass);
	static void SetupEvents (TypeInfo *klass);
	static void SetupFields (TypeInfo *klass);
	static void SetupMethods (TypeInfo *klass);
	static void SetupNestedTypes (TypeInfo *klass);
	static void SetupProperties (TypeInfo *klass);
	static void SetupTypeHierarchy (TypeInfo *klass);
	static void SetupInterfaces (TypeInfo *klass);

	static const dynamic_array<TypeInfo*>& GetStaticFieldData ();

	static size_t GetBitmapSize (const TypeInfo* klass);
	static void GetBitmap (TypeInfo* klass, size_t* bitmap, size_t& maxSetBit);

	static const Il2CppType* il2cpp_type_from_type_info(const TypeNameParseInfo& info, bool throwOnError, bool ignoreCase);

	static TypeInfo* GetDeclaringType(TypeInfo* klass);
private:
};

} /* namespace vm */
} /* namespace il2cpp */
