/**
 * @file stack.c
 * @brief 栈的统一接口实现
 * @details 提供栈数据结构的统一接口，支持多种底层实现（数组栈、链表栈）。
 *          使用工厂模式和特征表模式实现多态性，允许在运行时选择不同的栈实现。
 * @author oaheixiehao
 * @date 2025/7/3
 * @version 1.0
 */

#include <adt/stack.h>
#include <traits.h>
#include <stdlib.h>
#include <internal/stack_traits.h>
#include <internal/array_stack.h>
#include <internal/linked_stack.h>

/**
 * @brief 栈的内部结构定义
 * @details 所有栈实现都必须以此结构作为第一个成员，以实现多态性
 */
struct stack_t {
    trait_stack_t const *traits;  ///< 栈操作特征表，指向具体实现的函数指针
};

/**
 * @brief 创建指定类型的栈实例
 * @param type 栈的类型，支持STACK_TYPE_ARRAY_LIST和STACK_TYPE_LINKED_LIST
 * @return 新创建的栈指针，如果创建失败则返回NULL
 * @note 时间复杂度：O(1)
 * @details 使用工厂模式根据类型参数创建相应的栈实现。
 *          - STACK_TYPE_ARRAY_LIST: 创建基于动态数组的栈
 *          - STACK_TYPE_LINKED_LIST: 创建基于单链表的栈
 */
dsa_stack_t *stack_create(dsa_stack_type_t type) {
    switch (type) {
        case STACK_TYPE_ARRAY_LIST:
            return array_stack_create();
        case STACK_TYPE_LINKED_LIST:
            return linked_stack_create();
        default:
            return NULL;
    }
}

/**
 * @brief 获取栈中元素的数量
 * @param stack 栈的常量指针
 * @return 栈中元素的数量，如果栈为NULL则返回0
 * @note 时间复杂度：取决于具体实现（数组栈O(1)，链表栈O(n)）
 */
size_t stack_size(dsa_stack_t const *stack) {
    return stack ? stack->traits->basic->get_size(stack) : 0;
}

/**
 * @brief 检查栈是否为空
 * @param stack 栈的常量指针
 * @return 如果栈为空或NULL则返回true，否则返回false
 * @note 时间复杂度：O(1)
 */
bool stack_is_empty(dsa_stack_t const *stack) {
    return stack ? stack->traits->basic->is_empty(stack) : true;
}

/**
 * @brief 清空栈中的所有元素
 * @param stack 栈的指针
 * @return 操作结果码：DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示栈为NULL
 * @note 时间复杂度：O(n)，其中n为栈中元素数量
 * @warning 此函数不会释放元素本身的内存，调用者需要自行管理元素内存
 */
dsa_result_t stack_clear(dsa_stack_t *stack) {
    if (!stack) { return DSA_ERROR_NULL_POINTER; }
    return stack->traits->basic->clear(stack);
}

/**
 * @brief 销毁栈并释放相关内存
 * @param stack 栈的指针
 * @return 操作结果码：DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示栈为NULL
 * @note 时间复杂度：取决于具体实现（数组栈O(1)，链表栈O(n)）
 * @warning 此函数不会释放栈中存储的元素内存，调用者需要在销毁前清理元素
 */
dsa_result_t stack_destroy(dsa_stack_t *stack) {
    if (!stack) { return DSA_ERROR_NULL_POINTER; }
    return stack->traits->basic->destroy(stack);
}

/**
 * @brief 向栈顶压入一个元素
 * @param stack 栈的指针
 * @param element 要压入的元素指针
 * @return 操作结果码：DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示栈为NULL
 * @note 时间复杂度：取决于具体实现（数组栈平摊O(1)，链表栈O(1)）
 * @details 元素被添加到栈顶，遵循LIFO（后进先出）原则
 */
dsa_result_t stack_push(dsa_stack_t *stack, dsa_element_pt element) {
    if (!stack) { return DSA_ERROR_NULL_POINTER; }
    return stack->traits->push(stack, element);
}

/**
 * @brief 从栈顶弹出一个元素
 * @param stack 栈的指针
 * @return 弹出的元素指针，如果栈为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 * @details 移除并返回栈顶元素，遵循LIFO（后进先出）原则
 */
dsa_element_pt stack_pop(dsa_stack_t *stack) {
    if (!stack) { return NULL; }
    return stack->traits->pop(stack);
}

/**
 * @brief 查看栈顶元素但不移除
 * @param stack 栈的常量指针
 * @return 栈顶元素指针，如果栈为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 * @details 返回栈顶元素的指针，不改变栈的状态
 */
dsa_element_pt stack_peek(dsa_stack_t const *stack) {
    if (!stack) { return NULL; }
    return stack->traits->peek(stack);
}

/**
 * @brief 获取栈的类型标识
 * @param stack 栈的常量指针
 * @return 栈类型枚举值，如果栈为NULL则返回STACK_TYPE_ARRAY_LIST
 * @note 时间复杂度：O(1)
 */
dsa_stack_type_t stack_get_type(dsa_stack_t const *stack) {
    if (!stack) { return STACK_TYPE_ARRAY_LIST; }
    return stack->traits->get_type();
}

/**
 * @brief 获取栈的类型名称字符串
 * @param stack 栈的常量指针
 * @return 栈类型的中文描述字符串，如果栈为NULL则返回"无效栈"
 * @note 时间复杂度：O(1)
 */
char const *stack_get_type_name(dsa_stack_t const *stack) {
    if (!stack) { return "无效栈"; }
    return stack->traits->get_type_name();
}
