/**
 * @file dsa_common.h
 * @brief 数据结构与算法库的公共定义和工具
 * @details 本文件包含了DSA库中所有数据结构共用的类型定义、错误码枚举、
 *          以及用于安全访问元素的宏定义。
 * @author DSA项目组
 * @date 2025-06-17
 * @version 1.0
 */

#ifndef DSA_COMMON_H
#define DSA_COMMON_H

#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>  // 用于 uintptr_t
#include <stdio.h>   // 用于 fprintf
#include <stdlib.h>  // 用于 abort

/**
 * @typedef dsa_element_pt
 * @brief 通用元素指针类型
 * @details 用于表示指向任意类型数据的通用指针，提供类型安全的抽象接口。
 *          在具体使用时需要转换为相应的具体类型指针。
 */
typedef void *dsa_element_pt;

/**
 * @typedef dsa_const_element_pt
 * @brief 通用常量元素指针类型
 * @details 用于表示指向任意类型常量数据的通用指针，提供类型安全的只读抽象接口。
 *          与 dsa_element_pt 相对应，确保指向的数据不能被修改，适用于只读访问场景。
 *          在具体使用时需要转换为相应的具体类型常量指针。
 * @note 此类型保证指向的数据内容不可修改，提供额外的类型安全保障
 * @see dsa_element_pt
 */
typedef void const *dsa_const_element_pt;

/**
 * @typedef compare_func_t
 * @brief 通用比较函数指针类型
 * @details 定义了用于比较两个元素的函数指针类型，广泛用于排序、搜索等算法中。
 *          比较函数应该实现严格弱序关系，以确保算法的正确性。
 * @param lhs 左操作数，指向要比较的第一个元素的常量指针
 * @param rhs 右操作数，指向要比较的第二个元素的常量指针
 * @return 比较结果的整数值：
 *         - 负数：lhs < rhs（左操作数小于右操作数）
 *         - 零：  lhs == rhs（两操作数相等）
 *         - 正数：lhs > rhs（左操作数大于右操作数）
 * @note 比较函数必须满足以下性质：
 *       - 反对称性：如果 f(a,b) < 0，则 f(b,a) > 0
 *       - 传递性：如果 f(a,b) < 0 且 f(b,c) < 0，则 f(a,c) < 0
 *       - 等价传递性：如果 f(a,b) == 0 且 f(b,c) == 0，则 f(a,c) == 0
 * @warning 传入的指针参数不能为NULL，比较函数需要对此进行检查
 * @example
 * @code
 * // 整数比较函数示例
 * int int_compare(dsa_const_element_pt lhs, dsa_const_element_pt rhs) {
 *     int a = ELEMENT_VALUE(int, lhs);
 *     int b = ELEMENT_VALUE(int, rhs);
 *     return (a > b) - (a < b);  // 返回 -1, 0, 或 1
 * }
 * @endcode
 * @see dsa_const_element_pt
 */
typedef int (*compare_func_t)(dsa_const_element_pt lhs, dsa_const_element_pt rhs);

/**
 * @enum dsa_result_t
 * @brief 数据结构操作结果枚举
 * @details 定义了DSA库中所有操作可能返回的结果状态码，
 *          用于统一的错误处理和状态检查。
 */
typedef enum {
    DSA_SUCCESS = 0,                ///< 操作成功
    DSA_ERROR_NULL_POINTER,         ///< 空指针错误
    DSA_ERROR_INDEX_OUT_OF_BOUNDS,  ///< 索引越界
    DSA_ERROR_CAPACITY_FULL,        ///< 容量已满
    DSA_ERROR_EMPTY,                ///< 数据结构为空
    DSA_ERROR_MEMORY_ALLOCATION,    ///< 内存分配失败
    DSA_ERROR_INVALID_PARAMETER     ///< 无效参数
} dsa_result_t;

/**
 * @def ELEMENT_VALUE(type, ptr)
 * @brief 安全获取元素值的宏
 * @param type 要获取的数据类型
 * @param ptr 指向数据的指针
 * @return 指定类型的数据值
 * @details 在DEBUG模式下提供全面的安全检查，包括：
 *          - 空指针检查
 *          - 类型大小验证
 *          - 内存对齐检查
 *          在Release模式下进行基本的空指针检查，如果指针为空则终止程序。
 * @warning 在Release模式下，如果传入NULL指针会导致程序终止
 * @example
 * @code
 * int value = 42;
 * int *ptr = &value;
 * int result = ELEMENT_VALUE(int, ptr); // 返回42
 * @endcode
 */
#ifdef DEBUG
#define ELEMENT_VALUE(type, ptr) \
    (assert((ptr) != NULL), \
     assert(sizeof(type) > 0), \
     assert(((uintptr_t)(ptr) % _Alignof(type)) == 0), \
     *(type*)(ptr))
#else
// 即使在非DEBUG模式下也进行基本的安全检查
#define ELEMENT_VALUE(type, ptr) \
    ((ptr) != NULL ? *(type*)(ptr) : \
     (fprintf(stderr, "错误：ELEMENT_VALUE宏接收到NULL指针\n"), \
      abort(), *(type*)(ptr)))
#endif

/**
 * @def SAFE_ELEMENT_VALUE(type, ptr, default_value)
 * @brief 安全获取元素值的宏（带默认值）
 * @param type 要获取的数据类型
 * @param ptr 指向数据的指针
 * @param default_value 当指针无效时返回的默认值
 * @return 指定类型的数据值或默认值
 * @details 提供更安全的元素访问接口，在指针为空或内存对齐不正确时
 *          返回指定的默认值而不是终止程序。适用于需要优雅处理错误的场景。
 * @note 这个宏不会终止程序，即使在指针无效的情况下也会返回默认值
 * @example
 * @code
 * int *ptr = NULL;
 * int result = SAFE_ELEMENT_VALUE(int, ptr, -1); // 返回-1
 * @endcode
 */
#define SAFE_ELEMENT_VALUE(type, ptr, default_value) \
    ((ptr) != NULL && ((uintptr_t)(ptr) % _Alignof(type)) == 0 ? \
     *(type*)(ptr) : (default_value))

#endif // DSA_COMMON_H