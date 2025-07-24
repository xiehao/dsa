/**
 * @file static_array_list.h
 * @brief 静态数组数据结构的头文件
 * @author DSA项目组
 * @date 2025-06-17
 * @version 1.0
 */

#ifndef DSA_STATIC_ARRAY_LIST_H
#define DSA_STATIC_ARRAY_LIST_H

#include <stddef.h>
#include <ds/array_list.h>
#include <internal/array_list_traits.h>

/**
 * @brief 静态数组内部结构体
 * @details 基于预分配缓冲区的静态数组容器，容量固定不可变
 * @note 此结构体定义仅供内部使用，外部代码不应直接访问
 */
typedef struct {
    trait_array_list_t const *trait;       ///< 数组接口
    dsa_element_pt data;                   ///< 指向实际数据存储的指针 (通常是 VLA)
    size_t size;                           ///< 当前元素数量
    size_t capacity;                       ///< 数组的总容量 (固定)
    size_t element_size;                   ///< 单个元素的大小（字节）
} static_array_t;

/**
 * @brief 创建一个静态数组
 *
 * 该函数使用提供的数据缓冲区创建一个静态数组，数组的容量和元素大小由参数指定。
 * 静态数组使用预分配的内存空间，不会进行动态内存分配。
 *
 * @param data_buffer 指向数据缓冲区的指针，用于存储数组元素
 * @param capacity 数组的最大容量（元素个数）
 * @param element_size 每个元素的大小（字节数）
 *
 * @return dsa_array_list_t* 成功时返回指向创建的数组结构的指针，失败时返回NULL
 *
 * @pre data_buffer不能为NULL
 * @pre capacity必须大于0
 * @pre element_size必须大于0
 *
 * @note 调用者负责确保data_buffer有足够的空间（至少capacity * element_size字节）
 * @note 返回的数组指针需要适当的清理，建议使用相应的销毁函数
 *
 * @warning 传入的data_buffer必须在数组的整个生命周期内保持有效
 *
 * @see dsa_array_list_t
 *
 * @par 示例用法:
 * @code
 * int buffer[100];
 * dsa_array_list_t *arr = static_array_create(buffer, 100, sizeof(int));
 * if (arr != NULL) {
 *     // 使用数组
 * }
 * @endcode
 */
dsa_array_list_t *static_array_create(void *data_buffer, size_t capacity, size_t element_size);

#endif // DSA_STATIC_ARRAY_LIST_H
