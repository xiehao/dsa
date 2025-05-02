#include "dsa/stack.h"
#include "dsa/dynamic_array.h"
#include <stdlib.h>

// 在内部定义 Stack 结构
struct Stack {
    DynamicArray* array;
};

Stack* stack_create() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) {
        return NULL;
    }
    // 使用默认初始容量或传递 0 让 dynamic_array 处理。
    stack->array = dynamic_array_create(0);
    if (!stack->array) {
        free(stack);
        return NULL;
    }
    return stack;
}

void stack_destroy(Stack* stack) {
    if (stack) {
        // 销毁底层的动态数组
        dynamic_array_destroy(stack->array);
        // 释放栈结构本身
        free(stack);
    }
}

bool stack_push(Stack* stack, void* element) {
    if (!stack) {
        return false;
    }
    // 使用 dynamic_array_push_back 来压入元素
    return dynamic_array_push_back(stack->array, element);
}

void* stack_pop(Stack* stack) {
    if (!stack || dynamic_array_is_empty(stack->array)) {
        return NULL;
    }
    // 直接使用pop_back来移除并返回最后一个元素
    // pop_back会自动处理数组大小的更新
    return dynamic_array_pop_back(stack->array);
}

void* stack_peek(const Stack* stack) {
    if (!stack || dynamic_array_is_empty(stack->array)) {
        return NULL;
    }
    return dynamic_array_get(stack->array, dynamic_array_size(stack->array) - 1);
}

size_t stack_size(const Stack* stack) {
    return stack ? dynamic_array_size(stack->array) : 0;
}

bool stack_is_empty(const Stack* stack) {
    return stack ? dynamic_array_is_empty(stack->array) : true;
}