/**
 * @file deque.h
 * @brief 双端队列抽象数据类型的接口定义
 * @details 提供了双端队列(Deque)数据结构的统一接口，支持循环数组双端队列和双向链表双端队列两种实现。
 *          双端队列支持在两端进行高效的插入和删除操作，结合了栈和队列的特性。
 * @author DSA项目组
 * @date 2025-07-22
 * @version 1.0
 */

#ifndef DSA_DEQUE_H
#define DSA_DEQUE_H

#include <stdbool.h>
#include <stddef.h>
#include <common.h>

/**
 * @brief 双端队列的不透明指针类型
 *
 * 使用不透明指针隐藏双端队列的内部实现细节，提供更好的封装性。
 * 用户只能通过提供的API函数来操作双端队列，无法直接访问内部结构。
 */
typedef struct deque_t dsa_deque_t;

/**
 * @brief 双端队列类型枚举
 *
 * 定义了支持的双端队列实现类型，用于在创建双端队列时指定具体的实现方式。
 */
typedef enum {
    DEQUE_TYPE_CIRCULAR_ARRAY_LIST, /**< 循环数组双端队列：基于循环数组实现，内存局部性好 */
    DEQUE_TYPE_DOUBLY_LINKED_LIST,  /**< 双向链表双端队列：基于双向链表实现，动态内存分配 */
} dsa_deque_type_t;

/**
 * @brief 创建一个新的双端队列
 *
 * 根据指定的类型创建一个新的双端队列实例。新创建的双端队列为空，
 * 用户需要调用相应的插入函数来添加元素。
 *
 * @param type 双端队列类型，可以是循环数组双端队列或双向链表双端队列
 * @return 指向新创建的双端队列的指针，如果内存分配失败则返回NULL
 *
 * @note 调用者负责在不再需要时调用 deque_destroy() 来释放内存
 * @see deque_destroy()
 */
dsa_deque_t *deque_create(dsa_deque_type_t type);

/**
 * @brief 获取双端队列中元素的个数
 *
 * @param deque 指向双端队列的指针
 * @return 双端队列中元素的个数，如果双端队列为NULL则返回0
 *
 * @pre deque != NULL
 */
size_t deque_size(dsa_deque_t const *deque);

/**
 * @brief 检查双端队列是否为空
 *
 * @param deque 指向双端队列的指针
 * @return 如果双端队列为空或双端队列指针为NULL则返回true，否则返回false
 *
 * @pre deque != NULL
 */
bool deque_is_empty(dsa_deque_t const *deque);

/**
 * @brief 清空双端队列中的所有元素
 *
 * 移除双端队列中的所有元素，但保留双端队列结构本身。
 * 清空后的双端队列可以继续使用。
 *
 * @param deque 指向要清空的双端队列的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 清空成功
 * @retval DSA_ERROR_NULL_POINTER 双端队列指针为NULL
 *
 * @pre deque != NULL
 * @post deque_is_empty(deque) == true
 */
dsa_result_t deque_clear(dsa_deque_t *deque);

/**
 * @brief 销毁双端队列并释放所有相关内存
 *
 * 完全销毁双端队列，释放所有节点和双端队列结构本身占用的内存。
 * 销毁后的双端队列指针不能再使用。
 *
 * @param deque 指向要销毁的双端队列的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 销毁成功
 * @retval DSA_ERROR_NULL_POINTER 双端队列指针为NULL
 *
 * @pre deque != NULL
 * @post deque指针不再有效
 *
 * @warning 调用此函数后，deque指针将失效，不能再使用
 * @note 此函数不会释放双端队列中存储的元素，调用者负责管理元素本身的内存
 */
dsa_result_t deque_destroy(dsa_deque_t *deque);

/**
 * @brief 在双端队列的前端添加元素
 *
 * 将新元素添加到双端队列的前端（头部）。
 * 对于循环数组双端队列，如果容量不足会自动扩容。
 *
 * @param deque 指向双端队列的指针
 * @param element 指向要添加的元素的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 添加成功
 * @retval DSA_ERROR_NULL_POINTER 双端队列指针或元素指针为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 *
 * @pre deque != NULL
 * @pre element != NULL
 * @post deque_size(deque) 增加1
 */
dsa_result_t deque_add_first(dsa_deque_t *deque, dsa_element_pt element);

/**
 * @brief 在双端队列的后端添加元素
 *
 * 将新元素添加到双端队列的后端（尾部）。
 * 对于循环数组双端队列，如果容量不足会自动扩容。
 *
 * @param deque 指向双端队列的指针
 * @param element 指向要添加的元素的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 添加成功
 * @retval DSA_ERROR_NULL_POINTER 双端队列指针或元素指针为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 *
 * @pre deque != NULL
 * @pre element != NULL
 * @post deque_size(deque) 增加1
 */
dsa_result_t deque_add_last(dsa_deque_t *deque, dsa_element_pt element);

/**
 * @brief 从双端队列的前端移除并返回元素
 *
 * 移除并返回双端队列前端（头部）的元素。
 *
 * @param deque 指向双端队列的指针
 * @return 指向被移除元素数据的指针，如果双端队列为空则返回NULL
 *
 * @pre deque != NULL
 * @post 如果双端队列不为空，deque_size(deque) 减少1
 *
 * @note 调用者负责释放返回的数据指针所指向的内存（如果需要的话）
 */
dsa_element_pt deque_remove_first(dsa_deque_t *deque);

/**
 * @brief 从双端队列的后端移除并返回元素
 *
 * 移除并返回双端队列后端（尾部）的元素。
 *
 * @param deque 指向双端队列的指针
 * @return 指向被移除元素数据的指针，如果双端队列为空则返回NULL
 *
 * @pre deque != NULL
 * @post 如果双端队列不为空，deque_size(deque) 减少1
 *
 * @note 调用者负责释放返回的数据指针所指向的内存（如果需要的话）
 */
dsa_element_pt deque_remove_last(dsa_deque_t *deque);

/**
 * @brief 查看双端队列前端元素但不移除它
 *
 * 返回双端队列前端（头部）元素的指针，但不从双端队列中移除该元素。
 * 用于查看前端内容而不改变双端队列的状态。
 *
 * @param deque 指向双端队列的指针
 * @return 指向前端元素的指针，如果双端队列为空则返回NULL
 *
 * @pre deque != NULL
 * @note 返回的指针指向双端队列内部存储的数据，修改返回的数据会影响双端队列内容
 */
dsa_element_pt deque_peek_first(dsa_deque_t const *deque);

/**
 * @brief 查看双端队列后端元素但不移除它
 *
 * 返回双端队列后端（尾部）元素的指针，但不从双端队列中移除该元素。
 * 用于查看后端内容而不改变双端队列的状态。
 *
 * @param deque 指向双端队列的指针
 * @return 指向后端元素的指针，如果双端队列为空则返回NULL
 *
 * @pre deque != NULL
 * @note 返回的指针指向双端队列内部存储的数据，修改返回的数据会影响双端队列内容
 */
dsa_element_pt deque_peek_last(dsa_deque_t const *deque);

/**
 * @brief 获取双端队列类型
 *
 * @param deque 指向双端队列的指针
 * @return 双端队列类型枚举值，如果双端队列为NULL则返回默认类型
 *
 * @pre deque != NULL
 */
dsa_deque_type_t deque_get_type(dsa_deque_t const *deque);

/**
 * @brief 获取双端队列类型名称
 *
 * @param deque 指向双端队列的指针
 * @return 双端队列类型的字符串描述，如果双端队列为NULL则返回"Unknown"
 *
 * @pre deque != NULL
 */
char const *deque_get_type_name(dsa_deque_t const *deque);

#endif // DSA_DEQUE_H
