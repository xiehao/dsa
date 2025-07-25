/**
 * @file iterator_internal.h
 * @brief 迭代器内部结构定义
 * @details 定义了迭代器的内部实现结构，仅供内部使用
 * @author DSA项目组
 * @date 2025-07-24
 * @version 1.0
 */

#ifndef DSA_ITERATOR_INTERNAL_H
#define DSA_ITERATOR_INTERNAL_H

#include <traits.h>
#include <a/iterator.h>

/**
 * @brief 通用迭代器结构体
 * @details 所有迭代器的基础结构，包含trait接口指针
 * @note 此结构体定义仅供内部使用，外部代码不应直接访问
 */
struct dsa_iterator_t {
    trait_iterator_t const *trait;  ///< 迭代器接口指针
};

#endif // DSA_ITERATOR_INTERNAL_H
