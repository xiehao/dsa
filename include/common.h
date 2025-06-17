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