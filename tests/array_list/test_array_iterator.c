/**
 * @file test_array_iterator.c
 * @brief 数组迭代器的单元测试
 * @details 测试数组迭代器的各种功能，包括正向遍历、反向遍历、边界条件等
 * @author DSA项目组
 * @date 2025-07-24
 * @version 1.0
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <ds/array_list.h>
#include <ds/iterator.h>
#include <ds/array_list_iterator.h>

/**
 * @brief 测试静态数组迭代器的正向遍历
 */
static void test_static_array_forward_iteration(void **state) {
    (void)state; // 未使用的参数
    
    // 创建静态数组
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);
    
    // 添加测试数据
    int test_values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        assert_int_equal(array_list_push_back(arr, &test_values[i]), DSA_SUCCESS);
    }
    
    // 测试正向遍历
    dsa_iterator_t *iter = array_list_begin(arr);
    assert_non_null(iter);
    
    int index = 0;
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        assert_non_null(value);
        assert_int_equal(*value, test_values[index]);
        index++;
        iterator_next(iter);
    }
    
    assert_int_equal(index, 5); // 确保遍历了所有元素
    
    // 清理
    iterator_destroy(iter);
    array_list_destroy(arr);
}

/**
 * @brief 测试动态数组迭代器的正向遍历
 */
static void test_dynamic_array_forward_iteration(void **state) {
    (void)state; // 未使用的参数
    
    // 创建动态数组
    dsa_array_list_t *arr = array_list_create_dynamic(3);
    assert_non_null(arr);
    
    // 添加测试数据
    int test_values[] = {5, 15, 25};
    for (int i = 0; i < 3; i++) {
        int *value = malloc(sizeof(int));
        assert_non_null(value);
        *value = test_values[i];
        assert_int_equal(array_list_push_back(arr, value), DSA_SUCCESS);
    }
    
    // 测试正向遍历
    dsa_iterator_t *iter = array_list_begin(arr);
    assert_non_null(iter);
    
    int index = 0;
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        assert_non_null(value);
        assert_int_equal(*value, test_values[index]);
        index++;
        iterator_next(iter);
    }
    
    assert_int_equal(index, 3); // 确保遍历了所有元素
    
    // 清理
    iterator_destroy(iter);
    array_list_destroy_with_free(arr);
}

/**
 * @brief 测试数组迭代器的反向遍历
 */
static void test_array_reverse_iteration(void **state) {
    (void)state; // 未使用的参数
    
    // 创建静态数组
    int buffer[4];
    dsa_array_list_t *arr = array_list_create_static(buffer, 4, sizeof(int));
    assert_non_null(arr);
    
    // 添加测试数据
    int test_values[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        assert_int_equal(array_list_push_back(arr, &test_values[i]), DSA_SUCCESS);
    }
    
    // 测试反向遍历
    dsa_iterator_t *iter = array_list_end(arr);
    assert_non_null(iter);
    
    iterator_prev(iter); // 移动到最后一个有效元素
    
    int index = 3; // 从最后一个元素开始
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        assert_non_null(value);
        assert_int_equal(*value, test_values[index]);
        index--;
        iterator_prev(iter);
    }
    
    assert_int_equal(index, -1); // 确保遍历了所有元素
    
    // 清理
    iterator_destroy(iter);
    array_list_destroy(arr);
}

/**
 * @brief 测试空数组的迭代器
 */
static void test_empty_array_iterator(void **state) {
    (void)state; // 未使用的参数
    
    // 创建空的静态数组
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);
    
    // 测试空数组的迭代器
    dsa_iterator_t *begin_iter = array_list_begin(arr);
    dsa_iterator_t *end_iter = array_list_end(arr);
    
    assert_non_null(begin_iter);
    assert_non_null(end_iter);
    
    // 空数组的begin应该等于end
    assert_true(iterator_equals(begin_iter, end_iter));
    
    // 空数组的迭代器应该无效
    assert_false(iterator_is_valid(begin_iter));
    assert_false(iterator_is_valid(end_iter));
    
    // 清理
    iterator_destroy(begin_iter);
    iterator_destroy(end_iter);
    array_list_destroy(arr);
}

/**
 * @brief 测试迭代器的边界条件
 */
static void test_iterator_boundary_conditions(void **state) {
    (void)state; // 未使用的参数
    
    // 创建只有一个元素的数组
    int buffer[1];
    dsa_array_list_t *arr = array_list_create_static(buffer, 1, sizeof(int));
    assert_non_null(arr);
    
    int value = 42;
    assert_int_equal(array_list_push_back(arr, &value), DSA_SUCCESS);
    
    // 测试单元素数组的迭代器
    dsa_iterator_t *iter = array_list_begin(arr);
    assert_non_null(iter);
    
    // 应该指向有效元素
    assert_true(iterator_is_valid(iter));
    int *retrieved_value = (int *)iterator_get_value(iter);
    assert_non_null(retrieved_value);
    assert_int_equal(*retrieved_value, 42);
    
    // 移动到下一个位置应该变为无效
    iterator_next(iter);
    assert_false(iterator_is_valid(iter));
    
    // 清理
    iterator_destroy(iter);
    array_list_destroy(arr);
}

/**
 * @brief 测试迭代器的读写功能
 */
static void test_iterator_read_write(void **state) {
    (void)state; // 未使用的参数

    // 测试静态数组的读写
    int buffer[5];
    dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);

    // 添加测试数据
    int test_values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        assert_int_equal(array_list_push_back(arr, &test_values[i]), DSA_SUCCESS);
    }

    // 使用迭代器修改所有元素（乘以2）
    dsa_iterator_t *iter = array_list_begin(arr);
    assert_non_null(iter);

    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        assert_non_null(value);

        int new_value = (*value) * 2;
        assert_int_equal(iterator_set_value(iter, &new_value), DSA_SUCCESS);

        // 验证修改是否成功
        int *updated_value = (int *)iterator_get_value(iter);
        assert_non_null(updated_value);
        assert_int_equal(*updated_value, new_value);

        iterator_next(iter);
    }

    iterator_destroy(iter);

    // 验证所有元素都被正确修改
    for (size_t i = 0; i < 5; i++) {
        int *value = (int *)array_list_get(arr, i);
        assert_non_null(value);
        assert_int_equal(*value, test_values[i] * 2);
    }

    array_list_destroy(arr);
}

/**
 * @brief 测试迭代器set_value的错误处理
 */
static void test_iterator_set_value_error_handling(void **state) {
    (void)state; // 未使用的参数

    int buffer[3];
    dsa_array_list_t *arr = array_list_create_static(buffer, 3, sizeof(int));
    assert_non_null(arr);

    int value = 42;
    assert_int_equal(array_list_push_back(arr, &value), DSA_SUCCESS);

    dsa_iterator_t *iter = array_list_begin(arr);
    assert_non_null(iter);

    // 测试NULL迭代器
    assert_int_equal(iterator_set_value(NULL, &value), DSA_ERROR_INVALID_PARAMETER);

    // 测试NULL值
    assert_int_equal(iterator_set_value(iter, NULL), DSA_ERROR_INVALID_PARAMETER);

    // 移动到末尾位置（无效位置）
    dsa_iterator_t *end_iter = array_list_end(arr);
    assert_non_null(end_iter);

    // 尝试在无效位置设置值
    assert_int_equal(iterator_set_value(end_iter, &value), DSA_ERROR_INDEX_OUT_OF_BOUNDS);

    iterator_destroy(iter);
    iterator_destroy(end_iter);
    array_list_destroy(arr);
}

/**
 * @brief 测试动态数组迭代器的读写功能
 */
static void test_dynamic_array_iterator_read_write(void **state) {
    (void)state; // 未使用的参数

    dsa_array_list_t *arr = array_list_create_dynamic(3);
    assert_non_null(arr);

    // 添加测试数据
    for (int i = 0; i < 3; i++) {
        int *value = malloc(sizeof(int));
        assert_non_null(value);
        *value = i + 1;
        assert_int_equal(array_list_push_back(arr, value), DSA_SUCCESS);
    }

    // 使用迭代器修改元素
    dsa_iterator_t *iter = array_list_begin(arr);
    assert_non_null(iter);

    int index = 0;
    while (iterator_is_valid(iter)) {
        int *new_value = malloc(sizeof(int));
        assert_non_null(new_value);
        *new_value = (index + 1) * 10;  // 10, 20, 30

        assert_int_equal(iterator_set_value(iter, new_value), DSA_SUCCESS);

        // 验证修改
        int *updated_value = (int *)iterator_get_value(iter);
        assert_non_null(updated_value);
        assert_int_equal(*updated_value, (index + 1) * 10);

        index++;
        iterator_next(iter);
    }

    iterator_destroy(iter);
    array_list_destroy_with_free(arr);
}

/**
 * @brief 测试NULL指针的处理
 */
static void test_iterator_null_handling(void **state) {
    (void)state; // 未使用的参数

    // 测试NULL数组
    assert_null(array_list_begin(NULL));
    assert_null(array_list_end(NULL));

    // 测试NULL迭代器
    assert_null(iterator_next(NULL));
    assert_null(iterator_prev(NULL));
    assert_null(iterator_get_value(NULL));
    assert_false(iterator_is_valid(NULL));
    assert_true(iterator_equals(NULL, NULL));
    assert_false(iterator_equals(NULL, (dsa_iterator_t *)0x1));

    // 测试set_value的NULL处理
    int value = 42;
    assert_int_equal(iterator_set_value(NULL, &value), DSA_ERROR_INVALID_PARAMETER);

    // iterator_destroy应该安全处理NULL
    iterator_destroy(NULL); // 不应该崩溃
}

/**
 * @brief 主测试函数
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_static_array_forward_iteration),
        cmocka_unit_test(test_dynamic_array_forward_iteration),
        cmocka_unit_test(test_array_reverse_iteration),
        cmocka_unit_test(test_empty_array_iterator),
        cmocka_unit_test(test_iterator_boundary_conditions),
        cmocka_unit_test(test_iterator_read_write),
        cmocka_unit_test(test_iterator_set_value_error_handling),
        cmocka_unit_test(test_dynamic_array_iterator_read_write),
        cmocka_unit_test(test_iterator_null_handling),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
