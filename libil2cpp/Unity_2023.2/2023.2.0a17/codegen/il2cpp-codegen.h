#pragma once

#include "il2cpp-codegen-metadata.h"

struct Il2CppStringBuilder;
typedef Il2CppStringBuilder RuntimeStringBuilder;
#include "il2cpp-codegen-il2cpp.h"

#ifdef GC_H
#error It looks like this codegen only header ends up including gc.h from the boehm gc. We should not expose boehmgc to generated code
#endif
#ifdef MONO_CONFIG_H_WAS_INCLUDED
#error It looks like this codegen only header ends up including headers from libmono. We should not expose those to generated code
#endif
