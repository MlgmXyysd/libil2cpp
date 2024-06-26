#pragma once

#include "il2cpp-config.h"

#if IL2CPP_TARGET_QNX

#define IL2CPP_USES_POSIX_CLASS_LIBRARY_PAL 1

#define IL2CPP_HAVE_FUTIMENS 1
#define IL2CPP_HAVE_REWINDDIR 1

// QNX on 32-bit has no nsec field on stat by default
// https://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/s/stat_struct.html
#if IL2CPP_SIZEOF_VOID_P == 8
#define IL2CPP_HAVE_STAT_TIM 1
#endif

#include <limits.h>
#define IL2CPP_HAVE_SYS_UN 1

#define stat_ stat
#define fstat_ fstat
#define lstat_ lstat

#include <dirent.h>

#endif // IL2CPP_TARGET_LINUX
