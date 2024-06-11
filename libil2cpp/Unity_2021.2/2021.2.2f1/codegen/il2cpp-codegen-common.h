#pragma once

#ifdef _MSC_VER
#define IL2CPP_DISABLE_OPTIMIZATIONS __pragma(optimize("", off))
#define IL2CPP_ENABLE_OPTIMIZATIONS __pragma(optimize("", on))
#elif IL2CPP_TARGET_LINUX
#define IL2CPP_DISABLE_OPTIMIZATIONS
#define IL2CPP_ENABLE_OPTIMIZATIONS
#else
#define IL2CPP_DISABLE_OPTIMIZATIONS __attribute__ ((optnone))
#define IL2CPP_ENABLE_OPTIMIZATIONS
#endif


#if IL2CPP_ENABLE_WRITE_BARRIERS
void Il2CppCodeGenWriteBarrier(void** targetAddress, void* object);
void Il2CppCodeGenWriteBarrierForType(const Il2CppType* type, void** targetAddress, void* object);
void Il2CppCodeGenWriteBarrierForClass(Il2CppClass* klass, void** targetAddress, void* object);
#else
inline void Il2CppCodeGenWriteBarrier(void** targetAddress, void* object) {}
inline void Il2CppCodeGenWriteBarrierForType(const Il2CppType* type, void** targetAddress, void* object) {}
inline void Il2CppCodeGenWriteBarrierForClass(Il2CppClass* klass, void** targetAddress, void* object) {}
#endif
