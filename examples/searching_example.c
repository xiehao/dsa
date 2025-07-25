/**
 * @file searching_example.c
 * @brief 查找算法使用示例
 * @details 演示如何使用查找算法库进行线性查找、二分查找等操作
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <a/searching.h>
#include <ds/array_list.h>
#include <ds/array_list_iterator.h>
#include <ds/linked_list.h>
#include <ds/linked_list_iterator.h>

/**
 * @brief 整数比较函数
 */
static int int_compare(dsa_const_element_pt lhs, dsa_const_element_pt rhs) {
    if (!lhs || !rhs) {
        return 0;
    }
    
    int a = *(const int*)lhs;
    int b = *(const int*)rhs;
    
    return (a > b) - (a < b);
}

/**
 * @brief 打印数组内容
 */
static void print_array(dsa_array_list_t *arr, const char *title) {
    printf("%s: ", title);
    
    dsa_iterator_t *iter = array_list_begin(arr);
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        printf("%d ", *value);
        iterator_next(iter);
    }
    printf("\n");
    
    iterator_destroy(iter);
}

/**
 * @brief 演示线性查找
 */
static void demo_linear_search(void) {
    printf("=== 线性查找演示 ===\n");
    
    // 创建数组并添加数据
    int buffer[10];
    dsa_array_list_t *arr = array_list_create_static(buffer, 10, sizeof(int));
    
    int values[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    for (int i = 0; i < 10; i++) {
        array_list_push_back(arr, &values[i]);
    }
    
    print_array(arr, "原始数组");
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    // 查找元素5
    int target = 5;
    dsa_search_stats_t stats;
    dsa_iterator_t *result = dsa_linear_search(begin, end, &target, int_compare, &stats);
    
    if (result && !iterator_equals(result, end)) {
        int *found = (int*)iterator_get_value(result);
        printf("线性查找找到元素: %d\n", *found);
        dsa_search_stats_print(&stats, DSA_SEARCH_STRATEGY_LINEAR);
    } else {
        printf("线性查找未找到元素: %d\n", target);
    }
    
    if (result) iterator_destroy(result);
    
    // 查找不存在的元素
    int not_found = 8;
    result = dsa_linear_search(begin, end, &not_found, int_compare, &stats);
    
    if (result && !iterator_equals(result, end)) {
        int *found = (int*)iterator_get_value(result);
        printf("线性查找找到元素: %d\n", *found);
    } else {
        printf("线性查找未找到元素: %d\n", not_found);
        dsa_search_stats_print(&stats, DSA_SEARCH_STRATEGY_LINEAR);
    }
    
    if (result) iterator_destroy(result);
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
    printf("\n");
}

/**
 * @brief 演示二分查找
 */
static void demo_binary_search(void) {
    printf("=== 二分查找演示 ===\n");
    
    // 创建已排序的数组
    int buffer[10];
    dsa_array_list_t *arr = array_list_create_static(buffer, 10, sizeof(int));
    
    int sorted_values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i = 0; i < 10; i++) {
        array_list_push_back(arr, &sorted_values[i]);
    }
    
    print_array(arr, "已排序数组");
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    // 查找元素7
    int target = 7;
    dsa_search_stats_t stats;
    dsa_iterator_t *result = dsa_binary_search(begin, end, &target, int_compare, &stats);
    
    if (result && !iterator_equals(result, end)) {
        int *found = (int*)iterator_get_value(result);
        printf("二分查找找到元素: %d\n", *found);
        dsa_search_stats_print(&stats, DSA_SEARCH_STRATEGY_BINARY);
    } else {
        printf("二分查找未找到元素: %d\n", target);
    }
    
    if (result) iterator_destroy(result);
    
    // 查找不存在的元素
    int not_found = 15;
    result = dsa_binary_search(begin, end, &not_found, int_compare, &stats);
    
    if (result && !iterator_equals(result, end)) {
        int *found = (int*)iterator_get_value(result);
        printf("二分查找找到元素: %d\n", *found);
    } else {
        printf("二分查找未找到元素: %d\n", not_found);
        dsa_search_stats_print(&stats, DSA_SEARCH_STRATEGY_BINARY);
    }
    
    if (result) iterator_destroy(result);
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
    printf("\n");
}

/**
 * @brief 演示插入位置查找
 */
static void demo_insert_position_search(void) {
    printf("=== 插入位置查找演示 ===\n");
    
    // 创建已排序的数组
    int buffer[10];
    dsa_array_list_t *arr = array_list_create_static(buffer, 10, sizeof(int));
    
    int sorted_values[] = {1, 3, 5, 7, 9};
    for (int i = 0; i < 5; i++) {
        array_list_push_back(arr, &sorted_values[i]);
    }
    
    print_array(arr, "已排序数组");
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    // 查找插入位置
    int values_to_insert[] = {0, 2, 4, 6, 8, 10};
    int num_values = sizeof(values_to_insert) / sizeof(values_to_insert[0]);
    
    for (int i = 0; i < num_values; i++) {
        int value = values_to_insert[i];
        dsa_search_stats_t stats;
        
        // 使用线性查找插入位置
        dsa_iterator_t *pos = dsa_linear_search_insert_position(begin, end, &value, int_compare, &stats);
        if (pos) {
            printf("线性查找: 元素 %d 的插入位置找到\n", value);
            dsa_search_stats_print(&stats, DSA_SEARCH_STRATEGY_LINEAR);
            iterator_destroy(pos);
        }
        
        // 使用二分查找插入位置
        pos = dsa_binary_search_insert_position(begin, end, &value, int_compare, &stats);
        if (pos) {
            printf("二分查找: 元素 %d 的插入位置找到\n", value);
            dsa_search_stats_print(&stats, DSA_SEARCH_STRATEGY_BINARY);
            iterator_destroy(pos);
        }
        printf("\n");
    }
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 演示统一查找框架
 */
static void demo_unified_search_framework(void) {
    printf("=== 统一查找框架演示 ===\n");
    
    // 创建已排序的数组
    int buffer[8];
    dsa_array_list_t *arr = array_list_create_static(buffer, 8, sizeof(int));
    
    int sorted_values[] = {2, 4, 6, 8, 10, 12, 14, 16};
    for (int i = 0; i < 8; i++) {
        array_list_push_back(arr, &sorted_values[i]);
    }
    
    print_array(arr, "已排序数组");
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    int target = 10;
    dsa_search_stats_t stats;
    
    // 使用统一框架进行线性查找
    dsa_iterator_t *result = dsa_search(begin, end, &target, int_compare, 
                                       DSA_SEARCH_STRATEGY_LINEAR, &stats);
    if (result && !iterator_equals(result, end)) {
        int *found = (int*)iterator_get_value(result);
        printf("统一框架线性查找找到: %d\n", *found);
        dsa_search_stats_print(&stats, DSA_SEARCH_STRATEGY_LINEAR);
    }
    if (result) iterator_destroy(result);
    
    // 使用统一框架进行二分查找
    result = dsa_search(begin, end, &target, int_compare, 
                       DSA_SEARCH_STRATEGY_BINARY, &stats);
    if (result && !iterator_equals(result, end)) {
        int *found = (int*)iterator_get_value(result);
        printf("统一框架二分查找找到: %d\n", *found);
        dsa_search_stats_print(&stats, DSA_SEARCH_STRATEGY_BINARY);
    }
    if (result) iterator_destroy(result);
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
    printf("\n");
}

/**
 * @brief 演示最小元素查找
 */
static void demo_min_search(void) {
    printf("=== 最小元素查找演示 ===\n");
    
    // 创建无序数组
    int buffer[8];
    dsa_array_list_t *arr = array_list_create_static(buffer, 8, sizeof(int));
    
    int values[] = {7, 3, 9, 1, 5, 8, 2, 6};
    for (int i = 0; i < 8; i++) {
        array_list_push_back(arr, &values[i]);
    }
    
    print_array(arr, "无序数组");
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_search_stats_t stats;
    dsa_iterator_t *min_iter = dsa_linear_search_min(begin, end, int_compare, &stats);
    
    if (min_iter) {
        int *min_value = (int*)iterator_get_value(min_iter);
        printf("找到最小元素: %d\n", *min_value);
        dsa_search_stats_print(&stats, DSA_SEARCH_STRATEGY_LINEAR);
        iterator_destroy(min_iter);
    }
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
    printf("\n");
}

/**
 * @brief 主函数
 */
int main(void) {
    printf("查找算法演示程序\n");
    printf("================\n\n");
    
    demo_linear_search();
    demo_binary_search();
    demo_insert_position_search();
    demo_unified_search_framework();
    demo_min_search();
    
    printf("演示完成！\n");
    return 0;
}
