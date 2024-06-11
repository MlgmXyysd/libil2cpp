#pragma once

#include <string>
#include <vector>

namespace il2cpp
{
namespace os
{

// Use std::vector instead of std::stack so we're able to iterate it
typedef std::vector<methodPointerType> NativeMethodStack;

class StackTrace
{
public:
	static NativeMethodStack GetStackTrace ();
};

}
}
