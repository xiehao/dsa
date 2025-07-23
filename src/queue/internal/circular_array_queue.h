/**
 * @file circular_array_queue.h
 * @brief 循环数组队列内部接口声明
 * @details 声明了循环数组队列的创建函数，供队列工厂使用
 * @author DSA Team
 * @date 2025-07-22
 * @version 1.0
 */

#ifndef DSA_CIRCULAR_ARRAY_QUEUE_H
#define DSA_CIRCULAR_ARRAY_QUEUE_H

#include <adt/queue.h>

/**
 * @brief 创建循环数组队列实例
 * @return 成功返回新创建的队列指针，失败返回NULL
 * @note 此函数为内部接口，通常通过queue_create()调用
 * @details 创建基于循环数组的队列实现，具有良好的缓存局部性和高效的随机访问性能
 */
dsa_queue_t *circular_array_queue_create(void);

#endif // DSA_CIRCULAR_ARRAY_QUEUE_H
