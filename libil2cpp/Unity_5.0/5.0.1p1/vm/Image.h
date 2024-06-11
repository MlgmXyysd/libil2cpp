#pragma once

#include <stdint.h>
#include <vector>

struct TypeInfo;
struct MethodInfo;
struct Il2CppAssembly;
struct Il2CppDelegate;
struct Il2CppImage;
struct Il2CppType;
struct Il2CppGenericContext;
struct Il2CppGenericContainer;
struct Il2CppMethodGenericContainerData;
class AssemblyVector;

namespace il2cpp
{
namespace vm
{

class TypeNameParseInfo;

class Image
{
// exported
public:
	static const Il2CppImage* GetCorlib ();

public:
	static const char * GetName (Il2CppImage* image);
	static const char * GetFileName (const Il2CppImage* image);
	static Il2CppAssembly* GetAssembly(Il2CppImage* image);
	static MethodInfo* GetEntryPoint(const Il2CppImage* image);
	static Il2CppImage* GetExecutingImage();
	static Il2CppImage* GetCallingImage();
	static size_t GetNumTypes(const Il2CppImage* image);
	static const TypeInfo* const* GetTypes(const Il2CppImage* image);
	static TypeInfo* FromTypeNameParseInfo (Il2CppImage* image, const TypeNameParseInfo &info);
	static TypeInfo* ClassFromName (const Il2CppImage* image, const char* namespaze, const char *name);
};

} /* namespace vm */
} /* namespace il2cpp */
