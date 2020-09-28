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
            void enqueue(T&& element)
            {
                _count += 1;
                node* new_node = new node(std::forward<T>(element));
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

            void enqueue(T& element)
            {
                _count += 1;
                node* new_node = new node(element);
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
    }
}

#endif