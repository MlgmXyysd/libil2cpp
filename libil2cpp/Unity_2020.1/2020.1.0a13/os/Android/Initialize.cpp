#include "il2cpp-config.h"

#if IL2CPP_TARGET_ANDROID

#include "os/Initialize.h"
#include "utils/Logging.h"

#include <jni.h>
#include <android/log.h>

static void AndroidLogCallback(const char* message)
{
    __android_log_print(ANDROID_LOG_INFO, "IL2CPP", "%s", message);
}

JavaVM *sJavaVM = nullptr;

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    __android_log_print(ANDROID_LOG_INFO, "IL2CPP", "JNI_OnLoad");
    sJavaVM = jvm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved)
{
    __android_log_print(ANDROID_LOG_INFO, "IL2CPP", "JNI_OnUnload");
    sJavaVM = nullptr;
}

void il2cpp::os::Initialize()
{
    if (!utils::Logging::IsLogCallbackSet())
        utils::Logging::SetLogCallback(AndroidLogCallback);
}

void il2cpp::os::Uninitialize()
{
}

#endif
