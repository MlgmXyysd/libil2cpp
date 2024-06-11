#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX && !IL2CPP_TINY_WITHOUT_DEBUGGER

#include <clocale>
#include <locale.h>
#if IL2CPP_TARGET_DARWIN
#include <xlocale.h>
#endif
#include "os/Locale.h"

namespace il2cpp
{
namespace os
{
/*
* The following method is modified from the ICU source code. (http://oss.software.ibm.com/icu)
* Copyright (c) 1995-2003 International Business Machines Corporation and others
* All rights reserved.
*/
    std::string Locale::GetLocale()
    {
        const char* posix_locale = NULL;

        posix_locale = getenv("LC_ALL");
        if (posix_locale == 0)
        {
            posix_locale = getenv("LANG");
            if (posix_locale == 0)
            {
                posix_locale = setlocale(LC_ALL, NULL);
            }
        }

        if (posix_locale == NULL)
            return std::string();

#if IL2CPP_TARGET_JAVASCRIPT
        // This code is here due to a few factors:
        //   1. Emscripten gives us a "C" locale (the POSIX default).
        //   2. The Mono class library code uses managed exceptions for flow control.
        //   3. We need to support Emscripten builds with exceptions disabled.
        //   4. Our localization tables don't have any entry for the "C" locale.
        // These factors mean that with Emscripten, the class library code _always_
        // throws a managed exception to due CultureInfo processing. To make this
        // work without exceptions enabled, let's map "C" -> "en-us" for Emscripten.
        if (strcmp(posix_locale, "C.UTF-8") == 0)
            posix_locale = "en-us";
#endif

        if ((strcmp("C", posix_locale) == 0) || (strchr(posix_locale, ' ') != NULL)
            || (strchr(posix_locale, '/') != NULL))
        {
            /*
            * HPUX returns 'C C C C C C C'
            * Solaris can return /en_US/C/C/C/C/C on the second try.
            * Maybe we got some garbage.
            */
            return std::string();
        }

        return std::string(posix_locale);
    }

#if IL2CPP_SUPPORT_LOCALE_INDEPENDENT_PARSING
    static locale_t s_cLocale = NULL;
#endif

    void Locale::Initialize()
    {
#if IL2CPP_SUPPORT_LOCALE_INDEPENDENT_PARSING
        s_cLocale = newlocale(LC_ALL_MASK, "", NULL);
#endif
    }

    void Locale::UnInitialize()
    {
#if IL2CPP_SUPPORT_LOCALE_INDEPENDENT_PARSING
        if (s_cLocale)
            freelocale(s_cLocale);
        s_cLocale = NULL;
#endif
    }

#if IL2CPP_SUPPORT_LOCALE_INDEPENDENT_PARSING
    double Locale::DoubleParseLocaleIndependentImpl(char *ptr, char** endptr)
    {
        return strtod_l(ptr, endptr, s_cLocale);
    }

#endif
} /* namespace os */
} /* namespace il2cpp */

#endif
