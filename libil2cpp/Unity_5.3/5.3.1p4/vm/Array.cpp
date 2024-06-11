#include "il2cpp-config.h"
#include "gc/gc-internal.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/Profiler.h"
#include "class-internals.h"
#include "object-internals.h"
#include <assert.h>
#include <memory>

namespace il2cpp
{
namespace vm
{

int32_t Array::GetElementSize (const TypeInfo *klass)
{
	assert (klass->rank);
	return klass->element_size;
}

uint32_t Array::GetLength (Il2CppArray* array)
{
	return array->max_length;
}

uint32_t Array::GetByteLength (Il2CppArray* array)
{
	TypeInfo *klass;
	int length;
	int i;

	klass = array->obj.klass;

	if (array->bounds == NULL)
		length = array->max_length;
	else {
		length = 1;
		for (i = 0; i < klass->rank; ++ i)
			length *= array->bounds [i].length;
	}

	return length * GetElementSize (klass);
}

Il2CppArray* Array::New (TypeInfo *elementTypeInfo, il2cpp_array_size_t length)
{
	return NewSpecific (Class::GetArrayClass (elementTypeInfo, 1), length);
}


Il2CppArray* Array::New2 (TypeInfo *cm, uint32_t length1, uint32_t length2)
{
	il2cpp_array_size_t lengths [2];
	il2cpp_array_size_t *lower_bounds;
	//int pcount;
	//int rank;

	//pcount = mono_method_signature (cm)->param_count;
	//rank = cm->klass->rank;

	lengths [0] = length1;
	lengths [1] = length2;

	//g_assert (rank == pcount);
	
	NOT_IMPLEMENTED_NO_ASSERT (Array::New2,"IGNORING non-zero based arrays!");

	//if (cm->klass->byval_arg.type == MONO_TYPE_ARRAY) {
	//	lower_bounds = alloca (sizeof (uint32_t) * rank);
	//	memset (lower_bounds, 0, sizeof (uint32_t) * rank);
	//} else {
		lower_bounds = NULL;
	//}

	return Array::NewFull (cm, lengths, lower_bounds);
}

Il2CppArray* Array::New3 (TypeInfo *cm, uint32_t length1, uint32_t length2, uint32_t length3)
{
	il2cpp_array_size_t lengths [3];
	il2cpp_array_size_t *lower_bounds;
	//int pcount;
	//int rank;

	//pcount = mono_method_signature (cm)->param_count;
	//rank = cm->klass->rank;

	lengths [0] = length1;
	lengths [1] = length2;
	lengths [2] = length3;

	//g_assert (rank == pcount);
	
	NOT_IMPLEMENTED_NO_ASSERT (Array::New3,"IGNORING non-zero based arrays!");
	
	//if (cm->klass->byval_arg.type == MONO_TYPE_ARRAY) {
	//	lower_bounds = alloca (sizeof (uint32_t) * rank);
	//	memset (lower_bounds, 0, sizeof (uint32_t) * rank);
	//} else {
		lower_bounds = NULL;
	//}

	return Array::NewFull (cm, lengths, lower_bounds);
}

Il2CppArray* Array::New4 (TypeInfo *cm, uint32_t length1, uint32_t length2, uint32_t length3, uint32_t length4)
{
	il2cpp_array_size_t lengths [4];
	il2cpp_array_size_t *lower_bounds;
	//int pcount;
	//int rank;

	//pcount = mono_method_signature (cm)->param_count;
	//rank = cm->klass->rank;

	lengths[0] = length1;
	lengths[1] = length2;
	lengths[2] = length3;
	lengths[3] = length4;

	//g_assert (rank == pcount);

	NOT_IMPLEMENTED_NO_ASSERT (Array::New4, "IGNORING non-zero based arrays!");

	//if (cm->klass->byval_arg.type == MONO_TYPE_ARRAY) {
	//	lower_bounds = alloca (sizeof (uint32_t) * rank);
	//	memset (lower_bounds, 0, sizeof (uint32_t) * rank);
	//} else {
	lower_bounds = NULL;
	//}

	return Array::NewFull (cm, lengths, lower_bounds);
}

static void RaiseOverflowException()
{
	vm::Exception::Raise(vm::Exception::GetOverflowException("Arithmetic operation resulted in an overflow."));
}

Il2CppArray* Array::NewSpecific (TypeInfo *klass, il2cpp_array_size_t n)
{
	Il2CppObject *o;
	Il2CppArray *ao;
	uint32_t byte_len, elem_size;

	Class::Init (klass);
	assert (klass->rank);
	assert (klass->initialized);
	assert (klass->element_class->initialized);

	NOT_IMPLEMENTED_NO_ASSERT (Array::NewSpecific,"Not checking for overflow");
	NOT_IMPLEMENTED_NO_ASSERT (Array::NewSpecific,"Handle allocations with a GC descriptor");

	if (n < 0)
		RaiseOverflowException();

	//if (G_UNLIKELY (n > MONO_ARRAY_MAX_INDEX)) {
	//	arith_overflow ();
	//	return NULL;
	//}
	
	elem_size = il2cpp_array_element_size (klass);
	//if (CHECK_MUL_OVERFLOW_UN (n, elem_size)) {
	//	mono_gc_out_of_memory (MONO_ARRAY_MAX_SIZE);
	//	return NULL;
	//}
	byte_len = n * elem_size;
	//if (CHECK_ADD_OVERFLOW_UN (byte_len, sizeof (MonoArray))) {
	//	mono_gc_out_of_memory (MONO_ARRAY_MAX_SIZE);
	//	return NULL;
	//}
	byte_len += sizeof (Il2CppArray);
	if (!klass->has_references) {
		o = Object::AllocatePtrFree (byte_len, klass);
#if NEED_TO_ZERO_PTRFREE
		((Il2CppArray*)o)->bounds = NULL;
		memset ((char*)o + sizeof (Il2CppObject), 0, byte_len - sizeof (Il2CppObject));
#endif
	}
#if IL2CPP_HAS_GC_DESCRIPTORS
	else if (klass->gc_desc != GC_NO_DESCRIPTOR)
	{
		o = Object::AllocateSpec (byte_len, klass);
	}
#endif
	else {
		o = Object::Allocate (byte_len, klass);
	}

	ao = (Il2CppArray *)o;
	ao->max_length = n;
	
#if IL2CPP_ENABLE_PROFILER
	if (Profiler::ProfileAllocations ())
		Profiler::Allocation (o, klass);
#endif

	return ao;
}

Il2CppArray* Array::NewFull (TypeInfo *array_class, il2cpp_array_size_t *lengths, il2cpp_array_size_t *lower_bounds)
{

	il2cpp_array_size_t byte_len, len, bounds_size;
	Il2CppObject *o;
	Il2CppArray *array;
	int i;

	Class::Init (array_class);
	assert (array_class->rank);
	assert (array_class->initialized);
	assert (array_class->element_class->initialized);

	NOT_IMPLEMENTED_NO_ASSERT (Array::New2,"IGNORING non-zero based arrays!");
	NOT_IMPLEMENTED_NO_ASSERT (Array::NewSpecific,"Handle allocations with a GC descriptor");

	for (i = 0; i < array_class->rank; ++i)
	{
		if (lengths[i] < 0)
			RaiseOverflowException();
	}

	byte_len = il2cpp_array_element_size (array_class);
	len = 1;

	/* A single dimensional array with a 0 lower bound is the same as an szarray */
	if (array_class->rank == 1 && ((array_class->byval_arg->type == IL2CPP_TYPE_SZARRAY) || (lower_bounds && lower_bounds [0] == 0)))
	{
		len = lengths [0];
		if (len > IL2CPP_ARRAY_MAX_INDEX)//MONO_ARRAY_MAX_INDEX
			RaiseOverflowException();
		bounds_size = 0;
	}
	else
	{
		bounds_size = sizeof (Il2CppArrayBounds) * array_class->rank;

		for (i = 0; i < array_class->rank; ++i) {
			//if (lengths [i] > IL2CPP_ARRAY_MAX_INDEX) //MONO_ARRAY_MAX_INDEX
			//	arith_overflow ();
			//if (CHECK_MUL_OVERFLOW_UN (len, lengths [i]))
			//	mono_gc_out_of_memory (MONO_ARRAY_MAX_SIZE);
			len *= lengths [i];
		}
	}

	//if (CHECK_MUL_OVERFLOW_UN (byte_len, len))
	//	mono_gc_out_of_memory (MONO_ARRAY_MAX_SIZE);
	byte_len *= len;
	//if (CHECK_ADD_OVERFLOW_UN (byte_len, sizeof (MonoArray)))
	//	mono_gc_out_of_memory (MONO_ARRAY_MAX_SIZE);
	byte_len += sizeof (Il2CppArray);
	if (bounds_size) {
		/* align */
		//if (CHECK_ADD_OVERFLOW_UN (byte_len, 3))
		//	mono_gc_out_of_memory (MONO_ARRAY_MAX_SIZE);
		byte_len = (byte_len + 3) & ~3;
		//if (CHECK_ADD_OVERFLOW_UN (byte_len, bounds_size))
		//	mono_gc_out_of_memory (MONO_ARRAY_MAX_SIZE);
		byte_len += bounds_size;
	}
	/*
	 * Following three lines almost taken from mono_object_new ():
	 * they need to be kept in sync.
	 */
	if (!array_class->has_references) {
		o = Object::AllocatePtrFree (byte_len, array_class);
#if NEED_TO_ZERO_PTRFREE
		memset ((char*)o + sizeof (Il2CppObject), 0, byte_len - sizeof (Il2CppObject));
#endif
	}
#if IL2CPP_HAS_GC_DESCRIPTORS
	else if (array_class->gc_desc != GC_NO_DESCRIPTOR) {
		o = Object::AllocateSpec (byte_len, array_class);
	}
#endif
	else {
		o = Object::Allocate (byte_len, array_class);
	}
	o =  (Il2CppObject*)Object::Allocate (byte_len, array_class);

	array = (Il2CppArray*)o;
	array->max_length = len;

	if (bounds_size) {
		Il2CppArrayBounds *bounds = (Il2CppArrayBounds*)((char*)array + byte_len - bounds_size);
		array->bounds = bounds;
		for (i = 0; i < array_class->rank; ++i) {
			bounds [i].length = lengths [i];
			if (lower_bounds)
				bounds [i].lower_bound = lower_bounds [i];
		}
	}

#if IL2CPP_ENABLE_PROFILER
	if (Profiler::ProfileAllocations ())
		Profiler::Allocation (o, array_class);
#endif

	return array;
}

} /* namespace vm */
} /* namespace il2cpp */

char*
il2cpp_array_addr_with_size (Il2CppArray *array, int32_t size, uintptr_t idx)
{
	return ((char*)array) + sizeof(Il2CppArray) + size * idx;
}

int32_t
il2cpp_array_element_size (TypeInfo *ac)
{
	return il2cpp::vm::Array::GetElementSize (ac);
}
