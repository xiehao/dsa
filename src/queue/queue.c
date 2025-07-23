/**
 * @file queue.c
 * @brief 队列抽象数据类型的统一接口实现
 * @details 本文件提供了队列ADT的统一接口，支持多种底层实现（循环数组和双向链表）。
 *          通过工厂模式和特性接口，实现了不同队列实现的统一访问。
 * @author DSA Team
 * @date 2025-07-22
 * @version 1.0
 */

#include <adt/queue.h>
#include "internal/queue_traits.h"
#include "internal/circular_array_queue.h"
#include "internal/doubly_linked_queue.h"

/**
 * @struct queue_t
 * @brief 队列抽象数据类型结构体
 * @details 封装了队列的特性接口指针，实现多态行为
 */
struct queue_t {
    trait_queue_t const *traits;    ///< 队列特性接口指针
};

/**
 * @brief 创建指定类型的队列
 * @param type 队列类型枚举值
 * @return 成功返回新创建的队列指针，失败返回NULL
 * @note 时间复杂度：O(1)
 * @details 根据指定类型创建相应的队列实现：
 *          - QUEUE_TYPE_CIRCULAR_ARRAY_LIST: 创建循环数组队列
 *          - QUEUE_TYPE_DOUBLY_LINKED_LIST: 创建双向链表队列
 *
 * @par 使用示例:
 * @code
 * // 创建循环数组队列
 * dsa_queue_t *array_queue = queue_create(QUEUE_TYPE_CIRCULAR_ARRAY_LIST);
 *
 * // 创建双向链表队列
 * dsa_queue_t *linked_queue = queue_create(QUEUE_TYPE_DOUBLY_LINKED_LIST);
 * @endcode
 */
dsa_queue_t *queue_create(dsa_queue_type_t type) {
    switch (type) {
        case QUEUE_TYPE_CIRCULAR_ARRAY_LIST:
            return circular_array_queue_create();
        case QUEUE_TYPE_DOUBLY_LINKED_LIST:
            return doubly_linked_queue_create();
        default:
            return NULL;
    }
}

/**
 * @brief 获取队列中的元素数量
 * @param queue 队列指针
 * @return 队列中的元素数量，如果队列为NULL则返回0
 * @note 时间复杂度取决于具体实现：循环数组为O(1)，双向链表为O(n)
 */
size_t queue_size(dsa_queue_t const *queue) {
    return queue ? queue->traits->basic->get_size(queue) : 0;
}

/**
 * @brief 检查队列是否为空
 * @param queue 队列指针
 * @return 如果队列为空或NULL则返回true，否则返回false
 * @note 时间复杂度：O(1)
 */
bool queue_is_empty(dsa_queue_t const *queue) {
    return queue ? queue->traits->basic->is_empty(queue) : true;
}

/**
 * @brief 清空队列中的所有元素
 * @param queue 队列指针
 * @return 成功返回DSA_SUCCESS，失败返回相应错误码
 * @note 时间复杂度：O(n)，需要释放所有元素
 */
dsa_result_t queue_clear(dsa_queue_t *queue) {
    return queue ? queue->traits->basic->clear(queue) : DSA_SUCCESS;
}

/**
 * @brief 销毁队列并释放所有内存
 * @param queue 队列指针
 * @return 成功返回DSA_SUCCESS，失败返回相应错误码
 * @note 时间复杂度：O(n)，需要释放所有元素和队列结构
 * @warning 调用此函数后，队列指针将变为无效，不应再使用
 */
dsa_result_t queue_destroy(dsa_queue_t *queue) {
    return queue ? queue->traits->basic->destroy(queue) : DSA_SUCCESS;
}

/**
 * @brief 将元素入队到队列尾部
 * @param queue 队列指针
 * @param element 要入队的元素指针
 * @return 成功返回DSA_SUCCESS，失败返回DSA_ERROR_NULL_POINTER
 * @note 时间复杂度：循环数组为O(1)平摊，双向链表为O(1)
 * @details 元素将被添加到队列的尾部，遵循FIFO（先进先出）原则
 */
dsa_result_t queue_enqueue(dsa_queue_t *queue, dsa_element_pt element) {
    return queue ? queue->traits->linear->push_back(queue, element) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 从队列头部出队元素
 * @param queue 队列指针
 * @return 队列头部的元素指针，如果队列为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 * @details 返回并移除队列头部的元素，遵循FIFO（先进先出）原则
 * @warning 调用者负责释放返回元素的内存（如果需要）
 */
dsa_element_pt queue_dequeue(dsa_queue_t *queue) {
    return queue ? queue->traits->linear->pop_front(queue) : NULL;
}

/**
 * @brief 获取队列的类型标识
 * @param queue 队列指针
 * @return 队列类型枚举值，如果队列为NULL则返回默认类型
 */
dsa_queue_type_t queue_get_type(dsa_queue_t const *queue) {
    return queue ? queue->traits->get_type() : QUEUE_TYPE_CIRCULAR_ARRAY_LIST;
}

/**
 * @brief 获取队列的类型名称
 * @param queue 队列指针
 * @return 队列类型的描述字符串，如果队列为NULL则返回"未知队列类型"
 */
char const *queue_get_type_name(dsa_queue_t const *queue) {
    return queue ? queue->traits->get_type_name() : "未知队列类型";
}