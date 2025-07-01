/**
 * @file singly_linked.h
 * @brief 单向链表操作函数声明
 * @author oaheixiehao
 * @date 2025/6/23
 */

#ifndef SINGLY_LINKED_H
#define SINGLY_LINKED_H

#include <ds/linked_list.h>

/**
 * @brief 创建一个新的单向链表
 *
 * 该函数用于创建一个新的单向链表实例，并返回指向该链表的指针。
 * 新创建的链表为空链表，需要调用者负责后续的内存管理。
 *
 * @return dsa_linked_list_t* 指向新创建的单向链表的指针，如果创建失败则返回NULL
 *
 * @note 调用者需要确保在不再使用链表时释放相关内存
 * @see dsa_linked_list_t
 *
 * @par 示例:
 * @code
 * dsa_linked_list_t *list = singly_linked_create();
 * if (list != NULL) {
 *     // 使用链表...
 *     // 记得释放内存
 * }
 * @endcode
 */
dsa_linked_list_t *singly_linked_create();

#endif // SINGLY_LINKED_H