#ifndef DSA_DYNAMIC_ARRAY_H
#define DSA_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdbool.h>
#include <dsa/common.h> // 包含通用定义

// 动态数组的不透明指针类型
typedef struct DynamicArray DynamicArray;

/**
 * @brief 创建一个新的动态数组
 *
 * @param initial_capacity 数组的初始容量。如果为0，将使用默认容量
 * @return 指向新创建的DynamicArray的指针，如果分配失败则返回NULL
 */
DynamicArray* dynamic_array_create(size_t initial_capacity);

/**
 * @brief 销毁动态数组并释放其关联的内存
 *
 * @param array 指向要销毁的DynamicArray的指针
 */
void dynamic_array_destroy(DynamicArray* array);

/**
 * @brief 销毁动态数组并释放其所有元素
 *
 * 此函数首先释放数组中每个元素指向的内存，
 * 然后释放数组本身使用的内存。
 * 只有当数组存储的是堆分配的指针时才应使用此函数。
 *
 * @param array 指向要销毁的DynamicArray的指针
 */
void dynamic_array_destroy_with_free(DynamicArray* array);

/**
 * @brief 在动态数组末尾添加一个元素
 *
 * 如果需要，数组会自动调整大小
 * @param array 指向DynamicArray的指针
 * @param element 指向要添加的元素的指针。数组存储指针而不是副本
 * @return DSA_SUCCESS 如果元素添加成功；
 *         DSA_ERROR_NULL_POINTER 如果array或element为NULL；
 *         DSA_ERROR_MEMORY_ALLOCATION 如果内存分配失败
 */
dsa_result_t dynamic_array_push_back(DynamicArray* array, dsa_element_pt element);

/**
 * @brief 获取指定索引处的元素
 *
 * @param array 指向DynamicArray的指针
 * @param index 要检索的元素的索引
 * @return 指向指定索引处元素的指针，如果索引越界则返回NULL
 */
dsa_element_pt dynamic_array_get(const DynamicArray* array, size_t index);

/**
 * @brief 设置指定索引处的元素
 *
 * @param array 指向DynamicArray的指针
 * @param index 要设置元素的索引
 * @param element 指向新元素的指针。该索引处的旧元素会被覆盖但不会被释放
 * @return DSA_SUCCESS 如果元素设置成功；
 *         DSA_ERROR_NULL_POINTER 如果array或element为NULL；
 *         DSA_ERROR_INDEX_OUT_OF_BOUNDS 如果索引越界
 */
dsa_result_t dynamic_array_set(DynamicArray* array, size_t index, dsa_element_pt element);

/**
 * @brief 在动态数组的指定索引处插入一个元素
 *
 * 如果需要，数组会自动调整大小。索引 `index` 必须在 `[0, size]` 范围内。
 * 如果 `index == size`，则效果等同于 `dynamic_array_push_back`。
 * @param array 指向DynamicArray的指针
 * @param index 要插入元素的索引
 * @param element 指向要插入的元素的指针
 * @return DSA_SUCCESS 如果元素插入成功；
 *         DSA_ERROR_NULL_POINTER 如果array或element为NULL；
 *         DSA_ERROR_INDEX_OUT_OF_BOUNDS 如果索引无效；
 *         DSA_ERROR_MEMORY_ALLOCATION 如果内存分配失败。
 */
dsa_result_t dynamic_array_insert(DynamicArray* array, size_t index, dsa_element_pt element);

/**
 * @brief 从动态数组的指定索引处移除一个元素
 *
 * 索引 `index` 必须在 `[0, size - 1]` 范围内。
 * 后续元素将向前移动以填补空缺。
 *
 * @param array 指向DynamicArray的指针
 * @param index 要移除元素的索引
 * @return 指向被移除元素的指针，如果索引无效或数组为空则返回NULL
 */
dsa_element_pt dynamic_array_remove(DynamicArray* array, size_t index);

/**
 * @brief 移除并返回动态数组的最后一个元素
 *
 * @param array 指向DynamicArray的指针
 * @return 指向被移除元素的指针，如果数组为空则返回NULL
 */
dsa_element_pt dynamic_array_pop_back(DynamicArray* array);

/**
 * @brief 获取动态数组中当前的元素数量
 *
 * @param array 指向DynamicArray的指针
 * @return 数组中的元素数量
 */
size_t dynamic_array_size(const DynamicArray* array);

/**
 * @brief 获取动态数组的当前容量
 *
 * @param array 指向DynamicArray的指针
 * @return 数组的当前容量
 */
size_t dynamic_array_capacity(const DynamicArray* array);

/**
 * @brief 检查动态数组是否为空
 *
 * @param array 指向DynamicArray的指针
 * @return 如果数组为空返回true，否则返回false
 */
bool dynamic_array_is_empty(const DynamicArray* array);

/**
 * @brief 清空动态数组但保留容量
 *
 * 此函数将数组大小重置为0，但不释放元素指向的内存，
 * 也不释放数组的存储空间。数组可以继续使用。
 *
 * @param array 指向要清空的DynamicArray的指针
 */
void dynamic_array_clear(DynamicArray* array);

/**
 * @brief 清空动态数组并释放所有元素
 *
 * 此函数首先释放数组中每个元素指向的内存，
 * 然后将数组大小重置为0。数组结构本身保留，可以继续使用。
 *
 * @param array 指向要清空的DynamicArray的指针
 */
void dynamic_array_clear_with_free(DynamicArray* array);

#endif // DSA_DYNAMIC_ARRAY_H