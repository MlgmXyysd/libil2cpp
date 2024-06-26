#include "il2cpp-config.h"
#include <vector>
#include <map>
#include "os/MemoryMappedFile.h"
#include "os/Mutex.h"
#include "utils/StringUtils.h"
#include "vm/Class.h"
#include "vm/Image.h"
#include "vm/MetadataCache.h"
#include "vm/StackTrace.h"
#include "vm/Type.h"
#include "utils/HashUtils.h"
#include "utils/StdUnorderedMap.h"
#include "utils/StringUtils.h"

using il2cpp::utils::HashUtils;
using il2cpp::utils::StringUtils;


struct NamespaceAndNamePairHash
{
	size_t operator() (const std::pair<const char*, const char*>& pair) const
	{
		return HashUtils::Combine (StringUtils::Hash (pair.first), StringUtils::Hash (pair.second));
	}
};

struct NamespaceAndNamePairEquals
{
	bool operator() (const std::pair<const char*, const char*>& p1, const std::pair<const char*, const char*>& p2) const
	{
		return !strcmp (p1.first, p2.first) && !strcmp (p1.second, p2.second);
	}
};

struct NamespaceAndNamePairLess
{
	bool operator() (const std::pair<const char*, const char*>& p1, const std::pair<const char*, const char*>& p2) const
	{
		int namespaceCompare = strcmp (p1.first, p2.first);

		if (namespaceCompare < 0)
			return true;

		if (namespaceCompare > 0)
			return false;

		return strcmp (p1.second, p2.second) < 0;
	}
};

struct Il2CppNameToTypeDefinitionIndexHashTable : public unordered_map<std::pair<const char*, const char*>, TypeDefinitionIndex,
#if IL2CPP_HAS_UNORDERED_CONTAINER
	NamespaceAndNamePairHash, NamespaceAndNamePairEquals
#else
	NamespaceAndNamePairLess
#endif
>
{

};

namespace il2cpp
{
namespace vm
{

const Il2CppAssembly* Image::GetAssembly (const Il2CppImage* image)
{
	return MetadataCache::GetAssemblyFromIndex (image->assemblyIndex);
}

typedef il2cpp::vm::StackFrames::const_reverse_iterator StackReverseIterator;

static bool IsSystemType(TypeInfo* klass)
{
	return strcmp(klass->namespaze, "System") == 0 && strcmp(klass->name, "Type") == 0;
}

static bool IsSystemReflectionAssembly(TypeInfo* klass)
{
	return strcmp(klass->namespaze, "System.Reflection") == 0 && strcmp(klass->name, "Assembly") == 0;
}

static StackReverseIterator GetNextImageFromStack(StackReverseIterator first, StackReverseIterator last)
{
	for (StackReverseIterator it = first; it != last; it++)
	{
		TypeInfo* klass = it->method->declaring_type;
		if (klass->image != NULL && !IsSystemType(klass) && !IsSystemReflectionAssembly(klass))
		{
			return it;
		}
	}

	return last;
}

const Il2CppImage* Image::GetExecutingImage()
{
	const il2cpp::vm::StackFrames& stack = *StackTrace::GetStackFrames();
	StackReverseIterator imageIt = GetNextImageFromStack(stack.rbegin(), stack.rend());
	
	if (imageIt != stack.rend())
	{
		return imageIt->method->declaring_type->image;
	}

	// Fallback to corlib if no image is found
	return const_cast<Il2CppImage*>(Image::GetCorlib());
}

const Il2CppImage* Image::GetCallingImage()
{
	const il2cpp::vm::StackFrames& stack = *StackTrace::GetStackFrames();
	StackReverseIterator imageIt = GetNextImageFromStack(stack.rbegin(), stack.rend());

	if (imageIt != stack.rend())
	{
		imageIt = GetNextImageFromStack(++imageIt, stack.rend());

		if (imageIt != stack.rend())
		{
			return imageIt->method->declaring_type->image;
		}
	}

	// Fallback to corlib if no image is found
	return const_cast<Il2CppImage*>(Image::GetCorlib());
}

const char * Image::GetName (const Il2CppImage* image)
{
	return image->name;
}

const char * Image::GetFileName (const Il2CppImage* image)
{
	return image->name;
}

const MethodInfo* Image::GetEntryPoint (const Il2CppImage* image)
{
	return MetadataCache::GetMethodInfoFromMethodDefinitionIndex (image->entryPointIndex);
}

Il2CppImage* Image::GetCorlib ()
{
	return il2cpp_defaults.corlib;
}

static os::FastMutex s_ClassFromNameMutex;

TypeInfo* Image::ClassFromName (Il2CppImage* image, const char* namespaze, const char *name)
{
	if (!image->nameToClassHashTable)
	{
		os::FastAutoLock lock (&s_ClassFromNameMutex);
		if (!image->nameToClassHashTable)
		{
			image->nameToClassHashTable = new Il2CppNameToTypeDefinitionIndexHashTable ();
			for (int32_t index = 0; index < image->typeCount; index++)
			{
				TypeDefinitionIndex typeIndex = image->typeStart + index;
				const Il2CppTypeDefinition* typeDefinition = MetadataCache::GetTypeDefinitionFromIndex (typeIndex);

				// don't add nested types
				if (typeDefinition->declaringTypeIndex != kTypeIndexInvalid)
					continue;

				image->nameToClassHashTable->insert (std::make_pair (std::make_pair (MetadataCache::GetStringFromIndex (typeDefinition->namespaceIndex), MetadataCache::GetStringFromIndex (typeDefinition->nameIndex)), typeIndex));
			}
		}
	}

	Il2CppNameToTypeDefinitionIndexHashTable::const_iterator iter = image->nameToClassHashTable->find (std::make_pair (namespaze, name));
	if (iter != image->nameToClassHashTable->end ())
		return MetadataCache::GetTypeInfoFromTypeDefinitionIndex (iter->second);

	return NULL;
}

size_t Image::GetNumTypes(const Il2CppImage* image)
{
	return image->typeCount;
}

const TypeInfo* Image::GetType(const Il2CppImage* image, size_t index)
{
	return MetadataCache::GetTypeInfoFromTypeDefinitionIndex (image->typeStart + index);
}

static bool StringsMatch(const char* left, const char* right, bool ignoreCase)
{
	if (!ignoreCase)
	{
		return strcmp(left, right) == 0;
	}
	else
	{
		utils::StringUtils::CaseInsensitiveComparer comparer;
		return comparer(left, right);
	}
}

static TypeInfo* FindClassMatching (const Il2CppImage* image, const char* namespaze, const char *name, TypeInfo* declaringType, bool ignoreCase)
{
	for (size_t i = 0; i < image->typeCount; i++)
	{
		TypeInfo* type = MetadataCache::GetTypeInfoFromTypeDefinitionIndex (image->typeStart + i);
		if (type->declaringType == declaringType && StringsMatch(namespaze, type->namespaze, ignoreCase) && StringsMatch(name, type->name, ignoreCase))
		{
			return type;
		}
	}

	return NULL;
}

static TypeInfo* FindNestedType (TypeInfo* klass, const char* name)
{
	void* iter = NULL;
	while (TypeInfo* nestedType = Class::GetNestedTypes (klass, &iter))
	{
		if (!strcmp (name, nestedType->name))
			return nestedType;
	}

	return NULL;
}

TypeInfo* Image::FromTypeNameParseInfo (const Il2CppImage* image, const TypeNameParseInfo &info, bool ignoreCase)
{
	TypeInfo *parent_class = FindClassMatching (image, info.ns().c_str(), info.name().c_str(), NULL, ignoreCase);

	if (parent_class == NULL)
		return NULL;

	std::vector<std::string>::const_iterator it = info.nested ().begin ();

	while (it != info.nested ().end ())
	{
		parent_class = FindNestedType (parent_class, (*it).c_str ());

		if (parent_class == NULL)
			return NULL;

		++it;
	}

	return parent_class;
}

static os::FastMutex s_Mutex;
static std::vector<Image::EmbeddedResourceData> s_CachedResourceData;
static std::map<Il2CppReflectionAssembly*, void*> s_CachedMemoryMappedResourceFiles;

void Image::CacheMemoryMappedResourceFile(Il2CppReflectionAssembly* assembly, void* memoryMappedFile)
{
	os::FastAutoLock lock(&s_Mutex);
	s_CachedMemoryMappedResourceFiles[assembly] = memoryMappedFile;
}

void* Image::GetCachedMemoryMappedResourceFile(Il2CppReflectionAssembly* assembly)
{
	os::FastAutoLock lock(&s_Mutex);
	std::map<Il2CppReflectionAssembly*, void*>::iterator entry = s_CachedMemoryMappedResourceFiles.find(assembly);
	if (entry != s_CachedMemoryMappedResourceFiles.end())
		return entry->second;

	return NULL;
}

void Image::CacheResourceData(EmbeddedResourceRecord record, void* data)
{
	os::FastAutoLock lock(&s_Mutex);
	s_CachedResourceData.push_back(EmbeddedResourceData(record, data));
}

void* Image::GetCachedResourceData(const Il2CppImage* image, const std::string& name)
{
	os::FastAutoLock lock(&s_Mutex);
	for (std::vector<EmbeddedResourceData>::iterator it = s_CachedResourceData.begin(); it != s_CachedResourceData.end(); ++it)
	{
		if (it->record.image == image && it->record.name == name)
			return it->data;
	}

	return NULL;
}

void Image::ClearCachedResourceData()
{
	os::FastAutoLock lock(&s_Mutex);
	for (std::map<Il2CppReflectionAssembly*, void*>::iterator i = s_CachedMemoryMappedResourceFiles.begin(); i != s_CachedMemoryMappedResourceFiles.end(); ++i)
		os::MemoryMappedFile::Unmap(i->second);

	s_CachedMemoryMappedResourceFiles.clear();
	s_CachedResourceData.clear();
}

} /* namespace vm */
} /* namespace il2cpp */
