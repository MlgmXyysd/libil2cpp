#include "il2cpp-config.h"
#include "os/WindowsRuntime.h"
#include "vm/Exception.h"
#include "vm/WindowsRuntime.h"

namespace il2cpp
{
namespace vm
{

Il2CppIActivationFactory* WindowsRuntime::GetActivationFactory(const utils::StringView<Il2CppChar>& runtimeClassName)
{
	Exception::Raise(IL2CPP_REGDB_E_CLASSNOTREG);

	return NULL;
}

}
}