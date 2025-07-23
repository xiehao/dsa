/**
 * @file circular_array_queue.c
 * @brief 基于循环数组的队列实现
 * @details 本文件实现了基于循环数组的队列数据结构，提供FIFO（先进先出）操作。
 *          循环数组队列具有高效的内存访问模式、良好的缓存局部性，适用于性能敏感的场景。
 * @author DSA Team
 * @date 2025-07-22
 * @version 1.0
 */

#include <stdlib.h>
#include <stdio.h>
#include "internal/circular_array_queue.h"
#include <adt/deque.h>
#include "internal/queue_traits.h"

/**
 * @struct circular_array_queue_t
 * @brief 循环数组队列结构体
 * @details 封装了队列的特性接口和底层循环数组双端队列数据结构
 */
typedef struct {
    trait_queue_t const *trait;    ///< 队列特性接口指针
    dsa_deque_t *data;             ///< 底层循环数组双端队列数据结构
} circular_array_queue_t;

/**
 * @brief 获取循环数组队列中的元素数量
 * @param queue 队列容器指针
 * @return 队列中的元素数量，如果队列为NULL则返回0
 * @note 时间复杂度：O(1)，循环数组维护了元素计数
 */
static size_t circular_array_queue_size(dsa_const_container_pt queue) {
    circular_array_queue_t const *this = queue;
    return this ? deque_size(this->data) : 0;
}

/**
 * @brief 检查循环数组队列是否为空
 * @param queue 队列容器指针
 * @return 如果队列为空或NULL则返回true，否则返回false
 * @note 时间复杂度：O(1)
 */
static bool circular_array_queue_is_empty(dsa_const_container_pt queue) {
    circular_array_queue_t const *this = queue;
    return this ? deque_is_empty(this->data) : true;
}

/**
 * @brief 清空循环数组队列中的所有元素
 * @param queue 队列容器指针
 * @return 成功返回DSA_SUCCESS，失败返回DSA_ERROR_NULL_POINTER
 * @note 时间复杂度：O(n)，需要清理所有元素
 */
static dsa_result_t circular_array_queue_clear(dsa_container_pt queue) {
    circular_array_queue_t *this = queue;
    return this ? deque_clear(this->data) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 销毁循环数组队列并释放所有内存
 * @param queue 队列容器指针
 * @return 成功返回DSA_SUCCESS，失败返回DSA_ERROR_NULL_POINTER
 * @note 时间复杂度：O(n)，需要释放数组和队列结构本身
 * @warning 调用此函数后，队列指针将变为无效，不应再使用
 */
static dsa_result_t circular_array_queue_destroy(dsa_container_pt queue) {
    circular_array_queue_t *this = queue;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    dsa_result_t result = deque_destroy(this->data);
    free(this);
    return result;
}

/**
 * @brief 循环数组队列的基本操作接口
 * @details 定义了队列的基本操作函数指针，包括大小查询、空状态检查、清空和销毁操作
 */
static trait_basic_t const basic_trait = {
    .get_size = circular_array_queue_size,
    .is_empty = circular_array_queue_is_empty,
    .clear = circular_array_queue_clear,
    .destroy = circular_array_queue_destroy,
};

/**
 * @brief 将元素入队到循环数组队列的尾部
 * @param queue 队列容器指针
 * @param element 要入队的元素指针
 * @return 成功返回DSA_SUCCESS，失败返回DSA_ERROR_NULL_POINTER
 * @note 时间复杂度：O(1)平摊，循环数组支持高效的尾部插入
 * @details 元素将被添加到队列的尾部，遵循FIFO原则。当数组满时会自动扩容
 */
static dsa_result_t circular_array_queue_enqueue(dsa_container_pt queue, dsa_element_pt element) {
    circular_array_queue_t *this = queue;
    return this ? deque_add_last(this->data, element) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 从循环数组队列的头部出队元素
 * @param queue 队列容器指针
 * @return 队列头部的元素指针，如果队列为空或NULL则返回NULL
 * @note 时间复杂度：O(1)，循环数组支持常数时间的头部删除
 * @details 返回并移除队列头部的元素，遵循FIFO原则
 * @warning 调用者负责释放返回元素的内存（如果需要）
 */
static dsa_element_pt circular_array_queue_dequeue(dsa_container_pt queue) {
    circular_array_queue_t *this = queue;
    return this ? deque_remove_first(this->data) : NULL;
}

/**
 * @brief 循环数组队列的线性操作接口
 * @details 定义了队列的入队和出队操作函数指针
 */
static trait_linear_t const linear_trait = {
    .push_back = circular_array_queue_enqueue,
    .pop_front = circular_array_queue_dequeue,
};

/**
 * @brief 获取循环数组队列的类型标识
 * @return 返回QUEUE_TYPE_CIRCULAR_ARRAY_LIST类型枚举值
 */
static dsa_queue_type_t circular_array_queue_get_type(void) {
    return QUEUE_TYPE_CIRCULAR_ARRAY_LIST;
}

/**
 * @brief 获取循环数组队列的类型名称
 * @return 返回队列类型的中文描述字符串
 */
static char const *circular_array_queue_get_type_name(void) {
    return "循环队列 (Circular Queue)";
}

/**
 * @brief 循环数组队列的完整特性接口
 * @details 组合了基本操作、线性操作和类型信息的完整接口
 */
static trait_queue_t const queue_trait = {
    .basic = &basic_trait,
    .linear = &linear_trait,
    .get_type = circular_array_queue_get_type,
    .get_type_name = circular_array_queue_get_type_name,
};

/**
 * @brief 创建一个新的循环数组队列
 * @return 成功返回新创建的队列指针，失败返回NULL
 * @note 时间复杂度：O(1)
 * @details 分配队列结构内存并初始化底层循环数组，队列初始为空状态。
 *          循环数组具有良好的缓存局部性和高效的随机访问性能。
 * @warning 使用完毕后需要调用circular_array_queue_destroy释放内存
 *
 * @par 使用示例:
 * @code
 * dsa_queue_t *queue = circular_array_queue_create();
 * if (queue) {
 *     // 使用队列...
 *     queue_destroy(queue);
 * }
 * @endcode
 *
 * @par 性能特点:
 * - 入队操作：O(1)平摊时间复杂度
 * - 出队操作：O(1)时间复杂度
 * - 空间复杂度：O(n)，其中n为元素数量
 * - 内存局部性：优秀，适合缓存友好的访问模式
 */
dsa_queue_t *circular_array_queue_create(void) {
    circular_array_queue_t *queue = malloc(sizeof(circular_array_queue_t));
    if (!queue) {
        fprintf(stderr, "错误：无法为循环队列分配内存\n");
        return NULL;
    }
    queue->data = deque_create(DEQUE_TYPE_CIRCULAR_ARRAY_LIST);
    if (!queue->data) {
        fprintf(stderr, "错误：无法为底层循环数组分配内存\n");
        free(queue);
        return NULL;
    }
    queue->trait = &queue_trait;
    return (dsa_queue_t *) queue;
}
