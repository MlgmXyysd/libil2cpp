#pragma once

#include <stdint.h>

struct Il2CppString;
struct Il2CppArray;
struct Il2CppReflectionAssembly;
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
struct Il2CppAssembly;
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
	static Il2CppReflectionAssembly* GetAssemblyObject (Il2CppAssembly *assembly);
	static Il2CppReflectionField* GetFieldObject (TypeInfo *klass, FieldInfo *field);
	static Il2CppReflectionProperty* GetPropertyObject (TypeInfo *klass, PropertyInfo *property);
	static Il2CppReflectionEvent* GetEventObject(TypeInfo *klass, EventInfo *event);
	static Il2CppReflectionMethod* GetMethodObject (MethodInfo *method, TypeInfo *refclass);
	static Il2CppReflectionModule* GetModuleObject (Il2CppImage *image);
	static Il2CppReflectionType* GetTypeObject (const Il2CppType *type);
	static Il2CppArray* GetParamObjects (MethodInfo *method, TypeInfo *refclass);
	static CustomAttributesCache* GetCustomAttrsInfo (Il2CppObject *obj);
	static CustomAttributesCache* GetCustomAttrsInfo (FieldInfo *field);
	static CustomAttributesCache* GetCustomAttrsInfo (TypeInfo *klass);
	static CustomAttributesCache* GetCustomAttrsInfo (MethodInfo *method);

// internal
public:
	static void Initialize ();
	static bool CustomAttrsHasAttr(CustomAttributesCache *ainfo, TypeInfo *attr_klass);
	static TypeInfo* TypeGetHandle (Il2CppReflectionType* ref);

private:
	static CustomAttributesCache* GetCustomAttributesCacheFor (TypeInfo *klass);
	static CustomAttributesCache* GetCustomAttributesCacheFor (MethodInfo *method);
	static CustomAttributesCache* GetCustomAttributesCacheFor (PropertyInfo *property);
	static CustomAttributesCache* GetCustomAttributesCacheFor (FieldInfo *field);
	static CustomAttributesCache* GetCustomAttributesCacheFor (EventInfo *event);
	static CustomAttributesCache* GetCustomAttributesCacheFor (Il2CppReflectionParameter *param);
	static CustomAttributesCache* GetCustomAttributesCacheFor (Il2CppAssembly *assembly);
};

} /* namespace vm */
} /* namespace il2cpp */