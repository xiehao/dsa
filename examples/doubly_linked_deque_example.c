/**
 * @file doubly_linked_deque_example.c
 * @brief 双向链表双端队列使用示例
 * @author DSA Team
 * @date 2025-07-10
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

// 辅助函数：打印队列中的所有元素（从前到后）
static void print_all_elements(dsa_deque_t *deque) {
    if (deque_is_empty(deque)) {
        printf("队列为空\n");
        return;
    }
    
    printf("队列元素（从前到后）: ");
    size_t size = deque_size(deque);
    
    // 临时存储移除的元素
    int* temp[size];
    
    // 从前端移除所有元素并存储
    for (size_t i = 0; i < size; i++) {
        temp[i] = (int*)deque_remove_first(deque);
        printf("%d ", temp[i] ? *temp[i] : 0);
    }
    printf("\n");
    
    // 将元素按原顺序放回队列
    for (size_t i = 0; i < size; i++) {
        deque_add_last(deque, temp[i]);
    }
}

int main(void) {
    printf("=== 双向链表双端队列示例 ===\n\n");
    
    // 创建双端队列
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
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
    
    print_all_elements(deque);
    printf("\n");
    
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
    
    print_all_elements(deque);
    printf("\n");
    
    // 演示链表的动态特性 - 添加大量元素
    printf("--- 演示链表动态特性（添加大量元素）---\n");
    for (int i = 1; i <= 10; i++) {
        int* data = create_int(i * 1000);
        if (deque_add_first(deque, data) == DSA_SUCCESS) {
            if (i % 3 == 0) {  // 每3个元素打印一次状态
                char operation[50];
                snprintf(operation, sizeof(operation), "批量添加：已添加 %d 个元素", i);
                print_deque_status(deque, operation);
            }
        }
    }
    
    printf("最终状态：\n");
    print_deque_status(deque, "添加大量元素后");
    
    // 从前端移除元素
    printf("--- 从前端移除元素 ---\n");
    for (int i = 0; i < 5; i++) {
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
    for (int i = 0; i < 5; i++) {
        int* removed = (int*)deque_remove_last(deque);
        if (removed) {
            char operation[50];
            snprintf(operation, sizeof(operation), "从后端移除 %d", *removed);
            print_deque_status(deque, operation);
            free(removed);
        }
    }
    
    // 演示混合操作
    printf("--- 演示混合操作 ---\n");
    printf("交替进行前端添加、后端添加、前端移除、后端移除操作\n");
    
    for (int i = 1; i <= 5; i++) {
        // 前端添加
        int* data1 = create_int(i * 10000);
        deque_add_first(deque, data1);
        printf("前端添加 %d，", *data1);
        
        // 后端添加
        int* data2 = create_int(i * 20000);
        deque_add_last(deque, data2);
        printf("后端添加 %d，", *data2);
        
        // 前端移除
        if (!deque_is_empty(deque)) {
            int* removed1 = (int*)deque_remove_first(deque);
            printf("前端移除 %d，", removed1 ? *removed1 : 0);
            free(removed1);
        }
        
        // 后端移除
        if (!deque_is_empty(deque)) {
            int* removed2 = (int*)deque_remove_last(deque);
            printf("后端移除 %d", removed2 ? *removed2 : 0);
            free(removed2);
        }
        
        printf(" -> 当前大小: %zu\n", deque_size(deque));
    }
    
    printf("\n");
    print_deque_status(deque, "混合操作后");
    
    // 清空队列
    printf("--- 清空队列 ---\n");
    size_t remaining = deque_size(deque);
    printf("清空前队列大小: %zu\n", remaining);
    
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
    
    // 性能对比说明
    printf("\n--- 双向链表双端队列特点 ---\n");
    printf("优点：\n");
    printf("  1. 动态内存分配，理论上无容量限制\n");
    printf("  2. 插入和删除操作时间复杂度为 O(1)\n");
    printf("  3. 不需要预分配大块连续内存\n");
    printf("  4. 适合频繁插入删除的场景\n");
    printf("缺点：\n");
    printf("  1. 每个节点需要额外的指针开销\n");
    printf("  2. 内存访问局部性较差\n");
    printf("  3. 不支持随机访问\n");
    printf("  4. 相比数组实现有更多的内存分配开销\n");
    
    // 销毁双端队列
    deque_destroy(deque);
    printf("\n双端队列已销毁\n");
    printf("=== 示例结束 ===\n");
    
    return 0;
}
