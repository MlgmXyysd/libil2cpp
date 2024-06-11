#pragma once

#include <string>
#include "il2cpp-class-internals.h"

struct MethodInfo;
struct Il2CppGenericMethod;
struct Il2CppGenericContext;
struct Il2CppGenericInst;

namespace il2cpp
{
namespace metadata
{
    class GenericMethod
    {
    public:
        // exported

    public:
        //internal
        static const MethodInfo* GetMethod(const MethodInfo* methodDefinition, const Il2CppGenericInst* classInst, const Il2CppGenericInst* methodInst);
        static const MethodInfo* GetMethod(const Il2CppGenericMethod* gmethod);
        static void GetVirtualInvokeData(const MethodInfo* methodDefinition, const Il2CppGenericInst* classInst, const Il2CppGenericInst* methodInst, VirtualInvokeData* invokeData);
        static bool IsGenericAmbiguousMethodInfo(const MethodInfo* method);
        static const Il2CppGenericContext* GetContext(const Il2CppGenericMethod* gmethod);
        static std::string GetFullName(const Il2CppGenericMethod* gmethod);

        static void ClearStatics();

    private:
        static const MethodInfo* GetMethod(const Il2CppGenericMethod* gmethod, bool copyMethodPtr);
    };
} /* namespace vm */
} /* namespace il2cpp */
