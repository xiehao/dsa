/**
 * @file linked_list.c
 * @brief 链表数据结构的实现
 * @details 提供链表的基本操作，包括创建、插入、删除、查找等功能
 * @author DSA Team
 * @date 2025-06-28
 */

#include "ds/linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <common.h>
#include <internal/linked_list_traits.h>
#include <internal/singly_linked.h>

/**
 * @brief 链表结构体定义
 * @details 包含链表特性接口的指针，用于实现多态
 */
struct linked_list_t {
    trait_linked_list_t const *traits; /**< 链表特性接口指针 */
};

/**
 * @brief 创建指定类型的链表
 * @param type 链表类型
 * @return 成功返回链表指针，失败返回NULL
 * @see dsa_linked_list_type_t
 * @note 目前只实现了单向链表类型
 */
dsa_linked_list_t *linked_list_create(dsa_linked_list_type_t type) {
    switch (type) {
        case LINKED_LIST_TYPE_SINGLY:
            return singly_linked_create();
        case LINKED_LIST_TYPE_DOUBLY:
        // return NULL;
        case LINKED_LIST_TYPE_CIRCULAR:
            // return NULL;
        default:
            return NULL;
    }
}

/**
 * @brief 获取链表的大小
 * @param linked_list 链表指针
 * @return 链表中元素的数量，如果链表为NULL则返回0
 * @pre linked_list 可以为NULL
 */
size_t linked_list_size(const dsa_linked_list_t *linked_list) {
    return linked_list ? linked_list->traits->basic->get_size(linked_list) : 0;
}

/**
 * @brief 检查链表是否为空
 * @param linked_list 链表指针
 * @return 如果链表为空或NULL则返回true，否则返回false
 * @pre linked_list 可以为NULL
 */
bool linked_list_is_empty(const dsa_linked_list_t *linked_list) {
    return linked_list ? linked_list->traits->basic->is_empty(linked_list) : true;
}

/**
 * @brief 清空链表中的所有元素
 * @param linked_list 链表指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * @pre linked_list 不能为NULL
 */
dsa_result_t linked_list_clear(dsa_linked_list_t *linked_list) {
    if (!linked_list) { return DSA_ERROR_NULL_POINTER; }
    return linked_list->traits->basic->clear(linked_list);
}

/**
 * @brief 销毁链表并释放内存
 * @param linked_list 链表指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * @pre linked_list 不能为NULL
 * @warning 调用此函数后，链表指针将变为无效
 */
dsa_result_t linked_list_destroy(dsa_linked_list_t *linked_list) {
    if (!linked_list) { return DSA_ERROR_NULL_POINTER; }
    return linked_list->traits->basic->destroy(linked_list);
}

/**
 * @brief 获取指定索引位置的元素
 * @param linked_list 链表指针
 * @param index 元素索引位置
 * @return 成功返回元素指针，失败返回NULL
 * @pre linked_list 不能为NULL
 * @pre index 必须在有效范围内
 */
dsa_element_pt linked_list_get(const dsa_linked_list_t *linked_list, size_t index) {
    if (!linked_list) { return NULL; }
    return linked_list->traits->random_access->get_at(linked_list, index);
}

/**
 * @brief 设置指定索引位置的元素值
 * @param linked_list 链表指针
 * @param index 元素索引位置
 * @param data 要设置的数据指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * @retval DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引超出范围
 * @pre linked_list 不能为NULL
 * @pre index 必须在有效范围内
 */
dsa_result_t linked_list_set(dsa_linked_list_t *linked_list, size_t index, dsa_element_pt data) {
    if (!linked_list) { return DSA_ERROR_NULL_POINTER; }
    return linked_list->traits->random_access->set_at(linked_list, index, data);
}

/**
 * @brief 在指定索引位置插入元素
 * @param linked_list 链表指针
 * @param index 插入位置的索引
 * @param data 要插入的数据指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * @retval DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引超出范围
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 * @pre linked_list 不能为NULL
 * @pre index 必须在有效范围内（0到size）
 */
dsa_result_t linked_list_insert_at(dsa_linked_list_t *linked_list, size_t index, dsa_element_pt data) {
    if (!linked_list) { return DSA_ERROR_NULL_POINTER; }
    return linked_list->traits->random_access->insert_at(linked_list, index, data);
}

/**
 * @brief 移除指定索引位置的元素
 * @param linked_list 链表指针
 * @param index 要移除元素的索引位置
 * @return 成功返回被移除的元素指针，失败返回NULL
 * @pre linked_list 不能为NULL
 * @pre index 必须在有效范围内
 * @note 调用者负责释放返回的元素内存
 */
dsa_element_pt linked_list_remove_at(dsa_linked_list_t *linked_list, size_t index) {
    if (!linked_list) { return NULL; }
    return linked_list->traits->random_access->remove_at(linked_list, index);
}