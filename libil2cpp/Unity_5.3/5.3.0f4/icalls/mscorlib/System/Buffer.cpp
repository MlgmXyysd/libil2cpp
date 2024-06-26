#include "il2cpp-config.h"
#include <cassert>
#include <memory>
#include "icalls/mscorlib/System/Buffer.h"
#include "class-internals.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"

using il2cpp::vm::Class;

#ifdef _MSC_VER
# define ISNAN _isnan
#else
# define ISNAN isnan
#endif

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

bool Buffer::BlockCopyInternal (Il2CppArray * src, int src_offset, Il2CppArray * dest, int dest_offset, int count)
{
	IL2CPP_CHECK_ARG_NULL (src);
	IL2CPP_CHECK_ARG_NULL (dest);

	/* watch out for integer overflow */
	if (((uint32_t)src_offset > il2cpp::vm::Array::GetByteLength(src) - count) || ((uint32_t)dest_offset > il2cpp::vm::Array::GetByteLength(dest) - count))
		return false;

	char *src_buf = ((char*)il2cpp_array_addr_with_size (src, Class::GetInstanceSize (src->obj.klass->element_class), 0)) + src_offset;
	char *dest_buf = ((char*)il2cpp_array_addr_with_size (dest, Class::GetInstanceSize (dest->obj.klass->element_class), 0)) + dest_offset;

	if(src != dest)
		memcpy (dest_buf, src_buf, count);
	else
		memmove (dest_buf, src_buf, count); /* Source and dest are the same array */

	return true;
}

int32_t Buffer::ByteLengthInternal (Il2CppArray* arr)
{
	return il2cpp::vm::Array::GetByteLength (arr);
}

uint8_t Buffer::GetByteInternal (Il2CppArray* arr, int idx)
{
	return il2cpp_array_get (arr, uint8_t, idx);
}

void Buffer::SetByteInternal (Il2CppArray* arr, int idx, int value)
{
	il2cpp_array_set (arr, uint8_t, idx, value);
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
