/**
 * @file queue_traits.h
 * @brief 队列特性接口定义
 * @details 定义了队列数据结构的特性接口，用于实现不同队列实现的统一访问
 * @author DSA Team
 * @date 2025-07-22
 * @version 1.0
 */

#ifndef DSA_QUEUE_TRAITS_H
#define DSA_QUEUE_TRAITS_H

#include <traits.h>
#include <adt/queue.h>

/**
 * @struct trait_queue_t
 * @brief 队列特性接口结构体
 * @details 定义了队列数据结构的完整接口，包括基本操作、线性操作和类型信息
 */
typedef struct {
    trait_basic_t const *basic;              ///< 基本容器操作接口（大小、空状态、清空、销毁）
    trait_linear_t const *linear;            ///< 线性容器操作接口（入队、出队）
    dsa_queue_type_t (*get_type)(void);      ///< 获取队列类型的函数指针
    char const *(*get_type_name)(void);      ///< 获取队列类型名称的函数指针
} trait_queue_t;

#endif // DSA_QUEUE_TRAITS_H
