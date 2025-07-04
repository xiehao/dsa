/**
 * @file linked_list_trait.h
 * @brief 链表特性接口定义
 * @author oaheixiehao
 * @date 2025/6/23
 */

#ifndef DSA_LINKED_LIST_TRAITS_H
#define DSA_LINKED_LIST_TRAITS_H

#include <traits.h>
#include <ds/linked_list.h>

/**
 * @brief 链表特性结构体
 *
 * 该结构体定义了链表数据结构的特性接口，包含了基本特性、线性特性、
 * 随机访问特性以及链表特有的类型相关操作。
 *
 * @details
 * 该特性结构体采用组合模式，将不同层次的特性组合在一起：
 * - basic: 基本数据结构特性（如创建、销毁等）
 * - linear: 线性数据结构特性（如插入、删除、遍历等）
 * - random_access: 随机访问特性（如按索引访问等）
 * - get_type: 获取链表类型的函数指针
 * - get_type_name: 获取链表类型名称的函数指针
 */
typedef struct {
    /** @brief 指向基本特性接口的常量指针 */
    trait_basic_t const *basic;

    /** @brief 指向线性特性接口的常量指针 */
    trait_linear_t const *linear;

    /** @brief 指向随机访问特性接口的常量指针 */
    trait_random_access_t const *random_access;

    /**
     * @brief 获取链表类型的函数指针
     * @return 返回链表的类型枚举值
     */
    dsa_linked_list_type_t (*get_type)(void);

    /**
     * @brief 获取链表类型名称的函数指针
     * @return 返回链表类型的字符串描述
     */
    char const *(*get_type_name)(void);
} trait_linked_list_t;

#endif // DSA_LINKED_LIST_TRAITS_H
