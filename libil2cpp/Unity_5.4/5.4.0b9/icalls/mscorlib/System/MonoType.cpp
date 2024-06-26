#include "il2cpp-config.h"
#include "class-internals.h"
#include "object-internals.h"
#include <cassert>
#include "tabledefs.h"
#include "gc/Allocator.h"
#include "icalls/mscorlib/System/MonoType.h"
#include "utils/Functional.h"
#include "utils/StringUtils.h"
#include "utils/StdUnorderedMap.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Field.h"
#include "vm/GenericClass.h"
#include "vm/GenericContainer.h"
#include "vm/Method.h"
#include "vm/MetadataCache.h"
#include "vm/Reflection.h"
#include "vm/String.h"
#include "vm/Type.h"
#include "vm/Exception.h"


#include <vector>
#include <set>

using namespace il2cpp::vm;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

enum
{
	BFLAGS_IgnoreCase = 1,
	BFLAGS_DeclaredOnly = 2,
	BFLAGS_Instance = 4,
	BFLAGS_Static = 8,
	BFLAGS_Public = 0x10,
	BFLAGS_NonPublic = 0x20,
	BFLAGS_FlattenHierarchy = 0x40,
	BFLAGS_InvokeMethod = 0x100,
	BFLAGS_CreateInstance = 0x200,
	BFLAGS_GetField = 0x400,
	BFLAGS_SetField = 0x800,
	BFLAGS_GetProperty = 0x1000,
	BFLAGS_SetProperty = 0x2000,
	BFLAGS_ExactBinding = 0x10000,
	BFLAGS_SuppressChangeType = 0x20000,
	BFLAGS_OptionalParamBinding = 0x40000
};

struct Il2CppEventInfoHash
{
	size_t operator()(const EventInfo* eventInfo) const
	{
		return il2cpp::utils::StringUtils::Hash(eventInfo->name);
	}
};

struct Il2CppEventInfoCompare
{
	bool operator()(const EventInfo* event1, const EventInfo* event2) const
	{
		// You can't overload events
		return strcmp(event1->name, event2->name) == 0;
	}
};

struct Il2CppEventInfoLess
{
	bool operator()(const EventInfo* event1, const EventInfo* event2) const
	{
		// You can't overload events
		return strcmp(event1->name, event2->name) < 0;
	}
};

struct PropertyPair
{
	const PropertyInfo *property;
	Il2CppReflectionProperty *reflectionProperty;

	PropertyPair(const PropertyInfo *property, Il2CppReflectionProperty *reflectionProperty) : property(property), reflectionProperty(reflectionProperty)
	{
	}
};

typedef std::vector<PropertyPair, il2cpp::gc::Allocator<PropertyPair> > PropertyPairVector;

typedef unordered_map<
	const EventInfo*,
	Il2CppReflectionEvent*,
#if IL2CPP_HAS_UNORDERED_CONTAINER
	Il2CppEventInfoHash,
	Il2CppEventInfoCompare,
#else
	Il2CppEventInfoLess,
#endif
	il2cpp::gc::Allocator<std::pair<const EventInfo* const, Il2CppReflectionEvent*> > > EventMap;

static bool PropertyEqual(const PropertyInfo* prop1, const PropertyInfo* prop2)
{
	// Name check is not enough, property can be overloaded
	if (strcmp(prop1->name, prop2->name) != 0)
		return false;

	return vm::Method::IsSameOverloadSignature(prop1, prop2);
}

static bool PropertyPairVectorContains(const PropertyPairVector& properties, const PropertyInfo* property)
{
	for(PropertyPairVector::const_iterator it = properties.begin(), end = properties.end(); it != end; ++it)
		if(PropertyEqual(it->property, property))
			return true;

	return false;
}

static inline bool IsPublic(const FieldInfo* field)
{
	return (field->type->attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK) == FIELD_ATTRIBUTE_PUBLIC;
}

static inline bool IsPrivate(const FieldInfo* field)
{
	return (field->type->attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK) == FIELD_ATTRIBUTE_PRIVATE;
}

static inline bool IsStatic(const FieldInfo* field)
{
	return (field->type->attrs & FIELD_ATTRIBUTE_STATIC) != 0;
}

static inline bool IsPublic(const PropertyInfo* property)
{
	if (property->get != NULL && (property->get->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) == METHOD_ATTRIBUTE_PUBLIC)
		return true;

	if (property->set != NULL && (property->set->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) == METHOD_ATTRIBUTE_PUBLIC)
		return true;

	return false;
}

static inline bool IsPrivate(const PropertyInfo* property)
{
	if (property->get != NULL && (property->get->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) != METHOD_ATTRIBUTE_PRIVATE)
		return false;

	if (property->set != NULL && (property->set->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) != METHOD_ATTRIBUTE_PRIVATE)
		return false;

	return true;
}

static inline bool IsStatic(const PropertyInfo* property)
{
	if (property->get != NULL)
		return (property->get->flags & METHOD_ATTRIBUTE_STATIC) != 0;

	if (property->set != NULL)
		return (property->set->flags & METHOD_ATTRIBUTE_STATIC) != 0;

	return false;
}

static inline bool IsPublic(const MethodInfo* method)
{
	return (method->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) == METHOD_ATTRIBUTE_PUBLIC;
}

static inline bool IsPrivate(const MethodInfo* method)
{
	return (method->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) == METHOD_ATTRIBUTE_PRIVATE;
}

static inline bool IsStatic(const MethodInfo* method)
{
	return (method->flags & METHOD_ATTRIBUTE_STATIC) != 0;
}

// From MSDN: An event is considered public to reflection if it has at least one method or accessor that is public.
static inline bool IsPublic(const EventInfo* event)
{
	if (event->add != NULL && (event->add->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) == METHOD_ATTRIBUTE_PUBLIC)
		return true;

	if (event->remove != NULL && (event->remove->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) == METHOD_ATTRIBUTE_PUBLIC)
		return true;

	if (event->raise != NULL && (event->raise->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) == METHOD_ATTRIBUTE_PUBLIC)
		return true;

	return false;
}

static inline bool IsPrivate(const EventInfo* event)
{
	if (event->add != NULL && (event->add->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) != METHOD_ATTRIBUTE_PRIVATE)
		return false;

	if (event->remove != NULL && (event->remove->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) != METHOD_ATTRIBUTE_PRIVATE)
		return false;

	if (event->raise != NULL && (event->raise->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) != METHOD_ATTRIBUTE_PRIVATE)
		return false;

	return true;
}

static inline bool IsStatic(const EventInfo* event)
{
	if (event->add != NULL)
		return (event->add->flags & METHOD_ATTRIBUTE_STATIC) != 0;

	if (event->remove != NULL)
		return (event->remove->flags & METHOD_ATTRIBUTE_STATIC) != 0;

	if (event->raise != NULL)
		return (event->raise->flags & METHOD_ATTRIBUTE_STATIC) != 0;

	return false;
}

template <typename MemberInfo, typename NameFilter>
static bool CheckMemberMatch(const MemberInfo* member, const TypeInfo* type, const TypeInfo* originalType, int32_t bindingFlags, const NameFilter& nameFilter)
{
	uint32_t accessBindingFlag = IsPublic(member) ? BFLAGS_Public : BFLAGS_NonPublic;

	if ((bindingFlags & accessBindingFlag) == 0)
		return false;

	if (type != originalType && IsPrivate(member))	// Private members are not part of derived class
		return false;

	if (IsStatic(member))
	{
		if ((bindingFlags & BFLAGS_Static) == 0)
			return false;

		if ((bindingFlags & BFLAGS_FlattenHierarchy) == 0 && type != originalType)
			return false;
	}
	else if ((bindingFlags & BFLAGS_Instance) == 0)
	{
		return false;
	}

	if (!nameFilter(member->name))
		return false;

	return true;
}

static inline bool ValidBindingFlagsForGetMember(uint32_t bindingFlags)
{
	return (bindingFlags & BFLAGS_Static) != 0 || (bindingFlags & BFLAGS_Instance) != 0;
}

Il2CppReflectionAssembly* MonoType::get_Assembly (Il2CppReflectionType* type)
{
	TypeInfo* klass = Class::FromIl2CppType (type->type);

	return il2cpp::vm::Reflection::GetAssemblyObject (MetadataCache::GetAssemblyFromIndex (klass->image->assemblyIndex));
}

int MonoType::get_attributes (Il2CppReflectionType *type)
{
	TypeInfo *klass = Class::FromSystemType (type);

	return klass->flags;
}

int MonoType::GetArrayRank(Il2CppReflectionType *type)
{
	if (type->type->type != IL2CPP_TYPE_ARRAY && type->type->type != IL2CPP_TYPE_SZARRAY)
		assert("Type must be an array type");

	TypeInfo* klass = Class::FromIl2CppType (type->type);
	return klass->rank;
}

Il2CppReflectionType *  MonoType::get_DeclaringType(Il2CppReflectionMonoType *monoType)
{
	const Il2CppType* type = monoType->type.type;
	TypeInfo *typeInfo = NULL;

	if (type->byref)
		return NULL;
	if (type->type == IL2CPP_TYPE_VAR || type->type == IL2CPP_TYPE_MVAR)
	{
		const Il2CppGenericParameter* genericParameter = Type::GetGenericParameter (type);
		const Il2CppGenericContainer* container = MetadataCache::GetGenericContainerFromIndex (genericParameter->ownerIndex);
		typeInfo = GenericContainer::GetDeclaringType (container);
	}
	else
	{
		typeInfo = Class::GetDeclaringType(Class::FromIl2CppType(type));
	}

	return typeInfo ? Reflection::GetTypeObject (typeInfo->byval_arg) : NULL;
}

static inline bool
is_generic_parameter (const Il2CppType *type)
{
	return !type->byref && (type->type == IL2CPP_TYPE_VAR || type->type == IL2CPP_TYPE_MVAR);
}

bool MonoType::get_IsGenericParameter(Il2CppReflectionType* type)
{
	return is_generic_parameter (type->type);
}

Il2CppReflectionModule* MonoType::get_Module (Il2CppReflectionType* type)
{
	TypeInfo* klass = Class::FromIl2CppType (type->type);

	return il2cpp::vm::Reflection::GetModuleObject (klass->image);
}

Il2CppString * MonoType::get_Name(Il2CppReflectionType * type)
{
	TypeInfo* typeInfo = Class::FromIl2CppType (type->type);
	
	if (type->type->byref)
	{
		std::string n = il2cpp::utils::StringUtils::Printf ("%s&", typeInfo->name);
		return String::New (n.c_str());
	}
	else
	{
		return il2cpp::vm::String::NewWrapper (typeInfo->name);
	}
}

Il2CppString *  MonoType::get_Namespace(Il2CppReflectionType *type)
{
	TypeInfo* typeInfo = Class::FromIl2CppType (type->type);

	while (TypeInfo* declaringType = Class::GetDeclaringType (typeInfo))
		typeInfo = declaringType;
	
	if (typeInfo->namespaze [0] == '\0')
		return NULL;
	else
		return il2cpp::vm::String::NewWrapper (typeInfo->namespaze);
}

Il2CppReflectionType * MonoType::get_BaseType(Il2CppReflectionType * type)
{
	TypeInfo* klass = Class::FromIl2CppType (type->type);

	return klass->parent ? il2cpp::vm::Reflection::GetTypeObject (klass->parent->byval_arg) : NULL;
}

Il2CppArray* MonoType::GetConstructors_internal(Il2CppReflectionType* type, int32_t bflags, Il2CppReflectionType *reftype)
{
	static TypeInfo *System_Reflection_ConstructorInfo = NULL;
	TypeInfo *startklass, *klass, *refklass;
	Il2CppArray *res;
	const MethodInfo *method;
	Il2CppObject *member;
	int match;
	void* iter = NULL;
	typedef std::vector<Il2CppObject*, il2cpp::gc::Allocator<Il2CppObject*> > TempVec;
	TempVec tmp_vec;

	if (type->type->byref)
		return il2cpp::vm::Array::NewCached (il2cpp_defaults.method_info_class, 0);
	klass = startklass = Class::FromIl2CppType (type->type);

	refklass = Class::FromIl2CppType (reftype->type);

	if (!System_Reflection_ConstructorInfo)
		System_Reflection_ConstructorInfo = Class::FromName (il2cpp_defaults.corlib, "System.Reflection", "ConstructorInfo");

	iter = NULL;
	while ((method = Class::GetMethods (klass, &iter))) {
		match = 0;
		if (strcmp (method->name, ".ctor") && strcmp (method->name, ".cctor"))
			continue;
		if ((method->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) == METHOD_ATTRIBUTE_PUBLIC) {
			if (bflags & BFLAGS_Public)
				match++;
		} else {
			if (bflags & BFLAGS_NonPublic)
				match++;
		}
		if (!match)
			continue;
		match = 0;
		if (method->flags & METHOD_ATTRIBUTE_STATIC) {
			if (bflags & BFLAGS_Static)
				if ((bflags & BFLAGS_FlattenHierarchy) || (klass == startklass))
					match++;
		} else {
			if (bflags & BFLAGS_Instance)
				match++;
		}

		if (!match)
			continue;
		member = (Il2CppObject*)Reflection::GetMethodObject (method, refklass);

		tmp_vec.push_back(member);
	}

	res = il2cpp::vm::Array::NewCached (System_Reflection_ConstructorInfo, (il2cpp_array_size_t)tmp_vec.size());

	for (size_t i = 0; i < tmp_vec.size(); ++i)
		il2cpp_array_setref (res, i, tmp_vec[i]);

	return res;
}

mscorlib_System_Reflection_MethodInfo *  MonoType::GetCorrespondingInflatedMethod(Il2CppReflectionMonoType *, Il2CppReflectionMonoType *)
{
	NOT_SUPPORTED_IL2CPP(MonoType::GetCorrespondingInflatedMethod, "This icall is only used by System.Reflection.Emit.TypeBuilder.");
	return 0;
}

Il2CppReflectionType* MonoType::GetElementType(Il2CppReflectionType * type)
{
	TypeInfo *klass;

	klass = Class::FromIl2CppType (type->type);

	// GetElementType should only return a type for:
	// Array Pointer PassedByRef
	if (type->type->byref)
		return il2cpp::vm::Reflection::GetTypeObject (klass->byval_arg);
	else if (klass->element_class && IL2CPP_CLASS_IS_ARRAY (klass))
		return il2cpp::vm::Reflection::GetTypeObject (klass->element_class->byval_arg);
	else if (klass->element_class && type->type->type == IL2CPP_TYPE_PTR)
		return il2cpp::vm::Reflection::GetTypeObject (klass->element_class->byval_arg);
	else
		return NULL;
}

template <typename NameFilter>
static inline Il2CppReflectionField* GetFieldFromType(TypeInfo* type, TypeInfo* const originalType, int32_t bindingFlags, const NameFilter& nameFilter)
{
	void* iter = NULL;
	while (FieldInfo* field = Class::GetFields (type, &iter))
	{
		if (CheckMemberMatch(field, type, originalType, bindingFlags, nameFilter))
		{
			return Reflection::GetFieldObject(originalType, field);
		}
	}

	return NULL;
}

template <typename NameFilter>
static Il2CppReflectionField* GetFieldImpl(TypeInfo* type, int32_t bindingFlags, const NameFilter& nameFilter)
{
	Il2CppReflectionField* field = GetFieldFromType(type, type, bindingFlags, nameFilter);

	if (field == NULL && (bindingFlags & BFLAGS_DeclaredOnly) == 0)
	{
		TypeInfo* const originalType = type;
		type = Class::GetParent(type);

		while (field == NULL && type != NULL)
		{
			field = GetFieldFromType(type, originalType, bindingFlags, nameFilter);
			type = Class::GetParent (type);
		}
	}
	
	return field;
}

Il2CppReflectionField* MonoType::GetField(Il2CppReflectionType* _this, Il2CppString* name, int32_t bindingFlags)
{
	using namespace utils;
	using namespace utils::functional;

	if (_this->type->byref)
		return NULL;

	TypeInfo* type = Class::FromIl2CppType(_this->type);

	if (bindingFlags & BFLAGS_IgnoreCase)
	{
		return GetFieldImpl(type, bindingFlags, Filter<std::string, StringUtils::CaseInsensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
	}

	return GetFieldImpl(type, bindingFlags, Filter<std::string, StringUtils::CaseSensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
}

static inline void CollectTypeFields(TypeInfo* type, const TypeInfo* const originalType, int32_t bindingFlags, std::vector<FieldInfo*>& fields)
{
	void* iterator = NULL;
	FieldInfo* field = NULL;
	while ((field = Class::GetFields(type, &iterator)) != NULL)
	{
		if (CheckMemberMatch(field, type, originalType, bindingFlags, utils::functional::TrueFilter()))
			fields.push_back(field);
	}
}

Il2CppArray* MonoType::GetFields_internal(Il2CppReflectionType* _this, int bindingFlags, Il2CppReflectionType* reflectedType)
{
	if (reflectedType->type->byref || !ValidBindingFlagsForGetMember(bindingFlags))
		return Array::New (il2cpp_defaults.field_info_class, 0);

	std::vector<FieldInfo*> fields;
	TypeInfo* typeInfo = Class::FromIl2CppType(reflectedType->type);
	TypeInfo* const originalType = typeInfo;

	CollectTypeFields(typeInfo, typeInfo, bindingFlags, fields);

	if ((bindingFlags & BFLAGS_DeclaredOnly) == 0)
	{
		typeInfo = typeInfo->parent;

		while (typeInfo != NULL)
		{
			CollectTypeFields(typeInfo, originalType, bindingFlags, fields);
			typeInfo = typeInfo->parent;
		}
	}

	size_t fieldCount = fields.size();
	Il2CppArray* result = Array::NewCached(il2cpp_defaults.field_info_class, (il2cpp_array_size_t) fieldCount);

	for (size_t i = 0; i < fieldCount; i++)
	{
		il2cpp_array_setref(result, i, Reflection::GetFieldObject(originalType, fields[i]));
	}

	return result;
}

Il2CppString * MonoType::getFullName(Il2CppReflectionType * type,bool full_name, bool assembly_qualified)
{
	Il2CppTypeNameFormat format;

	if (full_name)
		format = assembly_qualified ?
			IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED :
			IL2CPP_TYPE_NAME_FORMAT_FULL_NAME;
	else
		format = IL2CPP_TYPE_NAME_FORMAT_REFLECTION;

	std::string name(Type::GetName (type->type, format));
	if (name.empty())
		return NULL;

	if (full_name && (type->type->type == IL2CPP_TYPE_VAR || type->type->type == IL2CPP_TYPE_MVAR))
	{
		return NULL;
	}

	return il2cpp::vm::String::NewWrapper(name.c_str());
}

Il2CppArray * MonoType::GetGenericArguments (Il2CppReflectionType* type)
{
	Il2CppArray *res;
	TypeInfo *klass, *pklass;

	klass = Class::FromIl2CppType (type->type);

	if (Class::IsGeneric (klass))
	{
		const Il2CppGenericContainer *container = MetadataCache::GetGenericContainerFromIndex (klass->genericContainerIndex);
		res = Array::New (il2cpp_defaults.systemtype_class, container->type_argc);
		for (int32_t i = 0; i < container->type_argc; ++i)
		{
			pklass = Class::FromGenericParameter (GenericContainer::GetGenericParameter (container, i));
			il2cpp_array_setref (res, i, Reflection::GetTypeObject (pklass->byval_arg));
		}
	}
	else if (klass->generic_class)
	{
		const Il2CppGenericInst *inst = klass->generic_class->context.class_inst;
		res = Array::New (il2cpp_defaults.systemtype_class, inst->type_argc);
		for (uint32_t i = 0; i < inst->type_argc; ++i)
			il2cpp_array_setref (res, i, Reflection::GetTypeObject (inst->type_argv [i]));
	}
	else
	{
		res = Array::New (il2cpp_defaults.systemtype_class, 0);
	}
	return res;
}

Il2CppArray* MonoType::GetInterfaces(Il2CppReflectionType* type)
{
	TypeInfo* klass = Class::FromIl2CppType (type->type);
	typedef std::set<TypeInfo*> InterfaceVector;
	InterfaceVector itfs;

	TypeInfo* currentType = klass;
	while (currentType != NULL)
	{
		void* iter = NULL;
		while (TypeInfo* itf = Class::GetInterfaces (currentType, &iter))
			itfs.insert (itf);

		currentType = Class::GetParent (currentType);
	}

	Il2CppArray* res = Array::New (il2cpp_defaults.systemtype_class, (il2cpp_array_size_t)itfs.size ());
	int i = 0;
	for (InterfaceVector::const_iterator iter = itfs.begin(); iter != itfs.end(); ++iter, ++i)
		il2cpp_array_setref (res, i, Reflection::GetTypeObject ((*iter)->byval_arg));

	return res;
}

static bool
method_public (MethodInfo* method)
{
	return (method->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) == METHOD_ATTRIBUTE_PUBLIC;
}

static bool
method_nonpublic (MethodInfo* method, bool start_klass)
{
	switch (method->flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK) {
		case METHOD_ATTRIBUTE_ASSEM:
			return (start_klass || il2cpp_defaults.generic_ilist_class);
		case METHOD_ATTRIBUTE_PRIVATE:
			return start_klass;
		case METHOD_ATTRIBUTE_PUBLIC:
			return false;
		default:
			return true;
	}
}

static bool
method_rt_special_name (MethodInfo* method)
{
	return (method->flags & METHOD_ATTRIBUTE_RESERVED_MASK) == METHOD_ATTRIBUTE_RT_SPECIAL_NAME;
}

static bool
method_static(MethodInfo* method)
{
	return (method->flags & METHOD_ATTRIBUTE_STATIC) != 0;
}

template <typename NameFilter>
void CollectTypeMethods(TypeInfo* type, const TypeInfo* originalType, uint32_t bindingFlags, const NameFilter& nameFilter, std::vector<const MethodInfo*>& methods, bool (&filledSlots)[65535])
{
	void* iter = NULL;
	while (const MethodInfo* method = Class::GetMethods (type, &iter))
	{
		if ((method->flags & METHOD_ATTRIBUTE_RT_SPECIAL_NAME) != 0 && (strcmp(method->name, ".ctor") == 0 || strcmp(method->name, ".cctor") == 0))
			continue;
		
		if (CheckMemberMatch(method, type, originalType, bindingFlags, nameFilter))
		{
			if ((method->flags & METHOD_ATTRIBUTE_VIRTUAL) != 0)
			{
				if (filledSlots[method->slot])
					continue;

				filledSlots[method->slot] = true;
			}

			methods.push_back(method);
		}
	}
}

template <typename NameFilter>
static Il2CppArray* GetMethodsByNameImpl(const Il2CppType* type, uint32_t bindingFlags, const NameFilter& nameFilter)
{
	std::vector<const MethodInfo*> methods;
	bool filledSlots[65535] = { 0 };

	TypeInfo* typeInfo = Class::FromIl2CppType(type);
	TypeInfo* const originalTypeInfo = typeInfo;

	CollectTypeMethods(typeInfo, typeInfo, bindingFlags, nameFilter, methods, filledSlots);

	if ((bindingFlags & BFLAGS_DeclaredOnly) == 0)
	{
		for (typeInfo = Class::GetParent(typeInfo); typeInfo != NULL; typeInfo = Class::GetParent(typeInfo))
		{
			CollectTypeMethods(typeInfo, originalTypeInfo, bindingFlags, nameFilter, methods, filledSlots);
		}
	}
	
	size_t methodCount = methods.size();
	Il2CppArray* result = Array::NewCached(il2cpp_defaults.method_info_class, (il2cpp_array_size_t)methodCount);

	for (size_t i = 0; i < methodCount; i++)
	{
		Il2CppReflectionMethod* method = Reflection::GetMethodObject(methods[i], originalTypeInfo);
		il2cpp_array_setref(result, i, method);
	}

	return result;
}

Il2CppArray* MonoType::GetMethodsByName(Il2CppReflectionType* _this, Il2CppString* name, int bindingFlags, bool ignoreCase, Il2CppReflectionType* type)
{
	using namespace utils;
	using namespace utils::functional;

	if (type->type->byref || !ValidBindingFlagsForGetMember(bindingFlags))
		return Array::NewCached(il2cpp_defaults.property_info_class, 0);

	if (name != NULL)
	{
		if (ignoreCase)
		{
			return GetMethodsByNameImpl(type->type, bindingFlags, Filter<std::string, StringUtils::CaseInsensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
		}

		return GetMethodsByNameImpl(type->type, bindingFlags, Filter<std::string, StringUtils::CaseSensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
	}

	return GetMethodsByNameImpl(type->type, bindingFlags, TrueFilter());
}

template <typename NameFilter>
static void CollectTypeProperties(TypeInfo* type, uint32_t bindingFlags, const NameFilter& nameFilter, TypeInfo* const originalType, PropertyPairVector& properties)
{
	void* iter = NULL;
	while (const PropertyInfo* property = Class::GetProperties (type, &iter))
	{
		if (CheckMemberMatch(property, type, originalType, bindingFlags, nameFilter))
		{
			if (PropertyPairVectorContains(properties, property))
				continue;

			properties.push_back(PropertyPair(property, Reflection::GetPropertyObject(originalType, property)));
		}
	}
}

template <typename NameFilter>
static Il2CppArray* GetPropertiesByNameImpl(const Il2CppType* type, uint32_t bindingFlags, const NameFilter& nameFilter)
{
	PropertyPairVector properties;
	TypeInfo* typeInfo = Class::FromIl2CppType(type);
	TypeInfo* const originalTypeInfo = typeInfo;

	properties.reserve(typeInfo->property_count);
	CollectTypeProperties(typeInfo, bindingFlags, nameFilter, originalTypeInfo, properties);

	if ((bindingFlags & BFLAGS_DeclaredOnly) == 0)
	{
		for (typeInfo = typeInfo->parent; typeInfo != NULL; typeInfo = typeInfo->parent)
		{
			CollectTypeProperties(typeInfo, bindingFlags, nameFilter, originalTypeInfo, properties);
		}
	}

	int i = 0;
	Il2CppArray* res = Array::NewCached(il2cpp_defaults.property_info_class, (il2cpp_array_size_t)properties.size());

	for (PropertyPairVector::const_iterator iter = properties.begin(); iter != properties.end(); iter++)
	{
		il2cpp_array_setref(res, i, iter->reflectionProperty);
		i++;
	}

	return res;
}

Il2CppArray* MonoType::GetPropertiesByName (Il2CppReflectionType* _this, Il2CppString* name, uint32_t bindingFlags, bool ignoreCase, Il2CppReflectionType* type)
{
	using namespace utils;
	using namespace utils::functional;

	if (type->type->byref || !ValidBindingFlagsForGetMember(bindingFlags))
		return Array::NewCached(il2cpp_defaults.property_info_class, 0);

	if (name != NULL)
	{
		if (ignoreCase)
		{
			return GetPropertiesByNameImpl(type->type, bindingFlags, Filter<std::string, StringUtils::CaseInsensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
		}

		return GetPropertiesByNameImpl(type->type, bindingFlags, Filter<std::string, StringUtils::CaseSensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
	}

	return GetPropertiesByNameImpl(type->type, bindingFlags, TrueFilter());
}

bool MonoType::IsByRefImpl(Il2CppReflectionType* type)
{
	return type->type->byref;
}

bool MonoType::IsCOMObjectImpl (Il2CppReflectionMonoType *)
{
	return false;	// il2cpp does not support COM objects, so this is always false
}

bool MonoType::IsPointerImpl(Il2CppReflectionType* type)
{
	return type->type->type == IL2CPP_TYPE_PTR;
}

bool MonoType::IsPrimitiveImpl(Il2CppReflectionType *type)
{
	return (!type->type->byref && (((type->type->type >= IL2CPP_TYPE_BOOLEAN) && (type->type->type <= IL2CPP_TYPE_R8)) || (type->type->type == IL2CPP_TYPE_I) || (type->type->type == IL2CPP_TYPE_U)));
}

template <typename NameFilter>
static inline Il2CppReflectionEvent* GetEventFromType(TypeInfo* const type, TypeInfo* const originalType, int32_t bindingFlags, const NameFilter& nameFilter)
{
	void* iter = NULL;
	while (const EventInfo* event = Class::GetEvents (type, &iter))
	{
		if (CheckMemberMatch(event, type, originalType, bindingFlags, nameFilter))
		{
			return Reflection::GetEventObject(originalType, event);
		}
	}

	return NULL;
}

template <typename NameFilter>
static Il2CppReflectionEvent* GetEventImpl(TypeInfo* type, int32_t bindingFlags, const NameFilter& nameFilter)
{
	Il2CppReflectionEvent* event = GetEventFromType(type, type, bindingFlags, nameFilter);

	if (event == NULL && (bindingFlags & BFLAGS_DeclaredOnly) == 0)
	{
		TypeInfo* const originalType = type;
		type = type->parent;

		while (event == NULL && type != NULL)
		{
			event = GetEventFromType(type, originalType, bindingFlags, nameFilter);
			type = type->parent;
		}
	}

	return event;
}

Il2CppReflectionEvent* MonoType::InternalGetEvent(Il2CppReflectionType* _this, Il2CppString* name, int32_t bindingFlags)
{
	using namespace utils;
	using namespace utils::functional;

	if (_this->type->byref || !ValidBindingFlagsForGetMember(bindingFlags))
		return NULL;

	TypeInfo* type = Class::FromIl2CppType(_this->type);

	if (bindingFlags & BFLAGS_IgnoreCase)
	{
		return GetEventImpl(type, bindingFlags, Filter<std::string, StringUtils::CaseInsensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
	}

	return GetEventImpl(type, bindingFlags, Filter<std::string, StringUtils::CaseSensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
}

static inline void CollectTypeEvents(TypeInfo* type, TypeInfo* const originalType, int32_t bindingFlags, EventMap& events)
{
	void* iter = NULL;
	while (const EventInfo* event = Class::GetEvents (type, &iter))
	{
		if (CheckMemberMatch(event, type, originalType, bindingFlags, utils::functional::TrueFilter()))
		{
			if (events.find(event) != events.end())
				continue;

			events[event] = Reflection::GetEventObject(originalType, event);
		}
	}
}

Il2CppArray* MonoType::GetEvents_internal(Il2CppReflectionType* _this, int32_t bindingFlags, Il2CppReflectionType* type)
{
	if (type->type->byref || !ValidBindingFlagsForGetMember(bindingFlags))
		return Array::New(il2cpp_defaults.event_info_class, 0);

	EventMap events;
	TypeInfo* typeInfo = Class::FromIl2CppType(type->type);

	CollectTypeEvents(typeInfo, typeInfo, bindingFlags, events);

	if ((bindingFlags & BFLAGS_DeclaredOnly) == 0)
	{
		TypeInfo* const originalType = typeInfo;
		typeInfo = Class::GetParent(typeInfo);
		
		while (typeInfo != NULL)
		{
			CollectTypeEvents(typeInfo, originalType, bindingFlags, events);
			typeInfo = Class::GetParent(typeInfo);
		}
	}

	int i = 0;
	Il2CppArray* result = Array::NewCached(il2cpp_defaults.event_info_class, (il2cpp_array_size_t)events.size());

	for (EventMap::const_iterator iter = events.begin(); iter != events.end(); iter++)
	{
		il2cpp_array_setref(result, i, iter->second);
		i++;
	}

	return result;
}

void MonoType::type_from_obj(void* /* System.MonoType */ type, Il2CppObject* obj)
{
	NOT_SUPPORTED_IL2CPP(MonoType::type_from_obj, "This icall is only used by System.MonoType constructor, which throws NotImplementedException right after this call.");
}

void* /* System.Reflection.ConstructorInfo */ MonoType::GetCorrespondingInflatedConstructor (void* /* System.MonoType */ self, void* /* System.Reflection.ConstructorInfo */ genericInfo)
{
	NOT_SUPPORTED_IL2CPP(MonoType::GetCorrespondingInflatedConstructor, "This icall is only used by System.Reflection.Emit.TypeBuilder.");
	return 0;
}

static inline bool CheckNestedTypeMatch(TypeInfo* nestedType, BindingFlags bindingFlags)
{
	uint32_t accessFlag = (nestedType->flags & TYPE_ATTRIBUTE_VISIBILITY_MASK) == TYPE_ATTRIBUTE_NESTED_PUBLIC ? BFLAGS_Public : BFLAGS_NonPublic;
	return (accessFlag & bindingFlags) != 0;
}

template <typename NameFilter>
Il2CppReflectionType* GetNestedTypeImpl(TypeInfo* typeInfo, BindingFlags bindingFlags, const NameFilter& nameFilter)
{
	void* iter = NULL;
	while (TypeInfo* nestedType = Class::GetNestedTypes (typeInfo, &iter))
	{
		if (CheckNestedTypeMatch(nestedType, bindingFlags) && nameFilter(nestedType->name))
			return Reflection::GetTypeObject(nestedType->byval_arg);
	}

	return NULL;
}

Il2CppReflectionType* MonoType::GetNestedType(Il2CppReflectionType* type, Il2CppString* name, BindingFlags bindingFlags)
{
	using namespace utils;
	using namespace utils::functional;

	bool validBindingFlags = (bindingFlags & BFLAGS_NonPublic) != 0 || (bindingFlags & BFLAGS_Public) != 0;

	if (type->type->byref || !validBindingFlags)
		return NULL;

	TypeInfo* typeInfo = Class::FromIl2CppType(type->type);
	
	// nested types are always generic type definitions, even for inflated types. As such we only store/retrieve them on
	// type definitions and generic type definitions. If we are a generic instance, use our generic type definition instead.
	if (typeInfo->generic_class)
		typeInfo = GenericClass::GetTypeDefinition (typeInfo->generic_class);

	if (bindingFlags & BFLAGS_IgnoreCase)
	{
		return GetNestedTypeImpl(typeInfo, bindingFlags, Filter<std::string, StringUtils::CaseInsensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
	}

	return GetNestedTypeImpl(typeInfo, bindingFlags, Filter<std::string, StringUtils::CaseSensitiveComparer>(StringUtils::Utf16ToUtf8(name->chars)));
}

Il2CppArray* MonoType::GetNestedTypes(Il2CppReflectionType* type, int32_t bindingFlags)
{
	bool validBindingFlags = (bindingFlags & BFLAGS_NonPublic) != 0 || (bindingFlags & BFLAGS_Public) != 0;

	if (type->type->byref || !validBindingFlags)
		return Array::New(il2cpp_defaults.monotype_class, 0);

	TypeInfo* typeInfo = Class::FromIl2CppType(type->type);

	// nested types are always generic type definitions, even for inflated types. As such we only store/retrieve them on
	// type definitions and generic type definitions. If we are a generic instance, use our generic type definition instead.
	if (typeInfo->generic_class)
		typeInfo = GenericClass::GetTypeDefinition (typeInfo->generic_class);

	std::vector<TypeInfo*> nestedTypes;
	
	void* iter = NULL;
	while (TypeInfo* nestedType = Class::GetNestedTypes (typeInfo, &iter))
	{
		if (CheckNestedTypeMatch(nestedType, bindingFlags))
			nestedTypes.push_back(nestedType);
	}

	size_t nestedTypeCount = nestedTypes.size();
	Il2CppArray* result = Array::New(il2cpp_defaults.monotype_class, (il2cpp_array_size_t)nestedTypeCount);

	for (size_t i = 0; i < nestedTypeCount; i++)
	{
		il2cpp_array_setref(result, i, Reflection::GetTypeObject(nestedTypes[i]->byval_arg));
	}

	return result;
}

void* /* System.Reflection.MethodBase */ MonoType::get_DeclaringMethod (void* /* System.MonoType */ self)
{
	NOT_IMPLEMENTED_ICALL (MonoType::get_DeclaringMethod);
	
	return 0;
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
