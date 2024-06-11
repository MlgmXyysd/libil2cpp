#pragma once

#include <stdint.h>
#include <string>

struct Il2CppException;
struct Il2CppImage;
struct TypeInfo;

namespace il2cpp
{
namespace vm
{

class Exception
{
// exported
public:
	static void Raise (Il2CppException* ex);
	static void RaiseOutOfMemoryException ();
	static void RaiseNullReferenceException ();

	////TODO: rename to NewFromClassNameAndMessage
	static Il2CppException* FromNameMsg (const Il2CppImage* image, const char *name_space, const char *name, const char *msg);

public:
	////TODO: rename all of these to NewXXX
	static Il2CppException* GetArgumentException (const char *arg, const char *msg);
	static Il2CppException* GetArgumentNullException (const char *arg);
	static Il2CppException* GetTypeInitializationException(const char *msg, Il2CppException* innerException);
	static Il2CppException* GetIndexOutOfRangeException ();
	static Il2CppException* GetNullReferenceException ();
	static Il2CppException* GetInvalidCastException (const char* msg);
	static Il2CppException* GetTypeLoadException ();
	static Il2CppException* GetOutOfMemoryException ();
	static Il2CppException* GetOverflowException (const char* msg);
	static Il2CppException* GetFormatException (const char* msg);
	static Il2CppException* GetSystemException ();
	static Il2CppException* GetNotSupportedException(const char* msg);
	static Il2CppException* GetArrayTypeMismatchException();
	static Il2CppException* GetTypeLoadException(const char* msg);
	static Il2CppException* GetEntryPointNotFoundException(const char* msg);
	static Il2CppException* GetDllNotFoundException(const char* msg);
	static Il2CppException* GetInvalidOperationException(const char* msg);
	static Il2CppException* GetThreadInterruptedException ();
	static Il2CppException* GetThreadStateException ();
	static Il2CppException* GetSynchronizationLockException (const char* msg);
	static Il2CppException* GetMissingMethodException(const char* msg);
	static Il2CppException* GetMarshalDirectiveException(const char* msg);
	static Il2CppException* GetTargetException(const char* msg);
	static Il2CppException* GetExecutionEngineException (const char* msg);

	static Il2CppException* GetMaxmimumNestedGenericsException();

	static std::string FormatException(const Il2CppException* ex);
	static std::string FormatStackTrace(const Il2CppException* ex);
	static std::string FormatInvalidCastException(const TypeInfo* fromType, const TypeInfo* toType);
};

} /* namespace vm */
} /* namespace il2cpp */
