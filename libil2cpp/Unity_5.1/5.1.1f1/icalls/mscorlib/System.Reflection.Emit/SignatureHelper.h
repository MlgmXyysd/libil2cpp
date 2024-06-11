#pragma once

struct Il2CppArray;
struct Il2CppReflectionSigHelper;

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
namespace Emit
{

class SignatureHelper
{
public:
	static Il2CppArray* get_signature_field (Il2CppReflectionSigHelper*);
	static Il2CppArray* get_signature_local (Il2CppReflectionSigHelper*);
};

} /* namespace Emit */
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
