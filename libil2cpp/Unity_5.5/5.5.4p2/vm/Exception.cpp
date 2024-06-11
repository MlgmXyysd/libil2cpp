#include "il2cpp-config.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/Runtime.h"
#include "vm/StackTrace.h"
#include "vm/String.h"
#include "Image.h"
#include "../utils/StringUtils.h"
#include <sstream>
#include "tabledefs.h"
#include "class-internals.h"
#include "object-internals.h"

namespace il2cpp
{
namespace vm
{
    NORETURN void Exception::Raise(Il2CppException* ex)
    {
        if (ex->trace_ips == NULL)
        {
            // Only write the stack trace if there is not one already in the exception.
            // When we exit managed try/finally and try/catch blocks with an exception, this method is
            // called with the original exception which already has the proper stack trace.
            // Getting the stack trace again here will lose the frames between the original throw
            // and the finally or catch block.
            const StackFrames& frames = *StackTrace::GetStackFrames();
            size_t i = frames.size() - 1;
            Il2CppArray* ips = Array::New(il2cpp_defaults.int_class, (il2cpp_array_size_t)frames.size());
            for (StackFrames::const_iterator iter = frames.begin(); iter != frames.end(); ++iter, --i)
            {
                il2cpp_array_set(ips, const MethodInfo*, i, (*iter).method);
            }

            IL2CPP_OBJECT_SETREF(ex, trace_ips, ips);
        }

        throw Il2CppExceptionWrapper(ex);
    }

    NORETURN void Exception::RaiseOutOfMemoryException()
    {
        Raise(GetOutOfMemoryException());
    }

    NORETURN void Exception::RaiseNullReferenceException()
    {
        Raise(GetNullReferenceException());
    }

    NORETURN void Exception::RaiseDivideByZeroException()
    {
        Raise(GetDivideByZeroException());
    }

    NORETURN void Exception::RaiseOverflowException()
    {
        Raise(GetOverflowException());
    }

    NORETURN void Exception::RaiseArgumentOutOfRangeException(const char* msg)
    {
        Raise(GetArgumentOutOfRangeException(msg));
    }

    NORETURN void Exception::RaiseCOMException(il2cpp_hresult_t hresult, const char* msg)
    {
        Il2CppException* exception = Exception::FromNameMsg(vm::Image::GetCorlib(), "System.Runtime.InteropServices", "COMException", msg);
        exception->hresult = hresult;
        Exception::Raise(exception);
    }

    NORETURN void Exception::Raise(il2cpp_hresult_t hresult)
    {
        switch (hresult)
        {
            case IL2CPP_E_NOTIMPL:
                Raise(FromNameMsg(Image::GetCorlib(), "System", "NotImplementedException", NULL));

            case IL2CPP_E_NOINTERFACE:
                Raise(GetInvalidCastException(NULL));

            case IL2CPP_E_POINTER:
                RaiseNullReferenceException();

            case IL2CPP_E_ABORT:
            case IL2CPP_COR_E_OPERATIONCANCELED:
                Raise(FromNameMsg(Image::GetCorlib(), "System", "OperationCanceledException", NULL));

            case IL2CPP_E_FAIL:
                RaiseCOMException(hresult, "Unspecified error");

            case IL2CPP_E_ACCESS_DENIED:
                Raise(GetUnauthorizedAccessException(NULL));

            case IL2CPP_E_OUTOFMEMORY:
                RaiseOutOfMemoryException();

            case IL2CPP_E_INVALIDARG:
                Raise(GetArgumentException(NULL, NULL));

            case IL2CPP_E_BOUNDS:
                Raise(GetIndexOutOfRangeException());

            case IL2CPP_E_CHANGED_STATE:
                RaiseCOMException(hresult, "A concurrent or interleaved operation changed the state of the object, invalidating this operation.");

            case IL2CPP_REGDB_E_CLASSNOTREG:
                RaiseCOMException(hresult, "Class not registered.");

            case IL2CPP_RPC_E_WRONG_THREAD:
                RaiseCOMException(hresult, "The application called an interface that was marshalled for a different thread.");

            case IL2CPP_RPC_E_DISCONNECTED:
                RaiseCOMException(hresult, "The object invoked has disconnected from its clients.");

            case IL2CPP_RO_E_CLOSED:
                Raise(FromNameMsg(Image::GetCorlib(), "System", "ObjectDisposedException", NULL));

            case IL2CPP_COR_E_EXCEPTION:
                Raise(FromNameMsg(Image::GetCorlib(), "System", "Exception", NULL));

            case IL2CPP_COR_E_PLATFORMNOTSUPPORTED:
                Raise(GetPlatformNotSupportedException("Operation is not supported on this platform."));

            default:
                RaiseCOMException(hresult);
        }
    }

    Il2CppException* Exception::FromNameMsg(const Il2CppImage* image, const char *name_space, const char *name, const char *msg)
    {
        Il2CppClass* exceptionClass = Class::FromName(image, name_space, name);
        Il2CppException* ex = (Il2CppException*)Object::New(exceptionClass);
        Runtime::ObjectInit((Il2CppObject*)ex);

        if (msg)
            IL2CPP_OBJECT_SETREF(ex, message, String::New(msg));

        return ex;
    }

    Il2CppException * Exception::GetArgumentException(const char *arg, const char *msg)
    {
        Il2CppException* ex = FromNameMsg(Image::GetCorlib(), "System", "ArgumentException", msg);

        if (arg)
        {
            Il2CppArgumentException *argex = (Il2CppArgumentException*)ex;
            IL2CPP_OBJECT_SETREF(argex, argName, String::New(arg));
        }

        return ex;
    }

    Il2CppException * Exception::GetArgumentNullException(const char *arg)
    {
        Il2CppException* ex = FromNameMsg(Image::GetCorlib(), "System", "ArgumentNullException", NULL);

        if (arg)
        {
            Il2CppArgumentException *argex = (Il2CppArgumentException*)ex;
            IL2CPP_OBJECT_SETREF(argex, argName, String::New(arg));
        }

        return ex;
    }

    Il2CppException * Exception::GetArgumentOutOfRangeException(const char *arg)
    {
        Il2CppException* ex = FromNameMsg(Image::GetCorlib(), "System", "ArgumentOutOfRangeException", NULL);

        if (arg)
        {
            Il2CppArgumentException *argex = (Il2CppArgumentException*)ex;
            IL2CPP_OBJECT_SETREF(argex, argName, String::New(arg));
        }

        return ex;
    }

    Il2CppException * Exception::GetTypeInitializationException(const char *msg, Il2CppException* innerException)
    {
        Il2CppException* ex = FromNameMsg(Image::GetCorlib(), "System", "TypeInitializationException", msg);

        if (innerException != NULL)
            IL2CPP_OBJECT_SETREF(ex, inner_ex, innerException);

        return ex;
    }

    Il2CppException * Exception::GetInvalidCastException(const char* msg)
    {
        return FromNameMsg(Image::GetCorlib(), "System", "InvalidCastException", msg);
    }

    Il2CppException * Exception::GetIndexOutOfRangeException()
    {
        return FromNameMsg(Image::GetCorlib(), "System", "IndexOutOfRangeException", NULL);
    }

    Il2CppException* Exception::GetNullReferenceException()
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "NullReferenceException", NULL);
    }

    Il2CppException* Exception::GetTypeLoadException()
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "TypeLoadException", NULL);
    }

    Il2CppException* Exception::GetOutOfMemoryException()
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "OutOfMemoryException", NULL);
    }

    Il2CppException* Exception::GetOverflowException()
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "OverflowException", NULL);
    }

    Il2CppException* Exception::GetOverflowException(const char* msg)
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "OverflowException", msg);
    }

    Il2CppException* Exception::GetFormatException(const char* msg)
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "FormatException", msg);
    }

    Il2CppException* Exception::GetSystemException()
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "SystemException", NULL);
    }

    Il2CppException* Exception::GetNotSupportedException(const char* msg)
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "NotSupportedException", msg);
    }

    Il2CppException* Exception::GetArrayTypeMismatchException()
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "ArrayTypeMismatchException", NULL);
    }

    Il2CppException* Exception::GetTypeLoadException(const char* msg)
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "TypeLoadException", msg);
    }

    Il2CppException* Exception::GetEntryPointNotFoundException(const char* msg)
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "EntryPointNotFoundException", msg);
    }

    Il2CppException* Exception::GetDllNotFoundException(const char* msg)
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "DllNotFoundException", msg);
    }

    Il2CppException * Exception::GetInvalidOperationException(const char* msg)
    {
        return FromNameMsg(Image::GetCorlib(), "System", "InvalidOperationException", msg);
    }

    Il2CppException* Exception::GetThreadInterruptedException()
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System.Threading", "ThreadInterruptedException", NULL);
    }

    Il2CppException* Exception::GetThreadAbortException()
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System.Threading", "ThreadAbortException", NULL);
    }

    Il2CppException* Exception::GetThreadStateException(const char* msg)
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System.Threading", "ThreadStateException", msg);
    }

    Il2CppException* Exception::GetSynchronizationLockException(const char* msg)
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System.Threading", "SynchronizationLockException", msg);
    }

    Il2CppException * Exception::GetMissingMethodException(const char* msg)
    {
        return FromNameMsg(Image::GetCorlib(), "System", "MissingMethodException", msg);
    }

    Il2CppException * Exception::GetMarshalDirectiveException(const char* msg)
    {
        return FromNameMsg(Image::GetCorlib(), "System.Runtime.InteropServices", "MarshalDirectiveException", msg);
    }

    Il2CppException * Exception::GetTargetException(const char* msg)
    {
        return FromNameMsg(Image::GetCorlib(), "System.Reflection", "TargetException", msg);
    }

    Il2CppException * Exception::GetExecutionEngineException(const char* msg)
    {
        return FromNameMsg(Image::GetCorlib(), "System", "ExecutionEngineException", msg);
    }

    Il2CppException * Exception::GetUnauthorizedAccessException(const char* msg)
    {
        return FromNameMsg(Image::GetCorlib(), "System", "UnauthorizedAccessException", msg);
    }

    Il2CppException * Exception::GetMaxmimumNestedGenericsException()
    {
        return GetNotSupportedException("IL2CPP encountered a managed type which it cannot convert ahead-of-time. The type uses generic or array types which are nested beyond the maximum depth which can be converted.");
    }

    Il2CppException* Exception::GetDivideByZeroException()
    {
        return FromNameMsg(vm::Image::GetCorlib(), "System", "DivideByZeroException", NULL);
    }

    Il2CppException* Exception::GetPlatformNotSupportedException(const char* msg)
    {
        return FromNameMsg(Image::GetCorlib(), "System", "PlatformNotSupportedException", msg);
    }

    Il2CppException* Exception::GetFileLoadException(const char* msg)
    {
        return FromNameMsg(Image::GetCorlib(), "System.IO", "FileLoadException", msg);
    }

    std::string Exception::FormatException(const Il2CppException* ex)
    {
        std::string exception_namespace = ex->object.klass->namespaze;
        std::string exception_type = ex->object.klass->name;
        if (ex->message)
            return exception_namespace + "." + exception_type + ": " + il2cpp::utils::StringUtils::Utf16ToUtf8(il2cpp::vm::String::GetChars(ex->message));
        else
            return exception_namespace + "." + exception_type;
    }

    std::string Exception::FormatInvalidCastException(const Il2CppClass* fromType, const Il2CppClass* toType)
    {
        std::stringstream message;

        if (fromType != NULL && toType != NULL)
            message << "Unable to cast object of type '" << fromType->name << "' to type '" << toType->name << "'.";

        return message.str();
    }

    std::string Exception::FormatStackTrace(const Il2CppException* ex)
    {
        if (ex->stack_trace)
            return il2cpp::utils::StringUtils::Utf16ToUtf8(il2cpp::vm::String::GetChars(ex->stack_trace));

        return "";
    }
} /* namespace vm */
} /* namespace il2cpp */
