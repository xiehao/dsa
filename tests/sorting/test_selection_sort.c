/**
 * @file test_selection_sort.c
 * @brief 选择排序算法单元测试
 * @details 测试直接选择排序和冒泡排序算法
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

#include <a/sorting.h>
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
 * @brief 测试直接选择排序 - 空序列
 */
static void test_selection_sort_direct_empty(void **state) {
    (void)state; // 未使用的参数
    
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_selection_sort_direct(begin, end, int_compare, &stats);
    
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(stats.element_count, 0);
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 测试直接选择排序 - 单元素
 */
static void test_selection_sort_direct_single_element(void **state) {
    (void)state; // 未使用的参数
    
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);
    
    int value = 42;
    array_list_push_back(arr, &value);
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_selection_sort_direct(begin, end, int_compare, &stats);
    
    assert_int_equal(result, DSA_SUCCESS);
    
    // 验证元素未改变
    int *sorted_value = (int *)iterator_get_value(begin);
    assert_int_equal(*sorted_value, 42);
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 测试直接选择排序 - 已排序序列
 */
static void test_selection_sort_direct_already_sorted(void **state) {
    (void)state; // 未使用的参数
    
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);
    
    int values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        array_list_push_back(arr, &values[i]);
    }
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_selection_sort_direct(begin, end, int_compare, &stats);
    
    assert_int_equal(result, DSA_SUCCESS);
    
    // 验证序列仍然有序
    assert_true(dsa_is_sorted(begin, end, int_compare));
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 测试直接选择排序 - 逆序序列
 */
static void test_selection_sort_direct_reverse_sorted(void **state) {
    (void)state; // 未使用的参数
    
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);
    
    int values[] = {5, 4, 3, 2, 1};
    for (int i = 0; i < 5; i++) {
        array_list_push_back(arr, &values[i]);
    }
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_selection_sort_direct(begin, end, int_compare, &stats);
    
    assert_int_equal(result, DSA_SUCCESS);
    
    // 验证序列已排序
    assert_true(dsa_is_sorted(begin, end, int_compare));
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 测试冒泡排序 - 空序列
 */
static void test_selection_sort_bubble_empty(void **state) {
    (void)state; // 未使用的参数
    
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_selection_sort_bubble(begin, end, int_compare, &stats);
    
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(stats.element_count, 0);
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 测试冒泡排序 - 随机序列
 */
static void test_selection_sort_bubble_random(void **state) {
    (void)state; // 未使用的参数
    
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);
    
    int values[] = {3, 1, 4, 2, 5};
    for (int i = 0; i < 5; i++) {
        array_list_push_back(arr, &values[i]);
    }
    
    dsa_iterator_t *begin = array_list_begin(arr);
    dsa_iterator_t *end = array_list_end(arr);
    
    dsa_sort_stats_t stats;
    dsa_result_t result = dsa_selection_sort_bubble(begin, end, int_compare, &stats);
    
    assert_int_equal(result, DSA_SUCCESS);
    
    // 验证序列已排序
    assert_true(dsa_is_sorted(begin, end, int_compare));
    
    iterator_destroy(begin);
    iterator_destroy(end);
    array_list_destroy(arr);
}

/**
 * @brief 测试选择排序 - 空指针参数
 */
static void test_selection_sort_null_parameters(void **state) {
    (void)state; // 未使用的参数
    
    dsa_sort_stats_t stats;
    
    // 测试空指针参数
    assert_int_equal(dsa_selection_sort_direct(NULL, NULL, int_compare, &stats), DSA_ERROR_NULL_POINTER);
    assert_int_equal(dsa_selection_sort_bubble(NULL, NULL, int_compare, &stats), DSA_ERROR_NULL_POINTER);
}

/**
 * @brief 选择排序测试主函数
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_selection_sort_direct_empty),
        cmocka_unit_test(test_selection_sort_direct_single_element),
        cmocka_unit_test(test_selection_sort_direct_already_sorted),
        cmocka_unit_test(test_selection_sort_direct_reverse_sorted),
        cmocka_unit_test(test_selection_sort_bubble_empty),
        cmocka_unit_test(test_selection_sort_bubble_random),
        cmocka_unit_test(test_selection_sort_null_parameters),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
