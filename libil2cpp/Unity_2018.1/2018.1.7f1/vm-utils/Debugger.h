#pragma once
#include "il2cpp-class-internals.h"

struct Il2CppSequencePoint;
struct Il2CppSequencePointExecutionContext;
struct Il2CppThreadUnwindState;

typedef void(*DebugInfoInitialization)();
typedef void(*ThreadCallback)(void*, uintptr_t);

#ifdef __cplusplus

#include <stdint.h>
#include "os/Thread.h"
#include "os/ThreadLocalValue.h"

namespace il2cpp
{
namespace utils
{
    class Debugger
    {
    public:
        static void SetAgentOptions(const char* options);
        static void RegisterInitializationCallbacks(DebugInfoInitialization sequencePointInit, DebugInfoInitialization executionContextInit, DebugInfoInitialization sourceFileMapInit, DebugInfoInitialization methodHeaderInit);
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

        static void AllocateThreadLocalData();
        static void FreeThreadLocalData();

        static Il2CppSequencePoint* GetSequencePoint(int id);

        static void AddSequencePoint(int id, const Il2CppMethodExecutionContextInfo* executionContextInfos, uint32_t executionContextInfoCount,
            const Il2CppMethodHeaderInfo *header, const MethodInfo* method, const char* sourceFile, uint8_t h1, uint8_t h2, uint8_t h3, uint8_t h4,
            uint8_t h5, uint8_t h6, uint8_t h7, uint8_t h8, uint8_t h9, uint8_t h10, uint8_t h11, uint8_t h12, uint8_t h13, uint8_t h14, uint8_t h15,
            uint8_t h16, uint32_t lineStart, uint32_t lineEnd, uint32_t columnStart, uint32_t columnEnd, int32_t ilOffset, SequencePointKind kind,
            bool isActive, uint8_t tryDepth, const Il2CppClass *catchType);

        static Il2CppSequencePoint* GetSequencePoints(void* *iter);
        static Il2CppSequencePoint* GetSequencePoints(const MethodInfo* method, void**iter);
        static void AddMethodExecutionContextInfo(MethodIndex index, const Il2CppMethodExecutionContextInfo& info);
        static Il2CppMethodExecutionContextInfo* GetMethodExecutionContextInfos(MethodIndex index, int *count);
        static void HandleException(Il2CppException *exc, Il2CppSequencePoint *sequencePoint);
        static void AddTypeSourceFile(const Il2CppClass *klass, const char *sourceFile);
        static const char** GetTypeSourceFiles(const Il2CppClass *klass, int& count);
        static void UserBreak();
        static bool IsLoggingEnabled();
        static void Log(int level, Il2CppString *category, Il2CppString *message);
        static bool IsSequencePointActive(Il2CppSequencePoint *seqPoint);
        static Il2CppMethodHeaderInfo* AddMethodHeaderInfo(MethodIndex index, int codeSize, int numScopes);
        static const Il2CppMethodHeaderInfo* GetMethodHeaderInfo(MethodIndex index);

    private:
        static os::ThreadLocalValue s_IsGlobalBreakpointActive;
        static void InitializeMethodToSequencePointMap();
    };
}
}
#endif //__cplusplus

typedef struct Il2CppSequencePointExecutionContext
{
    void** values;

#ifdef __cplusplus
    Il2CppSequencePointExecutionContext(void** values)
        : values(values)
    {
        il2cpp::utils::Debugger::PushExecutionContext(this);
    }

    ~Il2CppSequencePointExecutionContext()
    {
        il2cpp::utils::Debugger::PopExecutionContext();
    }

#endif //__cplusplus
} Il2CppSequencePointExecutionContext;

typedef struct Il2CppThreadUnwindState
{
    Il2CppSequencePoint** sequencePoints;
    Il2CppSequencePointExecutionContext** executionContexts;
    uint32_t frameCount;
} Il2CppThreadUnwindState;
