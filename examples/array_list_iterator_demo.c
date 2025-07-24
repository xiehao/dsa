/**
 * @file array_list_iterator_demo.c
 * @brief 数组列表迭代器使用示例
 * @details 演示如何使用数组列表的迭代器进行遍历操作
 * @author DSA项目组
 * @date 2025-07-24
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <ds/array_list.h>
#include <ds/iterator.h>
#include <ds/array_list_iterator.h>
#include <common.h>

/**
 * @brief 演示静态数组迭代器的使用
 */
static void demo_static_array_iterator(void) {
    printf("=== 静态数组迭代器演示 ===\n");
    
    // 创建静态数组
    int buffer[10];
    dsa_array_list_t *static_arr = array_list_create_static(buffer, 10, sizeof(int));
    if (!static_arr) {
        printf("创建静态数组失败\n");
        return;
    }
    
    // 添加一些元素
    for (int i = 0; i < 5; i++) {
        int value = (i + 1) * 10;
        array_list_push_back(static_arr, &value);
    }
    
    printf("数组大小: %zu\n", array_list_size(static_arr));
    
    // 使用迭代器正向遍历
    printf("正向遍历: ");
    dsa_iterator_t *iter = array_list_begin(static_arr);
    dsa_iterator_t *end_iter = array_list_end(static_arr);
    
    while (iterator_is_valid(iter) && !iterator_equals(iter, end_iter)) {
        int *value = (int *)iterator_get_value(iter);
        if (value) {
            printf("%d ", *value);
        }
        iterator_next(iter);
    }
    printf("\n");
    
    // 清理迭代器
    iterator_destroy(iter);
    iterator_destroy(end_iter);
    
    // 清理数组
    array_list_destroy(static_arr);
}

/**
 * @brief 演示动态数组迭代器的使用
 */
static void demo_dynamic_array_iterator(void) {
    printf("\n=== 动态数组迭代器演示 ===\n");

    // 创建动态数组
    dsa_array_list_t *dynamic_arr = array_list_create_dynamic(5);
    if (!dynamic_arr) {
        printf("创建动态数组失败\n");
        return;
    }

    printf("动态数组创建成功\n");

    // 添加一些元素（使用栈上的值，避免内存管理复杂性）
    for (int i = 0; i < 3; i++) {
        int *value = malloc(sizeof(int));
        if (!value) {
            printf("内存分配失败\n");
            array_list_destroy(dynamic_arr);
            return;
        }
        *value = (i + 1) * 5;
        dsa_result_t result = array_list_push_back(dynamic_arr, value);
        if (result != DSA_SUCCESS) {
            printf("添加元素失败: %d\n", result);
            free(value);
            array_list_destroy_with_free(dynamic_arr);
            return;
        }
    }

    printf("数组大小: %zu\n", array_list_size(dynamic_arr));

    // 使用迭代器正向遍历
    printf("正向遍历: ");
    dsa_iterator_t *iter = array_list_begin(dynamic_arr);
    if (!iter) {
        printf("创建迭代器失败\n");
        array_list_destroy_with_free(dynamic_arr);
        return;
    }

    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        if (value) {
            printf("%d ", *value);
        }
        iterator_next(iter);
    }
    printf("\n");

    // 清理迭代器
    iterator_destroy(iter);

    // 清理动态数组（包括元素内存）
    array_list_destroy_with_free(dynamic_arr);
    printf("动态数组演示完成\n");
}

/**
 * @brief 演示迭代器的反向遍历
 */
static void demo_reverse_iteration(void) {
    printf("\n=== 反向遍历演示 ===\n");
    
    // 创建静态数组
    int buffer[10];
    dsa_array_list_t *arr = array_list_create_static(buffer, 10, sizeof(int));
    if (!arr) {
        printf("创建数组失败\n");
        return;
    }
    
    // 添加元素
    for (int i = 0; i < 6; i++) {
        int value = i + 1;
        array_list_push_back(arr, &value);
    }
    
    printf("数组内容: ");
    
    // 从末尾开始反向遍历
    dsa_iterator_t *iter = array_list_end(arr);
    iterator_prev(iter);  // 移动到最后一个有效元素
    
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        if (value) {
            printf("%d ", *value);
        }
        iterator_prev(iter);
    }
    printf("\n");
    
    // 清理
    iterator_destroy(iter);
    array_list_destroy(arr);
}

/**
 * @brief 主函数
 */
int main(void) {
    printf("数组列表迭代器演示程序\n");
    printf("========================\n");

    demo_static_array_iterator();
    demo_dynamic_array_iterator();
    demo_reverse_iteration();

    printf("\n演示完成！\n");
    return 0;
}
