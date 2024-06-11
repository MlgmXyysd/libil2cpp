#pragma once

#include <stdint.h>
#include <vector>
#include "metadata/Il2CppTypeVector.h"

namespace il2cpp
{
namespace metadata
{

class FieldLayout
{
public:
	struct FieldLayoutData
	{
		std::vector<size_t> FieldOffsets;
		size_t classSize;
		size_t actualClassSize;
		uint8_t minimumAlignment;
	};

	static void LayoutFields (size_t parentSize, size_t actualParentSize, size_t parentAlignment, const Il2CppTypeVector& fieldTypes, FieldLayoutData& data);
};

} /* namespace metadata */
} /* namespace il2cpp */
