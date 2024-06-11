#include "il2cpp-config.h"
#include "object-internals.h"

#include <cassert>

#include "icalls/mscorlib/System.Reflection/AssemblyName.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Type.h"
#include "utils/StringUtils.h"

using il2cpp::vm::Array;
using il2cpp::vm::Class;
using il2cpp::vm::Object;
using il2cpp::vm::Runtime;
using il2cpp::vm::String;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Reflection
{

static Il2CppObject* CreateVersion (uint32_t major, uint32_t minor, uint32_t build, uint32_t revision)
{
	static const MethodInfo* versionContructor = NULL;
	if (!versionContructor)
		versionContructor = Class::GetMethodFromName (il2cpp_defaults.version, ".ctor", 4);

	Il2CppObject* version = Object::New (il2cpp_defaults.version);
	void* args[4] = { &major, &minor, &build, &revision };
	Runtime::Invoke (versionContructor, version, args, NULL);

	return version;
}

static Il2CppObject* CreateCulture (const char* cultureName)
{
	static const MethodInfo* createCultureMethod = NULL;
	if (!createCultureMethod)
		createCultureMethod = Class::GetMethodFromName (il2cpp_defaults.culture_info, "CreateCulture", 2);

	bool reference = false;
	void* args[2];
	args[0] = String::New (cultureName);
	args[1] = &reference;
	return Runtime::Invoke (createCultureMethod, NULL, args, NULL);
}

bool AssemblyName::ParseName (Il2CppReflectionAssemblyName* aname, Il2CppString* assemblyName)
{
	std::string str = utils::StringUtils::Utf16ToUtf8 (String::GetChars (assemblyName));

	il2cpp::vm::TypeNameParseInfo info;
	il2cpp::vm::TypeNameParser parser (str, info, false);

	if (!parser.ParseAssembly ())
		return false;

	const il2cpp::vm::TypeNameParseInfo::AssemblyName& parsedName = info.assembly_name ();
	IL2CPP_OBJECT_SETREF (aname, name, String::New (parsedName.name.c_str ()));
	aname->major = parsedName.major;
	aname->minor = parsedName.minor;
	aname->build = parsedName.build;
	aname->revision = parsedName.revision;
	aname->flags = parsedName.flags;
	aname->hashalg = parsedName.hash_alg;

	IL2CPP_OBJECT_SETREF (aname, version, CreateVersion (parsedName.major, parsedName.minor, parsedName.build, parsedName.revision));
	if (parsedName.culture.size ())
		IL2CPP_OBJECT_SETREF (aname, cultureInfo, CreateCulture (parsedName.culture.c_str ()));

	if (parsedName.public_key_token [0])
	{
		IL2CPP_OBJECT_SETREF (aname, keyToken, Array::New (il2cpp_defaults.byte_class, kPublicKeyByteLength));
		char* p = il2cpp_array_addr (aname->keyToken, char, 0);

		char buf[2] = {0};
		for (int i = 0, j = 0; i < kPublicKeyByteLength; i++)
		{
			buf[0] = parsedName.public_key_token[j++];
			*p = (char)(strtol (buf, NULL, 16) << 4);
			buf[0] = parsedName.public_key_token[j++];
			*p |= (char)strtol (buf, NULL, 16);
			p++;
		}
	}

	return true;
}

} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
