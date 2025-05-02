#ifndef DSA_DYNAMIC_ARRAY_H
#define DSA_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

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
 * @return 如果元素添加成功返回true，否则返回false（例如，分配失败）
 */
bool dynamic_array_add(DynamicArray* array, void* element);

/**
 * @brief 获取指定索引处的元素
 *
 * @param array 指向DynamicArray的指针
 * @param index 要检索的元素的索引
 * @return 指向指定索引处元素的指针，如果索引越界则返回NULL
 */
void* dynamic_array_get(const DynamicArray* array, size_t index);

/**
 * @brief 设置指定索引处的元素
 *
 * @param array 指向DynamicArray的指针
 * @param index 要设置元素的索引
 * @param element 指向新元素的指针。该索引处的旧元素会被覆盖但不会被释放
 * @return 指向被替换元素的指针，如果索引越界或发生错误则返回NULL
 */
void* dynamic_array_set(DynamicArray* array, size_t index, void* element);

/**
 * @brief 在动态数组的指定索引处插入一个元素
 *
 * 如果需要，数组会自动调整大小。索引 `index` 必须在 `[0, size]` 范围内。
 * 如果 `index == size`，则效果等同于 `dynamic_array_add`。
 * @param array 指向DynamicArray的指针
 * @param index 要插入元素的索引
 * @param element 指向要插入的元素的指针
 * @return 如果元素插入成功返回true，否则返回false（例如，索引无效、分配失败）
 */
bool dynamic_array_insert(DynamicArray* array, size_t index, void* element);

/**
 * @brief 移除并返回动态数组的最后一个元素
 *
 * @param array 指向DynamicArray的指针
 * @return 指向被移除元素的指针，如果数组为空则返回NULL
 */
void* dynamic_array_pop_back(DynamicArray* array);

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

#endif // DSA_DYNAMIC_ARRAY_H