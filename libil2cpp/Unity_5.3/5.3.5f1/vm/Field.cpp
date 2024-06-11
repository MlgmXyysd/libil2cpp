#include "il2cpp-config.h"
#include "utils/StringUtils.h"
#include "utils/BlobReader.h"
#include "vm/Class.h"
#include "vm/GenericClass.h"
#include "vm/Field.h"
#include "vm/Object.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Type.h"
#include <cassert>
#include <memory>
#include "class-internals.h"
#include "object-internals.h"
#include "tabledefs.h"
#include "utils/MemoryRead.h"

using namespace il2cpp::utils;

namespace il2cpp
{
namespace vm
{

static void
set_value (const Il2CppType *type, void *dest, void *value, bool deref_pointer);

const char* Field::GetName (FieldInfo *field)
{
	return field->name;
}

Il2CppClass* Field::GetParent (FieldInfo *field)
{
	return field->parent;
}

int Field::GetFlags (FieldInfo *field)
{
	return field->type->attrs;
}

size_t Field::GetOffset (FieldInfo *field)
{
	return field->offset;
}

void Field::GetValue (Il2CppObject *obj, FieldInfo *field, void *value)
{
	void *src;

	assert (obj);

	assert (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC));

	src = (char*)obj + field->offset;
	set_value (field->type, value, src, true);
}

uint32_t Field::GetToken (const FieldInfo *field)
{
	return field->token;
}

Il2CppObject* Field::GetValueObject (FieldInfo *field, Il2CppObject *obj)
{
	Il2CppClass* fieldType = Class::FromIl2CppType(field->type);

	if (field->type->attrs & FIELD_ATTRIBUTE_LITERAL)
	{
		if (fieldType->valuetype)
		{
			void* value = alloca(fieldType->instance_size - sizeof(Il2CppObject));
			Field::GetDefaultFieldValue(field, value);
			return Object::Box(fieldType, value);
		}
		else
		{
			Il2CppObject* value;
			Field::GetDefaultFieldValue(field, &value);
			return value;
		}
	}

	void* fieldAddress;
	if (field->type->attrs & FIELD_ATTRIBUTE_STATIC)
	{
		if (field->offset == THREAD_STATIC_FIELD_OFFSET)
		{
			NOT_IMPLEMENTED_NO_ASSERT(Field::GetValueObject, "Field::GetValueObject is not implemented for thread-static fields");
			return NULL;
		}
		
		Runtime::ClassInit(field->parent);
		fieldAddress = static_cast<uint8_t*>(field->parent->static_fields) + field->offset;
	}
	else
	{
		assert(obj);
		fieldAddress = reinterpret_cast<uint8_t*>(obj) + field->offset;
	}

	return Object::Box(fieldType, fieldAddress);
}

const Il2CppType* Field::GetType (FieldInfo *field)
{
	return field->type;
}

bool Field::HasAttribute (FieldInfo *field, Il2CppClass *attr_class)
{
	return Reflection::HasAttribute(field, attr_class);
}

bool Field::IsDeleted (FieldInfo *field)
{
	return false;
}

void Field::SetValue (Il2CppObject *obj, FieldInfo *field, void *value)
{
	void *dest;

	assert (!(field->type->attrs & FIELD_ATTRIBUTE_STATIC));

	dest = (char*)obj + field->offset;
	set_value (field->type, dest, value, false);
}

void Field::GetDefaultFieldValue (FieldInfo *field, void *value)
{
	const Il2CppType* type = NULL;
	const char* data;
	
	data = Class::GetFieldDefaultValue (field, &type);
	BlobReader::GetConstantValueFromBlob (type->type, data, value);
}

void Field::StaticGetValue (FieldInfo *field, void *value)
{
	void *src = NULL;

	assert (field->type->attrs & FIELD_ATTRIBUTE_STATIC);
	
	if (field->type->attrs & FIELD_ATTRIBUTE_LITERAL)
	{
		GetDefaultFieldValue (field, value);
		return;
	}

	// ensure parent is initialized so that static fields memory has been allocated
	Class::SetupFields(field->parent);

	if (field->offset == THREAD_STATIC_FIELD_OFFSET)
	{
		// Thread static
		NOT_IMPLEMENTED_NO_ASSERT(Field::StaticGetValue, "Field::StaticGetValue is not implemented for thread-static fields");
	}
	else
	{
		src = ((char*)field->parent->static_fields) + field->offset;
	}

	set_value (field->type, value, src, true);
}

void Field::StaticSetValue (FieldInfo *field, void *value)
{
	void *dest = NULL;

	assert(field->type->attrs & FIELD_ATTRIBUTE_STATIC);
	assert(!(field->type->attrs & FIELD_ATTRIBUTE_LITERAL));

	// ensure parent is initialized so that static fields memory has been allocated
	Class::Init(field->parent);

	if (field->offset == THREAD_STATIC_FIELD_OFFSET)
	{
		NOT_IMPLEMENTED_NO_ASSERT(Field::StaticSetValue, "Field::SetValueObject is not implemented for thread-static fields");
		return;
	}
	else
	{
		dest = ((char*)field->parent->static_fields) + field->offset;
	}

	set_value (field->type, dest, value, false);
}

static void
set_value (const Il2CppType *type, void *dest, void *value, bool deref_pointer)
{
	int t;
	if (type->byref) {
		/* object fields cannot be byref, so we don't need a
		   wbarrier here */
		void* *p = (void**)dest;
		*p = value;
		return;
	}
	t = type->type;
handle_enum:
	switch (t) {
	case IL2CPP_TYPE_BOOLEAN:
	case IL2CPP_TYPE_I1:
	case IL2CPP_TYPE_U1: {
		uint8_t *p = (uint8_t*)dest;
		*p = value ? *(uint8_t*)value : 0;
		return;
	}
	case IL2CPP_TYPE_I2:
	case IL2CPP_TYPE_U2:
	case IL2CPP_TYPE_CHAR: {
		uint16_t *p = (uint16_t*)dest;
		*p = value ? *(uint16_t*)value : 0;
		return;
	}
#if SIZEOF_VOID_P == 4
	case IL2CPP_TYPE_I:
	case IL2CPP_TYPE_U:
#endif
	case IL2CPP_TYPE_I4:
	case IL2CPP_TYPE_U4: {
		int32_t *p = (int32_t*)dest;
		*p = value ? *(int32_t*)value : 0;
		return;
	}
#if SIZEOF_VOID_P == 8
	case IL2CPP_TYPE_I:
	case IL2CPP_TYPE_U:
#endif
	case IL2CPP_TYPE_I8:
	case IL2CPP_TYPE_U8: {
		int64_t *p = (int64_t*)dest;
		*p = value ? *(int64_t*)value : 0;
		return;
	}
	case IL2CPP_TYPE_R4: {
		float *p = (float*)dest;
		*p = value ? *(float*)value : 0;
		return;
	}
	case IL2CPP_TYPE_R8: {
		double *p = (double*)dest;
		*p = value ? *(double*)value : 0;
		return;
	}
	case IL2CPP_TYPE_STRING:
	case IL2CPP_TYPE_SZARRAY:
	case IL2CPP_TYPE_CLASS:
	case IL2CPP_TYPE_OBJECT:
	case IL2CPP_TYPE_ARRAY:
		*(void**)dest = (deref_pointer? *(void**)value: value);
		//mono_gc_wbarrier_generic_store (dest, deref_pointer? *(void**)value: value);
		return;
	case IL2CPP_TYPE_FNPTR:
	case IL2CPP_TYPE_PTR: {
		void* *p = (void**)dest;
		*p = deref_pointer? *(void**)value: value;
		return;
	}
	case IL2CPP_TYPE_VALUETYPE:
		/* note that 't' and 'type->type' can be different */
		if (type->type == IL2CPP_TYPE_VALUETYPE && Type::IsEnum (type)) {
			t = Class::GetEnumBaseType (Type::GetClass (type))->type;
			goto handle_enum;
		} else {
			Il2CppClass *klass = Class::FromIl2CppType (type);
			int size = Class::GetValueSize (klass, NULL);
			if (value == NULL) {
				memset (dest, 0, size);
			} else {
				memcpy (dest, value, size);
				//mono_gc_wbarrier_value_copy (dest, value, size, klass);
			}
		}
		return;
	case IL2CPP_TYPE_GENERICINST:
		t = GenericClass::GetTypeDefinition (type->data.generic_class)->byval_arg->type;
		goto handle_enum;
	default:
		assert (0);
	}
}

const char* Field::GetData (FieldInfo *field)
{
	if (field->type->attrs & FIELD_ATTRIBUTE_HAS_DEFAULT)
	{
		const Il2CppType* type = NULL;
		return Class::GetFieldDefaultValue (field, &type);
	}
	else if (field->type->attrs & FIELD_ATTRIBUTE_HAS_FIELD_RVA)
	{
		NOT_IMPLEMENTED_NO_ASSERT (Field::GetData, "This works for array initialization data. Revisit any other RVA use case.");
		const Il2CppType* type = NULL;
		return Class::GetFieldDefaultValue (field, &type);
	}
	else
	{
		return NULL;
	}
}

bool Field::IsInstance (FieldInfo* field)
{
	return (field->type->attrs & FIELD_ATTRIBUTE_STATIC) == 0;
}

bool Field::IsNormalStatic (FieldInfo* field)
{
	if ((field->type->attrs & FIELD_ATTRIBUTE_STATIC) == 0)
		return false;

	if (field->offset == THREAD_STATIC_FIELD_OFFSET)
		return false;

	if ((field->type->attrs & FIELD_ATTRIBUTE_LITERAL) != 0)
		return false;

	return true;
}

bool Field::IsThreadStatic (FieldInfo* field)
{
	if ((field->type->attrs & FIELD_ATTRIBUTE_STATIC) == 0)
		return false;

	if (field->offset != -1)
		return false;

	if ((field->type->attrs & FIELD_ATTRIBUTE_LITERAL) != 0)
		return false;

	return true;
}

} /* namespace vm */
} /* namespace il2cpp */
