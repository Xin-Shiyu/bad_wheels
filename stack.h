#pragma once
#ifndef __NATIVA_COLLECTIONS_STACK
#define __NATIVA_COLLECTIONS_STACK

#include "collections_common.h"

namespace nativa
{
    namespace collections
    {
        template <class T, index_type size>
        class stack
        {
        public:
            template <class Te>
            void push(Te&& element)
            {
                __NATIVA_ENSURE_TYPE_SAFETY(T, Te);
                _base[_count] = element;
                ++_count;
            }

            const T& peek()
            {
                return _base[_count - 1];
            }

            T pop()
            {
                --_count;
                return _base[_count];
            }

            index_type count()
            {
                return _count;
            }

            bool is_empty()
            {
                return _count == 0;
            }

            bool is_full()
            {
                return _count == size;
            }

            const T& operator[](index_type index)
            {
                return _base[index];
            }
        private:
            index_type _count = 0;
            T _base[size];
        };
    }
}

#endif