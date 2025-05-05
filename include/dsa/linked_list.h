#ifndef DSA_LINKED_LIST_H
#define DSA_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include <dsa/common.h> // 包含通用定义

// 链表的不透明指针类型
typedef struct LinkedList LinkedList;

// 用于释放用户提供的数据的函数指针类型定义
typedef void (*FreeDataFunc)(ElementPtr data);

/**
 * @brief 创建一个新的链表。
 *
 * @return 指向新创建的 LinkedList 的指针，如果分配失败则返回 NULL。
 */
LinkedList* linked_list_create();

/**
 * @brief 销毁链表并释放其关联的内存。
 *
 * 如果提供了 `free_data_func`，它将在释放节点本身之前
 * 对每个元素的数据调用。
 *
 * @param list 指向要销毁的 LinkedList 的指针。
 * @param free_data_func 用于释放每个节点中存储的数据的函数指针。如果数据释放由其他地方处理或不需要，则可以为 NULL。
 */
void linked_list_destroy(LinkedList* list, FreeDataFunc free_data_func);

/**
 * @brief 在链表的开头添加一个元素。
 *
 * @param list 指向 LinkedList 的指针。
 * @param data 指向要添加的数据的指针。
 * @return 如果元素添加成功则返回 True，否则返回 false（例如，分配失败）。
 */
bool linked_list_add_first(LinkedList* list, ElementPtr data);

/**
 * @brief 在链表的末尾添加一个元素。
 *
 * @param list 指向 LinkedList 的指针。
 * @param data 指向要添加的数据的指针。
 * @return 如果元素添加成功则返回 True，否则返回 false（例如，分配失败）。
 */
bool linked_list_add_last(LinkedList* list, ElementPtr data);

/**
 * @brief 在链表的指定索引处插入一个元素。
 *
 * @param list 指向 LinkedList 的指针。
 * @param index 要插入元素的索引（从 0 开始）。
 * @param data 指向要插入的数据的指针。
 * @return 如果元素插入成功则返回 True，否则返回 false（例如，索引越界或分配失败）。
 */
bool linked_list_insert(LinkedList* list, size_t index, ElementPtr data);

/**
 * @brief 从链表的开头移除并返回元素。
 *
 * @param list 指向 LinkedList 的指针。
 * @return 指向被移除元素数据的指针，如果列表为空则返回 NULL。
 *         如果需要，调用者负责释放返回的数据。
 */
ElementPtr linked_list_remove_first(LinkedList* list);

/**
 * @brief 从链表的末尾移除并返回元素。
 *
 * @param list 指向 LinkedList 的指针。
 * @return 指向被移除元素数据的指针，如果列表为空则返回 NULL。
 *         如果需要，调用者负责释放返回的数据。
 */
ElementPtr linked_list_remove_last(LinkedList* list);

/**
 * @brief 从链表的指定索引处移除并返回元素。
 *
 * @param list 指向 LinkedList 的指针。
 * @param index 要移除元素的索引（从 0 开始）。
 * @return 指向被移除元素数据的指针，如果索引越界或列表为空则返回 NULL。
 *         如果需要，调用者负责释放返回的数据。
 */
ElementPtr linked_list_remove(LinkedList* list, size_t index);

/**
 * @brief 获取链表中指定索引处的元素但不移除它。
 *
 * @param list 指向 LinkedList 的指针。
 * @param index 要检索元素的索引（从 0 开始）。
 * @return 指向指定索引处数据的指针，如果索引越界则返回 NULL。
 */
ElementPtr linked_list_get(const LinkedList* list, size_t index);

/**
 * @brief Sets the data at the specified index.
 *
 * @param list A pointer to the LinkedList.
 * @param index The index to set the data at.
 * @param data A pointer to the new data. The old data at this index is overwritten but not freed by this function.
 * @return True if the data was set successfully, false if the index is out of bounds.
 */
bool linked_list_set(LinkedList* list, size_t index, ElementPtr data);

/**
 * @brief 获取链表中的元素数量。
 *
 * @param list 指向 LinkedList 的指针。
 * @return 列表中的元素数量。
 */
size_t linked_list_size(const LinkedList* list);

/**
 * @brief 检查链表是否为空。
 *
 * @param list 指向 LinkedList 的指针。
 * @return 如果列表为空则返回 True，否则返回 false。
 */
bool linked_list_is_empty(const LinkedList* list);

#endif // DSA_LINKED_LIST_H