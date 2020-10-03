#pragma once
#ifndef __NATIVA_COLLECTIONS_MEMORY_HELPER
#define __NATIVA_COLLECTIONS_MEMORY_HELPER

#include <new>

namespace nativa
{
    namespace helpers
    {
        template <class T>
        class memory_helper
        {
            static T* acquire(size_t count)
            {
                return static_cast<T*>(::operator new(alignof(T) * count));
            }

            static void release(T* ptr, size_t count)
            {
                ::operator delete(T*, count * alignof(T));
            }
        };
    }
}
#endif