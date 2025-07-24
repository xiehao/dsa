/**
 * @file dynamic_array_list.h
 * @brief 动态数组数据结构的头文件
 * @author dsa项目团队
 * @date 2025-06-16
 * @version 1.0
 */

#ifndef DSA_DYNAMIC_ARRAY_LIST_H
#define DSA_DYNAMIC_ARRAY_LIST_H

#include <stddef.h>
#include <ds/array_list.h>
#include <internal/array_list_traits.h>

/**
 * @defgroup DynamicArray 动态数组
 * @brief 动态数组数据结构实现
 * @{
 */

/**
 * @brief 动态数组内部结构体
 * @details 可动态调整大小的数组容器，支持自动扩容
 * @note 此结构体定义仅供内部使用，外部代码不应直接访问
 */
typedef struct {
    trait_array_list_t const *trait;       ///< 数组接口
    dsa_element_pt *data;                  ///< 指向元素指针数组的指针
    size_t size;                           ///< 当前元素数量
    size_t capacity;                       ///< 当前分配的容量
} dynamic_array_t;

/**
 * @brief 创建一个新的动态数组
 *
 * 此函数分配内存并初始化一个新的动态数组结构。
 * 动态数组可以根据需要自动调整大小。
 *
 * @param initial_capacity 数组的初始容量。如果为0，将使用默认容量
 * @return 指向新创建的DynamicArray的指针，如果分配失败则返回NULL
 *
 * @note 返回的指针需要通过相应的销毁函数释放内存
 * @warning 调用者有责任检查返回值是否为NULL
 *
 * @see dynamic_array_destroy()
 * @since 1.0
 */
dsa_array_list_t *dynamic_array_create(size_t initial_capacity);

/**
 * @}
 */

#endif // DSA_DYNAMIC_ARRAY_LIST_H
