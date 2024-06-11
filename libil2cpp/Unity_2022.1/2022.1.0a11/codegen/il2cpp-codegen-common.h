#pragma once

#if IL2CPP_ENABLE_WRITE_BARRIERS
void Il2CppCodeGenWriteBarrier(void** targetAddress, void* object);
void Il2CppCodeGenWriteBarrierForType(const Il2CppType* type, void** targetAddress, void* object);
void Il2CppCodeGenWriteBarrierForClass(Il2CppClass* klass, void** targetAddress, void* object);
#else
inline void Il2CppCodeGenWriteBarrier(void** targetAddress, void* object) {}
inline void Il2CppCodeGenWriteBarrierForType(const Il2CppType* type, void** targetAddress, void* object) {}
inline void Il2CppCodeGenWriteBarrierForClass(Il2CppClass* klass, void** targetAddress, void* object) {}
#endif
