#pragma once

#if RUNTIME_MONO
typedef MonoClass RuntimeClass;
typedef MonoMethod RuntimeMethod;
typedef MonoClassField RuntimeField;
typedef MonoType RuntimeType;
typedef MonoObject RuntimeObject;
typedef MonoImage RuntimeImage;
typedef MonoException RuntimeException;
typedef MonoArray RuntimeArray;
typedef MonoAssembly RuntimeAssembly;
typedef MonoString RuntimeString;
typedef MonoStringBuilder RuntimeStringBuilder;
typedef MonoDelegate RuntimeDelegate;
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
typedef Il2CppObject RuntimeObject;
typedef Il2CppImage RuntimeImage;
typedef Il2CppException RuntimeException;
typedef Il2CppArray RuntimeArray;
typedef Il2CppAssembly RuntimeAssembly;
typedef Il2CppString RuntimeString;
struct Il2CppStringBuilder;
typedef Il2CppStringBuilder RuntimeStringBuilder;
typedef Il2CppDelegate RuntimeDelegate;
#include "il2cpp-codegen-il2cpp.h"
#endif
