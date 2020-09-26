#pragma once
#ifndef __NATIVA_COLLECTIONS_DICTIONARY
#define __NATIVA_COLLECTIONS_DICTIONARY

#include "collections_common.h"
#include "span.h"

namespace nativa
{
	namespace collections
	{
        template <class TKey, class TValue>
        class dictionary
        {
        public:
            dictionary()
                : buckets(0, 7), entries(0, 7)
            {
                buckets_manager.manage(buckets);
                buckets.fill(-1);
                entries_manager.manage(entries);
            }
        private:
            struct entry
            {
                TKey key;
                TValue value;
                index_type next;
                unsigned hash_code;
            };

            span<index_type> buckets;
            span_manager<index_type> buckets_manager;
            span<entry> entries;
            span_manager<entry> entries_manager;
        };
    }
}

#endif