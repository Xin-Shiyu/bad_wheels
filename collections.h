#pragma once
#ifndef __NATIVA_COLLECTIONS
#define __NATIVA_COLLECTIONS

#include <initializer_list>
#include <memory>
#include <utility>

namespace nativa
{
	namespace collections
	{
		using index_type = long;

		template <class T>
		class span;
		template <class T>
		class span_manager;
		template <class T>
		class virtual_list;
		template <class T>
		class list;

		template <class T>
		class span
		{
		public:
			span(T* ptr = nullptr, index_type size = 0)
			{
				_ptr = ptr;
				_size = size;
			}

			T& operator[](index_type index)
			{
				return _ptr[index];
			}

			void rebind(T* ptr)
			{
				_ptr = ptr;
			}

			virtual void resize(index_type new_size)
			{
				_size = new_size;
			}

			index_type size()
			{
				return _size;
			}

		private:
			friend class span_manager<T>;
			T* _ptr;
			index_type _size = 0;
		};

		template <class T>
		class span_manager
		{
		public:
			friend class span<T>;

			span_manager()
			{
				_span = nullptr;
			}

			span_manager(span_manager<T>&& manager)
			{
				_span = manager._span;
			}

			void manage(span<T>& span)
			{
				_span = &span;
				_span->_ptr = new T[_span->size()];
			}

			~span_manager()
			{
				delete[] _span->_ptr;
			}

			void resize(index_type new_size, index_type copy_size)
			{
				T* new_ptr = new T[new_size];
				for (index_type i = 0; i < copy_size; ++i)
				{
					new_ptr[i] = std::move(_span->_ptr[i]);
				}
				delete[] _span->_ptr;
				_span->_ptr = new_ptr;
				_span->_size = new_size;
			}
			
		private:
			span<T>* _span;
		};

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
				if (index >= 0)
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

			void add(T& element)
			{
				extend();
				this->_memory[this->_count - 1] = element;
			}

			void add(T&& element)
			{
				add(element);
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

			void attach(virtual_list<T>& part)
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
			{
				attach(part);
			}

			void attach(std::initializer_list<T>&& init)
			{
				attach(list<T>(std::move(init)));
			}

			virtual_list<T> range(index_type begin, index_type end)
			{
				if (begin == 0 and end == this->_count) return *this;
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
			void extend(index_type diff = 1)
			{
				auto capacity = this->_memory.size();
				while (capacity < this->_count + diff)
				{
					capacity *= 2;
				}
				this->_manager.resize(capacity, this->_count);
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