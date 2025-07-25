/**
 * @file doubly_linked_list.h
 * @brief 双向链表数据结构的头文件
 * @author oaheixiehao
 * @date 2025/7/1
 * @version 1.0
 *
 * 本文件定义了双向链表数据结构的接口函数。
 * 双向链表是一种线性数据结构，其中每个节点包含数据和两个指针，
 * 分别指向前一个节点和后一个节点。
 */

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <ds/linked_list.h>
#include <internal/linked_list_traits.h>

/**
 * @brief 双链表节点结构
 * @details 双向链表的基本节点，包含指向前一个和后一个节点的指针，以及数据指针
 */
typedef struct doubly_node_t {
    struct doubly_node_t *previous; /**< 指向前一个节点的指针 */
    struct doubly_node_t *next; /**< 指向后一个节点的指针 */
    dsa_element_pt data; /**< 节点存储的数据指针 */
} doubly_node_t;

/**
 * @brief 双向链表结构
 * @details 双向链表的主要结构，包含特征表、哨兵节点和大小信息
 */
typedef struct {
    trait_linked_list_t const *traits; /**< 链表特征函数表指针 */
    doubly_node_t *head; /**< 头哨兵节点指针 */
    doubly_node_t *tail; /**< 尾哨兵节点指针 */
    size_t size; /**< 链表中元素的数量 */
} doubly_linked_t;

/**
 * @brief 创建一个新的双向链表
 *
 * 该函数用于创建并初始化一个新的双向链表实例。
 * 新创建的链表为空，不包含任何节点。
 *
 * @return dsa_linked_list_t* 指向新创建的双向链表的指针
 * @retval NULL 如果内存分配失败
 * @retval 非NULL 指向有效双向链表结构的指针
 *
 * @note 调用者有责任在不再需要链表时调用相应的销毁函数来释放内存
 * @warning 返回的指针必须进行NULL检查
 *
 * @par 示例用法:
 * @code
 * dsa_linked_list_t *list = doubly_linked_create();
 * if (list != NULL) {
 *     // 使用链表
 *     // ...
 *     // 记得释放内存
 * }
 * @endcode
 *
 * @see doubly_linked_destroy() 用于销毁链表
 * @see doubly_linked_insert() 用于插入节点
 * @see doubly_linked_delete() 用于删除节点
 */
dsa_linked_list_t *doubly_linked_create(void);

#endif // DOUBLY_LINKED_LIST_H
