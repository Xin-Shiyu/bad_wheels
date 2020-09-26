#pragma once
#ifndef __NATIVA_COLLECTIONS_HELPERS
#define __NATIVA_COLLECTIONS_HELPERS

#include <string>
#include <limits>

namespace nativa
{
    namespace helpers
    {
        const unsigned string_roll = 51;

        template <class T>
        constexpr unsigned get_hash_code(T value) 
        {   // 默认强转成 unsigned 作为哈希值
            return value;
        }

        template<>
        constexpr unsigned get_hash_code<const char*>(const char* value)
        {   // 用多项式字符串哈希算法
            unsigned res = 0;
            unsigned p = 1;
            while (value != '\0')
            {
                res += (*value) & (std::numeric_limits<char>::max() - std::numeric_limits<char>::min()) * p;
                ++value;
                p *= string_roll;
            }
            return res;
        }

        template<>
        unsigned get_hash_code<std::string>(std::string value)
        {
            return get_hash_code(value.c_str());
        }

        class hashable
        {
        public:
            virtual unsigned get_hash_code() = 0;
        };

        template<>
        unsigned get_hash_code<hashable>(hashable value)
        {
            return value.get_hash_code();
        }
    }
}
#endif