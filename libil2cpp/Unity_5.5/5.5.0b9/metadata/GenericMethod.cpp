#include "il2cpp-config.h"
#include "metadata/GenericMetadata.h"
#include "metadata/GenericMethod.h"
#include "metadata/GenericSharing.h"
#include "metadata/Il2CppGenericMethodCompare.h"
#include "metadata/Il2CppGenericMethodHash.h"
#include "os/Mutex.h"
#include "utils/Memory.h"
#include "vm/Class.h"
#include "vm/GenericClass.h"
#include "vm/MetadataAlloc.h"
#include "vm/MetadataCache.h"
#include "vm/MetadataLock.h"
#include "vm/Method.h"
#include "vm/Runtime.h"
#include "vm/Type.h"
#include "utils/Il2CppHashMap.h"
#include "class-internals.h"
#include "metadata.h"
#include <sstream>

using il2cpp::metadata::GenericMetadata;
using il2cpp::metadata::GenericSharing;
using il2cpp::os::FastAutoLock;
using il2cpp::vm::Class;
using il2cpp::vm::GenericClass;
using il2cpp::vm::MetadataCalloc;
using il2cpp::vm::MetadataCache;
using il2cpp::vm::Method;
using il2cpp::vm::Runtime;
using il2cpp::vm::Type;

namespace il2cpp
{
namespace metadata
{

typedef Il2CppHashMap<const Il2CppGenericMethod*, MethodInfo*, Il2CppGenericMethodHash, Il2CppGenericMethodCompare> Il2CppGenericMethodMap;
static Il2CppGenericMethodMap s_GenericMethodMap;

const MethodInfo* GenericMethod::GetMethod (const Il2CppGenericMethod* gmethod)
{
	FastAutoLock lock (&il2cpp::vm::g_MetadataLock);

	Il2CppGenericMethodMap::const_iterator iter = s_GenericMethodMap.find (gmethod);
	if (iter != s_GenericMethodMap.end ())
		return iter->second;

	const MethodInfo* methodDefinition = gmethod->methodDefinition;
	Il2CppClass* declaringClass = methodDefinition->declaring_type;
	if (gmethod->context.class_inst)
	{
		IL2CPP_ASSERT(!declaringClass->generic_class);
		Il2CppGenericClass* genericClassDeclaringType = GenericMetadata::GetGenericClass (methodDefinition->declaring_type, gmethod->context.class_inst);
		declaringClass = GenericClass::GetClass (genericClassDeclaringType);

		// we may fail if we cannot construct generic type
		if (!declaringClass)
			return NULL;
	}
	MethodInfo* newMethod = (MethodInfo*)MetadataCalloc (1, sizeof (MethodInfo));

	// we set this here because the initialization may recurse and try to retrieve the same generic method
	// this is safe because we *always* take the lock when retrieving the MethodInfo from a generic method.
	// if we move lock to only if MethodInfo needs constructed then we need to revisit this since we could return a partially initialized MethodInfo
	s_GenericMethodMap.insert (std::make_pair (gmethod, newMethod));

	newMethod->declaring_type = declaringClass;
	newMethod->flags = methodDefinition->flags;
	newMethod->iflags = methodDefinition->iflags;
	newMethod->slot = methodDefinition->slot;
	newMethod->name = methodDefinition->name;
	newMethod->is_generic = false;
	newMethod->is_inflated = true;
	newMethod->customAttributeIndex = methodDefinition->customAttributeIndex;
	newMethod->token = methodDefinition->token;

	newMethod->return_type = GenericMetadata::InflateIfNeeded (methodDefinition->return_type, &gmethod->context, true);

	newMethod->parameters_count = methodDefinition->parameters_count;
	newMethod->parameters = GenericMetadata::InflateParameters (methodDefinition->parameters, methodDefinition->parameters_count, &gmethod->context, true);

	newMethod->genericMethod = gmethod;

	if (!gmethod->context.method_inst)
	{
		if (methodDefinition->is_generic)
			newMethod->is_generic = true;

		if (!declaringClass->generic_class)
		{
			const Il2CppGenericContainer* container = methodDefinition->genericContainer;
			newMethod->genericContainer = container;
		}

		newMethod->methodDefinition = methodDefinition->methodDefinition;
	}
	else
	{
		// we only need RGCTX for generic instance methods
		newMethod->rgctx_data = GenericMetadata::InflateRGCTX (methodDefinition->methodDefinition->rgctxStartIndex, methodDefinition->methodDefinition->rgctxCount, &gmethod->context);
	}

	newMethod->invoker_method = MetadataCache::GetInvokerMethodPointer (methodDefinition, &gmethod->context);
	newMethod->methodPointer = MetadataCache::GetMethodPointer (methodDefinition, &gmethod->context);

	++il2cpp_runtime_stats.inflated_method_count;

	return newMethod;
}

const Il2CppGenericContext* GenericMethod::GetContext (const Il2CppGenericMethod* gmethod)
{
	return &gmethod->context;
}

static std::string FormatGenericArguments (const Il2CppGenericInst* inst)
{
	std::ostringstream sstream;
	if (inst)
	{
		sstream << "<";
		for (size_t i = 0; i < inst->type_argc; ++i)
		{
			if (i != 0)
				sstream << ", ";
			sstream << Type::GetName (inst->type_argv[i], IL2CPP_TYPE_NAME_FORMAT_FULL_NAME);
		}
		sstream << ">";
	}

	return sstream.str ();
}

std::string GenericMethod::GetFullName (const Il2CppGenericMethod* gmethod)
{
	const MethodInfo* method = gmethod->methodDefinition;
	std::ostringstream sstream;
	sstream << Type::GetName (gmethod->methodDefinition->declaring_type->byval_arg, IL2CPP_TYPE_NAME_FORMAT_FULL_NAME);
	sstream << FormatGenericArguments (gmethod->context.class_inst);
	sstream << "::";
	sstream << Method::GetName (method);
	sstream << FormatGenericArguments (gmethod->context.method_inst);

	return sstream.str ();
}

} /* namespace vm */
} /* namespace il2cpp */
