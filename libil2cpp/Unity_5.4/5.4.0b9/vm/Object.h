#pragma once

#include <stdint.h>

struct Il2CppString;
struct Il2CppObject;
struct TypeInfo;
struct MethodInfo;

namespace il2cpp
{
namespace vm
{

class Object
{
public:
	static Il2CppObject* Box (TypeInfo *klass, void* data);
	static TypeInfo* GetClass (Il2CppObject* obj);
	static int32_t GetHash (Il2CppObject* obj);
	static uint32_t GetSize (Il2CppObject* obj);
	static const MethodInfo* GetVirtualMethod (Il2CppObject *obj, const MethodInfo *method);
	static Il2CppObject * IsInst (Il2CppObject *obj, TypeInfo *klass);
	static Il2CppObject* New (TypeInfo *klass);
	static void* Unbox (Il2CppObject* obj);
	static void UnboxNullable(Il2CppObject* obj, TypeInfo* klass, void* storage);

	static Il2CppObject * Clone (Il2CppObject *obj);
	static Il2CppObject* NewPinned (TypeInfo *klass);
private:
	static Il2CppObject * NewAllocSpecific (TypeInfo *klass);
	static Il2CppObject* NewPtrFree (TypeInfo *klass);
	static Il2CppObject* Allocate (size_t size, TypeInfo *typeInfo);
	static Il2CppObject* AllocatePtrFree (size_t size, TypeInfo *typeInfo);
	static Il2CppObject* AllocateSpec (size_t size, TypeInfo *typeInfo);

	friend class Array;
	friend class RCW;
	friend class String;
};

} /* namespace vm */
} /* namespace il2cpp */

#define IL2CPP_OBJECT_SETREF(obj,fieldname,value) do {  \
        /*mono_gc_wbarrier_set_field ((Il2CppObject*)(obj), &((obj)->fieldname), (Il2CppObject*)value); */\
        (obj)->fieldname = (value); \
    } while (0)
