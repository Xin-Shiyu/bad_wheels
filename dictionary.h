#pragma once
#ifndef __NATIVA_COLLECTIONS_DICTIONARY
#define __NATIVA_COLLECTIONS_DICTIONARY

#include <initializer_list>
#include <utility>

#include "collections_common.h"
#include "hash_helpers.h"
#include "span.h"

namespace nativa
{
	namespace collections
	{
        template <class TKey, class TValue>
        class dictionary
        {
            constexpr static double redundancy = 2;
        public:
            dictionary()
                : buckets(0, 7), entries(0, 7)
            {
                buckets_manager.manage(buckets);
                buckets.fill(-1);
                entries_manager.manage(entries);
                entries_count = 0;
            }

            dictionary(std::initializer_list<std::pair<TKey, TValue>>&& init)
                : dictionary()
            {
                auto end = init.end();
                for (auto it = init.begin();
                    it != end;
                    ++it)
                {
                    add(it->first, it->second);
                }
            }

            TValue& operator[](TKey key)
            {
                return entries[find_entry(key)].value;
            }

            template <class TKeye, class TValuee>
            void add(TKeye&& key, TValuee&& value)
            {
                __NATIVA_ENSURE_TYPE_SAFETY(TKey, TKeye);
                __NATIVA_ENSURE_TYPE_SAFETY(TValue, TValuee);
                try_reserve();
                map_entry_key(std::forward<TKeye>(key), fill_in_value(std::forward<TValuee>(value)));
            }

            template <class TKeye>
            bool contains_key(TKeye&& key)
            {
                __NATIVA_ENSURE_TYPE_SAFETY(TKey, TKeye);
                return find_entry(key) != -1;
            }

            template <class TKeye>
            void remove_at(TKeye&& key)
            {
                __NATIVA_ENSURE_TYPE_SAFETY(TKey, TKeye);
                entries[find_entry(key)].hash_code = -1;
            }

            struct entry
            {
                TKey key;
                TValue value;
            private:
                friend class dictionary;
                friend class iterator;
                index_type next = -1;
                int hash_code = -1;
            };

            class iterator
            {
            public:
                entry& operator*()
                {
                    return _dictionary->entries[_index];
                }

                void operator++()
                {
                    ++_index;
                    while (_index < _dictionary->entries.size() && _dictionary->entries[_index].hash_code == -1)
                    {   // 如果没有到实际的末尾就直接跳过。
                        ++_index;
                    }
                }

                bool operator!=(iterator it)
                {
                    return (it._index != _index) || (it._dictionary != _dictionary);
                }
            private:
                friend class dictionary;
                index_type _index;
                dictionary* _dictionary;
                iterator(dictionary* my_dictionary, index_type index)
                {
                    _index = index;
                    _dictionary = my_dictionary;
                }
            };

            iterator begin()
            {
                return iterator(this, 0);
            }

            iterator end()
            {
                return iterator(this, entries.size());
            }

        private:
            span<index_type> buckets;
            span_manager<index_type> buckets_manager;
            span<entry> entries;
            span_manager<entry> entries_manager;
            index_type entries_count;

            index_type find_entry(TKey key)
            {
                auto hash = helpers::get_hash_code(key);
                auto index = buckets[hash % buckets.size()];
                if (index == -1) return -1;
                while (entries[index].hash_code != hash ||
                       entries[index].key != key)
                {
                    if (entries[index].next == -1) return -1;
                    index = entries[index].next;
                }
                return index;
            }

            index_type find_empty_entry()
            {
                auto len = entries.size();
                for (index_type i = 0; i < len; ++i)
                {
                    if (entries[i].hash_code == -1) return i;
                }
                return -1;
            }

            void map_entry_key(const TKey& key, index_type target_entry_index)
            {
                auto hash = helpers::get_hash_code(key);
                entries[target_entry_index].hash_code = hash;
                entries[target_entry_index].key = key;
                map_entry(hash, target_entry_index);
                entries_count += 1;
            }

            void try_reserve()
            {
                index_type expected_size = static_cast<index_type>(entries_count * redundancy);
                if (expected_size >= buckets.size())
                {
                    resize(expected_size);
                }
            }

            template <class TValuee>
            index_type fill_in_value(TValuee&& value)
            {
                __NATIVA_ENSURE_TYPE_SAFETY(TValue, TValuee);
                index_type target_entry_index = find_empty_entry();
                entries[target_entry_index].value = value;
                return target_entry_index;
            }

            void map_entry(int hash, index_type target_entry_index)
            {
                auto bucket_index = hash % buckets.size();
                if (buckets[bucket_index] == -1)
                {
                    buckets[bucket_index] = target_entry_index;
                }
                else
                {
                    auto iterated_entry_index = buckets[bucket_index];
                    while (entries[iterated_entry_index].next != -1)
                    {
                        iterated_entry_index = entries[iterated_entry_index].next;
                    }
                    entries[iterated_entry_index].next = target_entry_index;
                }
            }

            void resize(index_type new_count)
            {
                index_type next_prime = helpers::prime::get(new_count);
                buckets_manager.resize_empty(next_prime);
                buckets.fill(-1);
                auto old_entries_size = entries.size();
                for (index_type i = 0; i < old_entries_size; ++i)
                {
                    if (entries[i].hash_code != -1)
                    {
                        entries[i].next = -1; // 先摧毁链表，待会再重新建立
                    }
                }
                entries_manager.resize(next_prime, old_entries_size);
                for (index_type i = 0; i < next_prime; ++i)
                {
                    if (entries[i].hash_code != -1)
                    {
                        map_entry(entries[i].hash_code, i);
                    }
                }
            }
        };
    }
}

#endif