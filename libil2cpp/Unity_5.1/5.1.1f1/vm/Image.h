#pragma once

#include <stdint.h>
#include <vector>
#include <string>

struct TypeInfo;
struct MethodInfo;
struct Il2CppAssembly;
struct Il2CppDelegate;
struct Il2CppImage;
struct Il2CppType;
struct Il2CppGenericContext;
struct Il2CppGenericContainer;
class AssemblyVector;

namespace il2cpp
{
namespace vm
{

class TypeNameParseInfo;

struct EmbeddedResourceRecord
{
	EmbeddedResourceRecord(Il2CppImage* image, const std::string& name, uint32_t offset, uint32_t size)
		: image(image), name(name), offset(offset), size(size)
	{}

	Il2CppImage* image;
	std::string name;
	uint32_t offset;
	uint32_t size;
};

class Image
{
// exported
public:
	static const Il2CppImage* GetCorlib ();

public:
	static const char * GetName (Il2CppImage* image);
	static const char * GetFileName (const Il2CppImage* image);
	static Il2CppAssembly* GetAssembly(Il2CppImage* image);
	static const MethodInfo* GetEntryPoint (const Il2CppImage* image);
	static Il2CppImage* GetExecutingImage();
	static Il2CppImage* GetCallingImage();
	static size_t GetNumTypes(const Il2CppImage* image);
	static const TypeInfo* const* GetTypes(const Il2CppImage* image);
	static TypeInfo* FromTypeNameParseInfo (Il2CppImage* image, const TypeNameParseInfo &info);
	static TypeInfo* ClassFromName (const Il2CppImage* image, const char* namespaze, const char *name);

	struct EmbeddedResourceData
	{
		EmbeddedResourceData(EmbeddedResourceRecord record, void* data)
			: record(record), data(data)
		{}

		EmbeddedResourceRecord record;
		void* data;
	};

	static void CacheResourceData(EmbeddedResourceRecord record, void* data);
	static void* GetCachedResourceData(Il2CppImage* image, const std::string& name);
	static void ClearCachedResourceData();

private:
	static std::vector<EmbeddedResourceData> s_CachedResourceData;
};

} /* namespace vm */
} /* namespace il2cpp */
