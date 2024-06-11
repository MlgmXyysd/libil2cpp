#pragma once

#if RUNTIME_MONO
typedef MonoClass RuntimeClass;
typedef MonoMethod RuntimeMethod;
typedef MonoClassField RuntimeField;
typedef MonoType RuntimeType;
#include "il2cpp-codegen-mono.h"


#else
struct TypeInfo;
struct MethodInfo;
struct FieldInfo;
struct Il2CppType;
typedef Il2CppClass RuntimeClass;
typedef MethodInfo RuntimeMethod;
typedef FieldInfo RuntimeField;
typedef Il2CppType RuntimeType;
#include "il2cpp-codegen-il2cpp.h"
#endif
