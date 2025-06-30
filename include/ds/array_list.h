#ifndef DSA_ARRAY_LIST_H
#define DSA_ARRAY_LIST_H

#include <stddef.h>
#include <common.h>

/**
 * @file array_list.h
 * @brief 统一的数组接口，支持静态数组和动态数组
 * @author DSA Team
 * @version 1.0
 * @date 2025-06-17
 *
 * 本模块提供了统一的数组操作接口，底层可以是静态数组或动态数组。
 * 用户通过创建函数选择具体的数组类型，之后使用统一的操作函数。
 *
 * @section example_sec 使用示例
 * @code{.c}
 * // 创建静态数组
 * int buffer[10];
 * dsa_array_list_t* static_arr = array_list_create_static(buffer, 10, sizeof(int));
 *
 * // 创建动态数组
 * dsa_array_list_t* dynamic_arr = array_list_create_dynamic(10);
 *
 * // 添加元素
 * int value = 42;
 * array_list_push_back(static_arr, &value);
 *
 * // 获取元素
 * int* ptr = (int*)array_list_get(static_arr, 0);
 *
 * // 清理资源
 * array_list_destroy(static_arr);
 * array_list_destroy(dynamic_arr);
 * @endcode
 *
 * @note 所有函数都会进行参数有效性检查
 * @warning 静态数组不会自动扩容，需要注意容量限制
 */

/**
 * @defgroup DSA_ArrayList 数组模块
 * @brief 统一的数组接口实现
 * @{
 */

/**
 * @brief 数组结构体前向声明
 *
 * 隐藏内部实现细节，提供不透明的数组类型。
 * 用户只能通过提供的API函数操作数组。
 */
typedef struct array_list_t dsa_array_list_t;

/**
 * @brief 数组类型枚举
 *
 * 定义支持的数组类型，用于区分静态数组和动态数组的行为。
 */
typedef enum {
    ARRAY_LIST_TYPE_STATIC, ///< 静态数组：使用预分配的固定大小缓冲区
    ARRAY_LIST_TYPE_DYNAMIC ///< 动态数组：可自动扩容的数组
} dsa_array_list_type_t;

/**
 * @defgroup creation_destruction 创建和销毁函数
 * @brief 数组的创建、初始化和销毁相关函数
 * @{
 */

/**
 * @brief 创建静态数组
 *
 * 使用预分配的内存缓冲区创建静态数组。静态数组不会自动扩容，
 * 当达到容量限制时，插入操作会失败。
 *
 * @param[in] buffer 预分配的内存缓冲区，不能为NULL
 * @param[in] capacity 数组容量（元素个数），必须大于0
 * @param[in] element_size 单个元素的大小（字节），必须大于0
 *
 * @return dsa_array_list_t* 成功返回数组指针，失败返回NULL
 *
 * @retval NULL 参数无效（buffer为NULL、capacity为0或element_size为0）
 * @retval NULL 内存分配失败
 *
 * @note 调用者负责管理buffer的生命周期
 * @note 数组销毁时不会释放buffer
 *
 * @see array_list_destroy()
 * @since 1.0
 */
dsa_array_list_t *array_list_create_static(void *buffer, size_t capacity, size_t element_size);

/**
 * @brief 创建动态数组
 *
 * 创建可自动扩容的动态数组。当容量不足时，数组会自动扩容。
 *
 * @param[in] initial_capacity 初始容量，0表示使用默认容量
 *
 * @return dsa_array_list_t* 成功返回数组指针，失败返回NULL
 *
 * @retval NULL 内存分配失败
 *
 * @note 动态数组的元素大小在编译时确定
 * @note 数组销毁时会自动释放所有分配的内存
 *
 * @see array_list_destroy()
 * @see array_list_destroy_with_free()
 * @since 1.0
 */
dsa_array_list_t *array_list_create_dynamic(size_t initial_capacity);


/**
 * @brief 获取数组当前大小
 *
 * 返回数组中当前存储的元素个数。
 *
 * @param[in] array_list 数组指针
 *
 * @return size_t 数组大小
 *
 * @retval 0 array_list为NULL或数组为空
 *
 * @note 函数对NULL指针安全
 *
 * @see array_list_capacity()
 * @see array_list_is_empty()
 * @since 1.0
 */
size_t array_list_size(const dsa_array_list_t *array_list);

/**
 * @brief 获取数组容量
 *
 * 返回数组的最大容量（不触发扩容的情况下能存储的最大元素个数）。
 *
 * @param[in] array_list 数组指针
 *
 * @return size_t 数组容量
 *
 * @retval 0 array_list为NULL
 *
 * @note 函数对NULL指针安全
 * @note 对于动态数组，容量可能在插入操作时自动增长
 *
 * @see array_list_size()
 * @see array_list_is_full()
 * @since 1.0
 */
size_t array_list_capacity(const dsa_array_list_t *array_list);

/**
 * @brief 检查数组是否为空
 *
 * 检查数组中是否包含任何元素。
 *
 * @param[in] array_list 数组指针
 *
 * @return bool 空返回true，否则返回false
 *
 * @retval true array_list为NULL或数组为空
 * @retval false 数组包含至少一个元素
 *
 * @note 函数对NULL指针安全
 *
 * @see array_list_size()
 * @see array_list_is_full()
 * @since 1.0
 */
bool array_list_is_empty(const dsa_array_list_t *array_list);

/**
 * @brief 检查数组是否已满
 *
 * 检查数组是否已达到容量上限。对于动态数组，总是返回false
 * （因为可以自动扩容）。
 *
 * @param[in] array_list 数组指针
 *
 * @return bool 满返回true，否则返回false
 *
 * @retval false array_list为NULL
 * @retval false 动态数组（总是可以扩容）
 * @retval true 静态数组且大小等于容量
 * @retval false 静态数组且大小小于容量
 *
 * @note 函数对NULL指针安全
 * @note 动态数组总是返回false
 *
 * @see array_list_size()
 * @see array_list_capacity()
 * @see array_list_is_empty()
 * @since 1.0
 */
bool array_list_is_full(const dsa_array_list_t *array_list);

/**
 * @brief 清空数组
 *
 * 移除数组中的所有元素，但不释放元素指向的内存。数组容量保持不变。
 *
 * @param[in,out] array_list 数组指针，可以为NULL
 *
 * @return dsa_result_t 操作结果
 *
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER array_list为NULL
 *
 * @note 函数对NULL指针安全，但会返回错误码
 * @note 不会释放元素指向的内存
 * @note 如需释放元素内存，请使用 array_list_clear_with_free()
 *
 * @see array_list_clear_with_free()
 * @since 1.0
 */
dsa_result_t array_list_clear(dsa_array_list_t *array_list);

/**
 * @brief 清空数组并释放元素内存
 *
 * 移除数组中的所有元素，并释放每个元素指向的内存。
 *
 * @param[in,out] array_list 数组指针，可以为NULL
 *
 * @return dsa_result_t 操作结果
 *
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER array_list为NULL
 * @retval DSA_ERROR_MEMORY_OPERATION 内存释放过程中发生错误
 *
 * @note 函数对NULL指针安全，但会返回错误码
 * @note 对于动态数组：释放所有元素指向的内存，然后清空数组
 * @note 对于静态数组：只清空数组，不释放元素内存（静默降级为普通clear）
 * @warning 仅适用于元素为指针类型且指向malloc分配内存的情况
 *
 * @see array_list_clear()
 * @since 1.0
 */
dsa_result_t array_list_clear_with_free(dsa_array_list_t *array_list);

/**
 * @brief 销毁数组
 *
 * 释放数组结构体占用的内存。对于静态数组，不会释放用户提供的buffer；
 * 对于动态数组，会释放内部分配的存储空间，但不会释放元素指向的内存。
 *
 * @param[in] array_list 要销毁的数组指针，可以为NULL
 *
 * @return dsa_result_t 操作结果
 *
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER array_list为NULL
 * @retval DSA_ERROR_MEMORY_OPERATION 内存释放过程中发生错误
 *
 * @note 函数对NULL指针安全，但会返回错误码
 * @note 如果需要释放元素指向的内存，请使用 array_list_destroy_with_free()
 *
 * @see array_list_destroy_with_free()
 * @since 1.0
 */
dsa_result_t array_list_destroy(dsa_array_list_t *array_list);

/**
 * @brief 销毁动态数组并释放所有元素
 *
 * 销毁数组并释放数组中每个元素指向的内存。
 * 仅适用于存储指针类型元素的动态数组。
 *
 * @param[in] array_list 要销毁的数组指针，可以为NULL
 *
 * @return dsa_result_t 操作结果
 *
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER array_list为NULL
 * @retval DSA_ERROR_MEMORY_OPERATION 内存释放过程中发生错误
 * @retval DSA_ERROR_INVALID_OPERATION 对静态数组执行此操作
 *
 * @warning 仅适用于动态数组
 * @warning 仅适用于元素为指针类型的数组
 * @warning 元素指向的内存必须是通过malloc分配的
 *
 * @note 函数对NULL指针安全，但会返回错误码
 * @note 对静态数组调用此函数会返回错误码，但仍会执行基本的destroy操作
 *
 * @see array_list_destroy()
 * @since 1.0
 */
dsa_result_t array_list_destroy_with_free(dsa_array_list_t *array_list);

/** @} */ // end of creation_destruction

/**
 * @defgroup basic_operations 基本操作函数
 * @brief 数组的基本增删改查操作
 * @{
 */

/**
 * @brief 获取指定索引处的元素
 *
 * 返回指向指定索引处元素的指针。返回的指针指向数组内部存储，
 * 用户可以直接修改其内容。
 *
 * @param[in] array_list 数组指针，不能为NULL
 * @param[in] index 元素索引，必须小于数组大小
 *
 * @return dsa_element_pt 成功返回元素指针，失败返回NULL
 *
 * @retval NULL array_list为NULL
 * @retval NULL index超出数组范围
 *
 * @note 返回的指针在数组结构发生变化时可能失效
 * @note 对于动态数组，插入操作可能导致内存重新分配
 *
 * @see array_list_set()
 * @since 1.0
 */
dsa_element_pt array_list_get(const dsa_array_list_t *array_list, size_t index);

/**
 * @brief 设置指定索引处的元素
 *
 * 将指定索引处的元素设置为新值。会将element指向的数据
 * 复制到数组的内部存储中。
 *
 * @param[in,out] array_list 数组指针，不能为NULL
 * @param[in] index 元素索引，必须小于数组大小
 * @param[in] element 要设置的元素指针，不能为NULL
 *
 * @return dsa_result_t 操作结果
 *
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER array_list或element为NULL
 * @retval DSA_ERROR_OUT_OF_BOUNDS index超出数组范围
 *
 * @note 此函数会复制element指向的数据
 * @note 索引必须在有效范围内（小于数组大小）
 *
 * @see array_list_get()
 * @since 1.0
 */
dsa_result_t array_list_set(dsa_array_list_t *array_list, size_t index, dsa_element_pt element);

/**
 * @brief 在数组末尾添加元素
 *
 * 在数组末尾添加新元素。对于动态数组，如果容量不足会自动扩容；
 * 对于静态数组，如果容量不足操作会失败。
 *
 * @param[in,out] array_list 数组指针，不能为NULL
 * @param[in] element 要添加的元素指针，不能为NULL
 *
 * @return dsa_result_t 操作结果
 *
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER array_list或element为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 动态数组扩容失败
 * @retval DSA_ERROR_OUT_OF_BOUNDS 静态数组容量不足
 *
 * @note 此函数会复制element指向的数据
 * @note 动态数组会根据需要自动扩容
 *
 * @see array_list_pop_back()
 * @see array_list_insert()
 * @since 1.0
 */
dsa_result_t array_list_push_back(dsa_array_list_t *array_list, dsa_element_pt element);

/**
 * @brief 移除并返回数组末尾的元素
 *
 * 移除数组末尾的元素并返回其指针。返回的指针指向临时存储，
 * 用户应立即处理或复制数据。
 *
 * @param[in,out] array_list 数组指针，不能为NULL
 *
 * @return dsa_element_pt 成功返回被移除的元素指针，失败返回NULL
 *
 * @retval NULL array_list为NULL
 * @retval NULL 数组为空
 *
 * @note 返回的指针可能在后续操作中失效
 * @note 用户应立即处理返回的数据
 *
 * @see array_list_push_back()
 * @see array_list_remove()
 * @since 1.0
 */
dsa_element_pt array_list_pop_back(dsa_array_list_t *array_list);

/**
 * @brief 在指定索引处插入元素
 *
 * 在指定位置插入新元素，原有元素向后移动。对于动态数组，
 * 如果容量不足会自动扩容。
 *
 * @param[in,out] array_list 数组指针，不能为NULL
 * @param[in] index 插入位置的索引，可以等于数组大小（尾部插入）
 * @param[in] element 要插入的元素指针，不能为NULL
 *
 * @return dsa_result_t 操作结果
 *
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER array_list或element为NULL
 * @retval DSA_ERROR_OUT_OF_BOUNDS index超出有效范围
 * @retval DSA_ERROR_MEMORY_ALLOCATION 动态数组扩容失败
 *
 * @note index可以等于数组大小，表示在末尾插入
 * @note 此函数会复制element指向的数据
 * @note 插入操作的时间复杂度为O(n)
 *
 * @see array_list_remove()
 * @see array_list_push_back()
 * @since 1.0
 */
dsa_result_t array_list_insert(dsa_array_list_t *array_list, size_t index, dsa_element_pt element);

/**
 * @brief 移除指定索引处的元素
 *
 * 移除指定位置的元素，后续元素向前移动。返回被移除元素的指针。
 *
 * @param[in,out] array_list 数组指针，不能为NULL
 * @param[in] index 要移除元素的索引，必须小于数组大小
 *
 * @return dsa_element_pt 成功返回被移除的元素指针，失败返回NULL
 *
 * @retval NULL array_list为NULL
 * @retval NULL index超出数组范围
 * @retval NULL 数组为空
 *
 * @note 返回的指针可能在后续操作中失效
 * @note 移除操作的时间复杂度为O(n)
 *
 * @see array_list_insert()
 * @see array_list_pop_back()
 * @since 1.0
 */
dsa_element_pt array_list_remove(dsa_array_list_t *array_list, size_t index);

/** @} */ // end of basic_operations

/**
 * @defgroup query_functions 查询函数
 * @brief 数组状态和属性查询函数
 * @{
 */

/**
 * @brief 获取数组类型
 *
 * 返回数组的类型（静态或动态）。
 *
 * @param[in] array_list 数组指针，不能为NULL
 *
 * @return dsa_array_list_type_t 数组类型
 *
 * @retval ARRAY_LIST_TYPE_STATIC 静态数组
 * @retval ARRAY_LIST_TYPE_DYNAMIC 动态数组
 *
 * @warning array_list不能为NULL
 *
 * @see array_list_get_type_name()
 * @since 1.0
 */
dsa_array_list_type_t array_list_get_type(const dsa_array_list_t *array_list);

/**
 * @brief 获取数组类型名称字符串
 *
 * 返回描述数组类型的字符串，用于调试和日志输出。
 *
 * @param[in] array_list 数组指针
 *
 * @return const char* 类型名称字符串
 *
 * @retval "static" 静态数组
 * @retval "dynamic" 动态数组
 * @retval "unknown" array_list为NULL或类型未知
 *
 * @note 函数对NULL指针安全
 * @note 返回的字符串是常量，不需要释放
 *
 * @see array_list_get_type()
 * @since 1.0
 */
const char *array_list_get_type_name(const dsa_array_list_t *array_list);

/** @} */ // end of query_functions

/**
 * @defgroup utility_functions 其他操作函数
 * @brief 数组的实用工具函数
 * @{
 */

/**
 * @brief 打印数组信息（用于调试）
 *
 * 打印数组的基本信息，包括类型、大小、容量等，用于调试目的。
 *
 * @param[in] array_list 数组指针，可以为NULL
 *
 * @note 函数对NULL指针安全
 * @note 输出格式可能在不同版本间发生变化
 * @note 仅用于调试，不应在生产代码中依赖输出格式
 *
 * @since 1.0
 */
void array_list_print_info(const dsa_array_list_t *array_list);

/** @} */ // end of utility_functions

/**
 * @defgroup type_safe_functions 类型安全的便利函数
 * @brief 针对特定类型的便利函数（当前已注释）
 *
 * 这些函数提供了类型安全的接口，避免了类型转换的繁琐。
 * 当前版本中这些函数被注释掉了，可根据需要启用。
 *
 * @{
 */

// /**
//  * @brief 向整型数组添加元素
//  *
//  * 类型安全的整型元素添加函数，避免了手动类型转换。
//  *
//  * @param[in,out] trait 数组指针，不能为NULL
//  * @param[in] value 要添加的整型值
//  *
//  * @return dsa_result_t 操作结果
//  *
//  * @retval DSA_SUCCESS 操作成功
//  * @retval DSA_ERROR_NULL_POINTER array为NULL
//  * @retval DSA_ERROR_MEMORY_ALLOCATION 动态数组扩容失败
//  * @retval DSA_ERROR_OUT_OF_BOUNDS 静态数组容量不足
//  *
//  * @note 数组必须是为int类型创建的
//  *
//  * @see array_list_push_back()
//  * @see array_get_int()
//  * @since 1.0
//  */
// dsa_result_t array_push_back_int(dsa_array_list_t* trait, int value);
//
// /**
//  * @brief 从整型数组获取元素
//  *
//  * 类型安全的整型元素获取函数。
//  *
//  * @param[in] trait 数组指针，不能为NULL
//  * @param[in] index 元素索引，必须小于数组大小
//  * @param[out] value 输出参数，存储获取的值，不能为NULL
//  *
//  * @return dsa_result_t 操作结果
//  *
//  * @retval DSA_SUCCESS 操作成功
//  * @retval DSA_ERROR_NULL_POINTER array或value为NULL
//  * @retval DSA_ERROR_OUT_OF_BOUNDS index超出数组范围
//  *
//  * @note 数组必须是为int类型创建的
//  *
//  * @see array_list_get()
//  * @see array_set_int()
//  * @since 1.0
//  */
// dsa_result_t array_get_int(const dsa_array_list_t* trait, size_t index, int* value);
//
// /**
//  * @brief 向整型数组设置元素
//  *
//  * 类型安全的整型元素设置函数。
//  *
//  * @param[in,out] trait 数组指针，不能为NULL
//  * @param[in] index 元素索引，必须小于数组大小
//  * @param[in] value 要设置的整型值
//  *
//  * @return dsa_result_t 操作结果
//  *
//  * @retval DSA_SUCCESS 操作成功
//  * @retval DSA_ERROR_NULL_POINTER array为NULL
//  * @retval DSA_ERROR_OUT_OF_BOUNDS index超出数组范围
//  *
//  * @note 数组必须是为int类型创建的
//  *
//  * @see array_list_set()
//  * @see array_get_int()
//  * @since 1.0
//  */
// dsa_result_t array_set_int(dsa_array_list_t* trait, size_t index, int value);
//
// /**
//  * @brief 向双精度浮点数组添加元素
//  *
//  * 类型安全的双精度浮点元素添加函数。
//  *
//  * @param[in,out] trait 数组指针，不能为NULL
//  * @param[in] value 要添加的双精度浮点值
//  *
//  * @return dsa_result_t 操作结果
//  *
//  * @retval DSA_SUCCESS 操作成功
//  * @retval DSA_ERROR_NULL_POINTER array为NULL
//  * @retval DSA_ERROR_MEMORY_ALLOCATION 动态数组扩容失败
//  * @retval DSA_ERROR_OUT_OF_BOUNDS 静态数组容量不足
//  *
//  * @note 数组必须是为double类型创建的
//  *
//  * @see array_list_push_back()
//  * @see array_get_double()
//  * @since 1.0
//  */
// dsa_result_t array_push_back_double(dsa_array_list_t* trait, double value);
//
// /**
//  * @brief 从双精度浮点数组获取元素
//  *
//  * 类型安全的双精度浮点元素获取函数。
//  *
//  * @param[in] trait 数组指针，不能为NULL
//  * @param[in] index 元素索引，必须小于数组大小
//  * @param[out] value 输出参数，存储获取的值，不能为NULL
//  *
//  * @return dsa_result_t 操作结果
//  *
//  * @retval DSA_SUCCESS 操作成功
//  * @retval DSA_ERROR_NULL_POINTER array或value为NULL
//  * @retval DSA_ERROR_OUT_OF_BOUNDS index超出数组范围
//  *
//  * @note 数组必须是为double类型创建的
//  *
//  * @see array_list_get()
//  * @see array_set_double()
//  * @since 1.0
//  */
// dsa_result_t array_get_double(const dsa_array_list_t* trait, size_t index, double* value);
//
// /**
//  * @brief 向双精度浮点数组设置元素
//  *
//  * 类型安全的双精度浮点元素设置函数。
//  *
//  * @param[in,out] trait 数组指针，不能为NULL
//  * @param[in] index 元素索引，必须小于数组大小
//  * @param[in] value 要设置的双精度浮点值
//  *
//  * @return dsa_result_t 操作结果
//  *
//  * @retval DSA_SUCCESS 操作成功
//  * @retval DSA_ERROR_NULL_POINTER array为NULL
//  * @retval DSA_ERROR_OUT_OF_BOUNDS index超出数组范围
//  *
//  * @note 数组必须是为double类型创建的
//  *
//  * @see array_list_set()
//  * @see array_get_double()
//  * @since 1.0
//  */
// dsa_result_t array_set_double(dsa_array_list_t* trait, size_t index, double value);

/** @} */ // end of type_safe_functions

/** @} */ // end of DSA_Array

#endif // DSA_ARRAY_LIST_H
