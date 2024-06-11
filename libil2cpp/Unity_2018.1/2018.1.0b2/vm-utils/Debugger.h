#pragma once
#include <stdint.h>
#include "il2cpp-class-internals.h"
#include "os/Thread.h"
#include "os/ThreadLocalValue.h"

struct Il2CppSequencePoint;
struct Il2CppSequencePointExecutionContext;
struct Il2CppThreadUnwindState;

typedef void(*DebugInfoInitialization)();
typedef void(*ThreadCallback)(void*, uintptr_t);

namespace il2cpp
{
namespace utils
{
#if IL2CPP_MONO_DEBUGGER
    class Debugger
    {
    public:
        static void SetAgentOptions(const char* options);
        static void RegisterMethodInitializationMethod(DebugInfoInitialization sequencePointInit, DebugInfoInitialization executionContextInit, DebugInfoInitialization sourceFileMapInit);
        static void Init();
        static void Start();
        static void StartDebuggerThread();
        static void PushExecutionContext(Il2CppSequencePointExecutionContext* executionContext);
        static void PopExecutionContext();
        static Il2CppSequencePoint** PushSequencePoint();
        static void PopSequencePoint();
        typedef void (*OnBreakPointHitCallback) (Il2CppSequencePoint* sequencePoint);
        static void RegisterCallbacks(OnBreakPointHitCallback callback);
        static void SaveThreadContext(Il2CppThreadUnwindState* context);
        static void OnBreakPointHit(Il2CppSequencePoint *sequencePoint);
        static bool IsGlobalBreakpointActive();
        static bool GetIsDebuggerAttached();
        static void SetIsDebuggerAttached(bool attached);
        static bool IsDebuggerThread(os::Thread* thread);

        static void RegisterThreadStartedCallback(ThreadCallback start);
        static void RegisterThreadStoppedCallback(ThreadCallback end);
        static void AllocateThreadStatics();
        static void ThreadStart(Il2CppThread* thread);
        static void ThreadEnd(Il2CppThread* thread);

        static Il2CppSequencePoint* GetSequencePoint(size_t id);
        static void AddSequencePoint(size_t, const Il2CppSequencePoint&point);
        static Il2CppSequencePoint* GetSequencePoints(void* *iter);
        static Il2CppSequencePoint* GetSequencePoints(const MethodInfo* method, void**iter);
        static void AddMethodExecutionContextInfo(const char *methodName, const Il2CppMethodExecutionContextInfo& info);
        static Il2CppMethodExecutionContextInfo* GetMethodExecutionContextInfos(const char *methodName, int *count);
        static void HandleException(Il2CppException *exc, Il2CppSequencePoint *sequencePoint);
        static void AddTypeSourceFile(const Il2CppClass *klass, const char *sourceFile);
        static const char** GetTypeSourceFiles(const Il2CppClass *klass, int& count);
        static void UserBreak();
        static bool IsLoggingEnabled();
        static void Log(int level, Il2CppString *category, Il2CppString *message);
        static bool IsSequencePointActive(Il2CppSequencePoint *seqPoint);

    private:
        static os::ThreadLocalValue s_IsGlobalBreakpointActive;
        static void InitializeMethodToSequencePointMap();
    };
#endif // IL2CPP_MONO_DEBUGGER
}
}

struct Il2CppSequencePointExecutionContext
{
#if IL2CPP_MONO_DEBUGGER
    void** values;
#endif

    Il2CppSequencePointExecutionContext(void** values)
#if IL2CPP_MONO_DEBUGGER
        : values(values)
#endif
    {
#if IL2CPP_MONO_DEBUGGER
        il2cpp::utils::Debugger::PushExecutionContext(this);
#endif
    }

    ~Il2CppSequencePointExecutionContext()
    {
#if IL2CPP_MONO_DEBUGGER
        il2cpp::utils::Debugger::PopExecutionContext();
#endif
    }
};
