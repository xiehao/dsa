/**
 * @file linked_list_iterator_example.c
 * @brief 链表迭代器使用示例
 * @details 演示如何使用链表迭代器遍历不同类型的链表
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <ds/linked_list.h>
#include <ds/linked_list_iterator.h>
#include <ds/iterator.h>

/**
 * @brief 演示单链表迭代器的使用
 */
void demonstrate_singly_linked_list_iterator(void) {
    printf("=== 单链表迭代器示例 ===\n");
    
    // 创建单链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    if (!list) {
        printf("创建单链表失败\n");
        return;
    }
    
    // 添加一些数据
    int data[] = {1, 2, 3, 4, 5};
    int count = sizeof(data) / sizeof(data[0]);
    
    printf("添加数据: ");
    for (int i = 0; i < count; i++) {
        linked_list_push_back(list, &data[i]);
        printf("%d ", data[i]);
    }
    printf("\n");
    
    // 使用迭代器正向遍历
    printf("正向遍历: ");
    dsa_iterator_t *iter = linked_list_begin(list);
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        printf("%d ", *value);
        iterator_next(iter);
    }
    printf("\n");
    
    // 尝试反向遍历（单链表不支持）
    printf("尝试反向遍历: ");
    dsa_iterator_t *prev_result = iterator_prev(iter);
    if (prev_result == NULL) {
        printf("单链表不支持反向遍历\n");
    }
    
    // 清理
    iterator_destroy(iter);
    linked_list_destroy(list);
    printf("\n");
}

/**
 * @brief 演示双链表迭代器的使用
 */
void demonstrate_doubly_linked_list_iterator(void) {
    printf("=== 双链表迭代器示例 ===\n");
    
    // 创建双链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    if (!list) {
        printf("创建双链表失败\n");
        return;
    }
    
    // 添加一些数据
    int data[] = {10, 20, 30, 40, 50};
    int count = sizeof(data) / sizeof(data[0]);
    
    printf("添加数据: ");
    for (int i = 0; i < count; i++) {
        linked_list_push_back(list, &data[i]);
        printf("%d ", data[i]);
    }
    printf("\n");
    
    // 使用迭代器正向遍历
    printf("正向遍历: ");
    dsa_iterator_t *iter = linked_list_begin(list);
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        printf("%d ", *value);
        iterator_next(iter);
    }
    printf("\n");
    
    // 使用迭代器反向遍历
    printf("反向遍历: ");
    dsa_iterator_t *end_iter = linked_list_end(list);
    iterator_prev(end_iter); // 移动到最后一个有效元素
    
    while (iterator_is_valid(end_iter)) {
        int *value = (int *)iterator_get_value(end_iter);
        printf("%d ", *value);
        iterator_prev(end_iter);
    }
    printf("\n");
    
    // 清理
    iterator_destroy(iter);
    iterator_destroy(end_iter);
    linked_list_destroy(list);
    printf("\n");
}

/**
 * @brief 演示循环链表迭代器的使用
 */
void demonstrate_circular_linked_list_iterator(void) {
    printf("=== 循环链表迭代器示例 ===\n");
    
    // 创建循环链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    if (!list) {
        printf("创建循环链表失败\n");
        return;
    }
    
    // 添加一些数据
    int data[] = {100, 200, 300};
    int count = sizeof(data) / sizeof(data[0]);
    
    printf("添加数据: ");
    for (int i = 0; i < count; i++) {
        linked_list_push_back(list, &data[i]);
        printf("%d ", data[i]);
    }
    printf("\n");
    
    // 使用迭代器正向遍历
    printf("正向遍历: ");
    dsa_iterator_t *iter = linked_list_begin(list);
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        printf("%d ", *value);
        iterator_next(iter);
    }
    printf("\n");
    
    // 尝试反向遍历（循环链表不支持）
    printf("尝试反向遍历: ");
    dsa_iterator_t *prev_result = iterator_prev(iter);
    if (prev_result == NULL) {
        printf("循环链表不支持反向遍历\n");
    }
    
    // 清理
    iterator_destroy(iter);
    linked_list_destroy(list);
    printf("\n");
}

/**
 * @brief 演示迭代器的修改功能
 */
void demonstrate_iterator_modification(void) {
    printf("=== 迭代器修改示例 ===\n");
    
    // 创建双链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    if (!list) {
        printf("创建双链表失败\n");
        return;
    }
    
    // 添加一些数据
    int data[] = {1, 2, 3, 4, 5};
    int count = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < count; i++) {
        linked_list_push_back(list, &data[i]);
    }
    
    printf("原始数据: ");
    dsa_iterator_t *iter = linked_list_begin(list);
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        printf("%d ", *value);
        iterator_next(iter);
    }
    printf("\n");
    
    // 使用迭代器修改数据（将每个元素乘以10）
    iterator_destroy(iter);
    iter = linked_list_begin(list);
    
    int new_data[] = {10, 20, 30, 40, 50};
    int index = 0;
    while (iterator_is_valid(iter)) {
        iterator_set_value(iter, &new_data[index]);
        iterator_next(iter);
        index++;
    }
    
    printf("修改后数据: ");
    iterator_destroy(iter);
    iter = linked_list_begin(list);
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        printf("%d ", *value);
        iterator_next(iter);
    }
    printf("\n");
    
    // 清理
    iterator_destroy(iter);
    linked_list_destroy(list);
    printf("\n");
}

/**
 * @brief 主函数
 */
int main(void) {
    printf("链表迭代器使用示例\n");
    printf("====================\n\n");
    
    demonstrate_singly_linked_list_iterator();
    demonstrate_doubly_linked_list_iterator();
    demonstrate_circular_linked_list_iterator();
    demonstrate_iterator_modification();
    
    printf("示例程序结束\n");
    return 0;
}
