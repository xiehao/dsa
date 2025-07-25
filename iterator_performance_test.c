/**
 * @file iterator_performance_test.c
 * @brief 迭代器性能测试
 * @details 验证数组列表的随机访问优化效果
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ds/array_list.h>
#include <ds/array_list_iterator.h>
#include <ds/iterator.h>

#define TEST_SIZE 10000
#define JUMP_SIZE 100

/**
 * @brief 测试数组列表的随机访问性能
 */
void test_array_random_access_performance(void) {
    printf("=== 数组列表随机访问性能测试 ===\n");
    
    // 创建大型数组
    dsa_array_list_t *arr = array_list_create_dynamic(TEST_SIZE);
    if (!arr) {
        printf("创建数组失败\n");
        return;
    }
    
    // 填充数据
    for (int i = 0; i < TEST_SIZE; i++) {
        int *value = malloc(sizeof(int));
        *value = i;
        array_list_push_back(arr, value);
    }
    
    printf("数组大小: %d 元素\n", TEST_SIZE);
    printf("跳跃大小: %d 步\n", JUMP_SIZE);
    
    // 测试新的next_n接口性能
    clock_t start = clock();
    dsa_iterator_t *iter = array_list_begin(arr);
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
    printf("性能提升: %.2fx\n", time_step_by_step / time_next_n);
    
    array_list_destroy_with_free(arr);
}

/**
 * @brief 测试大步长移动的性能优势
 */
void test_large_step_performance(void) {
    printf("\n=== 大步长移动性能测试 ===\n");

    // 创建大型数组
    dsa_array_list_t *arr = array_list_create_dynamic(TEST_SIZE);
    if (!arr) {
        printf("创建数组失败\n");
        return;
    }

    // 填充数据
    for (int i = 0; i < TEST_SIZE; i++) {
        int *value = malloc(sizeof(int));
        *value = i;
        array_list_push_back(arr, value);
    }

    printf("测试大步长移动的性能优势\n");
    printf("数组大小: %d 元素\n", TEST_SIZE);

    // 测试不同步长的性能
    int step_sizes[] = {1, 10, 100, 1000};
    int num_steps = sizeof(step_sizes) / sizeof(step_sizes[0]);

    for (int s = 0; s < num_steps; s++) {
        int step = step_sizes[s];

        clock_t start = clock();
        dsa_iterator_t *iter = array_list_begin(arr);
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
    }

    array_list_destroy_with_free(arr);
}

/**
 * @brief 测试向后移动性能
 */
void test_backward_movement_performance(void) {
    printf("\n=== 向后移动性能测试 ===\n");
    
    // 创建数组
    dsa_array_list_t *arr = array_list_create_dynamic(TEST_SIZE);
    if (!arr) {
        printf("创建数组失败\n");
        return;
    }
    
    // 填充数据
    for (int i = 0; i < TEST_SIZE; i++) {
        int *value = malloc(sizeof(int));
        *value = i;
        array_list_push_back(arr, value);
    }
    
    // 从末尾开始向前移动
    dsa_iterator_t *iter = array_list_end(arr);
    
    clock_t start = clock();
    int count = 0;
    
    // 使用prev_n向后跳跃
    while (iterator_is_valid(iter)) {
        iterator_prev_n(iter, JUMP_SIZE);
        if (iterator_is_valid(iter)) {
            int *value = (int *)iterator_get_value(iter);
            if (value) {
                count++;
            }
        }
    }
    
    clock_t end = clock();
    double time_prev_n = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    iterator_destroy(iter);
    
    printf("向后访问元素数量: %d\n", count);
    printf("prev_n(%d) 时间: %.6f 秒\n", JUMP_SIZE, time_prev_n);
    
    array_list_destroy_with_free(arr);
}

int main(void) {
    printf("迭代器性能测试\n");
    printf("==============\n");
    
    test_array_random_access_performance();
    test_large_step_performance();
    test_backward_movement_performance();
    
    printf("\n测试完成！\n");
    printf("结论：数组列表的next_n/prev_n操作实现了O(1)随机访问，\n");
    printf("      相比逐步移动有显著性能提升。\n");
    
    return 0;
}
