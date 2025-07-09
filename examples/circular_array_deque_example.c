/**
 * @file circular_array_deque_example.c
 * @brief 循环数组双端队列使用示例
 * @author DSA Team
 * @date 2025-07-09
 */

#include <stdio.h>
#include <stdlib.h>
#include <adt/deque.h>

// 辅助函数：创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    if (data) {
        *data = value;
    }
    return data;
}

// 辅助函数：打印双端队列状态
static void print_deque_status(dsa_deque_t *deque, const char* operation) {
    printf("操作: %s\n", operation);
    printf("  大小: %zu\n", deque_size(deque));
    printf("  是否为空: %s\n", deque_is_empty(deque) ? "是" : "否");
    
    if (!deque_is_empty(deque)) {
        int* first = (int*)deque_peek_first(deque);
        int* last = (int*)deque_peek_last(deque);
        printf("  队首元素: %d\n", first ? *first : 0);
        printf("  队尾元素: %d\n", last ? *last : 0);
    }
    printf("\n");
}

int main(void) {
    printf("=== 循环数组双端队列示例 ===\n\n");
    
    // 创建双端队列
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_CIRCULAR_ARRAY_LIST);
    if (!deque) {
        fprintf(stderr, "错误：无法创建双端队列\n");
        return 1;
    }
    
    printf("双端队列类型: %s\n\n", deque_get_type_name(deque));
    print_deque_status(deque, "创建空双端队列");
    
    // 从前端添加元素
    printf("--- 从前端添加元素 ---\n");
    for (int i = 1; i <= 3; i++) {
        int* data = create_int(i * 10);
        if (deque_add_first(deque, data) == DSA_SUCCESS) {
            char operation[50];
            snprintf(operation, sizeof(operation), "从前端添加 %d", i * 10);
            print_deque_status(deque, operation);
        }
    }
    
    // 从后端添加元素
    printf("--- 从后端添加元素 ---\n");
    for (int i = 1; i <= 3; i++) {
        int* data = create_int(i * 100);
        if (deque_add_last(deque, data) == DSA_SUCCESS) {
            char operation[50];
            snprintf(operation, sizeof(operation), "从后端添加 %d", i * 100);
            print_deque_status(deque, operation);
        }
    }
    
    // 测试扩容 - 添加更多元素
    printf("--- 测试自动扩容 ---\n");
    for (int i = 1; i <= 5; i++) {
        int* data = create_int(i * 1000);
        if (deque_add_first(deque, data) == DSA_SUCCESS) {
            char operation[50];
            snprintf(operation, sizeof(operation), "扩容测试：添加 %d", i * 1000);
            print_deque_status(deque, operation);
        }
    }
    
    // 从前端移除元素
    printf("--- 从前端移除元素 ---\n");
    for (int i = 0; i < 3; i++) {
        int* removed = (int*)deque_remove_first(deque);
        if (removed) {
            char operation[50];
            snprintf(operation, sizeof(operation), "从前端移除 %d", *removed);
            print_deque_status(deque, operation);
            free(removed);
        }
    }
    
    // 从后端移除元素
    printf("--- 从后端移除元素 ---\n");
    for (int i = 0; i < 3; i++) {
        int* removed = (int*)deque_remove_last(deque);
        if (removed) {
            char operation[50];
            snprintf(operation, sizeof(operation), "从后端移除 %d", *removed);
            print_deque_status(deque, operation);
            free(removed);
        }
    }
    
    // 清空队列
    printf("--- 清空队列 ---\n");
    while (!deque_is_empty(deque)) {
        int* removed = (int*)deque_remove_first(deque);
        if (removed) {
            printf("移除元素: %d\n", *removed);
            free(removed);
        }
    }
    print_deque_status(deque, "清空队列");
    
    // 测试空队列操作
    printf("--- 测试空队列操作 ---\n");
    printf("从空队列移除前端元素: %s\n", 
           deque_remove_first(deque) ? "成功" : "失败（预期）");
    printf("从空队列移除后端元素: %s\n", 
           deque_remove_last(deque) ? "成功" : "失败（预期）");
    printf("查看空队列前端元素: %s\n", 
           deque_peek_first(deque) ? "有元素" : "无元素（预期）");
    printf("查看空队列后端元素: %s\n", 
           deque_peek_last(deque) ? "有元素" : "无元素（预期）");
    
    // 销毁双端队列
    deque_destroy(deque);
    printf("\n双端队列已销毁\n");
    printf("=== 示例结束 ===\n");
    
    return 0;
}
