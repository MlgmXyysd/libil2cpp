#include "il2cpp-config.h"
#include <cassert>
#include "icalls/mscorlib/System.Diagnostics/StackFrame.h"
#include "vm/Reflection.h"
#include "vm/StackTrace.h"
#include "class-internals.h"

using namespace il2cpp::vm;

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

bool StackFrame::get_frame_info (
	int32_t skip,
	bool needFileInfo,
	Il2CppReflectionMethod ** method,
	int32_t* iloffset,
	int32_t* native_offset,
	Il2CppString** file,
	int32_t* line,
	int32_t* column)
{
	const StackFrames& stack = *StackTrace::GetStackFrames ();

	int64_t index = stack.size () - skip - 1;
	if (index >= stack.size () || index < 0)
		return false;

	const Il2CppStackFrameInfo& info = stack[index];

	NOT_IMPLEMENTED_ICALL_NO_ASSERT (StackFrame::get_frame_info, "use gc write barrier");
	*method = Reflection::GetMethodObject (info.method, info.method->declaring_type);

	return true;
}

} /* namespace Diagnostics */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
