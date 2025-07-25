/**
 * @file linked_list_iterator_internal.h
 * @brief 链表迭代器内部结构定义
 * @details 定义了链表迭代器的内部实现结构，仅供内部使用
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#ifndef DSA_LINKED_LIST_ITERATOR_INTERNAL_H
#define DSA_LINKED_LIST_ITERATOR_INTERNAL_H

#include <stddef.h>
#include <stdbool.h>
#include <traits.h>
#include <ds/iterator.h>
#include <ds/linked_list.h>

/**
 * @brief 链表迭代器类型枚举
 * @details 用于区分不同类型的链表迭代器实现
 */
typedef enum {
    LINKED_LIST_ITERATOR_TYPE_SINGLY,   ///< 单链表迭代器
    LINKED_LIST_ITERATOR_TYPE_DOUBLY,   ///< 双链表迭代器
    LINKED_LIST_ITERATOR_TYPE_CIRCULAR, ///< 循环链表迭代器
} dsa_linked_list_iterator_type_t;

/**
 * @brief 链表迭代器内部结构体
 * @details 基于节点指针的链表迭代器实现，支持O(1)移动
 * @note 此结构体定义仅供内部使用，外部代码不应直接访问
 */
typedef struct {
    trait_iterator_t const *trait;                         ///< 迭代器接口
    dsa_const_container_pt container;                      ///< 指向容器的常量指针
    void *current_node;                                    ///< 当前节点指针
    void *end_marker;                                      ///< 结束标记（对于不同链表类型有不同含义）
    dsa_linked_list_iterator_type_t iterator_type;         ///< 迭代器类型
    size_t version;                                        ///< 迭代器版本号，用于失效检测
} linked_list_iterator_t;

#endif // DSA_LINKED_LIST_ITERATOR_INTERNAL_H
