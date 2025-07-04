/**
 * @file array_stack.h
 * @brief 数组栈内部接口声明
 * @details 定义了基于动态数组实现的栈的内部创建函数
 * @author oaheixiehao
 * @date 2025/7/3
 * @version 1.0
 */

#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H

#include <adt/stack.h>

/**
 * @brief 创建一个新的数组栈实例
 * @return 新创建的栈指针，如果内存分配失败则返回NULL
 * @note 时间复杂度：O(1)
 * @details 创建一个基于动态数组的栈，初始容量为0，支持自动扩容
 * @warning 调用者负责管理返回的栈指针的生命周期
 */
dsa_stack_t *array_stack_create(void);

#endif //ARRAY_STACK_H
