#include "il2cpp-config.h"
#include "GenericContainer.h"
#include "MetadataCache.h"

namespace il2cpp
{
namespace vm
{

TypeInfo* GenericContainer::GetDeclaringType (const Il2CppGenericContainer* genericContainer)
{
	if (genericContainer->is_method)
		return MetadataCache::GetMethodInfoFromMethodDefinitionIndex (genericContainer->ownerIndex)->declaring_type;

	return MetadataCache::GetTypeInfoFromTypeDefinitionIndex (genericContainer->ownerIndex);
}

const Il2CppGenericParameter* GenericContainer::GetGenericParameter (const Il2CppGenericContainer* genericContainer, uint16_t index)
{
	assert (index < genericContainer->type_argc);
	return MetadataCache::GetGenericParameterFromIndex (genericContainer->genericParameterStart + index);
}

} /* namespace vm */
} /* namespace il2cpp */
