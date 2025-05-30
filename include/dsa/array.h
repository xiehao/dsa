#ifndef DSA_ARRAY_H
#define DSA_ARRAY_H

#include <stddef.h>
#include <stdbool.h>
#include <dsa/common.h>

/**
 * @file array.h
 * @brief 统一的数组接口，支持静态数组和动态数组
 *
 * 本模块提供了统一的数组操作接口，底层可以是静态数组或动态数组。
 * 用户通过创建函数选择具体的数组类型，之后使用统一的操作函数。
 */

// 前向声明，隐藏内部实现
typedef struct array_t dsa_array_t;

/**
 * @brief 数组操作结果枚举
 */
typedef enum {
    ARRAY_SUCCESS = 0,              ///< 操作成功
    ARRAY_ERROR_NULL_POINTER,       ///< 空指针错误
    ARRAY_ERROR_INDEX_OUT_OF_BOUNDS,///< 索引越界
    ARRAY_ERROR_CAPACITY_FULL,      ///< 容量已满（仅静态数组）
    ARRAY_ERROR_EMPTY,              ///< 数组为空
    ARRAY_ERROR_MEMORY_ALLOCATION,  ///< 内存分配失败
    ARRAY_ERROR_INVALID_PARAMETER   ///< 无效参数
} dsa_array_result_t;

/**
 * @brief 数组类型枚举
 */
typedef enum {
    ARRAY_TYPE_STATIC,              ///< 静态数组
    ARRAY_TYPE_DYNAMIC              ///< 动态数组
} dsa_array_type_t;

// ============================================================================
// 创建和销毁函数
// ============================================================================

/**
 * @brief 创建静态数组
 *
 * @param buffer 预分配的内存缓冲区
 * @param capacity 数组容量
 * @param element_size 单个元素的大小（字节）
 * @return Array* 成功返回数组指针，失败返回NULL
 */
dsa_array_t* array_create_static(void* buffer, size_t capacity, size_t element_size);

/**
 * @brief 创建动态数组
 *
 * @param initial_capacity 初始容量，0表示使用默认容量
 * @return Array* 成功返回数组指针，失败返回NULL
 */
dsa_array_t* array_create_dynamic(size_t initial_capacity);

/**
 * @brief 销毁数组
 *
 * @param array 要销毁的数组指针
 */
void array_destroy(dsa_array_t* array);

/**
 * @brief 销毁动态数组并释放所有元素
 *
 * 仅适用于动态数组，会释放数组中每个元素指向的内存
 * @param array 要销毁的数组指针
 */
void array_destroy_with_free(dsa_array_t* array);

// ============================================================================
// 基本操作函数
// ============================================================================

/**
 * @brief 获取指定索引处的元素
 *
 * @param array 数组指针
 * @param index 元素索引
 * @return ElementPtr 成功返回元素指针，失败返回NULL
 */
dsa_element_pt array_get(const dsa_array_t* array, size_t index);

/**
 * @brief 设置指定索引处的元素
 *
 * @param array 数组指针
 * @param index 元素索引
 * @param element 要设置的元素
 * @return ArrayResult 操作结果
 */
dsa_array_result_t array_set(dsa_array_t* array, size_t index, dsa_element_pt element);

/**
 * @brief 在数组末尾添加元素
 *
 * @param array 数组指针
 * @param element 要添加的元素
 * @return ArrayResult 操作结果
 */
dsa_array_result_t array_push_back(dsa_array_t* array, dsa_element_pt element);

/**
 * @brief 移除并返回数组末尾的元素
 *
 * @param array 数组指针
 * @return ElementPtr 成功返回被移除的元素，失败返回NULL
 */
dsa_element_pt array_pop_back(dsa_array_t* array);

/**
 * @brief 在指定索引处插入元素
 *
 * @param array 数组指针
 * @param index 插入位置的索引
 * @param element 要插入的元素
 * @return ArrayResult 操作结果
 */
dsa_array_result_t array_insert(dsa_array_t* array, size_t index, dsa_element_pt element);

/**
 * @brief 移除指定索引处的元素
 *
 * @param array 数组指针
 * @param index 要移除元素的索引
 * @return ElementPtr 成功返回被移除的元素，失败返回NULL
 */
dsa_element_pt array_remove(dsa_array_t* array, size_t index);

// ============================================================================
// 查询函数
// ============================================================================

/**
 * @brief 获取数组当前大小
 *
 * @param array 数组指针
 * @return size_t 数组大小
 */
size_t array_size(const dsa_array_t* array);

/**
 * @brief 获取数组容量
 *
 * @param array 数组指针
 * @return size_t 数组容量
 */
size_t array_capacity(const dsa_array_t* array);

/**
 * @brief 检查数组是否为空
 *
 * @param array 数组指针
 * @return bool 空返回true，否则返回false
 */
bool array_is_empty(const dsa_array_t* array);

/**
 * @brief 检查数组是否已满
 *
 * @param array 数组指针
 * @return bool 满返回true，否则返回false（动态数组总是返回false）
 */
bool array_is_full(const dsa_array_t* array);

/**
 * @brief 获取数组类型
 *
 * @param array 数组指针
 * @return ArrayType 数组类型
 */
dsa_array_type_t array_get_type(const dsa_array_t* array);

/**
 * @brief 获取数组类型名称字符串
 *
 * @param array 数组指针
 * @return const char* 类型名称字符串
 */
const char* array_get_type_name(const dsa_array_t* array);

// ============================================================================
// 其他操作函数
// ============================================================================

/**
 * @brief 清空数组
 *
 * @param array 数组指针
 */
void array_clear(dsa_array_t* array);

/**
 * @brief 打印数组信息（用于调试）
 *
 * @param array 数组指针
 */
void array_print_info(const dsa_array_t* array);

// ============================================================================
// 类型安全的便利函数
// ============================================================================

/**
 * @brief 向整型数组添加元素
 *
 * @param array 数组指针
 * @param value 整型值
 * @return ArrayResult 操作结果
 */
dsa_array_result_t array_push_back_int(dsa_array_t* array, int value);

/**
 * @brief 从整型数组获取元素
 *
 * @param array 数组指针
 * @param index 元素索引
 * @param value 输出参数，存储获取的值
 * @return ArrayResult 操作结果
 */
dsa_array_result_t array_get_int(const dsa_array_t* array, size_t index, int* value);

/**
 * @brief 向整型数组设置元素
 *
 * @param array 数组指针
 * @param index 元素索引
 * @param value 要设置的值
 * @return ArrayResult 操作结果
 */
dsa_array_result_t array_set_int(dsa_array_t* array, size_t index, int value);

/**
 * @brief 向双精度浮点数组添加元素
 *
 * @param array 数组指针
 * @param value 双精度浮点值
 * @return ArrayResult 操作结果
 */
dsa_array_result_t array_push_back_double(dsa_array_t* array, double value);

/**
 * @brief 从双精度浮点数组获取元素
 *
 * @param array 数组指针
 * @param index 元素索引
 * @param value 输出参数，存储获取的值
 * @return ArrayResult 操作结果
 */
dsa_array_result_t array_get_double(const dsa_array_t* array, size_t index, double* value);

/**
 * @brief 向双精度浮点数组设置元素
 *
 * @param array 数组指针
 * @param index 元素索引
 * @param value 要设置的值
 * @return ArrayResult 操作结果
 */
dsa_array_result_t array_set_double(dsa_array_t* array, size_t index, double value);

#endif // DSA_ARRAY_H
