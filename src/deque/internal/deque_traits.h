/**
 * @file deque_traits.h
 * @brief 双端队列特性接口定义
 * @details 定义了双端队列数据结构的特性接口，用于实现不同双端队列实现的统一访问
 * @author DSA项目组
 * @date 2025-07-22
 * @version 1.0
 */

#ifndef DSA_DEQUE_TRAITS_H
#define DSA_DEQUE_TRAITS_H

#include <traits.h>
#include <adt/deque.h>

/**
 * @struct trait_deque_t
 * @brief 双端队列特性接口结构体
 * @details 定义了双端队列数据结构的完整接口，包括基本操作、线性操作和类型信息
 */
typedef struct {
    trait_basic_t const *basic;              ///< 基本容器操作接口（大小、空状态、清空、销毁）
    trait_linear_t const *linear;            ///< 线性容器操作接口（前端/后端插入删除）
    dsa_deque_type_t (*get_type)(void);      ///< 获取双端队列类型的函数指针
    char const *(*get_type_name)(void);      ///< 获取双端队列类型名称的函数指针
} trait_deque_t;

#endif // DSA_DEQUE_TRAITS_H
