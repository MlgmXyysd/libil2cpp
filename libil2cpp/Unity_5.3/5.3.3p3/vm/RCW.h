#pragma once

struct Il2CppGuid;
struct Il2CppIUnknown;
struct Il2CppRCW;

namespace il2cpp
{
namespace vm
{

class RCW
{
public:
	static void Initialize(Il2CppRCW* rcw, const Il2CppGuid& clsid);
	static Il2CppIUnknown* QueryInterface(Il2CppRCW* rcw, const Il2CppGuid& iid);
	static Il2CppRCW* Create(Il2CppIUnknown* unknown);

private:
	static void Cleanup(void* obj, void* data);
};

} /* namespace vm */
} /* namespace il2cpp */
