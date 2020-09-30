#pragma once
#ifndef __NATIVA_COLLECTIONS_LIST
#define __NATIVA_COLLECTIONS_LIST

#include <memory>
#include <limits>

#include "collections_common.h"
#include "span.h"

namespace nativa
{
	namespace collections
	{
		template <class T>
		class virtual_list;
		template <class T>
		class list;

		template <class T>
		class virtual_list
		{
		public:
			index_type find(T& element, index_type begin_index = 0)
			{
				for (index_type i = begin_index; i < _count; ++i)
				{
					if (_memory[i] == element) return i;
				}
				return -1;
			}

			index_type find(T&& element, index_type begin_index = 0)
			{
				return find(element, begin_index);
			}

			bool contains(T& element)
			{
				return find(element) != -1;
			}

			bool contains(T&& element)
			{
				return contains(element);
			}

			void reverse()
			{
				for (index_type i = 0, j = _count - 1; i < j; ++i, --j)
				{
					std::swap(_memory[i], _memory[j]);
				}
			}

			index_type count()
			{
				return _count;
			}

			T& operator[](index_type index)
			{
				if (0 <= index && index < _count)
				{
					return _memory[index];
				}
				else
				{
					return _memory[_count + index];
				}
			}

			T* begin()
			{
				return &_memory[0];
			}

			T* end()
			{
				return &_memory[_count];
			}

			virtual_list(span<T>&& memory)
				: _memory(std::move(memory)) {}

			virtual_list(span<T>&& memory, index_type count)
				: _memory(std::move(memory))
			{
				_count = count;
			}

		protected:
			span<T> _memory;
			index_type _count = 0;
		};

		template <class T>
		class list : public virtual_list<T>
		{
		public:
			list() : virtual_list<T>(span<T>(0, 8))
			{
				_manager.manage(this->_memory);
			}

			list(std::initializer_list<T>&& init)
				: virtual_list<T>(span<T>(0, init.size()))
			{
				_manager.manage(this->_memory);
				auto end = init.end();
				this->_count = init.size();
				index_type i = 0;
				for (auto it = init.begin();
					it != end;
					++it, ++i)
				{
					this->_memory[i] = *it;
				}
			}

			void reserve(index_type capacity)
			{
				this->_manager.resize(capacity);
			}

			template <class Te>
			void add(Te&& element)
			{
				__NATIVA_ENSURE_TYPE_SAFETY(T, Te);
				extend();
				this->_memory[this->_count - 1] = element;
			}

			void insert(index_type index, T& element)
			{
				extend();
				for (index_type i = this->_count - 1; i > index ; --i)
				{
					this->_memory[i] = std::move(this->_memory[i - 1]);
				}
				this->_memory[index] = element;
			}

			void insert(index_type index, T&& element)
			{
				insert(index, element);
			}

			void remove_at(index_type index)
			{
				index_type new_count = this->_count - 1;
				/*
				for (index_type i = index; i < new_count; ++i)
				{
					this->_memory[i] = this->_memory[i + 1];
				}
				*/
				move_left(index + 1, new_count, 1);
				this->_count = new_count;
			}

			void remove_all(T& element)
			{
				index_type new_count = 0;
				index_type slice_begin = 0;
				for (index_type i = 0; i < this->_count; ++i)
				{
					if (this->_memory[i] == element)
					{
						if (slice_begin != 0)
						{
							move_left(slice_begin, i - 1, i - new_count);
							slice_begin = 0;
						}
					}
					else
					{
						++new_count;
						if (slice_begin == 0)
						{
							slice_begin = i;
						}
					}
				}
				if (slice_begin != 0)
				{
					move_left(slice_begin, this->_count - 1, this->_count - new_count);
				}
				this->_count = new_count;
			}

			void remove_all(T&& element)
			{
				remove_all(element);
			}

			void attach(const virtual_list<T>& part)
			{
				auto new_start = this->_count;
				auto part_count = part.count();
				extend(part_count);
				for (index_type i = 0, j = new_start; i < part_count; ++i, ++j)
				{
					this->_memory[j] = part[i];
				}
			}

			void attach(virtual_list<T>&& part)
			{	// 这里不需要移动语义，virtual_list 的来源不应被移动
				attach(part);
			}

			void attach(std::initializer_list<T>&& init)
			{
				reserve(find_best_capacity(
					this->_memory.size(),
					this->_count,
					init.size());
				for (auto&& element : init)
				{
					add(element);
				}
			}

			virtual_list<T> range(index_type begin, index_type end)
			{
				if (begin == 0 && end == this->_count) return *this;
				auto length = end - begin;
				return virtual_list<T>(span<T>(&this->_memory[begin], length), length);
			}

			virtual_list<T> range(index_type begin)
			{
				return range(begin, this->_count);
			}

			void clear()
			{
				this->_count = 0;
			}

		private:
			static index_type find_best_capacity(
				index_type old_capacity,
				index_type old_count,
				index_type count_diff)
			{
				auto res = old_capacity;
				auto min = old_count + count_diff;
				while (res < min)
				{
					res *= 2;
				}
				return res;
			}

			void extend(index_type diff = 1)
			{
				this->_manager.resize(
					find_best_capacity(
						this->_memory.size(),
						this->_count,
						diff),
					this->_count);
				this->_count += diff;
			}

			void move_left(index_type from, index_type to, const index_type diff = 1)
			{
				from -= diff;
				to -= diff;
				for (index_type i = from; i <= to; ++i)
				{
					this->_memory[i] = std::move(this->_memory[i + diff]);
				}
			}

			span_manager<T> _manager;
		};
	}
}

#endif