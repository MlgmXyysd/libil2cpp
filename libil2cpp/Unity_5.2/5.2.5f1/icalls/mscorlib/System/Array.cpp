#include "il2cpp-config.h"
#include <cassert>
#include <memory>
#include "object-internals.h"
#include "class-internals.h"
#include "icalls/mscorlib/System/Array.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/Type.h"

#include <vector>
#include <sstream>

using namespace il2cpp::vm;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

void Array::ClearInternal (Il2CppArray * arr, int32_t idx, int32_t length)
{
	int sz = il2cpp_array_element_size (arr->obj.klass);
	memset (il2cpp_array_addr_with_size (arr, sz, idx), 0, length * sz);
}

Il2CppArray * Array::Clone (Il2CppArray * arr)
{
	TypeInfo *typeInfo = arr->obj.klass;
	const uint32_t elem_size = il2cpp::vm::Array::GetElementSize (typeInfo);
	
	if (arr->bounds == NULL)
	{
		il2cpp_array_size_t len = il2cpp::vm::Array::GetLength (arr);
		Il2CppArray *clone = (Il2CppArray *)il2cpp::vm::Array::NewFull (typeInfo, &len, NULL);
		memcpy(&clone->vector, &arr->vector, elem_size * len);

		return clone;
	}

	il2cpp_array_size_t size = elem_size;
	std::vector<il2cpp_array_size_t> lengths (typeInfo->rank);
	std::vector<il2cpp_array_size_t> lowerBounds (typeInfo->rank);

	for (int i = 0; i < typeInfo->rank; ++i)
	{
		lengths [i] = arr->bounds [i].length;
		size *= arr->bounds [i].length;
		lowerBounds [i] = arr->bounds [i].lower_bound;
	}

	Il2CppArray* clone = il2cpp::vm::Array::NewFull (typeInfo, &lengths[0], &lowerBounds[0]);
	memcpy (&clone->vector, &arr->vector, size);

	return clone;
}

static std::string FormatCreateInstanceException(const Il2CppType* type)
{
	std::string typeName = vm::Type::GetName(type, IL2CPP_TYPE_NAME_FORMAT_IL);

	std::stringstream message;
	message << "Unable to create an array of type '" << typeName << "'. IL2CPP needs to know about the array type at compile time, so please define a private static field like this:\n\nprivate static " << typeName << "[] _unused;\n\nin any MonoBehaviour class, and this exception should go away.";

	return message.str();
}

Il2CppArray * Array::CreateInstanceImpl (Il2CppReflectionType * elementType,Il2CppArray* lengths, Il2CppArray* bounds)
{
	il2cpp_array_size_t* i32lengths = NULL;
	il2cpp_array_size_t* i32bounds = NULL;
	if(lengths != NULL)
		i32lengths = (il2cpp_array_size_t*)il2cpp_array_addr_with_size (lengths, il2cpp_array_element_size (lengths->obj.klass), 0);
	
	if(bounds != NULL)
		i32bounds = (il2cpp_array_size_t*)il2cpp_array_addr_with_size (bounds, il2cpp_array_element_size (bounds->obj.klass), 0);
	
	TypeInfo* arrayType = il2cpp::vm::Class::GetArrayClassCached (il2cpp::vm::Class::FromIl2CppType (elementType->type), il2cpp::vm::Array::GetLength (lengths));

	if (arrayType == NULL)
		vm::Exception::Raise(vm::Exception::GetInvalidOperationException(FormatCreateInstanceException(elementType->type).c_str()));

	return (Il2CppArray *)il2cpp::vm::Array::NewFull (arrayType, i32lengths, i32bounds);
}

bool Array::FastCopy (Il2CppArray *source, int32_t source_idx, Il2CppArray *dest, int32_t dest_idx, int32_t length)
{
	int element_size;
	//void * dest_addr;
	//void * source_addr;
	TypeInfo *src_class;
	TypeInfo *dest_class;
	int i;
	
	NOT_IMPLEMENTED_ICALL_NO_ASSERT (Array::FastCopy, "review against mono version");
	if (source->obj.klass->rank != dest->obj.klass->rank)
		return false;

	if (source->bounds || dest->bounds)
		return false;

	if (source->obj.klass->element_class->valuetype != dest->obj.klass->element_class->valuetype)
		return false;

	/* there's no integer overflow since mono_array_length returns an unsigned integer */
	if ((dest_idx + length > il2cpp::vm::Array::GetLength (dest)) ||
		(source_idx + length > il2cpp::vm::Array::GetLength (source)))
		return false;

	src_class = source->obj.klass->element_class;
	dest_class = dest->obj.klass->element_class;

	/*
	 * Handle common cases.
	 */

	/* Case1: object[] -> valuetype[] (ArrayList::ToArray) */
	if (src_class == il2cpp_defaults.object_class && dest_class->valuetype)
	{
		/*
		int has_refs = false;//dest_class->has_references;

		for (i = source_idx; i < source_idx + length; ++i)
		{
			Il2CppObject *elem = il2cpp_array_get (source, Il2CppObject*, i);
			if (elem && !mono_object_isinst (elem, dest_class))
				return FALSE;
		}*/

		element_size = il2cpp_array_element_size (dest->obj.klass);
		memset (il2cpp_array_addr_with_size (dest, element_size, dest_idx), 0, element_size * length);
		for (i = 0; i < length; ++i)
		{
			Il2CppObject *elem = il2cpp_array_get (source, Il2CppObject*, source_idx + i);
			void *addr = il2cpp_array_addr_with_size (dest, element_size, dest_idx + i);
			if (!elem)
				continue;
			/*if (has_refs)
				mono_value_copy (addr, (char *)elem + sizeof (Il2CppObject), dest_class);
			else*/
				memcpy (addr, (char *)elem + sizeof (Il2CppObject), element_size);
		}
		return true;
	}

	/* Check if we're copying a char[] <==> (u)short[] */
	//if (src_class != dest_class) {
	//	if (dest_class->valuetype || dest_class->enumtype || src_class->valuetype || src_class->enumtype)
	//		return false;

	//	if (mono_class_is_subclass_of (src_class, dest_class, FALSE))
	//		;
	//	/* Case2: object[] -> reftype[] (ArrayList::ToArray) */
	//	else if (mono_class_is_subclass_of (dest_class, src_class, FALSE))
	//		for (i = source_idx; i < source_idx + length; ++i) {
	//			MonoObject *elem = mono_array_get (source, MonoObject*, i);
	//			if (elem && !mono_object_isinst (elem, dest_class))
	//				return FALSE;
	//		}
	//	else
	//		return FALSE;
	//}

	//if (dest_class->valuetype) {
	//	element_size = mono_array_element_size (source->obj.vtable->klass);
	//	source_addr = mono_array_addr_with_size (source, element_size, source_idx);
	//	if (dest_class->has_references) {
	//		mono_value_copy_array (dest, dest_idx, source_addr, length);
	//	} else {
	//		dest_addr = mono_array_addr_with_size (dest, element_size, dest_idx);
	//		memmove (dest_addr, source_addr, element_size * length);
	//	}
	//} else {
		memmove((uint8_t*)dest + sizeof(Il2CppArray) + dest_idx * il2cpp_array_element_size (dest->obj.klass),
			(uint8_t*)source + sizeof(Il2CppArray) + source_idx * il2cpp_array_element_size (source->obj.klass),
			length * il2cpp_array_element_size (dest->obj.klass));
		//mono_array_memcpy_refs (dest, dest_idx, source, source_idx, length);
	//}

	return true;
}

int32_t Array::GetLength (Il2CppArray * __this,int dimension)
{
	int32_t rank = __this->obj.klass->rank;
	il2cpp_array_size_t length;

	if ((dimension < 0) || (dimension >= rank))
		il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetIndexOutOfRangeException ());
	
	if (__this->bounds == NULL)
		length = __this->max_length;
	else
		length = __this->bounds [dimension].length;

#ifdef IL2CPP_BIG_ARRAYS
	if (length > G_MAXINT32)
		mono_raise_exception (mono_get_exception_overflow ());
#endif
	return length;
}

int32_t Array::GetLowerBound (Il2CppArray * __this, int32_t dimension)
{
	int32_t rank = __this->obj.klass->rank;

	if ((dimension < 0) || (dimension >= rank))
		Exception::Raise (Exception::GetIndexOutOfRangeException ());
	
	if (__this->bounds == NULL)
	return false;
	
	return __this->bounds [dimension].lower_bound;
}

int Array::GetRank (Il2CppArray * arr)
{
	return arr->obj.klass->rank;
}

Il2CppObject * Array::GetValue (Il2CppArray * __this, Il2CppArray* indices)
{
	TypeInfo *ac, *ic;
	Il2CppArray *ao, *io;
	int32_t i, pos, *ind;

	IL2CPP_CHECK_ARG_NULL (indices);

	io = (Il2CppArray *)indices;
	ic = (TypeInfo *)io->obj.klass;
	
	ao = (Il2CppArray *)__this;
	ac = (TypeInfo *)ao->obj.klass;

	assert (ic->rank == 1);
	if (io->bounds != NULL || io->max_length !=  ac->rank)
		Exception::Raise (Exception::GetArgumentException (NULL, NULL));

	ind = (int32_t *)io->vector;

	if (ao->bounds == NULL)
	{
		if (*ind < 0 || *ind >= ao->max_length)
			Exception::Raise (Exception::GetIndexOutOfRangeException ());

		return GetValueImpl (__this, *ind);
	}
	
	for (i = 0; i < ac->rank; i++)
		if ((ind [i] < ao->bounds [i].lower_bound) ||
			(ind [i] >= ao->bounds [i].length + ao->bounds [i].lower_bound))
			Exception::Raise (Exception::GetIndexOutOfRangeException ());

	pos = ind [0] - ao->bounds [0].lower_bound;
	for (i = 1; i < ac->rank; i++)
		pos = pos*ao->bounds [i].length + ind [i] - ao->bounds [i].lower_bound;

	return GetValueImpl (__this, pos);
}

Il2CppObject * Array::GetValueImpl (Il2CppArray * __this, int32_t pos)
{
	TypeInfo* typeInfo = __this->obj.klass;
	void **ea;

	ea = (void**)load_array_elema(__this, pos, typeInfo->element_size);

	if (typeInfo->element_class->valuetype)
		return il2cpp::vm::Object::Box (typeInfo->element_class, ea);

	return (Il2CppObject*)*ea;
}

void Array::SetValue (Il2CppArray * __this, Il2CppObject* value, Il2CppArray* idxs)
{
	TypeInfo *ac, *ic;
	int32_t i, pos, *ind;

	IL2CPP_CHECK_ARG_NULL (idxs);

	ic = idxs->obj.klass;
	ac = __this->obj.klass;

	assert (ic->rank == 1);
	if (idxs->bounds != NULL || idxs->max_length != ac->rank)
		Exception::Raise (Exception::GetArgumentException (NULL, NULL));

	ind = (int32_t *)idxs->vector;

	if (__this->bounds == NULL)
	{
		if (*ind < 0 || *ind >= __this->max_length)
			Exception::Raise (Exception::GetIndexOutOfRangeException ());

		SetValueImpl (__this, value, *ind);
		return;
	}
	
	for (i = 0; i < ac->rank; i++)
		if ((ind [i] < __this->bounds [i].lower_bound) ||
			(ind [i] >= (il2cpp_array_lower_bound_t)__this->bounds [i].length + __this->bounds [i].lower_bound))
			Exception::Raise (Exception::GetIndexOutOfRangeException ());

	pos = ind [0] - __this->bounds [0].lower_bound;
	for (i = 1; i < ac->rank; i++)
		pos = pos * __this->bounds [i].length + ind [i] -
			__this->bounds [i].lower_bound;

	SetValueImpl (__this, value, pos);
}

static void ThrowNoWidening ()
{
	Exception::Raise (Exception::GetArgumentException ("value", "not a widening conversion"));
}

static void ThrowInvalidCast (const TypeInfo* a, const TypeInfo* b)
{
	Exception::Raise (Exception::GetInvalidCastException (Exception::FormatInvalidCastException (b, a).c_str ()));
}

union WidenedValueUnion
{
	int64_t i64;
	uint64_t u64;
	double r64;
};

WidenedValueUnion ExtractWidenedValue (Il2CppTypeEnum type, void* value)
{
	WidenedValueUnion extractedValue = { 0 };
	switch (type)
	{
		case IL2CPP_TYPE_U1:
			extractedValue.u64 = *(uint8_t *)value;
			break;
		case IL2CPP_TYPE_CHAR:
		case IL2CPP_TYPE_U2:
			extractedValue.u64 = *(uint16_t *)value;
			break;
		case IL2CPP_TYPE_U4:
			extractedValue.u64 = *(uint32_t *)value;
			break;
		case IL2CPP_TYPE_U8:
			extractedValue.u64 = *(uint64_t *)value;
			break;
		case IL2CPP_TYPE_I1:
			extractedValue.i64 = *(int8_t *)value;
			break;
		case IL2CPP_TYPE_I2:
			extractedValue.i64 = *(int16_t *)value;
			break;
		case IL2CPP_TYPE_I4:
			extractedValue.i64 = *(int32_t *)value;
			break;
		case IL2CPP_TYPE_I8:
			extractedValue.i64 = *(int64_t *)value;
			break;
		case IL2CPP_TYPE_R4:
			extractedValue.r64 = *(float *)value;
			break;
		case IL2CPP_TYPE_R8:
			extractedValue.r64 = *(double *)value;
			break;
		default:
			assert (0);
			break;
	}

	return extractedValue;
}

static void CheckWideningConversion (size_t elementSize, size_t valueSize, size_t extra = 0)
{
	if (elementSize < valueSize + (extra))
		ThrowNoWidening ();
}

template <typename T>
static void AssignUnsigned (WidenedValueUnion value, void* elementAddress, Il2CppTypeEnum valueType, size_t elementSize, size_t valueSize)
{
	switch (valueType)
	{
		case IL2CPP_TYPE_U1:
		case IL2CPP_TYPE_U2:
		case IL2CPP_TYPE_U4:
		case IL2CPP_TYPE_U8:
		case IL2CPP_TYPE_CHAR:
			CheckWideningConversion (elementSize, valueSize);
			*(T*)elementAddress = (T)value.u64;
			break;
		/* You can't assign a signed value to an unsigned array. */
		case IL2CPP_TYPE_I1:
		case IL2CPP_TYPE_I2:
		case IL2CPP_TYPE_I4:
		case IL2CPP_TYPE_I8:
		/* You can't assign a floating point number to an integer array. */
		case IL2CPP_TYPE_R4:
		case IL2CPP_TYPE_R8:
			ThrowNoWidening ();
			break;
		default:
			assert (0);
			break;
	}
}

template <typename T>
static void AssignSigned (WidenedValueUnion value, void* elementAddress, Il2CppTypeEnum valueType, size_t elementSize, size_t valueSize)
{
	switch (valueType)
	{
		/* You can assign an unsigned value to a signed array if the array's
			element size is larger than the value size. */
		case IL2CPP_TYPE_U1:
		case IL2CPP_TYPE_U2:
		case IL2CPP_TYPE_U4:
		case IL2CPP_TYPE_U8:
		case IL2CPP_TYPE_CHAR:
			CheckWideningConversion (elementSize, valueSize, 1);
			*(T*)elementAddress = (T)value.u64;
			break;
		case IL2CPP_TYPE_I1:
		case IL2CPP_TYPE_I2:
		case IL2CPP_TYPE_I4:
		case IL2CPP_TYPE_I8:
			CheckWideningConversion (elementSize, valueSize);
			*(T*)elementAddress = (T)value.i64;
			break;
		/* You can't assign a floating point number to an integer array. */
		case IL2CPP_TYPE_R4:
		case IL2CPP_TYPE_R8:
			ThrowNoWidening ();
			break;
		default:
			assert (0);
			break;
	}
}

template <typename T>
static void AssignReal (WidenedValueUnion value, void* elementAddress, Il2CppTypeEnum valueType, size_t elementSize, size_t valueSize)
{
	switch (valueType)
	{
		/* All integers fit into the floating point value range. No need to check size. */
		case IL2CPP_TYPE_U1:
		case IL2CPP_TYPE_U2:
		case IL2CPP_TYPE_U4:
		case IL2CPP_TYPE_U8:
		case IL2CPP_TYPE_CHAR:
			*(T*)elementAddress = (T)value.u64;
			break;
		case IL2CPP_TYPE_I1:
		case IL2CPP_TYPE_I2:
		case IL2CPP_TYPE_I4:
		case IL2CPP_TYPE_I8:
			*(T*)elementAddress = (T)value.i64;
			break;
		case IL2CPP_TYPE_R4:
		case IL2CPP_TYPE_R8:
			CheckWideningConversion (elementSize, valueSize);
			*(T*)elementAddress = (T)value.r64;
			break;
		default:
			assert (0);
			break;
	}
}

void Array::SetValueImpl (Il2CppArray * __this,Il2CppObject * value, int index)
{
	TypeInfo* typeInfo = __this->obj.klass;
	TypeInfo* elementClass = Class::GetElementClass (typeInfo);

	int elementSize = Class::GetArrayElementSize (elementClass);
	void* elementAddress = il2cpp_array_addr_with_size (__this, elementSize, index);

	if (Class::IsNullable (elementClass))
	{
		NOT_IMPLEMENTED_ICALL (Array::SetValueImpl);
	}

	if (value == NULL)
	{
		memset (elementAddress, 0, elementSize);
		return;
	}

	if (!Class::IsValuetype (elementClass))
	{
		if (!Object::IsInst (value, elementClass))
			Exception::Raise (Exception::GetInvalidCastException (Exception::FormatInvalidCastException (__this->obj.klass->element_class, value->klass).c_str ()));
		il2cpp_array_setref (__this, index, value);
		return;
	}

	if (Object::IsInst (value, elementClass))
	{
		// write barrier
		memcpy (elementAddress, Object::Unbox (value), elementSize);
		return;
	}

	TypeInfo* valueClass = Object::GetClass (value);

	if (!Class::IsValuetype (valueClass))
		ThrowInvalidCast (elementClass, valueClass);

	int valueSize = Class::GetInstanceSize (valueClass) - sizeof (Il2CppObject);

	Il2CppTypeEnum elementType = Class::IsEnum (elementClass) ? Class::GetEnumBaseType (elementClass)->type : elementClass->byval_arg->type;
	Il2CppTypeEnum valueType = Class::IsEnum (valueClass) ? Class::GetEnumBaseType (valueClass)->type : valueClass->byval_arg->type;
	
	if (elementType == IL2CPP_TYPE_BOOLEAN)
	{
		switch (valueType)
		{
			case IL2CPP_TYPE_BOOLEAN:
				break;
			case IL2CPP_TYPE_CHAR:
			case IL2CPP_TYPE_U1:
			case IL2CPP_TYPE_U2:
			case IL2CPP_TYPE_U4:
			case IL2CPP_TYPE_U8:
			case IL2CPP_TYPE_I1:
			case IL2CPP_TYPE_I2:
			case IL2CPP_TYPE_I4:
			case IL2CPP_TYPE_I8:
			case IL2CPP_TYPE_R4:
			case IL2CPP_TYPE_R8:
				ThrowNoWidening ();
			default:
				ThrowInvalidCast (elementClass, valueClass);
		}
	}

	WidenedValueUnion widenedValue = ExtractWidenedValue (valueType, Object::Unbox(value));

	switch (elementType)
	{
	case IL2CPP_TYPE_U1:
		AssignUnsigned<uint8_t> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	case IL2CPP_TYPE_CHAR:
	case IL2CPP_TYPE_U2:
		AssignUnsigned<uint16_t> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	case IL2CPP_TYPE_U4:
		AssignUnsigned<uint32_t> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	case IL2CPP_TYPE_U8:
		AssignUnsigned<uint64_t> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	case IL2CPP_TYPE_I1:
		AssignSigned<int8_t> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	case IL2CPP_TYPE_I2:
		AssignSigned<int16_t> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	case IL2CPP_TYPE_I4:
		AssignSigned<int32_t> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	case IL2CPP_TYPE_I8:
		AssignSigned<int64_t> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	case IL2CPP_TYPE_R4:
		AssignReal<float> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	case IL2CPP_TYPE_R8:
		AssignReal<double> (widenedValue, elementAddress, valueType, elementSize, valueSize);
		break;
	default:
		ThrowInvalidCast (elementClass, valueClass);
		break;
	}
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
