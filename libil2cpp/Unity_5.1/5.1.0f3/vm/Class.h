#pragma once

#include <stdint.h>
#include "blob.h"
#include <vector>
#include "metadata/Il2CppTypeVector.h"

struct TypeInfo;
struct FieldInfo;
struct PropertyInfo;
struct MethodInfo;

struct Il2CppImage;
struct Il2CppReflectionType;
struct Il2CppType;
struct Il2CppDebugTypeInfo;
struct Il2CppGenericContext;
struct Il2CppGenericParam;

namespace il2cpp
{
namespace vm
{

class TypeNameParseInfo;

class Class
{
public:
	static TypeInfo* FromIl2CppType (const Il2CppType* type);
	static TypeInfo* FromName (const Il2CppImage* image, const char* namespaze, const char *name);
	static TypeInfo* FromSystemType (Il2CppReflectionType *type);
	static TypeInfo* FromGenericParameter (Il2CppGenericParam *param);
	static TypeInfo* GetElementClass (TypeInfo *klass);
	static const Il2CppType* GetEnumBaseType (TypeInfo *klass);
	static FieldInfo* GetFields (TypeInfo *klass, void* *iter);
	static FieldInfo* GetFieldFromName (TypeInfo *klass, const char* name);
	static MethodInfo* GetFinalizer (TypeInfo *klass);
	static int32_t GetInstanceSize (const TypeInfo *klass);
	static TypeInfo* GetInterfaces (TypeInfo *klass, void* *iter);
	static MethodInfo* GetMethods (const TypeInfo *klass, void* *iter);
	static MethodInfo* GetMethodFromName (TypeInfo *klass, const char* name, int argsCount);
	static MethodInfo* GetMethodFromNameFlags (TypeInfo *klass, const char* name, int argsCount, int32_t flags);
	static const char* GetName (TypeInfo *klass);
	static const char* GetNamespace (TypeInfo *klass);
	static size_t GetNumMethods(const TypeInfo* klass);
	static size_t GetNumProperties(const TypeInfo* klass);
	static size_t GetNumFields(const TypeInfo* klass);
	static TypeInfo* GetParent(TypeInfo *klass);
	static PropertyInfo* GetProperties (const TypeInfo *klass, void* *iter);
	static PropertyInfo* GetPropertyFromName (const TypeInfo *klass, const char* name);
	static int32_t GetValueSize (TypeInfo *klass, uint32_t *align);
	static bool HasParent (const TypeInfo *klass, const TypeInfo *parent);
	static bool IsAssignableFrom (TypeInfo *klass, TypeInfo *oklass);
	static bool IsGeneric (const TypeInfo *klass);
	static bool IsSubclassOf (TypeInfo *klass, TypeInfo *klassc, bool check_interfaces);
	static bool IsValuetype (const TypeInfo *klass);
	static bool HasDefaultConstructor (const TypeInfo* klass);
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
	static Il2CppImage* GetImage (TypeInfo* klass);
	static const Il2CppDebugTypeInfo *GetDebugInfo (const TypeInfo *klass);
	static const char *GetAssemblyName (const TypeInfo *klass);

public:
	//internal
	static int32_t GetInterfaceOffset (const TypeInfo *klass, TypeInfo *itf);
	static bool Init (TypeInfo *klass);

	static int Register (TypeInfo* typeInfo);
	static int RegisterSZArray (TypeInfo* typeInfo);
	static int RegisterArray (TypeInfo* typeInfo);

	static TypeInfo* GetArrayClass (TypeInfo *element_class, uint32_t rank);
	static TypeInfo* GetBoundedArrayClass (TypeInfo *element_class, uint32_t rank, bool bounded);
	static TypeInfo* GetInflatedGenericInstanceClass (TypeInfo* klass, const metadata::Il2CppTypeVector& types);
	static TypeInfo* InflateGenericClass (TypeInfo* klass, Il2CppGenericContext *context);
	static const Il2CppType* InflateGenericType (const Il2CppType* type, Il2CppGenericContext *context);

	static TypeInfo* GetArrayClassCached (TypeInfo *element_class, uint32_t rank)
	{
		return GetArrayClass (element_class, rank);
	}

	static MethodInfo* GetCCtor (TypeInfo *klass);
	static const char* GetFieldDefaultValue (const FieldInfo *field, const Il2CppType** type);
	static TypeInfo* GetPtrClass (const Il2CppType* type);
	static TypeInfo* GetPtrClass (TypeInfo* elementClass);
	static bool HasReferences (TypeInfo *klass);
	static void SetupFields (TypeInfo *klass);

	static const std::vector<TypeInfo*>& GetStaticFieldData ();

	static size_t GetBitmapSize (const TypeInfo* klass);
	static void GetBitmap (TypeInfo* klass, size_t* bitmap, size_t& maxSetBit);

	static const Il2CppType* il2cpp_type_from_type_info(const TypeNameParseInfo& info, bool throwOnError, bool ignoreCase);

	static TypeInfo* GetDeclaringType(TypeInfo* klass);
private:
};

} /* namespace vm */
} /* namespace il2cpp */
