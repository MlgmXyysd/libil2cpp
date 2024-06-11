#pragma once

#include <stdint.h>

struct Il2CppArray;
struct Il2CppObject;
struct Il2CppString;
struct TypeInfo;

namespace il2cpp
{
namespace vm
{

class Array
{
public:
	static int32_t GetElementSize (const TypeInfo *klass);
	static uint32_t GetLength (Il2CppArray* array);
	static uint32_t GetByteLength (Il2CppArray* array);
	static Il2CppArray* New (TypeInfo *elementTypeInfo, il2cpp_array_size_t length);
	static Il2CppArray* NewSpecific (TypeInfo *arrayTypeInfo, il2cpp_array_size_t length);
	static Il2CppArray* NewFull (TypeInfo *array_class, il2cpp_array_size_t *lengths, il2cpp_array_size_t *lower_bounds);
public:
	// internal
	static Il2CppArray* NewCached (TypeInfo *elementTypeInfo, il2cpp_array_size_t length)
	{
		return New (elementTypeInfo, length);
	}

	static char* GetFirstElementAddress (Il2CppArray *array);
};

} /* namespace vm */
} /* namespace il2cpp */

char* il2cpp_array_addr_with_size (Il2CppArray *array, int32_t size, uintptr_t idx);

extern "C"
{
	IL2CPP_EXPORT int il2cpp_array_element_size (const TypeInfo *ac);
}

#define load_array_elema(arr,idx,size) ((((uint8_t*)(arr)) + kIl2CppSizeOfArray) + ((size) * (idx)))

#define il2cpp_array_setwithsize(array, elementSize, index, value)  \
    do {    \
        void*__p = (void*) il2cpp_array_addr_with_size ((array), elementSize, (index)); \
        memcpy(__p, &(value), elementSize); \
    } while (0)
#define il2cpp_array_setrefwithsize(array, elementSize, index, value)  \
    do {    \
        void*__p = (void*) il2cpp_array_addr_with_size ((array), elementSize, (index)); \
        memcpy(__p, value, elementSize); \
	    } while (0)
#define il2cpp_array_addr(array,type,index) ((type*)(void*) il2cpp_array_addr_with_size (array, sizeof (type), index))
#define il2cpp_array_get(array,type,index) ( *(type*)il2cpp_array_addr ((array), type, (index)) )
#define il2cpp_array_set(array,type,index,value)    \
    do {    \
        type *__p = (type *) il2cpp_array_addr ((array), type, (index));    \
        *__p = (value); \
    } while (0)
#define il2cpp_array_setref(array,index,value)  \
    do {    \
        void* *__p = (void* *) il2cpp_array_addr ((array), void*, (index)); \
        /* il2cpp_gc_wbarrier_set_arrayref ((array), __p, (MonoObject*)(value));    */\
         *__p = (value);    \
    } while (0)
