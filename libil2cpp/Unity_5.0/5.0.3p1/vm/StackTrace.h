#pragma once

#include <stdint.h>
#include <vector>

#include "il2cpp-api-types.h"

namespace il2cpp
{

namespace vm
{

typedef std::vector<Il2CppStackFrameInfo> StackFrames;

class StackTrace
{
public:
	static void InitializeStackTracesForCurrentThread();
	static void CleanupStackTracesForCurrentThread();

	// Current thread functions
	static const StackFrames* GetStackFrames();
	static bool GetStackFrameAt(int32_t depth, Il2CppStackFrameInfo& frame);
	static void WalkFrameStack(Il2CppFrameWalkFunc callback, void* context);

	inline static int32_t GetStackDepth() { return GetStackFrames()->size(); }
	inline static bool GetTopStackFrame(Il2CppStackFrameInfo& frame) { return GetStackFrameAt(0, frame); }

	static void PushFrame(Il2CppStackFrameInfo& frame);
	static void PopFrame();

	// Remote thread functions
	static bool GetThreadStackFrameAt(Il2CppThread* thread, int32_t depth, Il2CppStackFrameInfo& frame);
	static void WalkThreadFrameStack(Il2CppThread* thread, Il2CppFrameWalkFunc callback, void* context);
	static int32_t GetThreadStackDepth(Il2CppThread* thread);
	static bool GetThreadTopStackFrame(Il2CppThread* thread, Il2CppStackFrameInfo& frame);
};

}

}