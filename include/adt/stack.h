#ifndef DSA_STACK_H
#define DSA_STACK_H

#include <stdbool.h>
#include <stddef.h>

// 栈结构的不透明指针类型
typedef struct Stack Stack;

/**
 * @brief 创建一个新的空栈。
 *
 * @return 指向新创建的栈的指针，如果内存分配失败则返回 NULL。
 */
Stack* stack_create();

/**
 * @brief 销毁栈并释放其关联的内存。
 *        注意：此函数不会释放栈中存储的元素。
 *        调用者负责管理元素本身的内存。
 *
 * @param stack 指向要销毁的栈的指针。
 */
void stack_destroy(Stack* stack);

/**
 * @brief 将元素压入栈顶。
 *
 * @param stack 指向栈的指针。
 * @param element 指向要压入的元素的指针。
 * @return 如果元素压入成功则返回 true，否则返回 false（例如，内存分配失败）。
 */
bool stack_push(Stack* stack, void* element);

/**
 * @brief 从栈顶移除并返回元素。
 *
 * @param stack 指向栈的指针。
 * @return 指向从栈顶移除的元素的指针，如果栈为空则返回 NULL。
 */
void* stack_pop(Stack* stack);

/**
 * @brief 返回栈顶的元素但不移除它。
 *
 * @param stack 指向栈的指针。
 * @return 指向栈顶元素的指针，如果栈为空则返回 NULL。
 */
void* stack_peek(const Stack* stack);

/**
 * @brief 返回栈中的元素数量。
 *
 * @param stack 指向栈的指针。
 * @return 栈中的元素数量。
 */
size_t stack_size(const Stack* stack);

/**
 * @brief 检查栈是否为空。
 *
 * @param stack 指向栈的指针。
 * @return 如果栈为空则返回 true，否则返回 false。
 */
bool stack_is_empty(const Stack* stack);

#endif // DSA_STACK_H