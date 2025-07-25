/**
 * @file simple_sort_test.c
 * @brief 简单的排序算法测试
 */

#include <stdio.h>
#include <stdlib.h>
#include <ds/sorting.h>
#include <ds/array_list.h>
#include <ds/array_list_iterator.h>

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

int main(void) {
    printf("简单排序算法测试\n");
    printf("================\n");
    
    // 创建静态数组
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    if (!arr) {
        printf("创建静态数组失败\n");
        return 1;
    }
    
    // 添加测试数据
    int test_data[] = {5, 2, 8, 1, 3};
    for (int i = 0; i < 5; i++) {
        array_list_push_back(arr, &test_data[i]);
    }
    
    print_array(arr, "原始数据");
    
    // 测试直接插入排序
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_insertion_sort_direct(begin, end, int_compare, &stats);
    
    if (result == DSA_SUCCESS) {
        print_array(arr, "插入排序后");
        printf("比较次数: %zu, 交换次数: %zu\n", stats.comparisons, stats.swaps);
    } else {
        printf("插入排序失败，错误码: %d\n", result);
    }
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
    
    printf("测试完成！\n");
    return 0;
}
