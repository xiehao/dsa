/**
 * @file stack_traits.h
 * @brief 栈特性接口定义
 * @details 定义了栈数据结构的特性接口，用于实现不同栈实现的统一访问
 * @author DSA项目组
 * @date 2025-07-22
 * @version 1.0
 */

#ifndef DSA_STACK_TRAITS_H
#define DSA_STACK_TRAITS_H

#include <traits.h>
#include <adt/stack.h>

/**
 * @struct trait_stack_t
 * @brief 栈容器特定接口
 * @details 定义了栈数据结构的LIFO（后进先出）操作
 */
typedef struct {
    trait_basic_t const *basic;
    /**
     * @brief 将元素压入栈顶
     * @param stack 栈容器指针
     * @param element 要压入的元素指针
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     */
    dsa_result_t (*push)(dsa_stack_t *stack, dsa_element_pt element);

    /**
     * @brief 弹出并返回栈顶元素
     * @param stack 栈容器指针
     * @return 栈顶元素指针，如果栈为空则返回NULL
     * @note 调用者负责释放返回元素的内存（如果需要）
     */
    dsa_element_pt (*pop)(dsa_stack_t *stack);

    /**
     * @brief 查看栈顶元素但不移除
     * @param stack 栈容器的常量指针
     * @return 栈顶元素指针，如果栈为空则返回NULL
     */
    dsa_element_pt (*peek)(dsa_stack_t const *stack);

    /**
     * @brief 获取链表类型的函数指针
     * @return 返回链表的类型枚举值
     */
    dsa_stack_type_t (*get_type)(void);

    /**
     * @brief 获取链表类型名称的函数指针
     * @return 返回链表类型的字符串描述
     */
    char const *(*get_type_name)(void);
} trait_stack_t;

#endif // DSA_STACK_TRAITS_H
