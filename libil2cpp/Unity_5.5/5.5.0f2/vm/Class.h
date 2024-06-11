#pragma once

#include <stdint.h>
#include <sstream>
#include "il2cpp-config.h"
#include "blob.h"
#include "class-internals.h"
#include "metadata/Il2CppTypeVector.h"
#include "utils/dynamic_array.h"
#include "class-internals.h"
#include "Exception.h"
#include "Type.h"

#if NET_4_0
#include "vm/MetadataCache.h"
#include "tabledefs.h"
#endif


struct Il2CppClass;
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

class LIBIL2CPP_CODEGEN_API Class
{
public:
	static Il2CppClass* FromIl2CppType (const Il2CppType* type);
	static Il2CppClass* FromName (const Il2CppImage* image, const char* namespaze, const char *name);
	static Il2CppClass* FromSystemType (Il2CppReflectionType *type);
	static Il2CppClass* FromGenericParameter (const Il2CppGenericParameter *param);
	static Il2CppClass* GetElementClass (Il2CppClass *klass);
	static const Il2CppType* GetEnumBaseType (Il2CppClass *klass);
	static const EventInfo* GetEvents (Il2CppClass *klass, void* *iter);
	static FieldInfo* GetFields (Il2CppClass *klass, void* *iter);
	static FieldInfo* GetFieldFromName (Il2CppClass *klass, const char* name);
	static const MethodInfo* GetFinalizer (Il2CppClass *klass);
	static int32_t GetInstanceSize (const Il2CppClass *klass);
	static Il2CppClass* GetInterfaces (Il2CppClass *klass, void* *iter);
	static const MethodInfo* GetMethods (Il2CppClass *klass, void* *iter);
	static const MethodInfo* GetMethodFromName (Il2CppClass *klass, const char* name, int argsCount);
	static const MethodInfo* GetMethodFromNameFlags (Il2CppClass *klass, const char* name, int argsCount, int32_t flags);
	static const char* GetName (Il2CppClass *klass);
	static const char* GetNamespace (Il2CppClass *klass);
	static Il2CppClass* GetNestedTypes (Il2CppClass *klass, void* *iter);
	static size_t GetNumMethods(const Il2CppClass* klass);
	static size_t GetNumProperties(const Il2CppClass* klass);
	static size_t GetNumFields(const Il2CppClass* klass);
	static Il2CppClass* GetParent(Il2CppClass *klass);
	static const PropertyInfo* GetProperties (Il2CppClass *klass, void* *iter);
	static const PropertyInfo* GetPropertyFromName (Il2CppClass *klass, const char* name);
	static int32_t GetValueSize (Il2CppClass *klass, uint32_t *align);
	static bool HasParent (Il2CppClass *klass, Il2CppClass *parent);
	// we assume that the Il2CppClass's have already been initialized in this case, like in code generation
	static bool HasParentUnsafe (const Il2CppClass* klass, const Il2CppClass* parent) { return klass->typeHierarchyDepth >= parent->typeHierarchyDepth && klass->typeHierarchy[parent->typeHierarchyDepth - 1] == parent; }
	static bool IsAssignableFrom (Il2CppClass *klass, Il2CppClass *oklass);
	static bool IsGeneric (const Il2CppClass *klass);
	static bool IsInflated (const Il2CppClass *klass);
	static bool IsSubclassOf (Il2CppClass *klass, Il2CppClass *klassc, bool check_interfaces);
	static bool IsValuetype (const Il2CppClass *klass);
	static bool HasDefaultConstructor (Il2CppClass* klass);
	static int GetFlags (const Il2CppClass *klass);
	static bool IsAbstract (const Il2CppClass *klass);
	static bool IsInterface (const Il2CppClass *klass);
	static bool IsNullable (const Il2CppClass *klass);
	static Il2CppClass* GetNullableArgument(const Il2CppClass* klass);
	static int GetArrayElementSize (const Il2CppClass *klass);
	static const Il2CppType* GetType (Il2CppClass *klass);
	static const Il2CppType* GetType (Il2CppClass *klass, const TypeNameParseInfo &info);
	static bool HasAttribute (Il2CppClass *klass, Il2CppClass *attr_class);
	static bool IsEnum (const Il2CppClass *klass);
	static const Il2CppImage* GetImage (Il2CppClass* klass);
	static const Il2CppDebugTypeInfo *GetDebugInfo (const Il2CppClass *klass);
	static const char *GetAssemblyName (const Il2CppClass *klass);

public:

#if NET_4_0
	static inline int32_t GetInterfaceOffsetGeneric(Il2CppClass *klass, Il2CppClass *itf, bool throwIfNotFound)
	{
		TypeDefinitionIndex itfTypeDefinitionIndex = itf->generic_class->typeDefinitionIndex;
		const Il2CppTypeDefinition* genericInterface = MetadataCache::GetTypeDefinitionFromIndex(itfTypeDefinitionIndex);
		const Il2CppGenericContainer* genericContainer = MetadataCache::GetGenericContainerFromIndex(genericInterface->genericContainerIndex);
		const Il2CppGenericInst* interfaceGenericInst = itf->generic_class->context.class_inst;
		int32_t genericParameterCount = genericContainer->type_argc;
		IL2CPP_ASSERT(interfaceGenericInst->type_argc == genericParameterCount);

		for (uint16_t i = 0; i < klass->interface_offsets_count; i++)
		{
			Il2CppClass* interfaceOnClass = klass->interfaceOffsets[i].interfaceType;

			if (interfaceOnClass->generic_class == NULL || interfaceOnClass->generic_class->typeDefinitionIndex != itfTypeDefinitionIndex)
				continue;

			const Il2CppGenericInst* interfaceOnClassGenericInst = interfaceOnClass->generic_class->context.class_inst;
			IL2CPP_ASSERT(interfaceOnClassGenericInst->type_argc == genericParameterCount);

			for (int32_t j = 0; j < genericParameterCount; j++)
			{
				const Il2CppGenericParameter* genericParameter = MetadataCache::GetGenericParameterFromIndex(genericContainer->genericParameterStart + j);
				const int32_t parameterVariance = genericParameter->flags & GENERIC_PARAMETER_ATTRIBUTE_VARIANCE_MASK;
				Il2CppClass* interfaceOnClassGenericParameterType = Class::FromIl2CppType(interfaceOnClassGenericInst->type_argv[j]);
				Il2CppClass* interfaceGenericParameterType = Class::FromIl2CppType(interfaceGenericInst->type_argv[j]);

				if (parameterVariance == GENERIC_PARAMETER_ATTRIBUTE_NON_VARIANT || Class::IsValuetype(interfaceOnClassGenericParameterType) || Class::IsValuetype(interfaceGenericParameterType))
				{
					if (interfaceGenericParameterType != interfaceOnClassGenericParameterType)
						goto CONTINUE_OUTER;
				}
				else if (parameterVariance == GENERIC_PARAMETER_ATTRIBUTE_COVARIANT)
				{
					if (!Class::IsAssignableFrom(interfaceGenericParameterType, interfaceOnClassGenericParameterType))
						goto CONTINUE_OUTER;
				}
				else // Contravariant
				{
					if (!Class::IsAssignableFrom(interfaceOnClassGenericParameterType, interfaceGenericParameterType))
						goto CONTINUE_OUTER;
				}
			}

			return klass->interfaceOffsets[i].offset;

		CONTINUE_OUTER:
			continue;
		}

		if (throwIfNotFound)
			RaiseExecutionEngineException(klass, itf);

		return -1;
	}
#endif

	//internal
	static inline int32_t GetInterfaceOffset(Il2CppClass *klass, Il2CppClass *itf, bool throwIfNotFound)
	{
		for (uint16_t i = 0; i < klass->interface_offsets_count; i++)
		{
			if (klass->interfaceOffsets[i].interfaceType == itf)
			{
				int32_t offset = klass->interfaceOffsets[i].offset;
				IL2CPP_ASSERT(offset != -1);
				return offset;
			}
		}

#if NET_4_0
		// If we get here and it's not a generic_class, we're screwed anyway,
		// so might as well not check it in release builds to let compiler generate better code
		IL2CPP_ASSERT(itf->generic_class != NULL);
		return GetInterfaceOffsetGeneric(klass, itf, throwIfNotFound);
#else

		if (throwIfNotFound)
			RaiseExecutionEngineException(klass, itf);

		return -1;
#endif
	}

	static bool Init (Il2CppClass *klass);

	static Il2CppClass* GetArrayClass (Il2CppClass *element_class, uint32_t rank);
	static Il2CppClass* GetBoundedArrayClass (Il2CppClass *element_class, uint32_t rank, bool bounded);
	static Il2CppClass* GetInflatedGenericInstanceClass (Il2CppClass* klass, const metadata::Il2CppTypeVector& types);
	static Il2CppClass* InflateGenericClass (Il2CppClass* klass, Il2CppGenericContext *context);
	static const Il2CppType* InflateGenericType (const Il2CppType* type, Il2CppGenericContext *context);

	static Il2CppClass* GetArrayClassCached (Il2CppClass *element_class, uint32_t rank)
	{
		return GetArrayClass (element_class, rank);
	}

	static const Il2CppGenericContainer* GetGenericContainer (Il2CppClass *klass);
	static const MethodInfo* GetCCtor (Il2CppClass *klass);
	static const char* GetFieldDefaultValue (const FieldInfo *field, const Il2CppType** type);
	static int GetFieldMarshaledSize(const FieldInfo *field);
	static Il2CppClass* GetPtrClass (const Il2CppType* type);
	static Il2CppClass* GetPtrClass (Il2CppClass* elementClass);
	static bool HasReferences (Il2CppClass *klass);
	static void SetupEvents (Il2CppClass *klass);
	static void SetupFields (Il2CppClass *klass);
	static void SetupMethods (Il2CppClass *klass);
	static void SetupNestedTypes (Il2CppClass *klass);
	static void SetupProperties (Il2CppClass *klass);
	static void SetupTypeHierarchy (Il2CppClass *klass);
	static void SetupInterfaces (Il2CppClass *klass);

	static const dynamic_array<Il2CppClass*>& GetStaticFieldData ();

	static size_t GetBitmapSize (const Il2CppClass* klass);
	static void GetBitmap (Il2CppClass* klass, size_t* bitmap, size_t& maxSetBit);

	static const Il2CppType* il2cpp_type_from_type_info(const TypeNameParseInfo& info, bool throwOnError, bool ignoreCase);

	static Il2CppClass* GetDeclaringType(Il2CppClass* klass);
private:
	static void RaiseExecutionEngineException(Il2CppClass* klass, Il2CppClass* itf)
	{
		std::stringstream message;
		message << "Attempting to use interface '" << Type::GetName(itf->byval_arg, IL2CPP_TYPE_NAME_FORMAT_IL) << "' on type '"
			<< Type::GetName(klass->byval_arg, IL2CPP_TYPE_NAME_FORMAT_IL)
			<< "' for which no ahead-of-time code was generated. This requires just-in-time execution, which is not supported by IL2CPP.";
		Exception::Raise(il2cpp::vm::Exception::GetExecutionEngineException(message.str().c_str()));
	}
};

} /* namespace vm */
} /* namespace il2cpp */
