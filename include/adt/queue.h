/**
 * @file queue.h
 * @brief 队列抽象数据类型的接口定义
 * @details 提供了队列(Queue)数据结构的统一接口，支持循环数组队列和双向链表队列两种实现。
 *          队列遵循先进先出(FIFO)的访问原则，支持入队、出队等基本操作。
 * @author DSA项目组
 * @date 2025-07-22
 * @version 1.0
 */

#ifndef DSA_QUEUE_H
#define DSA_QUEUE_H

#include <common.h>

/**
 * @brief 队列的不透明指针类型
 *
 * 使用不透明指针隐藏队列的内部实现细节，提供更好的封装性。
 * 用户只能通过提供的API函数来操作队列，无法直接访问内部结构。
 */
typedef struct queue_t dsa_queue_t;

/**
 * @brief 队列类型枚举
 *
 * 定义了支持的队列实现类型，用于在创建队列时指定具体的实现方式。
 */
typedef enum {
    QUEUE_TYPE_CIRCULAR_ARRAY_LIST, /**< 循环数组队列：基于循环数组实现，内存局部性好 */
    QUEUE_TYPE_DOUBLY_LINKED_LIST,  /**< 双向链表队列：基于双向链表实现，动态内存分配 */
} dsa_queue_type_t;

/**
 * @brief 创建一个新的队列
 *
 * 根据指定的类型创建一个新的队列实例。新创建的队列为空，
 * 用户需要调用相应的入队函数来添加元素。
 *
 * @param queue_type 队列类型，可以是循环数组队列或双向链表队列
 * @return 指向新创建的队列的指针，如果内存分配失败则返回NULL
 *
 * @note 调用者负责在不再需要时调用 queue_destroy() 来释放内存
 * @see queue_destroy()
 */
dsa_queue_t *queue_create(dsa_queue_type_t queue_type);

/**
 * @brief 获取队列中元素的个数
 *
 * @param queue 指向队列的指针
 * @return 队列中元素的个数，如果队列为NULL则返回0
 *
 * @pre queue != NULL
 */
size_t queue_size(dsa_queue_t const *queue);

/**
 * @brief 检查队列是否为空
 *
 * @param queue 指向队列的指针
 * @return 如果队列为空或队列指针为NULL则返回true，否则返回false
 *
 * @pre queue != NULL
 */
bool queue_is_empty(dsa_queue_t const *queue);

/**
 * @brief 清空队列中的所有元素
 *
 * 移除队列中的所有元素，但保留队列结构本身。
 * 清空后的队列可以继续使用。
 *
 * @param queue 指向要清空的队列的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 清空成功
 * @retval DSA_ERROR_NULL_POINTER 队列指针为NULL
 *
 * @pre queue != NULL
 * @post queue_is_empty(queue) == true
 */
dsa_result_t queue_clear(dsa_queue_t *queue);

/**
 * @brief 销毁队列并释放所有相关内存
 *
 * 完全销毁队列，释放所有节点和队列结构本身占用的内存。
 * 销毁后的队列指针不能再使用。
 *
 * @param queue 指向要销毁的队列的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 销毁成功
 * @retval DSA_ERROR_NULL_POINTER 队列指针为NULL
 *
 * @pre queue != NULL
 * @post queue指针不再有效
 *
 * @warning 调用此函数后，queue指针将失效，不能再使用
 */
dsa_result_t queue_destroy(dsa_queue_t *queue);

/**
 * @brief 将元素加入队列尾部（入队操作）
 *
 * 将新元素添加到队列的尾部，遵循FIFO（先进先出）原则。
 * 对于循环数组队列，如果容量不足会自动扩容。
 *
 * @param queue 指向队列的指针
 * @param element 指向要入队的元素的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 入队成功
 * @retval DSA_ERROR_NULL_POINTER 队列指针或元素指针为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 *
 * @pre queue != NULL
 * @pre element != NULL
 * @post queue_size(queue) 增加1
 */
dsa_result_t queue_enqueue(dsa_queue_t *queue, dsa_element_pt element);

/**
 * @brief 从队列头部移除并返回元素（出队操作）
 *
 * 移除并返回队列头部的元素，遵循FIFO（先进先出）原则。
 * 最先入队的元素将最先出队。
 *
 * @param queue 指向队列的指针
 * @return 指向被移除元素数据的指针，如果队列为空则返回NULL
 *
 * @pre queue != NULL
 * @post 如果队列不为空，queue_size(queue) 减少1
 *
 * @note 调用者负责释放返回的数据指针所指向的内存（如果需要的话）
 */
dsa_element_pt queue_dequeue(dsa_queue_t *queue);

/**
 * @brief 获取队列类型
 *
 * @param queue 指向队列的指针
 * @return 队列类型枚举值，如果队列为NULL则返回默认类型
 *
 * @pre queue != NULL
 */
dsa_queue_type_t queue_get_type(dsa_queue_t const *queue);

/**
 * @brief 获取队列类型名称
 *
 * @param queue 指向队列的指针
 * @return 队列类型的字符串描述，如果队列为NULL则返回"Unknown"
 *
 * @pre queue != NULL
 */
char const *queue_get_type_name(dsa_queue_t const *queue);

#endif // DSA_QUEUE_H
