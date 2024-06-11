#pragma once

#include <stdint.h>

struct Il2CppString;
struct Il2CppArray;
struct Il2CppReflectionAssembly;
struct Il2CppReflectionAssemblyName;
struct Il2CppReflectionField;
struct Il2CppReflectionMethod;
struct Il2CppReflectionModule;
struct Il2CppReflectionProperty;
struct Il2CppReflectionEvent;
struct Il2CppReflectionType;
struct Il2CppReflectionParameter;
struct TypeInfo;
struct FieldInfo;
struct MethodInfo;
struct PropertyInfo;
struct EventInfo;
struct TypeInfo;
struct CustomAttributesCache;
struct CustomAttributeTypeCache;
struct Il2CppAssembly;
struct Il2CppAssemblyName;
struct Il2CppImage;
struct Il2CppType;
struct Il2CppObject;

namespace il2cpp
{
namespace vm
{

class Reflection
{
// exported
public:
	static Il2CppReflectionAssembly* GetAssemblyObject (const Il2CppAssembly *assembly);
	static Il2CppReflectionAssemblyName* GetAssemblyNameObject (const Il2CppAssemblyName *assemblyName);
	static Il2CppReflectionField* GetFieldObject (TypeInfo *klass, FieldInfo *field);
	static Il2CppReflectionProperty* GetPropertyObject (TypeInfo *klass, const PropertyInfo *property);
	static Il2CppReflectionEvent* GetEventObject(TypeInfo *klass, const EventInfo *event);
	static Il2CppReflectionMethod* GetMethodObject (const MethodInfo *method, TypeInfo *refclass);
	static Il2CppReflectionModule* GetModuleObject (const Il2CppImage *image);
	static Il2CppReflectionType* GetTypeObject (const Il2CppType *type);
	static Il2CppArray* GetParamObjects (const MethodInfo *method, TypeInfo *refclass);
	static CustomAttributesCache* GetCustomAttrsInfo (Il2CppObject *obj);

	static bool HasAttribute (Il2CppObject *obj, TypeInfo *attribute);
	static bool HasAttribute (FieldInfo *field, TypeInfo *attribute);
	static bool HasAttribute (const MethodInfo *method, TypeInfo *attribute);
	static bool HasAttribute (TypeInfo *klass, TypeInfo *attribute);

	static bool IsType (Il2CppObject *obj);
	static bool IsField (Il2CppObject *obj);
	static bool IsAnyMethod (Il2CppObject *obj);
	static bool IsProperty(Il2CppObject *obj);
	static bool IsEvent (Il2CppObject *obj);

// internal
public:
	static void Initialize ();
	static bool CustomAttrsHasAttr (CustomAttributeTypeCache *ainfo, TypeInfo *attr_klass);
	static TypeInfo* TypeGetHandle (Il2CppReflectionType* ref);

private:
	static CustomAttributeTypeCache* GetCustomAttrsTypeInfo (Il2CppObject *obj);
	static CustomAttributeTypeCache* GetCustomAttributeTypeCacheFor (Il2CppReflectionParameter *parameter);
	static CustomAttributesCache* GetCustomAttributesCacheFor (TypeInfo *klass);
	static CustomAttributesCache* GetCustomAttributesCacheFor (const MethodInfo *method);
	static CustomAttributesCache* GetCustomAttributesCacheFor (const PropertyInfo *property);
	static CustomAttributesCache* GetCustomAttributesCacheFor (FieldInfo *field);
	static CustomAttributesCache* GetCustomAttributesCacheFor (const EventInfo *event);
	static CustomAttributesCache* GetCustomAttributesCacheFor (Il2CppReflectionParameter *param);
	static CustomAttributesCache* GetCustomAttributesCacheFor (const Il2CppAssembly *assembly);
};

} /* namespace vm */
} /* namespace il2cpp */
