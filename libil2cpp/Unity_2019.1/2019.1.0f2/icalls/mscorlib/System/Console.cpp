#include "il2cpp-config.h"

#if IL2CPP_TINY

#include "os/File.h"
#include "utils/StringUtils.h"

extern "C" void STDCALL Console_WriteLine(const char* message)
{
    il2cpp::os::FileHandle* fileHandle = il2cpp::os::File::GetStdOutput();
    size_t length = il2cpp::utils::StringUtils::StrLen(message);

    int error;
    il2cpp::os::File::Write(fileHandle, message, static_cast<int>(length), &error);
#if IL2CPP_TARGET_WINDOWS
    il2cpp::os::File::Write(fileHandle, "\r\n", 2, &error);
#else
    il2cpp::os::File::Write(fileHandle, "\n", 1, &error);
#endif
}

#endif
