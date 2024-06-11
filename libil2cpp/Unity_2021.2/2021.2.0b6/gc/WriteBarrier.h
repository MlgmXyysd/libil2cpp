#pragma once

#include <type_traits>

struct Il2CppObject;

namespace il2cpp
{
namespace gc
{
    class WriteBarrier
    {
    public:
        static void GenericStore(void** ptr, void* value);

        template<typename TPtr, typename TValue>
        static void GenericStore(TPtr** ptr, TValue* value)
        {
            static_assert((std::is_assignable<TPtr*&, TValue*>::value), "Pointers types are not assignment compatible");
            GenericStore((void**)ptr, (void*)value);
        }

        template<typename TPtr>
        static void GenericStoreNull(TPtr** ptr)
        {
            *ptr = NULL;
        }
    };
} /* gc */
} /* il2cpp */
