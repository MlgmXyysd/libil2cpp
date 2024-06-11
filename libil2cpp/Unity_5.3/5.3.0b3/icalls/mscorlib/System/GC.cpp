#include "il2cpp-config.h"

#include <cassert>

#include "icalls/mscorlib/System/GC.h"
#include "gc/gc-internal.h"
#include "vm/GC.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

void GC::SuppressFinalize(Il2CppObject *obj)
{
	///* delegates have no finalizers, but we register them to deal with the
	//* unmanaged->managed trampoline. We don't let the user suppress it
	//* otherwise we'd leak it.
	//*/
	//if ( obj->klass->delegate )
	//	return;

	/* FIXME: Need to handle case where obj has COM Callable Wrapper
	* generated for it that needs cleaned up, but user wants to suppress
	* their derived object finalizer. */

	// Note: il2cpp_gc_suppress_finalizer already throws in case obj was null.
	il2cpp_gc_suppress_finalizer (obj);
}

int32_t GC::GetGeneration (Il2CppObject* obj)
{
	return (int32_t)il2cpp_gc_get_generation(obj);
}

void GC::KeepAlive (Il2CppObject* obj)
{
	// do nothing. icall just prevents compilers from optimizing away references to object being passed in
}

void GC::InternalCollect(int generation)
{
	il2cpp_gc_collect (generation);
}

int32_t GC::get_MaxGeneration()
{
	return il2cpp_gc_max_generation ();
}

void GC::RecordPressure (int64_t bytesAllocated)
{
	il2cpp_gc_add_memory_pressure (bytesAllocated);
}

void GC::WaitForPendingFinalizers()
{
	il2cpp_gc_wait_for_pending_finalizers ();
}

int64_t GC::GetTotalMemory (bool forceFullCollection)
{
	if ( forceFullCollection )
		il2cpp_gc_collect (il2cpp_gc_max_generation());

	return il2cpp::vm::GC::GetUsedHeapSize ();
}

void GC::ReRegisterForFinalize (Il2CppObject* obj)
{
	// Note: il2cpp_gc_register_finalizer already throws in case obj was null.
	il2cpp_gc_register_finalizer (obj);
}

int32_t GC::CollectionCount (int32_t generation)
{
	return il2cpp_gc_collection_count (generation);
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
