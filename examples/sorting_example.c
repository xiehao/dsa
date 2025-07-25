/**
 * @file sorting_example.c
 * @brief 排序算法使用示例
 * @details 演示如何使用各种排序算法对不同容器进行排序
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ds/sorting.h>
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
    
    int a = ELEMENT_VALUE(int, lhs);
    int b = ELEMENT_VALUE(int, rhs);
    
    return (a > b) - (a < b);
}

/**
 * @brief 打印数组内容
 */
static void print_array(dsa_array_list_t *arr, const char *title) {
    printf("%s: ", title);
    
    dsa_iterator_t *iter = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    while (iterator_is_valid(iter) && !iterator_equals(iter, end)) {
        int *value = (int *)iterator_get_value(iter);
        if (value) {
            printf("%d ", *value);
        }
        iterator_next(iter);
    }
    printf("\n");
    
    iterator_destroy(iter);
    iterator_destroy(end);
}

/**
 * @brief 打印链表内容
 */
static void print_linked_list(dsa_linked_list_t *list, const char *title) {
    printf("%s: ", title);
    
    dsa_iterator_t *iter = linked_list_begin(list);
    dsa_iterator_t *end = linked_list_end(list);
    
    while (iterator_is_valid(iter) && !iterator_equals(iter, end)) {
        int *value = (int *)iterator_get_value(iter);
        if (value) {
            printf("%d ", *value);
        }
        iterator_next(iter);
    }
    printf("\n");
    
    iterator_destroy(iter);
    iterator_destroy(end);
}

/**
 * @brief 演示静态数组排序
 */
static void demo_static_array_sorting(void) {
    printf("=== 静态数组排序演示 ===\n");
    
    // 创建静态数组
    int buffer[10];
    dsa_array_list_t *arr = array_list_create_static(buffer, 10, sizeof(int));
    if (!arr) {
        printf("创建静态数组失败\n");
        return;
    }
    
    // 添加测试数据
    int test_data[] = {64, 34, 25, 12, 22, 11, 90};
    int data_size = sizeof(test_data) / sizeof(test_data[0]);
    
    for (int i = 0; i < data_size; i++) {
        array_list_push_back(arr, &test_data[i]);
    }
    
    print_array(arr, "原始数据");
    
    // 测试直接插入排序
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_insertion_sort_direct(begin, end, int_compare, &stats);
    
    if (result == DSA_SUCCESS) {
        print_array(arr, "直接插入排序后");
        dsa_sort_stats_print(&stats, DSA_SORT_INSERTION_DIRECT);
    } else {
        printf("直接插入排序失败\n");
    }
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 演示统一排序框架
 */
static void demo_unified_sorting_framework(void) {
    printf("\n=== 统一排序框架演示 ===\n");

    // 创建静态数组
    int buffer[6];
    dsa_array_list_t *arr = array_list_create_static(buffer, 6, sizeof(int));
    if (!arr) {
        printf("创建静态数组失败\n");
        return;
    }

    // 添加测试数据
    int test_data[] = {5, 2, 8, 1, 9, 3};
    for (int i = 0; i < 6; i++) {
        array_list_push_back(arr, &test_data[i]);
    }

    print_array(arr, "原始数据");

    // 测试统一插入排序框架 - 二分策略
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);

    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_insertion_sort(begin, end, int_compare, INSERTION_STRATEGY_BINARY, &stats);

    if (result == DSA_SUCCESS) {
        print_array(arr, "统一框架二分插入排序后");
        dsa_sort_stats_print(&stats, DSA_SORT_INSERTION_BINARY);
    } else {
        printf("统一框架二分插入排序失败\n");
    }

    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 演示链表排序
 */
static void demo_linked_list_sorting(void) {
    printf("\n=== 链表排序演示 ===\n");
    
    // 创建双向链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    if (!list) {
        printf("创建链表失败\n");
        return;
    }
    
    // 添加测试数据
    int test_data[] = {7, 3, 9, 1, 5, 4, 8, 2, 6};
    int data_size = sizeof(test_data) / sizeof(test_data[0]);
    
    for (int i = 0; i < data_size; i++) {
        int *value = malloc(sizeof(int));
        if (value) {
            *value = test_data[i];
            linked_list_push_back(list, value);
        }
    }
    
    print_linked_list(list, "原始数据");
    
    // 测试直接选择排序
    dsa_iterator_t *begin = linked_list_begin(list);
    dsa_iterator_t *end = linked_list_end(list);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_selection_sort_direct(begin, end, int_compare, &stats);
    
    if (result == DSA_SUCCESS) {
        print_linked_list(list, "直接选择排序后");
        dsa_sort_stats_print(&stats, DSA_SORT_SELECTION_DIRECT);
    } else {
        printf("直接选择排序失败\n");
    }
    
    iterator_destroy(begin);
    iterator_destroy(end);

    // 手动释放链表中的元素
    while (!linked_list_is_empty(list)) {
        int *element = (int *)linked_list_pop_front(list);
        if (element) {
            free(element);
        }
    }

    linked_list_destroy(list);
}

/**
 * @brief 演示冒泡排序
 */
static void demo_bubble_sort(void) {
    printf("\n=== 冒泡排序演示 ===\n");
    
    // 创建静态数组
    int buffer[8];
    dsa_array_list_t *arr = array_list_create_static(buffer, 8, sizeof(int));
    if (!arr) {
        printf("创建静态数组失败\n");
        return;
    }
    
    // 添加测试数据
    int test_data[] = {64, 34, 25, 12, 22, 11, 90, 88};
    int data_size = sizeof(test_data) / sizeof(test_data[0]);
    
    for (int i = 0; i < data_size; i++) {
        array_list_push_back(arr, &test_data[i]);
    }
    
    print_array(arr, "原始数据");
    
    // 测试冒泡排序
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_selection_sort_bubble(begin, end, int_compare, &stats);
    
    if (result == DSA_SUCCESS) {
        print_array(arr, "冒泡排序后");
        dsa_sort_stats_print(&stats, DSA_SORT_SELECTION_BUBBLE);
    } else {
        printf("冒泡排序失败\n");
    }
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 演示排序工具函数
 */
static void demo_sorting_utilities(void) {
    printf("\n=== 排序工具函数演示 ===\n");
    
    // 创建静态数组
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    if (!arr) {
        printf("创建静态数组失败\n");
        return;
    }
    
    // 添加已排序数据
    int sorted_data[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        array_list_push_back(arr, &sorted_data[i]);
    }
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    // 测试is_sorted函数
    bool is_sorted = dsa_is_sorted(begin, end, int_compare);
    printf("序列 [1, 2, 3, 4, 5] 是否已排序: %s\n", is_sorted ? "是" : "否");
    
    // 测试迭代器交换
    dsa_iterator_t *first = array_list_begin(arr);
    dsa_iterator_t *second = array_list_begin(arr);
    iterator_next(second);
    
    printf("交换前: ");
    print_array(arr, "");
    
    dsa_result_t swap_result = dsa_iterator_swap(first, second);
    if (swap_result == DSA_SUCCESS) {
        printf("交换后: ");
        print_array(arr, "");
    } else {
        printf("交换失败\n");
    }
    
    iterator_destroy(begin);
    iterator_destroy(end);
    iterator_destroy(first);
    iterator_destroy(second);
    array_list_destroy(arr);
}

/**
 * @brief 主函数
 */
int main(void) {
    printf("排序算法演示程序\n");
    printf("================\n");
    
    // 演示各种排序算法
    demo_static_array_sorting();
    demo_unified_sorting_framework();
    demo_linked_list_sorting();
    demo_bubble_sort();
    demo_sorting_utilities();
    
    printf("\n演示完成！\n");
    return 0;
}
