/**
 * @file linked_stack.h
 * @brief 链表栈内部接口声明
 * @details 定义了基于单链表实现的栈的内部创建函数
 * @author oaheixiehao
 * @date 2025/7/4
 * @version 1.0
 */

#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include <adt/stack.h>

/**
 * @brief 创建一个新的链表栈实例
 * @return 新创建的栈指针，如果内存分配失败则返回NULL
 * @note 时间复杂度：O(1)
 * @details 创建一个基于单链表的栈，完全动态内存分配，无容量限制
 * @warning 调用者负责管理返回的栈指针的生命周期
 */
dsa_stack_t *linked_stack_create(void);

#endif //LINKED_STACK_H
