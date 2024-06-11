#include "il2cpp-config.h"

#if RUNTIME_TINY

#include "il2cpp-codegen.h"

#include "vm/StackTrace.h"

void il2cpp_codegen_stacktrace_push_frame(TinyStackFrameInfo& frame)
{
    tiny::vm::StackTrace::PushFrame(frame);
}

void il2cpp_codegen_stacktrace_pop_frame()
{
    tiny::vm::StackTrace::PopFrame();
}

#endif
