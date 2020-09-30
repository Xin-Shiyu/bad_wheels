#pragma once
#ifndef __NATIVA_COLLECTIONS_QUEUE
#define __NATIVA_COLLECTIONS_QUEUE

#include <memory>

#include "collections_common.h"

namespace nativa
{
    namespace collections
    {
        template <class T>
        class queue
        { // 简单的基于链表的队列
        public:
            template <class Te>
            void enqueue(Te&& element)
            {
                __NATIVA_ENSURE_TYPE_SAFETY(T, Te);
                _count += 1;
                node* new_node = new node(std::forward<Te>(element));
                if (rear == nullptr)
                {
                    front = new_node;
                    rear = new_node;
                }
                else
                {
                    rear->next = new_node;
                    rear = rear->next;
                }
            }

            T dequeue()
            {
                _count -= 1;
                T res = std::move(front->value);
                node* next = front->next;
                delete front;
                front = next;
                return res;
            }

            const T& peek()
            {
                return front->value;
            }

            bool is_empty()
            {
                return front == nullptr;
            }

            index_type count()
            {
                return _count;
            }

            ~queue()
            {
                node* current_node = front;
                while (current_node != nullptr)
                {
                    node* next = current_node->next;
                    delete current_node;
                    current_node = next;
                }
            }
        private:
            struct node
            {
                T value;
                node* next = nullptr;
                node(T&& value) : value(value) {}
                node(T& value) : value(value) {}
            };

            node* front = nullptr;
            node* rear = nullptr;
            index_type _count = 0;
        };
    
        template <class T, index_type max_count>
        class circular_queue
        {
        public:
            template <class Te>
            void enqueue(Te&& element)
            {
                __NATIVA_ENSURE_TYPE_SAFETY(T, Te);
                base[before_front] = element;
                before_front = (before_front + 1) % max_count;
                _count += 1;
            }

            T dequeue()
            {
                T res = std::move(base[rear]);
                rear = (rear + 1) % max_count;
                _count -= 1;
                return res;
            }

            const T& peek()
            {
                return base[rear];
            }

            bool is_empty()
            {
                return _count == 0;
            }

            bool is_full()
            {
                return _count == max_count;
            }

            index_type count()
            {
                return _count;
            }
        private:
            T base[max_count];
            index_type before_front = 0;
            index_type rear = 0;
            index_type _count = 0;
        };
    }
}

#endif