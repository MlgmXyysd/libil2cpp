#include "il2cpp-config.h"
#include "il2cpp-api.h"
#include "icalls/mscorlib/System/Activator.h"
#include "object-internals.h"
#include "class-internals.h"
#include "metadata.h"
#include "vm/Image.h"
#include "vm/Class.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

Il2CppObject * Activator::CreateInstanceInternal (Il2CppReflectionType *type)
{
	TypeInfo* typeInfo = vm::Class::FromIl2CppType(type->type);

	if (typeInfo == NULL)
		return NULL;

	il2cpp::vm::Class::Init(typeInfo);

	//you could think "hey, shouldn't we call the constructor here? but we don't because this path is only hit for value
	//types, and they cannot have default constructors.  for reference types with constructors, the c# side of CreateInstance()
	//actually takes care of its own business by using reflection to create the object and invoke the constructor.
	return il2cpp_object_new(typeInfo);
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
