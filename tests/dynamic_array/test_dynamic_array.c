#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include "dsa/dynamic_array.h"

// 动态数组创建测试用例
static void test_dynamic_array_creation(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(10);
    assert_non_null(arr);
    assert_int_equal(dynamic_array_size(arr), 0);
    assert_int_equal(dynamic_array_capacity(arr), 10);
    dynamic_array_destroy(arr);
}

// 添加和获取元素测试用例
static void test_dynamic_array_add_get(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(2);
    assert_non_null(arr);

    int* val1 = malloc(sizeof(int)); *val1 = 10;
    int* val2 = malloc(sizeof(int)); *val2 = 20;

    dynamic_array_add(arr, val1);
    dynamic_array_add(arr, val2);

    assert_int_equal(dynamic_array_size(arr), 2);
    int* retrieved_val1 = (int*)dynamic_array_get(arr, 0);
    int* retrieved_val2 = (int*)dynamic_array_get(arr, 1);
    assert_non_null(retrieved_val1);
    assert_non_null(retrieved_val2);
    assert_int_equal(*retrieved_val1, 10);
    assert_int_equal(*retrieved_val2, 20);

    // 注意：CMocka 不会自动释放添加到数组中的数据。
    // 动态数组现在拥有这些指针，因此我们在销毁时释放它们。
    dynamic_array_destroy_with_free(arr);
}

// 添加元素触发调整大小的测试用例
static void test_dynamic_array_add_resize(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(1); // 从小容量开始
    assert_non_null(arr);
    assert_int_equal(dynamic_array_capacity(arr), 1);

    int* val1 = malloc(sizeof(int)); *val1 = 1;
    dynamic_array_add(arr, val1);
    assert_int_equal(dynamic_array_size(arr), 1);
    assert_int_equal(dynamic_array_capacity(arr), 1);

    int* val2 = malloc(sizeof(int)); *val2 = 2;
    dynamic_array_add(arr, val2); // 这应该触发调整大小
    assert_int_equal(dynamic_array_size(arr), 2);
    assert_true(dynamic_array_capacity(arr) > 1); // 容量应该增加了

    int* val3 = malloc(sizeof(int)); *val3 = 3;
    dynamic_array_add(arr, val3); // 可能触发另一次调整大小
    assert_int_equal(dynamic_array_size(arr), 3);
    assert_true(dynamic_array_capacity(arr) >= 3);

    // 验证调整大小后元素仍然正确
    assert_int_equal(*(int*)dynamic_array_get(arr, 0), 1);
    assert_int_equal(*(int*)dynamic_array_get(arr, 1), 2);
    assert_int_equal(*(int*)dynamic_array_get(arr, 2), 3);

    dynamic_array_destroy_with_free(arr);
}

// 设置元素测试用例
static void test_dynamic_array_set(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(5);
    int* val1 = malloc(sizeof(int)); *val1 = 1;
    int* val2 = malloc(sizeof(int)); *val2 = 2;
    dynamic_array_add(arr, val1);
    dynamic_array_add(arr, val2);

    int* new_val = malloc(sizeof(int)); *new_val = 99;
    void* old_val = dynamic_array_set(arr, 1, new_val);

    assert_non_null(old_val);
    assert_int_equal(*(int*)old_val, 2); // 检查被替换的值
    free(old_val); // 释放被替换元素的数据

    assert_int_equal(dynamic_array_size(arr), 2);
    assert_int_equal(*(int*)dynamic_array_get(arr, 0), 1);
    assert_int_equal(*(int*)dynamic_array_get(arr, 1), 99);

    dynamic_array_destroy_with_free(arr);
}

// 大小和容量测试用例
static void test_dynamic_array_size_capacity(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(3);
    assert_int_equal(dynamic_array_size(arr), 0);
    assert_int_equal(dynamic_array_capacity(arr), 3);

    int* val1 = malloc(sizeof(int)); *val1 = 1;
    dynamic_array_add(arr, val1);
    assert_int_equal(dynamic_array_size(arr), 1);
    assert_int_equal(dynamic_array_capacity(arr), 3);

    int* val2 = malloc(sizeof(int)); *val2 = 2;
    dynamic_array_add(arr, val2);
    int* val3 = malloc(sizeof(int)); *val3 = 3;
    dynamic_array_add(arr, val3);
    assert_int_equal(dynamic_array_size(arr), 3);
    assert_int_equal(dynamic_array_capacity(arr), 3);

    int* val4 = malloc(sizeof(int)); *val4 = 4;
    dynamic_array_add(arr, val4); // 调整大小
    assert_int_equal(dynamic_array_size(arr), 4);
    assert_true(dynamic_array_capacity(arr) > 3);

    dynamic_array_destroy_with_free(arr);
}

// 空数组操作测试用例
static void test_dynamic_array_empty(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(5);
    assert_int_equal(dynamic_array_size(arr), 0);
    assert_null(dynamic_array_get(arr, 0)); // 在空数组或越界时获取
    assert_null(dynamic_array_set(arr, 0, NULL)); // 在空数组或越界时设置
    dynamic_array_destroy(arr);
}

// 无效索引访问测试用例
static void test_dynamic_array_invalid_index(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(5);
    int* val1 = malloc(sizeof(int)); *val1 = 1;
    dynamic_array_add(arr, val1);

    assert_null(dynamic_array_get(arr, 1)); // 索引越界（大小为 1）
    assert_null(dynamic_array_get(arr, -1)); // 负索引
    assert_null(dynamic_array_set(arr, 1, NULL)); // 索引越界
    assert_null(dynamic_array_set(arr, -1, NULL)); // 负索引

    dynamic_array_destroy_with_free(arr);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dynamic_array_creation),
        cmocka_unit_test(test_dynamic_array_add_get),
        cmocka_unit_test(test_dynamic_array_add_resize),
        cmocka_unit_test(test_dynamic_array_set),
        cmocka_unit_test(test_dynamic_array_size_capacity),
        cmocka_unit_test(test_dynamic_array_empty),
        cmocka_unit_test(test_dynamic_array_invalid_index),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}