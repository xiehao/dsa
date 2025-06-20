/**
 * @file array_list_traits.h
 * @brief 数组特征和接口定义
 * @details 定义了数组相关的类型特征和操作接口，用于实现数组的多态性和类型安全
 * @author DSA项目组
 * @date 2025-06-17
 * @version 1.0
 */

#ifndef DSA_ARRAY_TRAITS_H
#define DSA_ARRAY_TRAITS_H

#include <traits.h>
#include <ds/array_list.h>

/**
 * @brief 数组基类操作函数表
 * @details 定义了数组类型的基本操作接口，用于实现数组的类型识别和操作
 * 
 * 这个结构体包含了数组类型相关的函数指针，用于：
 * - 获取数组的具体类型
 * - 获取数组类型的字符串表示
 * 
 * @note 这是一种面向对象的设计模式在C语言中的实现
 * @see dsa_array_list_type_t
 */
typedef struct {
    /**
     * @brief 获取数组类型
     * @details 返回当前数组实例的具体类型标识
     * @return dsa_array_type_t 数组类型枚举值
     * @retval 具体的数组类型值，如动态数组、静态数组等
     */
    dsa_array_list_type_t (*get_type)();
    
    /**
     * @brief 获取数组类型名称
     * @details 返回当前数组类型的字符串表示，用于调试和日志输出
     * @return char const* 指向类型名称字符串的常量指针
     * @retval 非空的字符串指针，包含类型的可读名称
     * @warning 返回的字符串不应被修改，且其生命周期由实现者管理
     */
    char const *(*get_type_name)();
} array_list_interface_t;

#endif // DSA_ARRAY_TRAITS_H