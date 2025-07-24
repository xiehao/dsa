/**
 * @file array_iterator_internal.h
 * @brief 数组迭代器内部结构定义
 * @details 定义了数组迭代器的内部实现结构，仅供内部使用
 * @author DSA项目组
 * @date 2025-07-24
 * @version 1.0
 */

#ifndef DSA_ARRAY_ITERATOR_INTERNAL_H
#define DSA_ARRAY_ITERATOR_INTERNAL_H

#include <stddef.h>
#include <stdbool.h>
#include <traits.h>
#include <ds/iterator.h>
#include <ds/array_list.h>

/**
 * @brief 数组迭代器类型枚举
 * @details 用于区分不同类型的数组迭代器实现
 */
typedef enum {
    ARRAY_ITERATOR_TYPE_STATIC,     ///< 静态数组迭代器
    ARRAY_ITERATOR_TYPE_DYNAMIC,    ///< 动态数组迭代器
} dsa_array_iterator_type_t;

/**
 * @brief 数组迭代器内部结构体
 * @details 基于索引的数组迭代器实现，支持O(1)访问和移动
 * @note 此结构体定义仅供内部使用，外部代码不应直接访问
 */
typedef struct {
    trait_iterator_t const *trait;             ///< 迭代器接口
    dsa_const_container_pt container;          ///< 指向容器的常量指针
    size_t current_index;                      ///< 当前迭代器位置索引
    size_t container_size;                     ///< 容器大小（缓存以提高性能）
    dsa_array_iterator_type_t iterator_type;   ///< 迭代器类型
} array_iterator_t;



#endif // DSA_ARRAY_ITERATOR_INTERNAL_H
