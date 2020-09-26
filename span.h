#pragma once
#ifndef __NATIVA_COLLECTIONS_SPAN
#define __NATIVA_COLLECTIONS_SPAN

#include "common.h"
#include <memory>

namespace nativa
{
	namespace collections
	{
		template <class T>
        class span;
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
    }
}

#endif