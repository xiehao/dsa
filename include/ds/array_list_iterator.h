/**
 * @file array_list_iterator.h
 * @brief 数组列表迭代器接口定义
 * @details 提供了数组列表的迭代器操作接口，支持静态数组和动态数组
 * @author DSA项目组
 * @date 2025-07-24
 * @version 1.0
 */

#ifndef DSA_ARRAY_LIST_ITERATOR_H
#define DSA_ARRAY_LIST_ITERATOR_H

#include <a/iterator.h>
#include <ds/array_list.h>

/**
 * @defgroup ArrayListIterator 数组列表迭代器
 * @brief 数组列表的迭代器操作接口
 * @{
 */

/**
 * @brief 创建指向数组列表开始位置的迭代器
 *
 * @param array 指向数组列表的常量指针
 * @return 指向数组列表第一个元素的迭代器，如果数组为空或无效则返回NULL
 * 
 * @note 调用者负责在不再需要时调用 iterator_destroy() 来释放迭代器内存
 * @note 支持静态数组和动态数组
 * @see iterator_destroy()
 * 
 * @par 示例用法:
 * @code
 * dsa_array_list_t *arr = array_list_create_static(buffer, 10, sizeof(int));
 * dsa_iterator_t *iter = array_list_begin(arr);
 * while (iterator_is_valid(iter)) {
 *     int *value = (int *)iterator_get_value(iter);
 *     printf("%d ", *value);
 *     iterator_next(iter);
 * }
 * iterator_destroy(iter);
 * @endcode
 */
dsa_iterator_t *array_list_begin(const dsa_array_list_t *array);

/**
 * @brief 创建指向数组列表结束位置的迭代器
 *
 * @param array 指向数组列表的常量指针
 * @return 指向数组列表末尾（最后一个元素之后）的迭代器，如果数组无效则返回NULL
 * 
 * @note 调用者负责在不再需要时调用 iterator_destroy() 来释放迭代器内存
 * @note 支持静态数组和动态数组
 * @note end迭代器不指向有效元素，主要用于边界检查
 * @see iterator_destroy()
 * 
 * @par 示例用法:
 * @code
 * dsa_array_list_t *arr = array_list_create_static(buffer, 10, sizeof(int));
 * dsa_iterator_t *begin_iter = array_list_begin(arr);
 * dsa_iterator_t *end_iter = array_list_end(arr);
 * 
 * while (!iterator_equals(begin_iter, end_iter)) {
 *     // 处理元素
 *     iterator_next(begin_iter);
 * }
 * 
 * iterator_destroy(begin_iter);
 * iterator_destroy(end_iter);
 * @endcode
 */
dsa_iterator_t *array_list_end(const dsa_array_list_t *array);

/** @} */ // ArrayListIterator

#endif // DSA_ARRAY_LIST_ITERATOR_H
