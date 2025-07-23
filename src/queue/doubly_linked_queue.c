/**
 * @file doubly_linked_queue.c
 * @brief 基于双向链表的队列实现
 * @details 本文件实现了基于双向链表的队列数据结构，提供FIFO（先进先出）操作。
 *          双向链表队列具有动态内存分配、无容量限制的特点，适用于数据量不确定的场景。
 * @author DSA Team
 * @date 2025-07-22
 * @version 1.0
 */

#include <stdlib.h>
#include <stdio.h>
#include "internal/doubly_linked_queue.h"
#include "internal/queue_traits.h"
#include <ds/linked_list.h>

/**
 * @struct doubly_linked_queue_t
 * @brief 双向链表队列结构体
 * @details 封装了队列的特性接口和底层双向链表数据结构
 */
typedef struct {
    trait_queue_t const *trait;    ///< 队列特性接口指针
    dsa_linked_list_t *data;       ///< 底层双向链表数据结构
} doubly_linked_queue_t;

/**
 * @brief 获取双向链表队列中的元素数量
 * @param queue 队列容器指针
 * @return 队列中的元素数量，如果队列为NULL则返回0
 * @note 时间复杂度：O(n)，因为需要遍历链表计算长度
 */
static size_t doubly_linked_queue_size(dsa_const_container_pt queue) {
    doubly_linked_queue_t const *this = queue;
    return this ? linked_list_size(this->data) : 0;
}

/**
 * @brief 检查双向链表队列是否为空
 * @param queue 队列容器指针
 * @return 如果队列为空或NULL则返回true，否则返回false
 * @note 时间复杂度：O(1)
 */
static bool doubly_linked_queue_is_empty(dsa_const_container_pt queue) {
    doubly_linked_queue_t const *this = queue;
    return this ? linked_list_is_empty(this->data) : true;
}

/**
 * @brief 清空双向链表队列中的所有元素
 * @param queue 队列容器指针
 * @return 成功返回DSA_SUCCESS，失败返回DSA_ERROR_NULL_POINTER
 * @note 时间复杂度：O(n)，需要释放所有节点
 */
static dsa_result_t doubly_linked_queue_clear(dsa_container_pt queue) {
    doubly_linked_queue_t *this = queue;
    return this ? linked_list_clear(this->data) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 销毁双向链表队列并释放所有内存
 * @param queue 队列容器指针
 * @return 成功返回DSA_SUCCESS，失败返回DSA_ERROR_NULL_POINTER
 * @note 时间复杂度：O(n)，需要释放所有节点和队列结构本身
 * @warning 调用此函数后，队列指针将变为无效，不应再使用
 */
static dsa_result_t doubly_linked_queue_destroy(dsa_container_pt queue) {
    doubly_linked_queue_t *this = queue;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    dsa_result_t result = linked_list_destroy(this->data);
    free(this);
    return result;
}

/**
 * @brief 双向链表队列的基本操作接口
 * @details 定义了队列的基本操作函数指针，包括大小查询、空状态检查、清空和销毁操作
 */
static trait_basic_t const basic_trait = {
    .get_size = doubly_linked_queue_size,
    .is_empty = doubly_linked_queue_is_empty,
    .clear = doubly_linked_queue_clear,
    .destroy = doubly_linked_queue_destroy,
};

/**
 * @brief 将元素入队到双向链表队列的尾部
 * @param queue 队列容器指针
 * @param element 要入队的元素指针
 * @return 成功返回DSA_SUCCESS，失败返回DSA_ERROR_NULL_POINTER
 * @note 时间复杂度：O(1)，双向链表支持常数时间的尾部插入
 * @details 元素将被添加到队列的尾部，遵循FIFO原则
 */
static dsa_result_t doubly_linked_queue_enqueue(dsa_container_pt queue, dsa_element_pt element) {
    doubly_linked_queue_t *this = queue;
    return this ? linked_list_push_back(this->data, element) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 从双向链表队列的头部出队元素
 * @param queue 队列容器指针
 * @return 队列头部的元素指针，如果队列为空或NULL则返回NULL
 * @note 时间复杂度：O(1)，双向链表支持常数时间的头部删除
 * @details 返回并移除队列头部的元素，遵循FIFO原则
 * @warning 调用者负责释放返回元素的内存（如果需要）
 */
static dsa_element_pt doubly_linked_queue_dequeue(dsa_container_pt queue) {
    doubly_linked_queue_t *this = queue;
    return this ? linked_list_pop_front(this->data) : NULL;
}

/**
 * @brief 双向链表队列的线性操作接口
 * @details 定义了队列的入队和出队操作函数指针
 */
static trait_linear_t const linear_trait = {
    .push_back = doubly_linked_queue_enqueue,
    .pop_front = doubly_linked_queue_dequeue,
};

/**
 * @brief 获取双向链表队列的类型标识
 * @return 返回QUEUE_TYPE_DOUBLY_LINKED_LIST类型枚举值
 */
static dsa_queue_type_t doubly_linked_queue_get_type(void) {
    return QUEUE_TYPE_DOUBLY_LINKED_LIST;
}

/**
 * @brief 获取双向链表队列的类型名称
 * @return 返回队列类型的中文描述字符串
 */
static char const *doubly_linked_queue_get_type_name(void) {
    return "双链队列 (Doubly Linked Queue)";
}

/**
 * @brief 双向链表队列的完整特性接口
 * @details 组合了基本操作、线性操作和类型信息的完整接口
 */
static trait_queue_t const queue_trait = {
    .basic = &basic_trait,
    .linear = &linear_trait,
    .get_type = doubly_linked_queue_get_type,
    .get_type_name = doubly_linked_queue_get_type_name,
};

/**
 * @brief 创建一个新的双向链表队列
 * @return 成功返回新创建的队列指针，失败返回NULL
 * @note 时间复杂度：O(1)
 * @details 分配队列结构内存并初始化底层双向链表，队列初始为空状态
 * @warning 使用完毕后需要调用doubly_linked_queue_destroy释放内存
 *
 * @par 使用示例:
 * @code
 * dsa_queue_t *queue = doubly_linked_queue_create();
 * if (queue) {
 *     // 使用队列...
 *     queue_destroy(queue);
 * }
 * @endcode
 */
dsa_queue_t *doubly_linked_queue_create(void) {
    doubly_linked_queue_t *queue = malloc(sizeof(doubly_linked_queue_t));
    if (!queue) {
        fprintf(stderr, "错误：无法为双链队列分配内存\n");
        return NULL;
    }
    queue->data = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    if (!queue->data) {
        fprintf(stderr, "错误：无法为底层双链表分配内存\n");
        free(queue);
        return NULL;
    }
    queue->trait = &queue_trait;
    return (dsa_queue_t *) queue;
}
