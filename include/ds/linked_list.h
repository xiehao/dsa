/**
 * @file linked_list.h
 * @brief 链表数据结构的接口定义
 * @author DSA项目组
 * @date 2025-07-01
 * @version 1.0
 * 
 * 本文件定义了通用链表数据结构的API，支持单向链表、双向链表和循环链表三种类型。
 * 提供了创建、销毁、插入、删除、查询等基本操作。
 */

#ifndef DSA_LINKED_LIST_H
#define DSA_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include <common.h>

/**
 * @brief 链表的不透明指针类型
 * 
 * 使用不透明指针隐藏链表的内部实现细节，提供更好的封装性。
 * 用户只能通过提供的API函数来操作链表，无法直接访问内部结构。
 */
typedef struct linked_list_t dsa_linked_list_t;

/**
 * @brief 链表类型枚举
 * 
 * 定义了支持的链表类型，用于在创建链表时指定具体的实现方式。
 */
typedef enum {
    LINKED_LIST_TYPE_SINGLY,   /**< 单向链表 */
    LINKED_LIST_TYPE_DOUBLY,   /**< 双向链表 */
    LINKED_LIST_TYPE_CIRCULAR, /**< 循环链表 */
} dsa_linked_list_type_t;

/**
 * @brief 创建一个新的链表
 *
 * 根据指定的类型创建一个新的链表实例。新创建的链表为空，
 * 用户需要调用相应的插入函数来添加元素。
 *
 * @param type 链表类型，可以是单向、双向或循环链表
 * @return 指向新创建的链表的指针，如果内存分配失败则返回NULL
 * 
 * @note 调用者负责在不再需要时调用 linked_list_destroy() 来释放内存
 * @see linked_list_destroy()
 */
dsa_linked_list_t *linked_list_create(dsa_linked_list_type_t type);

/**
 * @brief 获取链表中元素的个数
 *
 * @param linked_list 指向链表的指针
 * @return 链表中元素的个数，如果链表为NULL则返回0
 * 
 * @pre linked_list != NULL
 */
size_t linked_list_size(const dsa_linked_list_t *linked_list);

/**
 * @brief 检查链表是否为空
 *
 * @param linked_list 指向链表的指针
 * @return 如果链表为空或链表指针为NULL则返回true，否则返回false
 * 
 * @pre linked_list != NULL
 */
bool linked_list_is_empty(const dsa_linked_list_t *linked_list);

/**
 * @brief 清空链表中的所有元素
 *
 * 移除链表中的所有元素，但保留链表结构本身。
 * 清空后的链表可以继续使用。
 *
 * @param linked_list 指向要清空的链表的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 清空成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * 
 * @pre linked_list != NULL
 * @post linked_list_is_empty(linked_list) == true
 */
dsa_result_t linked_list_clear(dsa_linked_list_t *linked_list);

/**
 * @brief 销毁链表并释放所有相关内存
 *
 * 完全销毁链表，释放所有节点和链表结构本身占用的内存。
 * 销毁后的链表指针不能再使用。
 *
 * @param linked_list 指向要销毁的链表的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 销毁成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * 
 * @pre linked_list != NULL
 * @post linked_list指针不再有效
 * 
 * @warning 调用此函数后，linked_list指针将失效，不能再使用
 */
dsa_result_t linked_list_destroy(dsa_linked_list_t *linked_list);

/**
 * @brief 获取链表中指定索引处的元素
 *
 * 返回指定索引位置的元素，但不从链表中移除该元素。
 * 索引从0开始计数。
 *
 * @param linked_list 指向链表的指针
 * @param index 要获取元素的索引位置（从0开始）
 * @return 指向指定索引处数据的指针，如果索引越界或链表为NULL则返回NULL
 * 
 * @pre linked_list != NULL
 * @pre index < linked_list_size(linked_list)
 * 
 * @note 返回的指针指向链表内部存储的数据，修改返回的数据会影响链表内容
 */
dsa_element_pt linked_list_get(const dsa_linked_list_t *linked_list, size_t index);

/**
 * @brief 设置链表中指定索引处的数据
 *
 * 将指定索引位置的元素替换为新的数据。原有数据会被覆盖，
 * 但不会被此函数自动释放。
 *
 * @param linked_list 指向链表的指针
 * @param index 要设置数据的索引位置
 * @param data 指向新数据的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 设置成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * @retval DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引越界
 * 
 * @pre linked_list != NULL
 * @pre index < linked_list_size(linked_list)
 * 
 * @warning 此函数不会释放被覆盖的原数据，调用者需要自行管理内存
 */
dsa_result_t linked_list_set(dsa_linked_list_t *linked_list, size_t index, dsa_element_pt data);

/**
 * @brief 在链表的指定索引处插入一个元素
 *
 * 在指定的索引位置插入新元素，原有元素会向后移动。
 * 如果索引等于链表长度，则在末尾插入。
 *
 * @param linked_list 指向链表的指针
 * @param index 要插入元素的索引位置（从0开始）
 * @param data 指向要插入数据的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 插入成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * @retval DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引越界
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 * 
 * @pre linked_list != NULL
 * @pre index <= linked_list_size(linked_list)
 * @post linked_list_size(linked_list) 增加1
 */
dsa_result_t linked_list_insert_at(dsa_linked_list_t *linked_list, size_t index, dsa_element_pt data);

/**
 * @brief 从链表的指定索引处移除并返回元素
 *
 * 移除指定索引位置的元素并返回其数据指针。
 * 后续元素会向前移动以填补空隙。
 *
 * @param list 指向链表的指针
 * @param index 要移除元素的索引位置（从0开始）
 * @return 指向被移除元素数据的指针，如果索引越界或链表为空则返回NULL
 * 
 * @pre list != NULL
 * @pre index < linked_list_size(list)
 * @post linked_list_size(list) 减少1
 * 
 * @note 调用者负责释放返回的数据指针所指向的内存（如果需要的话）
 */
dsa_element_pt linked_list_remove_at(dsa_linked_list_t *list, size_t index);

/**
 * @brief 在链表的开头添加一个元素
 *
 * 在链表的第一个位置插入新元素，原有元素向后移动。
 * 这是一个O(1)时间复杂度的操作。
 *
 * @param list 指向链表的指针
 * @param data 指向要添加数据的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 添加成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 * 
 * @pre list != NULL
 * @post linked_list_size(list) 增加1
 */
dsa_result_t linked_list_push_front(dsa_linked_list_t* list, dsa_element_pt data);

/**
 * @brief 在链表的末尾添加一个元素
 *
 * 在链表的最后位置添加新元素。对于单向链表，
 * 这可能是O(n)操作，对于双向链表是O(1)操作。
 *
 * @param list 指向链表的指针
 * @param data 指向要添加数据的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 添加成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 * 
 * @pre list != NULL
 * @post linked_list_size(list) 增加1
 */
dsa_result_t linked_list_push_back(dsa_linked_list_t* list, dsa_element_pt data);

/**
 * @brief 从链表的开头移除并返回元素
 *
 * 移除并返回链表的第一个元素。这是一个O(1)时间复杂度的操作。
 *
 * @param list 指向链表的指针
 * @return 指向被移除元素数据的指针，如果链表为空则返回NULL
 * 
 * @pre list != NULL
 * @post 如果链表不为空，linked_list_size(list) 减少1
 * 
 * @note 调用者负责释放返回的数据指针所指向的内存（如果需要的话）
 */
dsa_element_pt linked_list_pop_front(dsa_linked_list_t* list);

/**
 * @brief 从链表的末尾移除并返回元素
 *
 * 移除并返回链表的最后一个元素。对于单向链表，
 * 这可能是O(n)操作，对于双向链表是O(1)操作。
 *
 * @param list 指向链表的指针
 * @return 指向被移除元素数据的指针，如果链表为空则返回NULL
 * 
 * @pre list != NULL
 * @post 如果链表不为空，linked_list_size(list) 减少1
 * 
 * @note 调用者负责释放返回的数据指针所指向的内存（如果需要的话）
 */
dsa_element_pt linked_list_pop_back(dsa_linked_list_t* list);

#endif // DSA_LINKED_LIST_H
