#include "il2cpp-config.h"
#include <algorithm>
#include "metadata/ArrayMetadata.h"
#include "metadata/GenericMetadata.h"
#include "metadata/GenericMethod.h"
#include "metadata/FieldLayout.h"
#include "metadata/Il2CppTypeVector.h"
#include "os/Atomic.h"
#include "os/Mutex.h"
#include "utils/Memory.h"
#include "utils/StringUtils.h"
#include "vm/Assembly.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Field.h"
#include "vm/GenericClass.h"
#include "vm/GenericContainer.h"
#include "vm/Image.h"
#include "vm/MetadataAlloc.h"
#include "vm/MetadataCache.h"
#include "vm/MetadataLock.h"
#include "vm/Property.h"
#include "vm/Runtime.h"
#include "vm/Reflection.h"
#include "vm/Thread.h"
#include "vm/Type.h"
#include "vm/Object.h"
#include "class-internals.h"
#include "object-internals.h"
#include "tabledefs.h"
#include "gc/gc-internal.h"
#include "utils/StdUnorderedMap.h"
#include "utils/StringUtils.h"
#include <cassert>
#include <string>
#include <memory.h>
#include <algorithm>
#include <limits>

#if IL2CPP_DEBUGGER_ENABLED
	#include "il2cpp-debugger.h"
#endif

using il2cpp::metadata::ArrayMetadata;
using il2cpp::metadata::FieldLayout;
using il2cpp::metadata::GenericMetadata;
using il2cpp::metadata::GenericMethod;
using il2cpp::metadata::Il2CppTypeVector;
using il2cpp::os::FastAutoLock;

namespace il2cpp
{
namespace vm
{

static dynamic_array<TypeInfo*> s_staticFieldData;
static int32_t s_FinalizerSlot = -1;
static int32_t s_GetHashCodeSlot = -1;

static void SetupGCDescriptor (TypeInfo* klass);
static void GetBitmapNoInit (TypeInfo* klass, size_t* bitmap, size_t& maxSetBit, size_t parentOffset);
static TypeInfo* ResolveGenericInstanceType (TypeInfo*, const il2cpp::vm::TypeNameParseInfo&, bool, bool);
static bool InitLocked (TypeInfo *klass, const FastAutoLock& lock);

TypeInfo* Class::FromIl2CppType (const Il2CppType* type)
{
	switch (type->type)
	{
	case IL2CPP_TYPE_OBJECT:
		return il2cpp_defaults.object_class;
	case IL2CPP_TYPE_VOID:
		return il2cpp_defaults.void_class;
	case IL2CPP_TYPE_BOOLEAN:
		return il2cpp_defaults.boolean_class;
	case IL2CPP_TYPE_CHAR:
		return il2cpp_defaults.char_class;
	case IL2CPP_TYPE_I1:
		return il2cpp_defaults.sbyte_class;
	case IL2CPP_TYPE_U1:
		return il2cpp_defaults.byte_class;
	case IL2CPP_TYPE_I2:
		return il2cpp_defaults.int16_class;
	case IL2CPP_TYPE_U2:
		return il2cpp_defaults.uint16_class;
	case IL2CPP_TYPE_I4:
		return il2cpp_defaults.int32_class;
	case IL2CPP_TYPE_U4:
		return il2cpp_defaults.uint32_class;
	case IL2CPP_TYPE_I:
		return il2cpp_defaults.int_class;
	case IL2CPP_TYPE_U:
		return il2cpp_defaults.uint_class;
	case IL2CPP_TYPE_I8:
		return il2cpp_defaults.int64_class;
	case IL2CPP_TYPE_U8:
		return il2cpp_defaults.uint64_class;
	case IL2CPP_TYPE_R4:
		return il2cpp_defaults.single_class;
	case IL2CPP_TYPE_R8:
		return il2cpp_defaults.double_class;
	case IL2CPP_TYPE_STRING:
		return il2cpp_defaults.string_class;
	case IL2CPP_TYPE_TYPEDBYREF:
		return il2cpp_defaults.typed_reference_class;
	case IL2CPP_TYPE_ARRAY:
	{
		TypeInfo* elementClass = FromIl2CppType (type->data.array->etype);
		return Class::GetBoundedArrayClass (elementClass, type->data.array->rank, true);
	}
	case IL2CPP_TYPE_PTR:
		return Class::GetPtrClass (type->data.type);
	case IL2CPP_TYPE_FNPTR:
		NOT_IMPLEMENTED (Class::FromIl2CppType);
		return NULL; //mono_fnptr_class_get (type->data.method);
	case IL2CPP_TYPE_SZARRAY:
		{
			TypeInfo* elementClass = FromIl2CppType (type->data.type);
			return Class::GetArrayClass (elementClass, 1);
		}
	case IL2CPP_TYPE_CLASS:
	case IL2CPP_TYPE_VALUETYPE:
		return Type::GetClass (type);
	case IL2CPP_TYPE_GENERICINST:
		return GenericClass::GetClass (type->data.generic_class);
	case IL2CPP_TYPE_VAR:
		return Class::FromGenericParameter (Type::GetGenericParameter (type));
	case IL2CPP_TYPE_MVAR:
		return Class::FromGenericParameter (Type::GetGenericParameter (type));
	default:
		NOT_IMPLEMENTED (Class::FromIl2CppType);
	}
	
	return NULL;
}

/* From ECMA-335, I.8.7 Assignment compatibility:

	The reduced type of a type T is the following:

	1. If the underlying type of T is:
		a. int8, or unsigned int8, then its reduced type is int8.
		b. int16, or unsigned int16, then its reduced type is int16.
		c. int32, or unsigned int32, then its reduced type is int32.
		d. int64, or unsigned int64, then its reduced type is int64.
		e. native int, or unsigned native int, then its reduced type is native int.
	2. Otherwise, the reduced type is itself.
*/
static inline const TypeInfo* GetReducedType(const TypeInfo* type)
{
	switch (type->byval_arg->type)
	{
	case IL2CPP_TYPE_I1:
	case IL2CPP_TYPE_U1:
		return il2cpp_defaults.sbyte_class;
		
	case IL2CPP_TYPE_I2:
	case IL2CPP_TYPE_U2:
		return il2cpp_defaults.int16_class;

	case IL2CPP_TYPE_I4:
	case IL2CPP_TYPE_U4:
		return il2cpp_defaults.int32_class;

	case IL2CPP_TYPE_I8:
	case IL2CPP_TYPE_U8:
		return il2cpp_defaults.int64_class;

	case IL2CPP_TYPE_I:
	case IL2CPP_TYPE_U:
		return il2cpp_defaults.int_class;

	default:
		return type;
	}
}

TypeInfo* Class::FromSystemType (Il2CppReflectionType *type)
{
	return Class::FromIl2CppType (type->type);
}

static void SetupInterfacesLocked (TypeInfo* klass, const FastAutoLock& lock)
{
	if (klass->generic_class)
	{
		TypeInfo* genericTypeDefinition = GenericClass::GetTypeDefinition (klass->generic_class);
		Il2CppGenericContext* context = &klass->generic_class->context;

		if (genericTypeDefinition->interfaces_count > 0 && klass->implementedInterfaces == NULL)
		{
			assert (genericTypeDefinition->interfaces_count == klass->interfaces_count);
			klass->implementedInterfaces = (TypeInfo**)MetadataCalloc (genericTypeDefinition->interfaces_count, sizeof (TypeInfo*));
			for (uint16_t i = 0; i < genericTypeDefinition->interfaces_count; i++)
				klass->implementedInterfaces[i] = Class::FromIl2CppType (GenericMetadata::InflateIfNeeded (MetadataCache::GetInterfaceFromIndex (genericTypeDefinition->typeDefinition->interfacesStart + i), context, false));
		}
	}
	else if (klass->rank > 0)
	{
		ArrayMetadata::SetupArrayInterfaces(klass, lock);
	}
	else
	{
		if (klass->interfaces_count > 0 && klass->implementedInterfaces == NULL)
		{
			klass->implementedInterfaces = (TypeInfo**)MetadataCalloc (klass->interfaces_count, sizeof (TypeInfo*));
			for (uint16_t i = 0; i < klass->interfaces_count; i++)
				klass->implementedInterfaces[i] = Class::FromIl2CppType (MetadataCache::GetInterfaceFromIndex (klass->typeDefinition->interfacesStart + i));
		}
	}
}

typedef unordered_map<const Il2CppGenericParameter*, TypeInfo*> GenericParameterMap;
static GenericParameterMap s_GenericParameterMap;

TypeInfo* Class::FromGenericParameter (const Il2CppGenericParameter *param)
{
	assert (param->ownerIndex != kGenericContainerIndexInvalid);

	FastAutoLock lock (&g_MetadataLock);

	GenericParameterMap::const_iterator iter = s_GenericParameterMap.find (param);
	if (iter != s_GenericParameterMap.end ())
		return iter->second;

	TypeInfo* klass = (TypeInfo*)MetadataCalloc (1, sizeof (TypeInfo));

	klass->name = MetadataCache::GetStringFromIndex (param->nameIndex);
	klass->namespaze = "";

	const Il2CppGenericContainer* container = MetadataCache::GetGenericContainerFromIndex (param->ownerIndex);
	klass->image = GenericContainer::GetDeclaringType (container)->image;

	klass->initialized = true;

	klass->parent = il2cpp_defaults.object_class;
	klass->castClass = klass->element_class = klass;

	klass->flags = TYPE_ATTRIBUTE_PUBLIC;

	Il2CppType* thisArg = (Il2CppType*)MetadataCalloc (1, sizeof (Il2CppType));
	Il2CppType* byvalArg = (Il2CppType*)MetadataCalloc (1, sizeof (Il2CppType));
	thisArg->type = byvalArg->type = container->is_method ? IL2CPP_TYPE_MVAR : IL2CPP_TYPE_VAR;
	thisArg->data.genericParameterIndex = byvalArg->data.genericParameterIndex = MetadataCache::GetIndexForGenericParameter (param);
	thisArg->byref = true;

	klass->this_arg = thisArg;
	klass->byval_arg = byvalArg;

	klass->instance_size = sizeof (void*);
	klass->thread_static_fields_size = -1;
	klass->native_size = -1;
	klass->size_inited = true;

	s_GenericParameterMap.insert (std::make_pair (param, klass));

	return klass;
}

TypeInfo* Class::GetElementClass (TypeInfo *klass)
{
	return klass->element_class;
}

const Il2CppType* Class::GetEnumBaseType (TypeInfo *klass)
{
	if (klass->element_class == klass)
		/* SRE or broken types */
		return NULL;
	else
		return klass->element_class->byval_arg;
}

const EventInfo* Class::GetEvents (TypeInfo *klass, void* *iter)
{
	if (!iter)
		return NULL;

	if (!*iter)
	{
		Class::SetupEvents (klass);
		if (klass->event_count == 0)
			return NULL;

		*iter = const_cast<EventInfo*> (klass->events);
		return klass->events;
	}

	const EventInfo* eventInfo = (const EventInfo*)*iter;
	eventInfo++;
	if (eventInfo < klass->events + klass->event_count)
	{
		*iter = const_cast<EventInfo*> (eventInfo);
		return eventInfo;
	}

	return NULL;
}

FieldInfo* Class::GetFields (TypeInfo *klass, void* *iter)
{

	if (!iter)
		return NULL;

	if (!*iter)
	{
		Class::SetupFields (klass);
		if (klass->field_count == 0)
			return NULL;

		*iter = klass->fields;
		return klass->fields;
	}

	FieldInfo* fieldAddress = (FieldInfo*)*iter;
	fieldAddress++;
	if (fieldAddress < klass->fields + klass->field_count)
	{
		*iter = fieldAddress;
		return fieldAddress;
	}

	return NULL;
}

FieldInfo* Class::GetFieldFromName (TypeInfo *klass, const char* name)
{
	while (klass)
	{
		void* iter = NULL;
		FieldInfo* field;
		while ((field = GetFields (klass, &iter)))
		{
			if (strcmp (name, Field::GetName (field)) != 0)
				continue;

			return field;
		}

		klass = klass->parent;
	}

	return NULL;
}

const MethodInfo* Class::GetFinalizer (TypeInfo *klass)
{
	if (!klass->initialized)
		Class::Init (klass);

	if (!klass->has_finalize)
		return NULL;

	return klass->vtable [s_FinalizerSlot];
}

int32_t Class::GetInstanceSize (const TypeInfo *klass)
{
	assert(klass->size_inited);
	return klass->instance_size;
}

TypeInfo* Class::GetInterfaces (TypeInfo *klass, void* *iter)
{
	if (!iter)
		return NULL;

	if (!*iter)
	{
		Class::SetupInterfaces (klass);
		if (klass->interfaces_count == 0)
			return NULL;

		*iter = &klass->implementedInterfaces[0];
		return klass->implementedInterfaces[0];
	}

	TypeInfo** interfaceAddress = (TypeInfo**)*iter;
	interfaceAddress++;
	if (interfaceAddress < &klass->implementedInterfaces[klass->interfaces_count])
	{
		*iter = interfaceAddress;
		return *interfaceAddress;
	}

	return NULL;
}

const MethodInfo* Class::GetMethods (TypeInfo *klass, void* *iter)
{
	if (!iter)
		return NULL;

	if (!*iter)
	{
		Class::SetupMethods (klass);
		if (klass->method_count == 0)
			return NULL;

		*iter = &klass->methods[0];
		return klass->methods[0];
	}

	const MethodInfo** methodAddress = (const MethodInfo**)*iter;
	methodAddress++;
	if (methodAddress < &klass->methods[klass->method_count])
	{
		*iter = methodAddress;
		return *methodAddress;
	}

	return NULL;
}

const MethodInfo* Class::GetMethodFromName (TypeInfo *klass, const char* name, int argsCount)
{
	return GetMethodFromNameFlags (klass, name, argsCount, 0);
}

const MethodInfo* Class::GetMethodFromNameFlags (TypeInfo *klass, const char* name, int argsCount, int32_t flags)
{
	Class::Init (klass);

	while (klass != NULL)
	{
		void* iter = NULL;
		while (const MethodInfo* method = Class::GetMethods (klass, &iter))
		{
			if (method->name[0] == name[0] &&
				!strcmp(name, method->name) &&
				(argsCount == -1 || method->parameters_count == argsCount) &&
				((method->flags & flags) == flags))
			{
				return method;
			}
		}

		klass = klass->parent;
	}

	return NULL;
}

const char* Class::GetName (TypeInfo *klass)
{
	return klass->name;
}

const char* Class::GetNamespace (TypeInfo *klass)
{
	return klass->namespaze;
}

TypeInfo* Class::GetNestedTypes (TypeInfo *klass, void* *iter)
{
	if (!iter)
		return NULL;

	if (klass->generic_class)
	{
		assert (0 && "Class::GetNestedTypes should only be called on non-generic types and generic type definitions");
		return NULL;
	}

	if (!*iter)
	{
		Class::SetupNestedTypes (klass);
		if (klass->nested_type_count == 0)
			return NULL;

		*iter = &klass->nestedTypes[0];
		return klass->nestedTypes[0];
	}

	TypeInfo** nestedTypeAddress = (TypeInfo**)*iter;
	nestedTypeAddress++;
	if (nestedTypeAddress < &klass->nestedTypes[klass->nested_type_count])
	{
		*iter = nestedTypeAddress;
		return *nestedTypeAddress;
	}

	return NULL;
}

size_t Class::GetNumMethods(const TypeInfo* klass)
{
	return klass->method_count;
}

size_t Class::GetNumProperties(const TypeInfo* klass)
{
	return klass->property_count;
}

size_t Class::GetNumFields(const TypeInfo* klass)
{
	return klass->field_count;
}

TypeInfo* Class::GetParent (TypeInfo *klass)
{
	return klass->parent;
}

const PropertyInfo* Class::GetProperties (TypeInfo *klass, void* *iter)
{
	if (!iter)
		return NULL;

	if (!*iter)
	{
		Class::SetupProperties (klass);
		if (klass->property_count == 0)
			return NULL;

		*iter = const_cast<PropertyInfo*> (klass->properties);
		return klass->properties;
	}

	const PropertyInfo* property = (const PropertyInfo*)*iter;
	property++;
	if (property < klass->properties + klass->property_count)
	{
		*iter = const_cast<PropertyInfo*> (property);
		return property;
	}

	return NULL;
}

const PropertyInfo* Class::GetPropertyFromName (TypeInfo *klass, const char* name)
{
	while (klass)
	{
		void* iter = NULL;
		while (const PropertyInfo* prop = GetProperties (klass, &iter))
		{
			if (strcmp (name, Property::GetName (prop)) != 0)
				continue;

			return prop;
		}

		klass = klass->parent;
	}

	return NULL;
}

int32_t Class::GetValueSize (TypeInfo *klass, uint32_t *align)
{
	int32_t size;

	if (!klass->init_pending)
		Init (klass);

	assert (klass->valuetype);

	size = Class::GetInstanceSize (klass) - sizeof (Il2CppObject);
	
	if (align)
		*align = klass->minimumAlignment;

	return size;
}

bool Class::HasParent (TypeInfo *klass, TypeInfo *parent)
{
	Class::SetupTypeHierarchy (klass);
	Class::SetupTypeHierarchy (parent);

	return HasParentUnsafe (klass, parent);
}

bool Class::IsAssignableFrom (TypeInfo *klass, TypeInfo *oklass)
{
	// Cast to original class - fast path
	if (klass == oklass)
		return true;

	Class::Init(klass);
	Class::Init(oklass);

	// Following checks are always going to fail for interfaces
	if (!IsInterface (klass))
	{
		// Array
		if (klass->rank)
		{
			if (oklass->rank != klass->rank)
				return false;

			if (oklass->castClass->valuetype)
			{
				// Full array covariance is defined only for reference types.
				// For value types, array element reduced types must match
				return GetReducedType (klass->castClass) == GetReducedType (oklass->castClass);
			}

			return Class::IsAssignableFrom (klass->castClass, oklass->castClass);
		}
		// System.Object
		else if (klass == il2cpp_defaults.object_class)
		{
			return true;
		}
		// Left is System.Nullable<>
		else if (Class::IsNullable(klass))
		{
			if (Class::IsNullable(oklass))
				NOT_IMPLEMENTED (Class::IsAssignableFrom);
			TypeInfo* nullableArg = Class::GetNullableArgument(klass);
			return Class::IsAssignableFrom(nullableArg, oklass);
		}

		return HasParentUnsafe (oklass, klass);
	}

	while (oklass)
	{
		for (uint16_t i = 0; i < oklass->interfaces_count; i++)
		{
			if (oklass->implementedInterfaces[i] == klass)
				return true;
		}

		// Check the interfaces we may have grafted on to the type (e.g IList,
		// ICollection, IEnumerable for array types).
		for (uint16_t i = 0; i < oklass->interface_offsets_count; i++)
		{
			if (oklass->interfaceOffsets[i].interfaceType == klass)
				return true;
		}

		oklass = oklass->parent;
	}

	return false;
}

bool Class::IsGeneric (const TypeInfo *klass)
{
	return klass->is_generic;
}

bool Class::IsInflated(const TypeInfo *klass)
{
	return klass->generic_class != NULL;
}

bool Class::IsSubclassOf (TypeInfo *klass, TypeInfo *klassc, bool check_interfaces)
{
	Class::SetupTypeHierarchy (klass);
	Class::SetupTypeHierarchy (klassc);
	Class::SetupInterfaces (klass);

	if (check_interfaces && IsInterface (klassc) && !IsInterface (klass))
	{
		TypeInfo *oklass = klass;

		while(oklass)
		{
			Class::SetupInterfaces (oklass);
			// TODO: we probably need to implement a faster check here
			for (uint16_t i = 0; i < oklass->interfaces_count; i++)
			{
				if (oklass->implementedInterfaces[i] == klassc)
					return true;
			}

			oklass = oklass->parent;
		}

	}
	else if (check_interfaces && IsInterface (klassc) && IsInterface (klass))
	{
		// TODO: we probably need to implement a faster check here
		for (uint16_t i = 0; i < klass->interfaces_count; i++)
		{
			if (klass->implementedInterfaces[i] == klassc)
				return true;
		}
	}
	else
	{
		if (!IsInterface (klass) && HasParentUnsafe (klass, klassc))
			return true;
	}

	/*
	 * MS.NET thinks interfaces are a subclass of Object, so we think it as
	 * well.
	 */
	if (klassc == il2cpp_defaults.object_class)
		return true;

	return false;
}

bool Class::IsValuetype (const TypeInfo *klass)
{
	return klass->valuetype;
}


int32_t Class::GetInterfaceOffset (TypeInfo *klass, TypeInfo *itf)
{
	// TODO: this should only be needed in reflection case.
	// In codegen case we are operating on instance that must have
	// been initialized before we allocated it
	Class::Init (klass);
	for (uint16_t i = 0; i < klass->interface_offsets_count; i++)
	{
		if (klass->interfaceOffsets[i].interfaceType == itf)
		{
			int32_t offset = klass->interfaceOffsets[i].offset;
			assert (offset != -1);
			return offset;
		}
	}
	
	return -1;
}

enum FieldLayoutKind
{
	FIELD_LAYOUT_INSTANCE,
	FIELD_LAYOUT_STATIC,
	FIELD_LAYOUT_THREADSTATIC,
};


static void SetupFieldOffsets (FieldLayoutKind fieldLayoutKind, TypeInfo* klass, size_t size, const std::vector<size_t>& fieldOffsets)
{
	assert(size < std::numeric_limits<uint32_t>::max());
	if (fieldLayoutKind == FIELD_LAYOUT_INSTANCE)
		klass->instance_size = static_cast<uint32_t>(size);
	if (fieldLayoutKind == FIELD_LAYOUT_STATIC)
		klass->static_fields_size = static_cast<uint32_t>(size);
	if (fieldLayoutKind == FIELD_LAYOUT_THREADSTATIC)
		klass->thread_static_fields_size = static_cast<uint32_t>(size);

	if (!(klass->flags & TYPE_ATTRIBUTE_EXPLICIT_LAYOUT))
	{
		size_t fieldIndex = 0;
		for (uint16_t i = 0; i < klass->field_count; i++)
		{
			FieldInfo* field = klass->fields + i;
			if (fieldLayoutKind == FIELD_LAYOUT_INSTANCE && field->type->attrs & FIELD_ATTRIBUTE_STATIC)
				continue;
			if (fieldLayoutKind == FIELD_LAYOUT_STATIC && !Field::IsNormalStatic (field))
				continue;
			if (fieldLayoutKind == FIELD_LAYOUT_THREADSTATIC && !Field::IsThreadStatic (field))
				continue;

			if (fieldLayoutKind == FIELD_LAYOUT_THREADSTATIC)
			{
				field->offset = THREAD_STATIC_FIELD_OFFSET;
				fieldIndex++;
				continue;
			}

			field->offset = static_cast<int32_t>(fieldOffsets[fieldIndex]);
			fieldIndex++;
		}
	}
}

static void ValidateFieldOffsets (FieldLayoutKind fieldLayoutKind, TypeInfo* klass, size_t size, const std::vector<size_t>& fieldOffsets)
{
	if (fieldLayoutKind == FIELD_LAYOUT_INSTANCE && klass->parent && !(klass->flags & TYPE_ATTRIBUTE_EXPLICIT_LAYOUT))
		assert (klass->instance_size == size);
	if (fieldLayoutKind == FIELD_LAYOUT_STATIC)
		assert (klass->static_fields_size == size);
	if (fieldLayoutKind == FIELD_LAYOUT_THREADSTATIC)
	assert (klass->thread_static_fields_size == size);

	if (!(klass->flags & TYPE_ATTRIBUTE_EXPLICIT_LAYOUT))
	{
		size_t fieldIndex = 0;
		for (uint16_t i = 0; i < klass->field_count; i++)
		{
			FieldInfo* field = klass->fields + i;
			if (fieldLayoutKind == FIELD_LAYOUT_INSTANCE && field->type->attrs & FIELD_ATTRIBUTE_STATIC)
				continue;
			if (fieldLayoutKind == FIELD_LAYOUT_STATIC && !Field::IsNormalStatic (field))
				continue;
			if (fieldLayoutKind == FIELD_LAYOUT_THREADSTATIC && !Field::IsThreadStatic (field))
				continue;

			if (fieldLayoutKind == FIELD_LAYOUT_THREADSTATIC)
			{
				assert (fieldOffsets[fieldIndex] == -1);
				fieldIndex++;
				continue;
			}

			assert (field->offset == fieldOffsets[fieldIndex]);
			fieldIndex++;
		}
	}
}

static void LayoutFieldsLocked (TypeInfo *klass, const FastAutoLock& lock)
{
	if (Class::IsGeneric (klass))
		return;

	size_t instanceSize = 0;
	size_t actualSize = 0;
	if (klass->parent)
	{
		assert (klass->parent->size_inited);
		klass->has_references |= klass->parent->has_references;
		instanceSize = klass->parent->instance_size;
		actualSize = klass->parent->actualSize;
		if (klass->valuetype)
			klass->minimumAlignment = 1;
		else
			klass->minimumAlignment = klass->parent->minimumAlignment;
	}
	else
	{
		actualSize = instanceSize = sizeof (Il2CppObject);
		klass->minimumAlignment = sizeof (Il2CppObject*);
	}

	if (klass->field_count)
	{
		for (uint16_t i = 0; i < klass->field_count; i++)
		{
			FieldInfo* field = klass->fields + i;
			if (!Field::IsInstance (field))
				continue;

			const Il2CppType* ftype = Type::GetUnderlyingType (field->type);

			if (Type::IsEmptyType(ftype))
			{
				std::stringstream message;
				message << "The field '" << field->name << "' in type '" << klass->name << "' has a type which was not generated by il2cpp.exe. Consider using a generic type which is not nested so deeply.";
				il2cpp::vm::Exception::Raise(il2cpp::vm::Exception::GetExecutionEngineException(message.str().c_str()));
			}

			if (Type::IsReference (ftype) || (Type::IsStruct (ftype) && Class::HasReferences (Class::FromIl2CppType (ftype))))
				klass->has_references = true;
		}

		Il2CppTypeVector fieldTypes;
		Il2CppTypeVector staticFieldTypes;
		Il2CppTypeVector threadStaticFieldTypes;

		for (uint16_t i = 0; i < klass->field_count; i++)
		{
			FieldInfo* field = klass->fields + i;

			const Il2CppType* ftype = Type::GetUnderlyingType (field->type);

			if (Field::IsInstance (field))
				fieldTypes.push_back (ftype);
			else if (Field::IsNormalStatic (field))
				staticFieldTypes.push_back (ftype);
			else if (Field::IsThreadStatic (field))
				threadStaticFieldTypes.push_back (ftype);
		}

		FieldLayout::FieldLayoutData layoutData;
		FieldLayout::FieldLayoutData staticLayoutData;
		FieldLayout::FieldLayoutData threadStaticLayoutData;

		FieldLayout::LayoutFields (instanceSize, actualSize, klass->minimumAlignment, fieldTypes, layoutData);

		instanceSize = layoutData.classSize;

		// This is a value type with no instance fields, but at least one static field.
		if (klass->valuetype && fieldTypes.size() == 0)
		{
			instanceSize = IL2CPP_SIZEOF_STRUCT_WITH_NO_INSTANCE_FIELDS + sizeof(Il2CppObject);
			klass->actualSize = IL2CPP_SIZEOF_STRUCT_WITH_NO_INSTANCE_FIELDS + sizeof(Il2CppObject);
		}

		// need to set this in case there are no fields in a generic instance type
		if (klass->generic_class)
			klass->instance_size = static_cast<uint32_t>(instanceSize);

		klass->size_inited = true;

		FieldLayout::LayoutFields (0, 0, 1, staticFieldTypes, staticLayoutData);
		FieldLayout::LayoutFields (0, 0, 1, threadStaticFieldTypes, threadStaticLayoutData);

		klass->minimumAlignment = layoutData.minimumAlignment;
		klass->actualSize = static_cast<uint32_t>(layoutData.actualClassSize);

		size_t staticSize = staticLayoutData.classSize;
		size_t threadStaticSize = threadStaticLayoutData.classSize;

		for (size_t i = 0; i < threadStaticLayoutData.FieldOffsets.size (); ++i)
			threadStaticLayoutData.FieldOffsets[i] = -1;

		if (klass->generic_class)
		{
			SetupFieldOffsets (FIELD_LAYOUT_INSTANCE, klass, instanceSize, layoutData.FieldOffsets);
			SetupFieldOffsets (FIELD_LAYOUT_STATIC, klass, staticSize, staticLayoutData.FieldOffsets);
			SetupFieldOffsets (FIELD_LAYOUT_THREADSTATIC, klass, threadStaticSize, threadStaticLayoutData.FieldOffsets);
		}
		else
		{
#if IL2CPP_ENABLE_VALIDATE_FIELD_LAYOUT
			ValidateFieldOffsets (FIELD_LAYOUT_INSTANCE, klass, instanceSize, layoutData.FieldOffsets);
			ValidateFieldOffsets (FIELD_LAYOUT_STATIC, klass, staticSize, staticLayoutData.FieldOffsets);
			ValidateFieldOffsets (FIELD_LAYOUT_THREADSTATIC, klass, threadStaticSize, threadStaticLayoutData.FieldOffsets);
#endif
		}
	}
	else
	{
		// need to set this in case there are no fields in a generic instance type
		if (klass->generic_class)
			klass->instance_size = static_cast<uint32_t>(instanceSize);
		
		// Always set the actual size, as a derived class without fields could end up
		// with the wrong actual size (i.e. sizeof may be incorrect), if the last
		// field of the base class doesn't go to an alignment boundary and the compiler ABI
		// uses that extra space (as clang does).
		klass->actualSize = static_cast<uint32_t>(actualSize);
	}

	if (klass->static_fields_size)
	{
		klass->static_fields = il2cpp_gc_alloc_fixed(klass->static_fields_size, NULL);
		s_staticFieldData.push_back (klass);

		il2cpp_runtime_stats.class_static_data_size += klass->static_fields_size;
	}
	if (klass->thread_static_fields_size)
		klass->thread_static_fields_offset = il2cpp::vm::Thread::AllocThreadStaticData (klass->thread_static_fields_size);
}

static void SetupFieldsFromDefinition (TypeInfo* klass)
{
	if (klass->field_count == 0)
	{
		klass->fields = NULL;
		return;
	}

	FieldInfo* fields = (FieldInfo*)MetadataCalloc (klass->field_count, sizeof (FieldInfo));
	FieldInfo* newField = fields;

	FieldIndex start = klass->typeDefinition->fieldStart;
	assert (klass->typeDefinition->fieldStart != kFieldIndexInvalid);
	FieldIndex end = start + klass->field_count;

	for (FieldIndex fieldIndex = start; fieldIndex < end; ++fieldIndex)
	{
		const Il2CppFieldDefinition* fieldDefinition = MetadataCache::GetFieldDefinitionFromIndex (fieldIndex);

		newField->type = MetadataCache::GetIl2CppTypeFromIndex (fieldDefinition->typeIndex);
		newField->name = MetadataCache::GetStringFromIndex (fieldDefinition->nameIndex);
		newField->parent = klass;
		newField->offset = MetadataCache::GetFieldOffsetFromIndex (fieldIndex);
		newField->customAttributeIndex = fieldDefinition->customAttributeIndex;
		newField->token = fieldDefinition->token;

		newField++;
	}

	klass->fields = fields;
}

// passing lock to ensure we have acquired it. We can add asserts later
void SetupFieldsLocked (TypeInfo *klass, const FastAutoLock& lock)
{
	if (klass->size_inited)
		return;

	if (klass->parent && !klass->parent->size_inited)
		SetupFieldsLocked (klass->parent, lock);

	if (klass->generic_class)
	{
		// for generic instance types, they just inflate the fields of their generic type definition
		// initialize the generic type definition and delegate to the generic logic
		InitLocked (GenericClass::GetTypeDefinition (klass->generic_class), lock);
		GenericClass::SetupFields (klass);
	}
	else
	{
		SetupFieldsFromDefinition (klass);
	}

	if (!Class::IsGeneric (klass))
		LayoutFieldsLocked (klass, lock);

	klass->size_inited = true;
}

void Class::SetupFields (TypeInfo *klass)
{
	if (!klass->size_inited)
	{
		FastAutoLock lock (&g_MetadataLock);
		SetupFieldsLocked (klass, lock);
	}
}

// passing lock to ensure we have acquired it. We can add asserts later
void SetupMethodsLocked (TypeInfo *klass, const FastAutoLock& lock)
{
	if ((!klass->method_count && !klass->rank) || klass->methods)
		return;

	if (klass->generic_class)
	{
		InitLocked (GenericClass::GetTypeDefinition (klass->generic_class), lock);
		GenericClass::SetupMethods (klass);
	}
	else if (klass->rank)
	{
		InitLocked (klass->element_class, lock);
		ArrayMetadata::SetupArrayVTable (klass, lock);
	}
	else
	{
		if (klass->method_count == 0)
		{
			klass->methods = NULL;
			return;
		}

		klass->methods = (const MethodInfo**)IL2CPP_CALLOC (klass->method_count, sizeof (MethodInfo*));
		MethodInfo* methods = (MethodInfo*)IL2CPP_CALLOC (klass->method_count, sizeof (MethodInfo));
		MethodInfo* newMethod = methods;

		MethodIndex start = klass->typeDefinition->methodStart;
		assert (start != kFieldIndexInvalid);
		MethodIndex end = start + klass->method_count;

		for (MethodIndex index = start; index < end; ++index)
		{
			const Il2CppMethodDefinition* methodDefinition = MetadataCache::GetMethodDefinitionFromIndex (index);

			newMethod->name = MetadataCache::GetStringFromIndex (methodDefinition->nameIndex);
			newMethod->method = MetadataCache::GetMethodPointerFromIndex (methodDefinition->methodIndex);
			newMethod->invoker_method = MetadataCache::GetMethodInvokerFromIndex (methodDefinition->invokerIndex);
			newMethod->declaring_type = klass;
			newMethod->return_type = MetadataCache::GetIl2CppTypeFromIndex (methodDefinition->returnType);

			ParameterInfo* parameters = (ParameterInfo*)IL2CPP_CALLOC (methodDefinition->parameterCount, sizeof (ParameterInfo));
			ParameterInfo* newParameter = parameters;
			for (uint16_t paramIndex = 0; paramIndex < methodDefinition->parameterCount; ++paramIndex)
			{
				const Il2CppParameterDefinition* parameterDefinition = MetadataCache::GetParameterDefinitionFromIndex (methodDefinition->parameterStart + paramIndex);
				newParameter->name = MetadataCache::GetStringFromIndex (parameterDefinition->nameIndex);
				newParameter->position = paramIndex;
				newParameter->token = parameterDefinition->token;
				newParameter->customAttributeIndex = parameterDefinition->customAttributeIndex;
				newParameter->parameter_type = MetadataCache::GetIl2CppTypeFromIndex (parameterDefinition->typeIndex);

				newParameter++;
			}
			newMethod->parameters = parameters;

			newMethod->customAttributeIndex = methodDefinition->customAttributeIndex;
			newMethod->flags = methodDefinition->flags;
			newMethod->iflags = methodDefinition->iflags;
			newMethod->slot = methodDefinition->slot;
			newMethod->parameters_count = static_cast<const uint8_t>(methodDefinition->parameterCount);
			newMethod->is_inflated = false;
			newMethod->token = methodDefinition->token;
			newMethod->methodDefinition = methodDefinition;
			newMethod->genericContainer = MetadataCache::GetGenericContainerFromIndex (methodDefinition->genericContainerIndex);
			if (newMethod->genericContainer)
				newMethod->is_generic = true;

			klass->methods[index - start] = newMethod;

			newMethod++;
		}
	}
}

void Class::SetupMethods (TypeInfo *klass)
{
	if (klass->method_count || klass->rank)
	{
		FastAutoLock lock (&g_MetadataLock);
		SetupMethodsLocked (klass, lock);
	}
}

void SetupNestedTypesLocked (TypeInfo *klass, const FastAutoLock& lock)
{
	if (klass->generic_class)
		return;

	if (klass->nested_type_count > 0)
	{
		klass->nestedTypes = (TypeInfo**)MetadataCalloc (klass->nested_type_count, sizeof (TypeInfo*));
		for (uint16_t i = 0; i < klass->nested_type_count; i++)
			klass->nestedTypes[i] = MetadataCache::GetNestedTypeFromIndex (klass->typeDefinition->nestedTypesStart + i);
	}
}

void Class::SetupNestedTypes (TypeInfo *klass)
{
	if (klass->generic_class)
		return;
	
	if (klass->nested_type_count)
	{
		FastAutoLock lock (&g_MetadataLock);
		SetupNestedTypesLocked (klass, lock);
	}
}

static void SetupVTable (TypeInfo *klass, const FastAutoLock& lock)
{
	if (klass->generic_class)
	{
		TypeInfo* genericTypeDefinition = GenericClass::GetTypeDefinition (klass->generic_class);
		Il2CppGenericContext* context = &klass->generic_class->context;
		if (genericTypeDefinition->interface_offsets_count > 0 && klass->interfaceOffsets == NULL)
		{
			klass->interface_offsets_count = genericTypeDefinition->interface_offsets_count;
			klass->interfaceOffsets = (Il2CppRuntimeInterfaceOffsetPair*)MetadataCalloc (genericTypeDefinition->interface_offsets_count, sizeof (Il2CppRuntimeInterfaceOffsetPair));
			for (uint16_t i = 0; i < genericTypeDefinition->interface_offsets_count; i++)
			{
				Il2CppInterfaceOffsetPair interfaceOffset = MetadataCache::GetInterfaceOffsetIndex (genericTypeDefinition->typeDefinition->interfaceOffsetsStart + i);
				klass->interfaceOffsets[i].offset = interfaceOffset.offset;
				klass->interfaceOffsets[i].interfaceType = Class::FromIl2CppType (GenericMetadata::InflateIfNeeded (MetadataCache::GetIl2CppTypeFromIndex (interfaceOffset.interfaceTypeIndex), context, false));
			}
		}

		if (genericTypeDefinition->vtable_count > 0)
		{
			klass->vtable_count = genericTypeDefinition->vtable_count;
			klass->vtable = (const MethodInfo**)MetadataCalloc (genericTypeDefinition->vtable_count, sizeof (MethodInfo*));
			for (uint16_t i = 0; i < genericTypeDefinition->vtable_count; i++)
			{
				EncodedMethodIndex vtableMethodIndex = MetadataCache::GetVTableMethodFromIndex (genericTypeDefinition->typeDefinition->vtableStart + i);
				const MethodInfo* method = MetadataCache::GetMethodInfoFromIndex (vtableMethodIndex);
				if (GetEncodedIndexType (vtableMethodIndex) == kIl2CppMetadataUsageMethodRef)
				{
					const Il2CppGenericMethod* genericMethod = GenericMetadata::Inflate (method->genericMethod, context);
					klass->vtable[i] = GenericMethod::GetMethod (genericMethod);
				}
				else
				{
					if (method && Class::IsGeneric (method->declaring_type))
					{
						const Il2CppGenericMethod* gmethod = MetadataCache::GetGenericMethod (method, context->class_inst, NULL);
						method = GenericMethod::GetMethod (gmethod);
					}
					klass->vtable[i] = method;
				}
			}
		}
	}
	else if (klass->rank)
	{
		InitLocked (klass->element_class, lock);
		ArrayMetadata::SetupArrayVTable (klass, lock);
	}
	else
	{
		if (klass->interface_offsets_count > 0 && klass->interfaceOffsets == NULL)
		{
			klass->interfaceOffsets = (Il2CppRuntimeInterfaceOffsetPair*)MetadataCalloc (klass->interface_offsets_count, sizeof (Il2CppRuntimeInterfaceOffsetPair));
			for (uint16_t i = 0; i < klass->interface_offsets_count; i++)
			{
				Il2CppInterfaceOffsetPair interfaceOffset = MetadataCache::GetInterfaceOffsetIndex (klass->typeDefinition->interfaceOffsetsStart + i);
				klass->interfaceOffsets[i].offset = interfaceOffset.offset;
				klass->interfaceOffsets[i].interfaceType = Class::FromIl2CppType (MetadataCache::GetIl2CppTypeFromIndex (interfaceOffset.interfaceTypeIndex));
			}
		}

		if (klass->vtable_count > 0)
		{
			klass->vtable = (const MethodInfo**)MetadataCalloc (klass->vtable_count, sizeof (MethodInfo*));
			for (uint16_t i = 0; i < klass->vtable_count; i++)
			{
				klass->vtable[i] = MetadataCache::GetMethodInfoFromIndex (MetadataCache::GetVTableMethodFromIndex (klass->typeDefinition->vtableStart + i));
			}
		}
	}
}

static void SetupEventsLocked (TypeInfo *klass, const FastAutoLock& lock)
{
	if (klass->generic_class)
	{
		InitLocked (GenericClass::GetTypeDefinition (klass->generic_class), lock);
		GenericClass::SetupEvents (klass);
	}
	else if (klass->rank > 0)
	{
		// do nothing, arrays have no events
		assert (klass->event_count == 0);
	}
	else if (klass->event_count != 0)
	{
		// we need methods initialized since we reference them via index below
		SetupMethodsLocked (klass, lock);

		EventInfo* events = (EventInfo*)IL2CPP_CALLOC (klass->event_count, sizeof (EventInfo));
		EventInfo* newEvent = events;

		EventIndex start = klass->typeDefinition->eventStart;
		assert (klass->typeDefinition->eventStart != kEventIndexInvalid);
		EventIndex end = start + klass->event_count;

		for (EventIndex eventIndex = start; eventIndex < end; ++eventIndex)
		{
			const Il2CppEventDefinition* eventDefinition = MetadataCache::GetEventDefinitionFromIndex (eventIndex);

			newEvent->eventType = MetadataCache::GetIl2CppTypeFromIndex (eventDefinition->typeIndex);
			newEvent->name = MetadataCache::GetStringFromIndex (eventDefinition->nameIndex);
			newEvent->parent = klass;

			if (eventDefinition->add != kMethodIndexInvalid)
				newEvent->add = klass->methods[eventDefinition->add];

			if (eventDefinition->remove != kMethodIndexInvalid)
				newEvent->remove = klass->methods[eventDefinition->remove];

			if (eventDefinition->raise != kMethodIndexInvalid)
				newEvent->raise = klass->methods[eventDefinition->raise];

			newEvent->customAttributeIndex = eventDefinition->customAttributeIndex;
			newEvent->token = eventDefinition->token;

			newEvent++;
		}

		klass->events = events;
	}
}


void Class::SetupEvents (TypeInfo *klass)
{
	if (!klass->events && klass->event_count)
	{
		FastAutoLock lock (&g_MetadataLock);
		SetupEventsLocked (klass, lock);
	}
}

static void SetupPropertiesLocked (TypeInfo *klass, const FastAutoLock& lock)
{
	if (klass->generic_class)
	{
		InitLocked (GenericClass::GetTypeDefinition (klass->generic_class), lock);
		GenericClass::SetupProperties (klass);
	}
	else if (klass->property_count != 0)
	{
		// we need methods initialized since we reference them via index below
		SetupMethodsLocked (klass, lock);

		PropertyInfo* properties = (PropertyInfo*)IL2CPP_CALLOC (klass->property_count, sizeof (PropertyInfo));
		PropertyInfo* newProperty = properties;

		PropertyIndex start = klass->typeDefinition->propertyStart;
		assert (klass->typeDefinition->propertyStart != kPropertyIndexInvalid);
		PropertyIndex end = start + klass->property_count;

		for (PropertyIndex propertyIndex = start; propertyIndex < end; ++propertyIndex)
		{
			const Il2CppPropertyDefinition* propertyDefinition = MetadataCache::GetPropertyDefinitionFromIndex (propertyIndex);
			
			newProperty->name = MetadataCache::GetStringFromIndex (propertyDefinition->nameIndex);
			newProperty->parent = klass;

			if (propertyDefinition->get != kMethodIndexInvalid)
				newProperty->get = klass->methods[propertyDefinition->get];

			if (propertyDefinition->set != kMethodIndexInvalid)
				newProperty->set = klass->methods[propertyDefinition->set];

			newProperty->attrs = propertyDefinition->attrs;
			newProperty->customAttributeIndex = propertyDefinition->customAttributeIndex;
			newProperty->token = propertyDefinition->token;

			newProperty++;
		}

		klass->properties = properties;
	}
}

void Class::SetupProperties (TypeInfo *klass)
{
	if (!klass->properties && klass->property_count)
	{
		FastAutoLock lock (&g_MetadataLock);
		SetupPropertiesLocked (klass, lock);
	}
}

static void SetupTypeHierarchyLocked (TypeInfo *klass, const FastAutoLock& lock)
{
	if (klass->typeHierarchy != NULL)
		return;

	if (klass->parent && !klass->parent->typeHierarchy)
		SetupTypeHierarchyLocked (klass->parent, lock);
	if (klass->parent)
		klass->typeHierarchyDepth = klass->parent->typeHierarchyDepth + 1;
	else
		klass->typeHierarchyDepth = 1;

	klass->typeHierarchy = (TypeInfo**)MetadataCalloc (klass->typeHierarchyDepth, sizeof (TypeInfo*));

	if (klass->parent) {
		klass->typeHierarchy [klass->typeHierarchyDepth - 1] = klass;
		memcpy (klass->typeHierarchy, klass->parent->typeHierarchy, klass->parent->typeHierarchyDepth * sizeof (void*));
	} else {
		klass->typeHierarchy [0] = klass;
	}
}

void Class::SetupTypeHierarchy (TypeInfo *klass)
{
	FastAutoLock lock (&g_MetadataLock);
	SetupTypeHierarchyLocked (klass, lock);
}

void Class::SetupInterfaces (TypeInfo *klass)
{
	FastAutoLock lock (&g_MetadataLock);
	SetupInterfacesLocked (klass, lock);
}

static bool InitLocked (TypeInfo *klass, const FastAutoLock& lock)
{
	if (klass->initialized)
		return true;

	NOT_IMPLEMENTED_NO_ASSERT (Class::Init, "Audit and compare to mono version");

	klass->init_pending = true;

	if (klass->generic_class)
		InitLocked (GenericClass::GetTypeDefinition (klass->generic_class), lock);

	if (klass->byval_arg->type == IL2CPP_TYPE_ARRAY || klass->byval_arg->type == IL2CPP_TYPE_SZARRAY)
	{
		TypeInfo *element_class = klass->element_class;
		if (!element_class->initialized)
			InitLocked (element_class, lock);
	}

	SetupInterfacesLocked (klass, lock);

	if (klass->parent && !klass->parent->initialized)
		InitLocked (klass->parent, lock);

	SetupMethodsLocked (klass, lock);
	SetupTypeHierarchyLocked (klass, lock);
	SetupVTable (klass, lock);
	if (!klass->size_inited)
		SetupFieldsLocked (klass, lock);

	SetupEventsLocked (klass, lock);
	SetupPropertiesLocked (klass, lock);
	SetupNestedTypesLocked (klass, lock);

	if (klass == il2cpp_defaults.object_class)
	{
		for (uint16_t slot = 0; slot < klass->vtable_count; slot++)
		{
			const MethodInfo* vmethod = klass->vtable[slot];
			if (!strcmp (vmethod->name, "GetHashCode"))
				s_GetHashCodeSlot = slot;
			else if (!strcmp (vmethod->name, "Finalize"))
				s_FinalizerSlot = slot;
		}
		assert (s_FinalizerSlot > 0);
		assert (s_GetHashCodeSlot > 0);
	}

	if (!Class::IsGeneric (klass))
		SetupGCDescriptor (klass);

#if IL2CPP_DEBUGGER_ENABLED
	// Gab: not sure this is the best place to put the typeload event notification,
	// we probably want to send a typeload event for all the know types at startup.
	il2cpp_debugger_notify_type_load (klass);
#endif

	if (klass->generic_class)
	{
		const Il2CppTypeDefinition* typeDefinition = GenericClass::GetTypeDefinition (klass->generic_class)->typeDefinition;
		klass->rgctx_data = GenericMetadata::InflateRGCTX (typeDefinition->rgctxStartIndex, typeDefinition->rgctxCount, &klass->generic_class->context);
	}

	klass->initialized = true;
	klass->init_pending = false;

	++il2cpp_runtime_stats.initialized_class_count;

	return true;
}

bool Class::Init (TypeInfo *klass)
{
	if (!klass->initialized)
	{
		FastAutoLock lock (&g_MetadataLock);
		InitLocked (klass, lock);
	}

	return true;
}

TypeInfo* Class::FromName (Il2CppImage* image, const char* namespaze, const char *name)
{
	return Image::ClassFromName (image, namespaze, name);
}

TypeInfo* Class::GetArrayClass (TypeInfo *element_class, uint32_t rank)
{
	return GetBoundedArrayClass (element_class, rank, false);
}

TypeInfo* Class::GetBoundedArrayClass (TypeInfo *eclass, uint32_t rank, bool bounded)
{
	return ArrayMetadata::GetBoundedArrayClass (eclass, rank, bounded);
}

TypeInfo* Class::GetInflatedGenericInstanceClass (TypeInfo* klass, const metadata::Il2CppTypeVector& types)
{
	assert (Class::IsGeneric (klass));

	const Il2CppGenericInst* inst = MetadataCache::GetGenericInst (types);
	Il2CppGenericClass* gclass = GenericMetadata::GetGenericClass (klass, inst);

	return GenericClass::GetClass (gclass);
}

TypeInfo* Class::InflateGenericClass (TypeInfo* klass, Il2CppGenericContext *context)
{
	const Il2CppType* inflated = InflateGenericType (klass->byval_arg, context);

	return FromIl2CppType (inflated);
}

const Il2CppType* Class::InflateGenericType (const Il2CppType* type, Il2CppGenericContext* context)
{
	return GenericMetadata::InflateIfNeeded (type, context, true);
}

bool Class::HasDefaultConstructor (TypeInfo* klass)
{
	const char ctorName[] = ".ctor";
	void* iter = NULL;
	while (const MethodInfo* method = Class::GetMethods (klass, &iter))
	{
		if (strncmp(method->name, ctorName, utils::StringUtils::LiteralLength(ctorName)) == 0 && method->parameters_count == 0)
			return true;
	}

	return false;
}

int Class::GetFlags (const TypeInfo *klass)
{
	return klass->flags;
}

bool Class::IsAbstract (const TypeInfo *klass)
{
	return (klass->flags & TYPE_ATTRIBUTE_ABSTRACT) != 0;
}

bool Class::IsInterface (const TypeInfo *klass)
{
	return (klass->flags & TYPE_ATTRIBUTE_INTERFACE) || (klass->byval_arg->type == IL2CPP_TYPE_VAR) || (klass->byval_arg->type == IL2CPP_TYPE_MVAR);
}

bool Class::IsNullable (const TypeInfo *klass)
{
	return klass->generic_class != NULL &&
		GenericClass::GetTypeDefinition (klass->generic_class) == il2cpp_defaults.generic_nullable_class;
}

TypeInfo* Class::GetNullableArgument(const TypeInfo* klass)
{
	return Class::FromIl2CppType(klass->generic_class->context.class_inst->type_argv[0]);
}

int Class::GetArrayElementSize (const TypeInfo *klass)
{
	const Il2CppType *type = klass->byval_arg;
	
handle_enum:
	switch (type->type)
	{
	case IL2CPP_TYPE_I1:
	case IL2CPP_TYPE_U1:
	case IL2CPP_TYPE_BOOLEAN:
		return 1;

	case IL2CPP_TYPE_I2:
	case IL2CPP_TYPE_U2:
	case IL2CPP_TYPE_CHAR:
		return 2;

	case IL2CPP_TYPE_I4:
	case IL2CPP_TYPE_U4:
	case IL2CPP_TYPE_R4:
		return 4;

	case IL2CPP_TYPE_I:
	case IL2CPP_TYPE_U:
	case IL2CPP_TYPE_PTR:
	case IL2CPP_TYPE_CLASS:
	case IL2CPP_TYPE_STRING:
	case IL2CPP_TYPE_OBJECT:
	case IL2CPP_TYPE_SZARRAY:
	case IL2CPP_TYPE_ARRAY:
	case IL2CPP_TYPE_VAR:
	case IL2CPP_TYPE_MVAR:
		return sizeof (Il2CppObject*);

	case IL2CPP_TYPE_I8:
	case IL2CPP_TYPE_U8:
	case IL2CPP_TYPE_R8:
		return 8;

	case IL2CPP_TYPE_VALUETYPE:
		if (Type::IsEnum (type))
		{
			type = Class::GetEnumBaseType (Type::GetClass (type));
			klass = klass->element_class;
			goto handle_enum;
		}
		return Class::GetInstanceSize (klass) - sizeof (Il2CppObject);

	case IL2CPP_TYPE_GENERICINST:
		type = GenericClass::GetTypeDefinition (type->data.generic_class)->byval_arg;
		goto handle_enum;

	case IL2CPP_TYPE_VOID:
		return 0;
		
	default:
		// g_error ("unknown type 0x%02x in mono_class_array_element_size", type->type);
		NOT_IMPLEMENTED_NO_ASSERT (Class::GetArrayElementSize, "Should throw a nice error");
	}

	return -1;
}

const Il2CppType* Class::GetType (TypeInfo *klass)
{
	return klass->byval_arg;
}

const Il2CppType* Class::GetType (TypeInfo *klass, const TypeNameParseInfo &info)
{
	// Attempt to resolve a generic type definition.
	if (Class::IsGeneric (klass))
		klass = ResolveGenericInstanceType(klass, info, false, false);

	bool bounded = false;

	std::vector<int32_t>::const_iterator it = info.modifiers ().begin ();

	if (info.is_bounded ())
	{
		bounded = false;
	}

	while (it != info.modifiers ().end ())
	{
		if (*it == 0)
		{
			// byref is always the last modifier, so we can return here.
			return klass->this_arg;
		}
		
		if (*it == -1)
		{
			klass = Class::GetPtrClass (klass);
		} else if (*it == -2)
		{
			bounded = true;
		} else
		{
			klass = Class::GetBoundedArrayClass (klass, *it, bounded);
		}

		++it;
	}

	if (klass == NULL)
		return NULL;

	return klass->byval_arg;
}

bool Class::HasAttribute (TypeInfo *klass, TypeInfo *attr_class)
{
	return Reflection::HasAttribute (klass, attr_class);
}

bool Class::IsEnum (const TypeInfo *klass)
{
	return klass->enumtype;
}

const Il2CppImage* Class::GetImage (TypeInfo *klass)
{
	return klass->image;
}

const Il2CppGenericContainer* Class::GetGenericContainer (TypeInfo *klass)
{
	return MetadataCache::GetGenericContainerFromIndex (klass->genericContainerIndex);
}

const MethodInfo* Class::GetCCtor (TypeInfo *klass)
{
	if (!klass->has_cctor)
		return NULL;

	return GetMethodFromNameFlags (klass, ".cctor", -1, METHOD_ATTRIBUTE_SPECIAL_NAME);
}

const char* Class::GetFieldDefaultValue (const FieldInfo *field, const Il2CppType** type)
{
	const Il2CppFieldDefaultValue *entry = MetadataCache::GetFieldDefaultValueForField (field);
	if (entry != NULL)
	{
		*type = MetadataCache::GetIl2CppTypeFromIndex (entry->typeIndex);
		if (entry->dataIndex == kDefaultValueIndexNull)
			return NULL;

		return (const char*)MetadataCache::GetFieldDefaultValueDataFromIndex (entry->dataIndex);
	}
	return NULL;
}

int Class::GetFieldMarshaledSize(const FieldInfo *field)
{
	int marshaledFieldSize = MetadataCache::GetFieldMarshaledSizeForField(field);
	if (marshaledFieldSize != -1)
		return marshaledFieldSize;

	if (field->type->type == IL2CPP_TYPE_BOOLEAN)
		return 4;
	if (field->type->type == IL2CPP_TYPE_CHAR)
		return 1;

	size_t size = metadata::FieldLayout::GetTypeSizeAndAlignment(field->type).size;
	assert(size < static_cast<size_t>(std::numeric_limits<int>::max()));
	return static_cast<int>(size);
}

TypeInfo* Class::GetPtrClass (const Il2CppType* type)
{
	return GetPtrClass (Class::FromIl2CppType (type));
}

TypeInfo* Class::GetPtrClass (TypeInfo* elementClass)
{
	FastAutoLock lock(&g_MetadataLock);

	TypeInfo* pointerClass = MetadataCache::GetPointerType(elementClass);
	if (pointerClass)
		return pointerClass;

	pointerClass = (TypeInfo*)MetadataCalloc (1, sizeof (TypeInfo));

	pointerClass->namespaze = elementClass->namespaze;
	pointerClass->name = il2cpp::utils::StringUtils::StringDuplicate(il2cpp::utils::StringUtils::Printf("%s*", elementClass->name).c_str());

	pointerClass->image = elementClass->image;
	pointerClass->initialized = true;
	pointerClass->flags = TYPE_ATTRIBUTE_CLASS | (elementClass->flags & TYPE_ATTRIBUTE_VISIBILITY_MASK);
	pointerClass->instance_size = sizeof(void*);

	Il2CppType* this_arg = (Il2CppType*)MetadataCalloc (1, sizeof (Il2CppType));
	Il2CppType* byval_arg = (Il2CppType*)MetadataCalloc (1, sizeof (Il2CppType));
	this_arg->type = byval_arg->type = IL2CPP_TYPE_PTR;
	this_arg->data.type = byval_arg->data.type = elementClass->byval_arg;
	this_arg->byref = true;

	pointerClass->this_arg = this_arg;
	pointerClass->byval_arg = byval_arg;

	pointerClass->parent = NULL;
	pointerClass->castClass = pointerClass->element_class = elementClass;

	MetadataCache::AddPointerType(elementClass, pointerClass);

	return pointerClass;
}

bool Class::HasReferences (TypeInfo *klass)
{
	if (klass->init_pending) {
		/* Be conservative */
		return true;
	} else {
		Init (klass);

		return klass->has_references;
	}
}

const dynamic_array<TypeInfo*>& Class::GetStaticFieldData ()
{
	return s_staticFieldData;
}

const Il2CppDebugTypeInfo *Class::GetDebugInfo (const TypeInfo *klass)
{
#if IL2CPP_DEBUGGER_ENABLED
	return klass->debug_info;
#else
	return NULL;
#endif
}

const size_t kWordSize = (8 * sizeof (size_t));

static inline void set_bit (size_t* bitmap, size_t index)
{
	bitmap[index / kWordSize] |= (size_t)1 << (index % kWordSize);
}

size_t Class::GetBitmapSize (const TypeInfo* klass)
{
	size_t maxBits = klass->instance_size / sizeof (void*);
	size_t maxWords = 1 + (maxBits / sizeof (size_t));
	return sizeof (size_t) * maxWords;
}

void Class::GetBitmap (TypeInfo* klass, size_t* bitmap, size_t& maxSetBit)
{
	Class::Init (klass);
	return il2cpp::vm::GetBitmapNoInit (klass, bitmap, maxSetBit, 0);
}

const char *Class::GetAssemblyName (const TypeInfo *klass)
{
	return klass->image->name;
}

void GetBitmapNoInit (TypeInfo* klass, size_t* bitmap, size_t& maxSetBit, size_t parentOffset)
{
	TypeInfo* currentClass = klass;

	while (currentClass)
	{
		for (uint16_t index = 0; index < currentClass->field_count; index++)
		{
			FieldInfo* field = currentClass->fields + index;
			if (field->type->attrs & (FIELD_ATTRIBUTE_STATIC | FIELD_ATTRIBUTE_HAS_FIELD_RVA))
				continue;

			assert (!field->type->byref);

			size_t offset = parentOffset + field->offset;

			const Il2CppType* type = Type::GetUnderlyingType (field->type);

			switch (type->type)
			{
			case IL2CPP_TYPE_I1:
			case IL2CPP_TYPE_U1:
			case IL2CPP_TYPE_BOOLEAN:
			case IL2CPP_TYPE_I2:
			case IL2CPP_TYPE_U2:
			case IL2CPP_TYPE_CHAR:
			case IL2CPP_TYPE_I4:
			case IL2CPP_TYPE_U4:
			case IL2CPP_TYPE_I8:
			case IL2CPP_TYPE_U8:
			case IL2CPP_TYPE_I:
			case IL2CPP_TYPE_U:
			case IL2CPP_TYPE_R4:
			case IL2CPP_TYPE_R8:
			case IL2CPP_TYPE_PTR:
			case IL2CPP_TYPE_FNPTR:
				break;
			case IL2CPP_TYPE_STRING:
			case IL2CPP_TYPE_SZARRAY:
			case IL2CPP_TYPE_CLASS:
			case IL2CPP_TYPE_OBJECT:
			case IL2CPP_TYPE_ARRAY:
			case IL2CPP_TYPE_VAR:
			case IL2CPP_TYPE_MVAR:
				assert(0 == (field->offset % sizeof(void*)));
				set_bit (bitmap, offset / sizeof (void*));
				maxSetBit = std::max (maxSetBit, offset / sizeof (void*));
				break;
			case IL2CPP_TYPE_GENERICINST:
				if (!Type::GenericInstIsValuetype (type))
				{
					assert (0 == (field->offset % sizeof (void*)));
					set_bit (bitmap, offset / sizeof (void*));
					maxSetBit = std::max (maxSetBit, offset / sizeof (void*));
					break;
				}
				else
				{
					/* fall through */
				}
			case IL2CPP_TYPE_VALUETYPE:
			{
				TypeInfo* fieldClass = Class::FromIl2CppType (field->type);
				Class::Init (fieldClass);
				if (fieldClass->has_references)
					GetBitmapNoInit (fieldClass, bitmap, maxSetBit, offset - sizeof (Il2CppObject) /* nested field offset includes padding for boxed structure. Remove for struct fields */);
				break;
			}
			default:
				NOT_IMPLEMENTED (Class::GetClassBitmap);
				break;
			}
		}

		currentClass = currentClass->parent;
	}
}

void SetupGCDescriptor (TypeInfo* klass)
{
	const size_t kMaxAllocaSize = 1024;
	size_t bitmapSize = Class::GetBitmapSize (klass);
	size_t* bitmap = NULL;
	std::vector<size_t> buffer (0);

	if (bitmapSize > kMaxAllocaSize)
	{
		buffer.resize (bitmapSize / sizeof(size_t));
		bitmap = (size_t*)&buffer[0];
	}
	else
	{
		bitmap = (size_t*)alloca (bitmapSize);
	}

	memset (bitmap, 0, bitmapSize);
	size_t maxSetBit = 0;
	GetBitmapNoInit (klass, bitmap, maxSetBit, 0);

	if (klass == il2cpp_defaults.string_class)
		klass->gc_desc = GC_NO_DESCRIPTOR;
	else if (klass->rank)
		klass->gc_desc = GC_NO_DESCRIPTOR;
	else
		klass->gc_desc = il2cpp_gc_make_descr_for_object (bitmap, (int)maxSetBit + 1);
}

#define CHECK_IF_NULL(v)	\
	if ( (v) == NULL && throwOnError ) \
		Exception::Raise (Exception::GetTypeLoadException ()); \
	if ( (v) == NULL ) \
		return NULL;

static TypeInfo * resolve_generic_instance_internal(const il2cpp::vm::TypeNameParseInfo &info, TypeInfo *generic_class, Il2CppTypeVector &generic_arguments, bool throwOnError)
{
	TypeInfo *klass = NULL;

	const Il2CppGenericContainer* container = Class::GetGenericContainer (generic_class);
	if (container->type_argc != generic_arguments.size ())
		il2cpp::vm::Exception::Raise(il2cpp::vm::Exception::GetArgumentException("name", "The number of generic arguments provided doesn't equal the arity of the generic type definition."));

	if (info.assembly_name().name.empty())
	{
		const Il2CppImage* image = Image::GetExecutingImage();
		klass = MetadataCache::GetGenericInstanceType(generic_class, generic_arguments);

		if (klass == NULL && image != Image::GetCorlib())
		{
			// Try mscorlib
			image = (Il2CppImage*)Image::GetCorlib();
			klass = MetadataCache::GetGenericInstanceType(generic_class, generic_arguments);
		}
	}
	else
	{
		const Il2CppAssembly *assembly = Assembly::Load(info.assembly_name().name.c_str());

		CHECK_IF_NULL(assembly);

		Il2CppImage *image = (Il2CppImage*)Assembly::GetImage(assembly);

		CHECK_IF_NULL(image);

		klass = MetadataCache::GetGenericInstanceType(generic_class, generic_arguments);
	}

	return klass;
}

static TypeInfo* ResolveGenericInstanceType(TypeInfo* klass, const TypeNameParseInfo& info, bool throwOnError, bool ignoreCase)
{
	if (info.has_generic_arguments())
	{
		Il2CppTypeVector generic_arguments;
		generic_arguments.reserve(info.type_arguments().size());

		std::vector<TypeNameParseInfo>::const_iterator it = info.type_arguments().begin();
		while (it != info.type_arguments().end())
		{
			const Il2CppType * generic_argument = Class::il2cpp_type_from_type_info(*it, throwOnError, ignoreCase);

			CHECK_IF_NULL(generic_argument);

			generic_arguments.push_back(generic_argument);

			++it;
		}

		klass = resolve_generic_instance_internal(info, klass, generic_arguments, throwOnError);

		CHECK_IF_NULL(klass);
	}

	if (klass != NULL)
		Class::Init(klass);

	return klass;
}

static TypeInfo* resolve_parse_info_internal(const TypeNameParseInfo& info, bool throwOnError, bool ignoreCase)
{
	TypeInfo *klass = NULL;

	if (info.assembly_name().name.empty())
	{
		const Il2CppImage* image = Image::GetExecutingImage();
		klass = Image::FromTypeNameParseInfo(image, info, ignoreCase);

		if (klass == NULL && image != Image::GetCorlib())
		{
			// Try mscorlib
			image = (Il2CppImage*)Image::GetCorlib();
			klass = Image::FromTypeNameParseInfo(image, info, ignoreCase);
		}
	}
	else
	{
		const Il2CppAssembly *assembly = Assembly::Load(info.assembly_name().name.c_str());

		CHECK_IF_NULL(assembly);

		Il2CppImage *image = (Il2CppImage*)Assembly::GetImage(assembly);

		CHECK_IF_NULL(image);

		klass = Image::FromTypeNameParseInfo(image, info, ignoreCase);
	}

	return klass;
}

const Il2CppType* Class::il2cpp_type_from_type_info(const TypeNameParseInfo& info, bool throwOnError, bool ignoreCase)
{
	TypeInfo *klass = resolve_parse_info_internal(info, throwOnError, ignoreCase);

	CHECK_IF_NULL(klass);

	klass = ResolveGenericInstanceType(klass, info, throwOnError, ignoreCase);

	CHECK_IF_NULL(klass);

	const Il2CppType *type = Class::GetType(klass, info);

	CHECK_IF_NULL(type);

	return type;
}

TypeInfo* Class::GetDeclaringType(TypeInfo* klass)
{
	return klass->declaringType;
}

} /* namespace vm */
} /* namespace il2cpp */
