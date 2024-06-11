#include "il2cpp-config.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
#include "Debugger.h"
#include "os/Thread.h"
#include "os/c-api/Allocator.h"
#include "vm/MetadataCache.h"
#include "vm/Thread.h"
#include "utils/Environment.h"
#include "utils/dynamic_array.h"
#include "utils/StringUtils.h"
#include "utils/Il2CppHashMap.h"
#include "utils/HashUtils.h"
#include "VmStringUtils.h"
#include <deque>
#include <vector>
#include <string>

#if IL2CPP_MONO_DEBUGGER

static il2cpp::os::ThreadLocalValue s_SequencePoints; // std::deque<Il2CppSequencePoint*>
static il2cpp::os::ThreadLocalValue s_ExecutionContexts; // il2cpp::utils::dynamic_array<Il2CppSequencePointExecutionContext*>

struct Il2CppThreadUnwindState
{
    Il2CppSequencePoint** sequencePoints;
    Il2CppSequencePointExecutionContext** executionContexts;
    uint32_t frameCount;
};

struct MonoDebuggerRuntimeCallbacks
{
    void(*il2cpp_debugger_save_thread_context)(Il2CppThreadUnwindState* context);
    void (*set_global_breakpoint_active)();
};

struct MonoContext;
extern "C"
{
void mono_debugger_agent_parse_options(const char *options);
void mono_debugger_agent_init();
void mono_debugger_run_debugger_thread_func(void* arg);
void debugger_agent_single_step_from_context(MonoContext *ctx, uint64_t sequencePointId);
void mono_debugger_il2cpp_init();
void unity_debugger_agent_breakpoint(Il2CppSequencePoint* sequencePoint);
void mono_debugger_install_runtime_callbacks(MonoDebuggerRuntimeCallbacks* cbs);
void* il2cpp_alloc(size_t size);
int32_t unity_debugger_agent_is_global_breakpoint_active();
void unity_debugger_agent_handle_exception(Il2CppException *exc, Il2CppSequencePoint *sequencePoint);
int32_t il2cpp_mono_methods_match(const MethodInfo* left, const MethodInfo* right);
void mono_debugger_agent_user_break();
int32_t mono_debugger_agent_debug_log_is_enabled();
void mono_debugger_agent_debug_log(int level, Il2CppString *category, Il2CppString *message);
int32_t unity_sequence_point_active(Il2CppSequencePoint *seqPoint);
}

namespace il2cpp
{
namespace utils
{
    static os::Thread* s_DebuggerThread;
    static bool s_IsDebuggerAttached = false;
    static bool s_IsDebuggerInitialized = false;
    static std::string s_AgentOptions;

    static ThreadCallback s_ThreadStartCallback;
    static ThreadCallback s_ThreadEndCallback;

    typedef Il2CppHashMap<size_t, Il2CppSequencePoint, il2cpp::utils::PassThroughHash<size_t> > SequencePointMap;
    static SequencePointMap s_sequencePointMap;
    typedef std::vector<Il2CppSequencePoint*> SequencePointList;
    typedef Il2CppHashMap<const MethodInfo*, SequencePointList*, il2cpp::utils::PointerHash<MethodInfo> > MethodToSequencePointsMap;
    static MethodToSequencePointsMap s_methodToSequencePoints;
    typedef Il2CppHashMap<const MethodInfo*, const MethodInfo*, il2cpp::utils::PointerHash<MethodInfo> > MethodToMethodMap;
    static MethodToMethodMap s_uninflatedMethodToInflated;

    typedef dynamic_array<Il2CppMethodExecutionContextInfo> ExecutionContextInfoList;
    typedef Il2CppHashMap<const char*, ExecutionContextInfoList, StringUtils::StringHasher<const char*>, VmStringUtils::CaseSensitiveComparer> MethodExecutionContextMap;
    static MethodExecutionContextMap *s_pExecutionContextMap;

    typedef dynamic_array<const char*> FileNameList;
    typedef Il2CppHashMap<const Il2CppClass*, FileNameList, il2cpp::utils::PointerHash<Il2CppClass> > TypeSourceFileMap;
    static TypeSourceFileMap *s_typeSourceFiles;

    static DebugInfoInitialization s_SequencePointInitializationCallback;
    static DebugInfoInitialization s_ExecutionContextInitializationCallback;
    static DebugInfoInitialization s_SourceFileMapInitializationCallback;
    void Debugger::RegisterMethodInitializationMethod(DebugInfoInitialization sequencePointInit, DebugInfoInitialization executionContextInit, DebugInfoInitialization sourceFileMapInit)
    {
        s_SequencePointInitializationCallback = sequencePointInit;
        s_ExecutionContextInitializationCallback = executionContextInit;
        s_SourceFileMapInitializationCallback = sourceFileMapInit;
    }

#if defined(RUNTIME_IL2CPP)
    void breakpoint_callback(Il2CppSequencePoint* sequencePoint)
    {
        unity_debugger_agent_breakpoint(sequencePoint);
    }

#endif

    static void InitializeMonoSoftDebugger(const char* options)
    {
#if defined(RUNTIME_IL2CPP)
        IL2CPP_ASSERT(s_ExecutionContextInitializationCallback);
        IL2CPP_ASSERT(s_SequencePointInitializationCallback);
        IL2CPP_ASSERT(s_SourceFileMapInitializationCallback);

        s_pExecutionContextMap = new MethodExecutionContextMap();
        s_typeSourceFiles = new TypeSourceFileMap();

        mono_debugger_il2cpp_init();
        mono_debugger_agent_parse_options(options);
        mono_debugger_agent_init();

        MonoDebuggerRuntimeCallbacks cbs;
        cbs.il2cpp_debugger_save_thread_context = Debugger::SaveThreadContext;
        mono_debugger_install_runtime_callbacks(&cbs);

        il2cpp::utils::Debugger::RegisterCallbacks(breakpoint_callback);

        register_allocator(il2cpp_alloc);

        s_IsDebuggerInitialized = true;
#else
        IL2CPP_ASSERT(0 && "The managed debugger is only supported for the libil2cpp runtime backend.");
#endif
    }

    void Debugger::SetAgentOptions(const char* options)
    {
        s_AgentOptions = options;
    }

    void Debugger::Start()
    {
        if (s_IsDebuggerInitialized)
        {
            vm::MetadataCache::InitializeAllMethodMetadata();

            s_ExecutionContextInitializationCallback();
            s_SequencePointInitializationCallback();
            s_SourceFileMapInitializationCallback();

            InitializeMethodToSequencePointMap();
            Debugger::StartDebuggerThread();
        }
    }

    static bool TryInitializeDebugger(const std::string& options)
    {
        if (StringUtils::StartsWith(options, "--debugger-agent"))
        {
            InitializeMonoSoftDebugger(options.substr(options.find("=") + 1).c_str());
            return true;
        }

        return false;
    }

    void Debugger::Init()
    {
        if (!s_AgentOptions.empty())
        {
            TryInitializeDebugger(s_AgentOptions);
        }
        else
        {
            const std::vector<UTF16String>& args = Environment::GetMainArgs();
            for (std::vector<UTF16String>::const_iterator arg = args.begin(); arg != args.end(); ++arg)
            {
                std::string argument = StringUtils::Utf16ToUtf8(*arg);
                if (TryInitializeDebugger(argument))
                    break;
            }
        }
    }

    static Debugger::OnBreakPointHitCallback s_Callback;
    void Debugger::RegisterCallbacks(OnBreakPointHitCallback callback)
    {
        s_Callback = callback;
    }

    void Debugger::StartDebuggerThread()
    {
#if defined(RUNTIME_IL2CPP)
        s_DebuggerThread = new os::Thread();
        s_DebuggerThread->Run(mono_debugger_run_debugger_thread_func, NULL);
#else
        IL2CPP_ASSERT(0 && "The managed debugger is only supported for the libil2cpp runtime backend.");
#endif
    }

    void Debugger::SaveThreadContext(Il2CppThreadUnwindState* context)
    {
        IL2CPP_ASSERT(!IsDebuggerThread(os::Thread::GetCurrentThread()));

        if (context->sequencePoints != NULL)
            free(context->sequencePoints);
        if (context->executionContexts != NULL)
            free(context->executionContexts);

        il2cpp::utils::dynamic_array<Il2CppSequencePointExecutionContext*>* executionContexts;
        s_ExecutionContexts.GetValue(reinterpret_cast<void**>(&executionContexts));

        std::deque<Il2CppSequencePoint*>* sequencePoints;
        s_SequencePoints.GetValue(reinterpret_cast<void**>(&sequencePoints));

        context->frameCount = (uint32_t)std::min(executionContexts->size(), sequencePoints->size());
        if (context->frameCount > 0)
        {
            context->sequencePoints = (Il2CppSequencePoint**)malloc(context->frameCount * sizeof(Il2CppSequencePoint*));
            context->executionContexts = (Il2CppSequencePointExecutionContext**)malloc(context->frameCount * sizeof(Il2CppSequencePointExecutionContext*));

            for (size_t i = 0; i < context->frameCount; ++i)
            {
                context->sequencePoints[i] = (*sequencePoints)[i];
                context->executionContexts[i] = (*executionContexts)[i];
            }
        }
    }

    void Debugger::OnBreakPointHit(Il2CppSequencePoint *sequencePoint)
    {
#if defined(RUNTIME_IL2CPP)
        if (IsGlobalBreakpointActive())
        {
            debugger_agent_single_step_from_context(NULL, sequencePoint->id);
        }
        else if (s_Callback)
        {
            s_Callback(sequencePoint);
        }
        else
            IL2CPP_DEBUG_BREAK();
#else
        IL2CPP_ASSERT(0 && "The managed debugger is only supported for the libil2cpp runtime backend.");
#endif
    }

    void Debugger::PushExecutionContext(Il2CppSequencePointExecutionContext* executionContext)
    {
        il2cpp::utils::dynamic_array<Il2CppSequencePointExecutionContext*>* executionContexts;
        s_ExecutionContexts.GetValue(reinterpret_cast<void**>(&executionContexts));

        executionContexts->push_back(executionContext);
    }

    void Debugger::PopExecutionContext()
    {
        il2cpp::utils::dynamic_array<Il2CppSequencePointExecutionContext*>* executionContexts;
        s_ExecutionContexts.GetValue(reinterpret_cast<void**>(&executionContexts));

        executionContexts->pop_back();
    }

    Il2CppSequencePoint** Debugger::PushSequencePoint()
    {
        std::deque<Il2CppSequencePoint*>* sequencePoints;
        s_SequencePoints.GetValue(reinterpret_cast<void**>(&sequencePoints));
        sequencePoints->push_back(NULL);
        return &(sequencePoints->back());
    }

    void Debugger::PopSequencePoint()
    {
        std::deque<Il2CppSequencePoint*>* sequencePoints;
        s_SequencePoints.GetValue(reinterpret_cast<void**>(&sequencePoints));
        sequencePoints->pop_back();
    }

    bool Debugger::IsGlobalBreakpointActive()
    {
        if (!Debugger::GetIsDebuggerAttached())
            return false;
#if defined(RUNTIME_IL2CPP)
        return unity_debugger_agent_is_global_breakpoint_active();
#else
        IL2CPP_ASSERT(0 && "The managed debugger is only supported for the libil2cpp runtime backend.");
        return false;
#endif
    }

    bool Debugger::GetIsDebuggerAttached()
    {
        return s_IsDebuggerAttached;
    }

    void Debugger::SetIsDebuggerAttached(bool attached)
    {
        s_IsDebuggerAttached = attached;
    }

    bool Debugger::IsDebuggerThread(os::Thread* thread)
    {
        return thread == s_DebuggerThread;
    }

    void Debugger::RegisterThreadStartedCallback(ThreadCallback start)
    {
        s_ThreadStartCallback = start;
    }

    void Debugger::RegisterThreadStoppedCallback(ThreadCallback end)
    {
        s_ThreadEndCallback = end;
    }

    static void AllocateThreadLocalData()
    {
        il2cpp::utils::dynamic_array<Il2CppSequencePointExecutionContext*>* executionContexts;
        s_ExecutionContexts.GetValue(reinterpret_cast<void**>(&executionContexts));
        if (executionContexts == NULL)
        {
            executionContexts = new il2cpp::utils::dynamic_array<Il2CppSequencePointExecutionContext*>();
            s_ExecutionContexts.SetValue(executionContexts);
        }

        std::deque<Il2CppSequencePoint*>* sequencePoints;
        s_SequencePoints.GetValue(reinterpret_cast<void**>(&sequencePoints));
        if (sequencePoints == NULL)
        {
            sequencePoints = new std::deque<Il2CppSequencePoint*>();
            s_SequencePoints.SetValue(sequencePoints);
        }
    }

    static void FreeThreadLocalData()
    {
        il2cpp::utils::dynamic_array<Il2CppSequencePointExecutionContext*>* executionContexts;
        s_ExecutionContexts.GetValue(reinterpret_cast<void**>(&executionContexts));
        delete executionContexts;
        s_ExecutionContexts.SetValue(NULL);

        std::deque<Il2CppSequencePoint*>* sequencePoints;
        s_SequencePoints.GetValue(reinterpret_cast<void**>(&sequencePoints));
        delete sequencePoints;
        s_SequencePoints.SetValue(NULL);
    }

    void Debugger::AllocateThreadStatics()
    {
        if (s_IsDebuggerInitialized)
            AllocateThreadLocalData();
    }

    void Debugger::ThreadStart(Il2CppThread* thread)
    {
        if (s_IsDebuggerInitialized)
        {
            IL2CPP_ASSERT(s_ThreadStartCallback);
            s_ThreadStartCallback(NULL, (uintptr_t)thread->GetInternalThread()->tid);
        }
    }

    void Debugger::ThreadEnd(Il2CppThread* thread)
    {
        if (s_IsDebuggerInitialized)
        {
            IL2CPP_ASSERT(s_ThreadEndCallback);
            s_ThreadEndCallback(NULL, (uintptr_t)thread->GetInternalThread()->tid);
            FreeThreadLocalData();
        }
    }

    Il2CppSequencePoint* Debugger::GetSequencePoint(size_t id)
    {
        SequencePointMap::iterator it = s_sequencePointMap.find(id);
        if (it == s_sequencePointMap.end())
            return NULL;

        return &it->second;
    }

    void Debugger::AddSequencePoint(size_t id, const Il2CppSequencePoint& point)
    {
        s_sequencePointMap.add(id, point);
    }

    Il2CppSequencePoint* Debugger::GetSequencePoints(const MethodInfo* method, void**iter)
    {
        if (!iter)
            return NULL;

        if (method->is_inflated)
            method = method->genericMethod->methodDefinition;

        MethodToSequencePointsMap::const_iterator entry = s_methodToSequencePoints.find(method);
        if (entry == s_methodToSequencePoints.end())
        {
            //the sequence point map doesn't have uninflated methods, need to map the incoming method to
            //an inflated method.  il2cpp_mono_methods_match has a case for this.
            MethodToMethodMap::const_iterator inflated = s_uninflatedMethodToInflated.find(method);
            if (inflated != s_uninflatedMethodToInflated.end())
            {
                method = inflated->second;
            }
            else
            {
                for (MethodToSequencePointsMap::iterator mapIter = s_methodToSequencePoints.begin(); mapIter != s_methodToSequencePoints.end(); ++mapIter)
                {
                    if (il2cpp_mono_methods_match(method, mapIter->first))
                    {
                        s_uninflatedMethodToInflated.add(method, mapIter->first);
                        method = mapIter->first;
                        break;
                    }
                }
            }

            entry = s_methodToSequencePoints.find(method);
            if (entry == s_methodToSequencePoints.end())
                return NULL;
        }

        if (!*iter)
        {
            SequencePointList::iterator *pIter = new SequencePointList::iterator();
            *pIter = entry->second->begin();
            *iter = pIter;
            return **pIter;
        }

        SequencePointList::iterator *pIter = (SequencePointList::iterator*)*iter;
        (*pIter)++;
        if (*pIter != entry->second->end())
        {
            return **pIter;
        }
        else
        {
            delete pIter;
            *iter = NULL;
        }

        return NULL;
    }

    Il2CppSequencePoint* Debugger::GetSequencePoints(void* *iter)
    {
        if (!iter)
            return NULL;

        if (!*iter)
        {
            SequencePointMap::iterator *pIter = new SequencePointMap::iterator();
            *pIter = s_sequencePointMap.begin();
            *iter = pIter;
            return &(**pIter).second;
        }

        SequencePointMap::iterator *pIter = (SequencePointMap::iterator*)*iter;
        (*pIter)++;
        if (*pIter != s_sequencePointMap.end())
        {
            return &(**pIter).second;
        }
        else
        {
            delete pIter;
            *iter = NULL;
        }

        return NULL;
    }

    void Debugger::AddMethodExecutionContextInfo(const char *methodName, const Il2CppMethodExecutionContextInfo& info)
    {
        MethodExecutionContextMap::iterator it = s_pExecutionContextMap->find(methodName);
        if (it != s_pExecutionContextMap->end())
        {
            it->second.push_back(info);
        }
        else
        {
            ExecutionContextInfoList infos;
            infos.push_back(info);
            s_pExecutionContextMap->add(methodName, infos);
        }
    }

    Il2CppMethodExecutionContextInfo* Debugger::GetMethodExecutionContextInfos(const char *methodName, int *count)
    {
        MethodExecutionContextMap::iterator it = s_pExecutionContextMap->find(methodName);
        if (it == s_pExecutionContextMap->end())
        {
            *count = 0;
            return NULL;
        }

        ExecutionContextInfoList& infos = it->second;
        *count = static_cast<int>(infos.size());
        return infos.data();
    }

    void Debugger::HandleException(Il2CppException *exc, Il2CppSequencePoint *sequencePoint)
    {
        unity_debugger_agent_handle_exception(exc, sequencePoint);
    }

    void Debugger::InitializeMethodToSequencePointMap()
    {
        for (SequencePointMap::iterator point = s_sequencePointMap.begin(); point != s_sequencePointMap.end(); ++point)
        {
            if (point->second.method != NULL)
            {
                const MethodInfo *method = point->second.method;
                if (method->is_inflated)
                    method = method->genericMethod->methodDefinition;

                SequencePointList* list;
                MethodToSequencePointsMap::iterator existingList = s_methodToSequencePoints.find(method);
                if (existingList == s_methodToSequencePoints.end())
                {
                    list = new SequencePointList();
                    s_methodToSequencePoints.add(method, list);
                }
                else
                {
                    list = existingList->second;
                }
                list->push_back(&point->second);
            }
        }
    }

    void Debugger::AddTypeSourceFile(const Il2CppClass *klass, const char *sourceFile)
    {
        TypeSourceFileMap::iterator it = s_typeSourceFiles->find(klass);
        if (it == s_typeSourceFiles->end())
        {
            FileNameList names;
            names.push_back(sourceFile);
            s_typeSourceFiles->add(klass, names);
        }
        else
        {
            it->second.push_back(sourceFile);
        }
    }

    const char** Debugger::GetTypeSourceFiles(const Il2CppClass *klass, int& count)
    {
        TypeSourceFileMap::iterator it = s_typeSourceFiles->find(klass);
        if (it == s_typeSourceFiles->end())
        {
            count = 0;
            return NULL;
        }

        count = (int)it->second.size();
        return it->second.data();
    }

    void Debugger::UserBreak()
    {
        if (s_IsDebuggerAttached)
            mono_debugger_agent_user_break();
    }

    bool Debugger::IsLoggingEnabled()
    {
        return mono_debugger_agent_debug_log_is_enabled();
    }

    void Debugger::Log(int level, Il2CppString *category, Il2CppString *message)
    {
        if (s_IsDebuggerAttached)
            mono_debugger_agent_debug_log(level, category, message);
    }

    bool Debugger::IsSequencePointActive(Il2CppSequencePoint *seqPoint)
    {
        return unity_sequence_point_active(seqPoint);
    }
}
}

#endif // IL2CPP_MONO_DEBUGGER
