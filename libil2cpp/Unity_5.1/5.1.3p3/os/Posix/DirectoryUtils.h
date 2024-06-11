#pragma once

#if IL2CPP_TARGET_POSIX

namespace il2cpp
{
namespace os
{

bool Match (const std::string name, size_t nameIndex, const std::string& pattern, const size_t patternIndex);
bool Match (const std::string name, const std::string& pattern);
std::string CollapseAdjacentStars(const std::string& pattern);

}
}

#endif // IL2CPP_TARGET_POSIX
