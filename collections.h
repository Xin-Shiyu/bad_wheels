#pragma once
#ifndef __NATIVA_COLLECTIONS
#define __NATIVA_COLLECTIONS

#include <initializer_list>
#include <memory>

namespace nativa
{
	namespace collections
	{
		using index_type = long;

		template <class T>
		class span_manager;

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

			void manage(span<T>& span)
			{
				_span = &span;
				_span->_ptr = new T[_span->size()];
			}

			~span_manager()
			{
				delete[] _span->_ptr;
			}

			void resize(index_type new_size)
			{
				T* new_ptr = new T[new_size];
				for (index_type i = 0; i < new_size; ++i)
				{
					new_ptr[i] = _span->_ptr[i];
				}
				delete[] _span->_ptr;
				_span->_ptr = new_ptr;
				_span->_size = new_size;
			}
			
		private:
			span<T>* _span = nullptr;
		};

		template <class T>
		class virtual_list
		{
		public:
			index_type find(T element, index_type begin_index = 0)
			{
				for (index_type i = begin_index; i < _count; ++i)
				{
					if (_memory[i] == element) return i;
				}
				return -1;
			}

			bool contains(T element)
			{
				return find(element) != -1;
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

			void add(T element)
			{
				extend();
				this->_memory[this->_count] = element;
			}

			void insert(index_type index, T element)
			{
				extend();
				for (index_type i = index + 1; i < this->_count; ++i)
				{
					this->_memory[i] = this->_memory[i - 1];
				}
				this->_memory[index] = element;
			}

			void remove_at(index_type index)
			{
				int new_count = this->_count - 1;
				for (index_type i = index; i < new_count; ++i)
				{
					this->_memory[i] = this->_memory[i + 1];
				}
				this->_count = new_count;
			}

			virtual_list<T> range(index_type begin, index_type end)
			{
				return virtual_list<T>(span<T>(&this->_memory[begin], end - begin), end - begin);
			}

			void clear()
			{
				this->_count = 0;
			}

		private:
			void grow_capacity()
			{
				this->_manager.resize(this->_memory.size() * 2);
			}

			void extend()
			{
				if (this->_memory.size() == this->_count)
				{
					grow_capacity();
				}
				this->_count += 1;
			}

			span_manager<T> _manager;
		};
	}
}

#endif