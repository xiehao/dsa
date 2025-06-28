#ifndef DSA_LINKED_LIST_H
#define DSA_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include <common.h>

// 链表的不透明指针类型
typedef struct linked_list_t dsa_linked_list_t;

typedef enum {
    LINKED_LIST_TYPE_SINGLY,
    LINKED_LIST_TYPE_DOUBLY,
    LINKED_LIST_TYPE_CIRCULAR,
} dsa_linked_list_type_t;

/**
 * @brief 创建一个新的链表。
 *
 * @return 指向新创建的 LinkedList 的指针，如果分配失败则返回 NULL。
 */
dsa_linked_list_t *linked_list_create(dsa_linked_list_type_t type);

size_t linked_list_size(const dsa_linked_list_t *linked_list);

bool linked_list_is_empty(const dsa_linked_list_t *linked_list);

dsa_result_t linked_list_clear(dsa_linked_list_t *linked_list);

/**
 * @brief 销毁链表并释放其关联的内存。
 *
 * 如果提供了 `free_data_func`，它将在释放节点本身之前
 * 对每个元素的数据调用。
 *
 * @param linked_list 指向要销毁的 LinkedList 的指针。
 */
dsa_result_t linked_list_destroy(dsa_linked_list_t *linked_list);

/**
 * @brief 获取链表中指定索引处的元素但不移除它。
 *
 * @param linked_list 指向 LinkedList 的指针。
 * @param index 要检索元素的索引（从 0 开始）。
 * @return 指向指定索引处数据的指针，如果索引越界则返回 NULL。
 */
dsa_element_pt linked_list_get(const dsa_linked_list_t *linked_list, size_t index);

/**
 * @brief Sets the data at the specified index.
 *
 * @param linked_list A pointer to the LinkedList.
 * @param index The index to set the data at.
 * @param data A pointer to the new data. The old data at this index is overwritten but not freed by this function.
 * @return True if the data was set successfully, false if the index is out of bounds.
 */
dsa_result_t linked_list_set(dsa_linked_list_t *linked_list, size_t index, dsa_element_pt data);

/**
 * @brief 在链表的指定索引处插入一个元素。
 *
 * @param linked_list 指向 LinkedList 的指针。
 * @param index 要插入元素的索引（从 0 开始）。
 * @param data 指向要插入的数据的指针。
 * @return 如果元素插入成功则返回 True，否则返回 false（例如，索引越界或分配失败）。
 */
dsa_result_t linked_list_insert_at(dsa_linked_list_t *linked_list, size_t index, dsa_element_pt data);

/**
 * @brief 从链表的指定索引处移除并返回元素。
 *
 * @param list 指向 LinkedList 的指针。
 * @param index 要移除元素的索引（从 0 开始）。
 * @return 指向被移除元素数据的指针，如果索引越界或列表为空则返回 NULL。
 *         如果需要，调用者负责释放返回的数据。
 */
dsa_element_pt linked_list_remove_at(dsa_linked_list_t *list, size_t index);

// /**
//  * @brief 在链表的开头添加一个元素。
//  *
//  * @param list 指向 LinkedList 的指针。
//  * @param data 指向要添加的数据的指针。
//  * @return 如果元素添加成功则返回 True，否则返回 false（例如，分配失败）。
//  */
// bool linked_list_add_first(dsa_linked_list_t* list, dsa_element_pt data);
//
// /**
//  * @brief 在链表的末尾添加一个元素。
//  *
//  * @param list 指向 LinkedList 的指针。
//  * @param data 指向要添加的数据的指针。
//  * @return 如果元素添加成功则返回 True，否则返回 false（例如，分配失败）。
//  */
// bool linked_list_add_last(dsa_linked_list_t* list, dsa_element_pt data);
//
// /**
//  * @brief 从链表的开头移除并返回元素。
//  *
//  * @param list 指向 LinkedList 的指针。
//  * @return 指向被移除元素数据的指针，如果列表为空则返回 NULL。
//  *         如果需要，调用者负责释放返回的数据。
//  */
// dsa_element_pt linked_list_remove_first(dsa_linked_list_t* list);
//
// /**
//  * @brief 从链表的末尾移除并返回元素。
//  *
//  * @param list 指向 LinkedList 的指针。
//  * @return 指向被移除元素数据的指针，如果列表为空则返回 NULL。
//  *         如果需要，调用者负责释放返回的数据。
//  */
// dsa_element_pt linked_list_remove_last(dsa_linked_list_t* list);

#endif // DSA_LINKED_LIST_H
