/**
 * @file stack.h
 * @brief 栈抽象数据类型的接口定义
 * @details 提供了栈(Stack)数据结构的统一接口，支持数组栈和链表栈两种实现。
 *          栈遵循后进先出(LIFO)的访问原则，支持压栈、弹栈、查看栈顶等基本操作。
 * @author DSA项目组
 * @date 2025-07-22
 * @version 1.0
 */

#ifndef DSA_STACK_H
#define DSA_STACK_H

#include <common.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief 栈的不透明指针类型
 *
 * 使用不透明指针隐藏栈的内部实现细节，提供更好的封装性。
 * 用户只能通过提供的API函数来操作栈，无法直接访问内部结构。
 */
typedef struct stack_t dsa_stack_t;

/**
 * @brief 栈类型枚举
 *
 * 定义了支持的栈实现类型，用于在创建栈时指定具体的实现方式。
 */
typedef enum {
    STACK_TYPE_ARRAY_LIST,  /**< 数组栈：基于动态数组实现，内存局部性好 */
    STACK_TYPE_LINKED_LIST, /**< 链表栈：基于单链表实现，动态内存分配 */
} dsa_stack_type_t;

/**
 * @brief 创建一个新的栈
 *
 * 根据指定的类型创建一个新的栈实例。新创建的栈为空，
 * 用户需要调用相应的压栈函数来添加元素。
 *
 * @param type 栈类型，可以是数组栈或链表栈
 * @return 指向新创建的栈的指针，如果内存分配失败则返回NULL
 *
 * @note 调用者负责在不再需要时调用 stack_destroy() 来释放内存
 * @see stack_destroy()
 */
dsa_stack_t *stack_create(dsa_stack_type_t type);

/**
 * @brief 获取栈中元素的个数
 *
 * @param stack 指向栈的指针
 * @return 栈中元素的个数，如果栈为NULL则返回0
 *
 * @pre stack != NULL
 */
size_t stack_size(dsa_stack_t const *stack);

/**
 * @brief 检查栈是否为空
 *
 * @param stack 指向栈的指针
 * @return 如果栈为空或栈指针为NULL则返回true，否则返回false
 *
 * @pre stack != NULL
 */
bool stack_is_empty(dsa_stack_t const *stack);

/**
 * @brief 清空栈中的所有元素
 *
 * 移除栈中的所有元素，但保留栈结构本身。
 * 清空后的栈可以继续使用。
 *
 * @param stack 指向要清空的栈的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 清空成功
 * @retval DSA_ERROR_NULL_POINTER 栈指针为NULL
 *
 * @pre stack != NULL
 * @post stack_is_empty(stack) == true
 */
dsa_result_t stack_clear(dsa_stack_t *stack);

/**
 * @brief 销毁栈并释放所有相关内存
 *
 * 完全销毁栈，释放所有节点和栈结构本身占用的内存。
 * 销毁后的栈指针不能再使用。
 *
 * @param stack 指向要销毁的栈的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 销毁成功
 * @retval DSA_ERROR_NULL_POINTER 栈指针为NULL
 *
 * @pre stack != NULL
 * @post stack指针不再有效
 *
 * @warning 调用此函数后，stack指针将失效，不能再使用
 * @note 此函数不会释放栈中存储的元素，调用者负责管理元素本身的内存
 */
dsa_result_t stack_destroy(dsa_stack_t *stack);

/**
 * @brief 将元素压入栈顶（压栈操作）
 *
 * 将新元素添加到栈的顶部，遵循LIFO（后进先出）原则。
 * 对于数组栈，如果容量不足会自动扩容。
 *
 * @param stack 指向栈的指针
 * @param element 指向要压入的元素的指针
 * @return 操作结果状态码
 * @retval DSA_SUCCESS 压栈成功
 * @retval DSA_ERROR_NULL_POINTER 栈指针或元素指针为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 *
 * @pre stack != NULL
 * @pre element != NULL
 * @post stack_size(stack) 增加1
 */
dsa_result_t stack_push(dsa_stack_t *stack, dsa_element_pt element);

/**
 * @brief 从栈顶移除并返回元素（弹栈操作）
 *
 * 移除并返回栈顶的元素，遵循LIFO（后进先出）原则。
 * 最后压入的元素将最先弹出。
 *
 * @param stack 指向栈的指针
 * @return 指向被移除元素数据的指针，如果栈为空则返回NULL
 *
 * @pre stack != NULL
 * @post 如果栈不为空，stack_size(stack) 减少1
 *
 * @note 调用者负责释放返回的数据指针所指向的内存（如果需要的话）
 */
dsa_element_pt stack_pop(dsa_stack_t *stack);

/**
 * @brief 查看栈顶元素但不移除它（窥视操作）
 *
 * 返回栈顶元素的指针，但不从栈中移除该元素。
 * 用于查看栈顶内容而不改变栈的状态。
 *
 * @param stack 指向栈的指针
 * @return 指向栈顶元素的指针，如果栈为空则返回NULL
 *
 * @pre stack != NULL
 * @note 返回的指针指向栈内部存储的数据，修改返回的数据会影响栈内容
 */
dsa_element_pt stack_peek(dsa_stack_t const *stack);

/**
 * @brief 获取栈类型
 *
 * @param stack 指向栈的指针
 * @return 栈类型枚举值，如果栈为NULL则返回默认类型
 *
 * @pre stack != NULL
 */
dsa_stack_type_t stack_get_type(dsa_stack_t const *stack);

/**
 * @brief 获取栈类型名称
 *
 * @param stack 指向栈的指针
 * @return 栈类型的字符串描述，如果栈为NULL则返回"Unknown"
 *
 * @pre stack != NULL
 */
char const *stack_get_type_name(dsa_stack_t const *stack);

#endif // DSA_STACK_H
