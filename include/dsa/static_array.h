#ifndef DSA_STATIC_ARRAY_H
#define DSA_STATIC_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

// 定义静态数组结构
// 使用 C99 的 VLA (Variable Length Array) 特性，在栈上分配
// 注意：此结构体本身不直接包含数组，而是指向在栈上分配的数组
// 实际使用时，通常结合 alloca 或直接在函数作用域内声明 VLA
typedef struct {
    void *data;         // 指向实际数据存储的指针 (通常是 VLA)
    size_t size;        // 当前元素数量
    size_t capacity;    // 数组的总容量 (固定)
    size_t element_size; // 单个元素的大小（字节）
} StaticArray;

/**
 * @brief 初始化一个静态数组。
 *        注意：此函数不分配内存，它假设 data 指向的内存区域已经存在且足够大。
 *        通常与 VLA 或 alloca 结合使用。
 * @param arr 指向要初始化的 StaticArray 结构的指针。
 * @param data_buffer 指向预先分配的内存缓冲区的指针。
 * @param capacity 数组的固定容量。
 * @param element_size 单个元素的大小（字节）。
 * @return 如果成功则返回 true，否则返回 false。
 */
bool static_array_init(StaticArray *arr, void *data_buffer, size_t capacity, size_t element_size);

/**
 * @brief 销毁静态数组（主要是清理结构体，不释放外部管理的内存）。
 * @param arr 指向要销毁的 StaticArray 结构的指针。
 */
void static_array_destroy(StaticArray *arr);

/**
 * @brief 获取静态数组中指定索引处的元素。
 * @param arr 指向 StaticArray 结构的指针。
 * @param index 要获取元素的索引。
 * @return 指向元素的指针，如果索引无效则返回 NULL。
 */
void* static_array_get(const StaticArray *arr, size_t index);

/**
 * @brief 设置静态数组中指定索引处的值。
 * @param arr 指向 StaticArray 结构的指针。
 * @param index 要设置元素的索引。
 * @param value 指向要设置的值的指针。
 * @return 如果成功则返回 true，否则返回 false（例如索引无效）。
 */
bool static_array_set(StaticArray *arr, size_t index, const void *value);

/**
 * @brief 获取静态数组的当前大小（元素数量）。
 * @param arr 指向 StaticArray 结构的指针。
 * @return 数组的大小。
 */
size_t static_array_size(const StaticArray *arr);

/**
 * @brief 获取静态数组的容量。
 * @param arr 指向 StaticArray 结构的指针。
 * @return 数组的容量。
 */
size_t static_array_capacity(const StaticArray *arr);

/**
 * @brief 检查静态数组是否为空。
 * @param arr 指向 StaticArray 结构的指针。
 * @return 如果数组为空则返回 true，否则返回 false。
 */
bool static_array_is_empty(const StaticArray *arr);

/**
 * @brief 检查静态数组是否已满。
 * @param arr 指向 StaticArray 结构的指针。
 * @return 如果数组已满则返回 true，否则返回 false。
 */
bool static_array_is_full(const StaticArray *arr);

/**
 * @brief 在静态数组末尾添加一个元素。
 * @param arr 指向 StaticArray 结构的指针。
 * @param value 指向要添加的值的指针。
 * @return 如果成功添加则返回 true，如果数组已满则返回 false。
 */
bool static_array_push_back(StaticArray *arr, const void *value);

/**
 * @brief 从静态数组末尾移除一个元素。
 * @param arr 指向 StaticArray 结构的指针。
 * @return 如果成功移除则返回 true，如果数组为空则返回 false。
 */
bool static_array_pop_back(StaticArray *arr);

/**
 * @brief 在静态数组的指定索引处插入一个元素。
 * @param arr 指向 StaticArray 结构的指针。
 * @param index 要插入位置的索引。
 * @param value 指向要插入的值的指针。
 * @return 如果成功插入则返回 true，如果数组已满或索引无效则返回 false。
 */
bool static_array_insert(StaticArray *arr, size_t index, const void *value);

/**
 * @brief 从静态数组的指定索引处删除一个元素。
 * @param arr 指向 StaticArray 结构的指针。
 * @param index 要删除元素的索引。
 * @return 如果成功删除则返回 true，如果索引无效或数组为空则返回 false。
 */
bool static_array_delete(StaticArray *arr, size_t index);

/**
 * @brief 清空静态数组（将大小重置为0，不改变容量或释放内存）。
 * @param arr 指向 StaticArray 结构的指针。
 */
void static_array_clear(StaticArray *arr);

#endif // DSA_STATIC_ARRAY_H