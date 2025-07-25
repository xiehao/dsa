/**
 * @file linked_list_iterator.h
 * @brief 链表迭代器的统一接口定义
 * @details 提供了链表迭代器操作接口，支持单链表、双链表和循环链表的遍历访问
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#ifndef DSA_LINKED_LIST_ITERATOR_H
#define DSA_LINKED_LIST_ITERATOR_H

#include <stddef.h>
#include <stdbool.h>
#include <common.h>
#include <ds/linked_list.h>

// 前向声明迭代器类型
typedef struct dsa_iterator_t dsa_iterator_t;

/**
 * @defgroup LinkedListIterator 链表迭代器操作
 * @brief 链表迭代器的创建和操作接口
 * @{
 */

/**
 * @brief 创建指向链表开始位置的迭代器
 *
 * @param linked_list 指向链表的常量指针
 * @return 指向链表第一个元素的迭代器，如果链表为空或无效则返回NULL
 * 
 * @note 调用者负责在不再需要时调用 iterator_destroy() 来释放迭代器内存
 * @note 支持单链表、双链表和循环链表
 * @see iterator_destroy()
 * 
 * @par 示例用法:
 * @code
 * dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
 * dsa_iterator_t *iter = linked_list_begin(list);
 * while (iterator_is_valid(iter)) {
 *     int *value = (int *)iterator_get_value(iter);
 *     printf("%d ", *value);
 *     iterator_next(iter);
 * }
 * iterator_destroy(iter);
 * @endcode
 */
dsa_iterator_t *linked_list_begin(const dsa_linked_list_t *linked_list);

/**
 * @brief 创建指向链表结束位置的迭代器
 *
 * @param linked_list 指向链表的常量指针
 * @return 指向链表末尾（最后一个元素之后）的迭代器，如果链表无效则返回NULL
 * 
 * @note 调用者负责在不再需要时调用 iterator_destroy() 来释放迭代器内存
 * @note 支持单链表、双链表和循环链表
 * @note end迭代器不指向有效元素，主要用于边界检查
 * @see iterator_destroy()
 * 
 * @par 示例用法:
 * @code
 * dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
 * dsa_iterator_t *begin_iter = linked_list_begin(list);
 * dsa_iterator_t *end_iter = linked_list_end(list);
 * 
 * while (!iterator_equals(begin_iter, end_iter)) {
 *     // 处理元素
 *     iterator_next(begin_iter);
 * }
 * 
 * iterator_destroy(begin_iter);
 * iterator_destroy(end_iter);
 * @endcode
 */
dsa_iterator_t *linked_list_end(const dsa_linked_list_t *linked_list);

/** @} */ // LinkedListIterator

#endif // DSA_LINKED_LIST_ITERATOR_H
