#include "il2cpp-config.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
#include "vm/Array.h"
#include "vm/Object.h"
#include "vm/Reflection.h"
#include "icalls/mscorlib/System.Diagnostics/StackTrace.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Diagnostics
{
    Il2CppArray* StackTrace::get_trace(Il2CppException *exc, int32_t skip, bool need_file_info)
    {
        /* Exception is not thrown yet */
        if (exc->trace_ips == NULL)
            return vm::Array::New(il2cpp_defaults.stack_frame_class, 0);

        int len = vm::Array::GetLength(exc->trace_ips);
        Il2CppArray* stackFrames = vm::Array::New(il2cpp_defaults.stack_frame_class, len > skip ? len - skip : 0);

        for (int i = skip; i < len; i++)
        {
            Il2CppStackFrame* stackFrame = NULL;
#if IL2CPP_MONO_DEBUGGER
            stackFrame = il2cpp_array_get(exc->trace_ips, Il2CppStackFrame*, i);
#else
            stackFrame = (Il2CppStackFrame*)vm::Object::New(il2cpp_defaults.stack_frame_class);
            MethodInfo* method = il2cpp_array_get(exc->trace_ips, MethodInfo*, i);

            IL2CPP_OBJECT_SETREF(stackFrame, method, vm::Reflection::GetMethodObject(method, NULL));
#endif
            il2cpp_array_setref(stackFrames, i, stackFrame);
        }

        return stackFrames;
    }
} /* namespace Diagnostics */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
