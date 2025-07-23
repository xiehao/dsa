/**
 * @file doubly_linked_queue.h
 * @brief 双向链表队列内部接口声明
 * @details 声明了双向链表队列的创建函数，供队列工厂使用
 * @author DSA Team
 * @date 2025-07-22
 * @version 1.0
 */

#ifndef DSA_DOUBLY_LINKED_QUEUE_H
#define DSA_DOUBLY_LINKED_QUEUE_H

#include <adt/queue.h>

/**
 * @brief 创建双向链表队列实例
 * @return 成功返回新创建的队列指针，失败返回NULL
 * @note 此函数为内部接口，通常通过queue_create()调用
 * @details 创建基于双向链表的队列实现，具有动态内存分配和无容量限制的特点
 */
dsa_queue_t *doubly_linked_queue_create(void);

#endif // DSA_DOUBLY_LINKED_QUEUE_H
