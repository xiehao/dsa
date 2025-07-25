//
// Created by oaheixiehao on 2025/7/2.
//

#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

#include <ds/linked_list.h>
#include <internal/linked_list_traits.h>

/**
 * @brief 循环链表节点结构
 * @details 定义了循环链表的基本节点结构，包含数据指针和下一个节点的指针
 */
typedef struct circular_node_t {
    struct circular_node_t *next; /**< 指向下一个节点的指针 */
    dsa_element_pt data; /**< 存储的数据元素指针 */
} circular_node_t;

/**
 * @brief 循环链表结构体
 * @details 定义了循环链表的主体结构，包含特性指针、头节点和大小信息
 */
typedef struct {
    trait_linked_list_t const *traits; /**< 链表特性接口指针 */
    circular_node_t *head; /**< 头节点指针（哨兵节点） */
    size_t size; /**< 链表中元素的数量 */
} circular_linked_t;

/**
 * @brief 创建一个新的循环链表
 *
 * 该函数用于创建一个新的循环链表实例，并返回指向该链表的指针。
 * 新创建的链表为空链表，需要调用者负责后续的内存管理。
 *
 * @return dsa_linked_list_t* 指向新创建的循环链表的指针，如果创建失败则返回NULL
 *
 * @note 调用者需要确保在不再使用链表时释放相关内存
 * @see dsa_linked_list_t
 */
dsa_linked_list_t *circular_linked_list_create(void);

#endif //CIRCULAR_LINKED_LIST_H
