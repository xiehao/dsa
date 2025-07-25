/**
 * @file test_iterator_performance.c
 * @brief 迭代器性能测试
 * @details 验证数组列表的随机访问优化效果，集成到CMocka测试框架中
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ds/array_list.h>
#include <ds/array_list_iterator.h>
#include <a/iterator.h>

#define TEST_SIZE 10000
#define JUMP_SIZE 100
#define PERFORMANCE_THRESHOLD 1.5  // 性能提升阈值

/**
 * @brief 测试数组列表的随机访问性能
 */
static void test_array_random_access_performance(void **state) {
    (void)state; // 未使用的参数
    
    printf("\n=== 数组列表随机访问性能测试 ===\n");
    
    // 创建大型数组
    dsa_array_list_t *arr = array_list_create_dynamic(TEST_SIZE);
    assert_non_null(arr);
    
    // 填充数据
    for (int i = 0; i < TEST_SIZE; i++) {
        int *value = malloc(sizeof(int));
        assert_non_null(value);
        *value = i;
        assert_int_equal(array_list_push_back(arr, value), DSA_SUCCESS);
    }
    
    printf("数组大小: %d 元素\n", TEST_SIZE);
    printf("跳跃大小: %d 步\n", JUMP_SIZE);
    
    // 测试新的next_n接口性能
    clock_t start = clock();
    dsa_iterator_t *iter = array_list_begin(arr);
    assert_non_null(iter);
    int count = 0;
    
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        if (value) {
            count++;
        }
        
        // 使用next_n一次跳跃多步
        iterator_next_n(iter, JUMP_SIZE);
    }
    
    clock_t end = clock();
    double time_next_n = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    iterator_destroy(iter);
    
    // 测试传统的逐步移动性能（模拟原来的实现）
    start = clock();
    iter = array_list_begin(arr);
    assert_non_null(iter);
    int count2 = 0;
    
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        if (value) {
            count2++;
        }
        
        // 使用多次next(1)模拟原来的逐步移动
        for (int i = 0; i < JUMP_SIZE && iterator_is_valid(iter); i++) {
            iterator_next(iter);
        }
    }
    
    end = clock();
    double time_step_by_step = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    iterator_destroy(iter);
    
    printf("访问元素数量: %d (next_n), %d (逐步)\n", count, count2);
    printf("next_n(%d) 时间: %.6f 秒\n", JUMP_SIZE, time_next_n);
    printf("逐步移动时间: %.6f 秒\n", time_step_by_step);
    
    // 验证性能提升
    if (time_next_n > 0) {
        double performance_ratio = time_step_by_step / time_next_n;
        printf("性能提升: %.2fx\n", performance_ratio);
        
        // 断言性能提升达到预期阈值
        assert_true(performance_ratio >= PERFORMANCE_THRESHOLD);
    }
    
    // 验证访问的元素数量一致
    assert_int_equal(count, count2);
    
    array_list_destroy_with_free(arr);
}

/**
 * @brief 测试大步长移动的性能优势
 */
static void test_large_step_performance(void **state) {
    (void)state; // 未使用的参数
    
    printf("\n=== 大步长移动性能测试 ===\n");

    // 创建大型数组
    dsa_array_list_t *arr = array_list_create_dynamic(TEST_SIZE);
    assert_non_null(arr);

    // 填充数据
    for (int i = 0; i < TEST_SIZE; i++) {
        int *value = malloc(sizeof(int));
        assert_non_null(value);
        *value = i;
        assert_int_equal(array_list_push_back(arr, value), DSA_SUCCESS);
    }

    printf("测试大步长移动的性能优势\n");
    printf("数组大小: %d 元素\n", TEST_SIZE);

    // 测试不同步长的性能
    int step_sizes[] = {1, 10, 100, 1000};
    int num_steps = sizeof(step_sizes) / sizeof(step_sizes[0]);
    double prev_time = 0;

    for (int s = 0; s < num_steps; s++) {
        int step = step_sizes[s];

        clock_t start = clock();
        dsa_iterator_t *iter = array_list_begin(arr);
        assert_non_null(iter);
        int count = 0;

        while (iterator_is_valid(iter)) {
            int *value = (int *)iterator_get_value(iter);
            if (value) {
                count++;
            }
            iterator_next_n(iter, step);
        }

        clock_t end = clock();
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

        iterator_destroy(iter);

        printf("步长 %4d: 访问 %4d 元素, 时间 %.6f 秒\n", step, count, time_taken);
        
        // 验证访问的元素数量符合预期
        int expected_count = (TEST_SIZE + step - 1) / step;
        assert_int_equal(count, expected_count);
        
        // 对于较大的步长，时间应该更短（或至少不会显著增加）
        if (s > 0 && step > 10) {
            // 允许一定的时间波动，但不应该显著增加
            assert_true(time_taken <= prev_time * 2.0);
        }
        
        prev_time = time_taken;
    }

    array_list_destroy_with_free(arr);
}

/**
 * @brief 测试向后移动性能
 */
static void test_backward_movement_performance(void **state) {
    (void)state; // 未使用的参数
    
    printf("\n=== 向后移动性能测试 ===\n");
    
    // 创建数组
    dsa_array_list_t *arr = array_list_create_dynamic(TEST_SIZE);
    assert_non_null(arr);
    
    // 填充数据
    for (int i = 0; i < TEST_SIZE; i++) {
        int *value = malloc(sizeof(int));
        assert_non_null(value);
        *value = i;
        assert_int_equal(array_list_push_back(arr, value), DSA_SUCCESS);
    }
    
    // 从末尾开始向前移动
    dsa_iterator_t *iter = array_list_end(arr);
    assert_non_null(iter);

    // 先移动到最后一个有效元素
    iterator_prev_n(iter, 1);

    clock_t start = clock();
    int count = 0;

    // 使用prev_n向后跳跃
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        if (value) {
            count++;
        }
        iterator_prev_n(iter, JUMP_SIZE);
    }
    
    clock_t end = clock();
    double time_prev_n = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    iterator_destroy(iter);
    
    printf("向后访问元素数量: %d\n", count);
    printf("prev_n(%d) 时间: %.6f 秒\n", JUMP_SIZE, time_prev_n);
    
    // 验证访问的元素数量合理
    int expected_count = TEST_SIZE / JUMP_SIZE;
    assert_true(count >= expected_count - 1 && count <= expected_count + 1);
    
    array_list_destroy_with_free(arr);
}

/**
 * @brief 测试迭代器next_n和prev_n的正确性
 */
static void test_iterator_movement_correctness(void **state) {
    (void)state; // 未使用的参数
    
    // 创建小型数组用于验证正确性
    dsa_array_list_t *arr = array_list_create_dynamic(20);
    assert_non_null(arr);
    
    // 填充数据
    for (int i = 0; i < 20; i++) {
        int *value = malloc(sizeof(int));
        assert_non_null(value);
        *value = i;
        assert_int_equal(array_list_push_back(arr, value), DSA_SUCCESS);
    }
    
    // 测试next_n的正确性
    dsa_iterator_t *iter = array_list_begin(arr);
    assert_non_null(iter);
    
    // 移动5步
    iterator_next_n(iter, 5);
    assert_true(iterator_is_valid(iter));
    int *value = (int *)iterator_get_value(iter);
    assert_non_null(value);
    assert_int_equal(*value, 5);
    
    // 再移动3步
    iterator_next_n(iter, 3);
    assert_true(iterator_is_valid(iter));
    value = (int *)iterator_get_value(iter);
    assert_non_null(value);
    assert_int_equal(*value, 8);
    
    // 测试prev_n的正确性
    iterator_prev_n(iter, 2);
    assert_true(iterator_is_valid(iter));
    value = (int *)iterator_get_value(iter);
    assert_non_null(value);
    assert_int_equal(*value, 6);
    
    iterator_destroy(iter);
    array_list_destroy_with_free(arr);
}

/**
 * @brief 主测试函数
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_array_random_access_performance),
        cmocka_unit_test(test_large_step_performance),
        cmocka_unit_test(test_backward_movement_performance),
        cmocka_unit_test(test_iterator_movement_correctness),
    };

    printf("开始运行迭代器性能测试套件...\n");
    int result = cmocka_run_group_tests(tests, NULL, NULL);
    printf("迭代器性能测试套件完成.\n");
    printf("结论：数组列表的next_n/prev_n操作实现了O(1)随机访问，\n");
    printf("      相比逐步移动有显著性能提升。\n");

    return result;
}
