#include "../../include/adt/stack.h"
#include "ds/array_list.h"
#include <stdlib.h>

// 在内部定义 Stack 结构
struct Stack {
    dsa_array_list_t* array;
};

Stack* stack_create() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) {
        return NULL;
    }
    // 使用统一数组接口创建动态数组
    stack->array = array_list_create_dynamic(0);
    if (!stack->array) {
        free(stack);
        return NULL;
    }
    return stack;
}

void stack_destroy(Stack* stack) {
    if (stack) {
        // 销毁底层的数组
        array_list_destroy(stack->array);
        // 释放栈结构本身
        free(stack);
    }
}

bool stack_push(Stack* stack, void* element) {
    if (!stack) {
        return false;
    }
    // 使用统一数组接口来压入元素
    return array_list_push_back(stack->array, element) == DSA_SUCCESS;
}

void* stack_pop(Stack* stack) {
    if (!stack || array_list_is_empty(stack->array)) {
        return NULL;
    }
    // 直接使用pop_back来移除并返回最后一个元素
    return array_list_pop_back(stack->array);
}

void* stack_peek(const Stack* stack) {
    if (!stack || array_list_is_empty(stack->array)) {
        return NULL;
    }
    return array_list_get(stack->array, array_list_size(stack->array) - 1);
}

size_t stack_size(const Stack* stack) {
    return stack ? array_list_size(stack->array) : 0;
}

bool stack_is_empty(const Stack* stack) {
    return stack ? array_list_is_empty(stack->array) : true;
}