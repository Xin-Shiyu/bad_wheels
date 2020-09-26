#pragma once
#ifndef __NATIVA_COLLECTIONS_HELPERS
#define __NATIVA_COLLECTIONS_HELPERS

#include <string>
#include <limits>

namespace nativa
{
    namespace helpers
    {
        #pragma region get_hash_code
        template <class T>
        constexpr int get_hash_code(T value) 
        {   // 默认强转成 unsigned 作为哈希值
            return value;
        }

        const int string_roll = 51;
        template<>
        constexpr int get_hash_code<const char*>(const char* value)
        {   // 用多项式字符串哈希算法
            int res = 0;
            int p = 1;
            while (value != '\0')
            {
                res += (*value) & (std::numeric_limits<char>::max() - std::numeric_limits<char>::min()) * p;
                ++value;
                p *= string_roll;
            }
            return res;
        }

        template<>
        int get_hash_code<std::string>(std::string value)
        {
            return get_hash_code(value.c_str());
        }
        #pragma endregion
        
        class prime
        {
        public:
            int get(int min)
            {
                for (int i = 0; i < 72; ++i)
                {
                    if (primes[i] >= min)
                    {
                        return primes[i];
                    }
                }
            }
        private:
            const int primes[72]
            {
                3,
                7, 
                11,
                17,
                23,
                29,
                37,
                47,
                59,
                71,
                89,
                107,
                131,
                163,
                197,
                239,
                293,
                353,
                431,
                521,
                631,
                761,
                919,
                1103,
                1327,
                1597,
                1931,
                2333,
                2801,
                3371,
                4049,
                4861,
                5839,
                7013,
                8419,
                10103,
                12143,
                14591,
                17519,
                21023,
                25229,
                30293,
                36353,
                43627,
                52361,
                62851,
                75431,
                90523,
                108631,
                130363,
                156437,
                187751,
                225307,
                270371,
                324449,
                389357,
                467237,
                560689,
                672827,
                807403,
                968897,
                1162687,
                1395263,
                1674319,
                2009191,
                2411033,
                2893249,
                3471899,
                4166287,
                4999559,
                5999471,
                7199369
            }; // 以上素数来自 .NET Core 的 HashHelpers
        };
    }
}
#endif