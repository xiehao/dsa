#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include "../src/array/internal/dynamic_array.h"
#include "dsa/common.h" // 包含通用定义

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
static void test_dynamic_array_push_back_get(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(2);
    assert_non_null(arr);

    int* val1 = malloc(sizeof(int)); *val1 = 10;
    int* val2 = malloc(sizeof(int)); *val2 = 20;

    dynamic_array_push_back(arr, val1);
    dynamic_array_push_back(arr, val2);

    assert_int_equal(dynamic_array_size(arr), 2);
    int* retrieved_val1 = (int*)dynamic_array_get(arr, 0);
    int* retrieved_val2 = (int*)dynamic_array_get(arr, 1);
    assert_non_null(retrieved_val1);
    assert_non_null(retrieved_val2);
    assert_int_equal(ELEMENT_VALUE(int, retrieved_val1), 10);
    assert_int_equal(ELEMENT_VALUE(int, retrieved_val2), 20);

    // 注意：CMocka 不会自动释放添加到数组中的数据。
    // 动态数组现在拥有这些指针，因此我们在销毁时释放它们。
    dynamic_array_destroy_with_free(arr);
}

// 添加元素触发调整大小的测试用例
static void test_dynamic_array_push_back_resize(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(1); // 从小容量开始
    assert_non_null(arr);
    assert_int_equal(dynamic_array_capacity(arr), 1);

    int* val1 = malloc(sizeof(int)); *val1 = 1;
    dynamic_array_push_back(arr, val1);
    assert_int_equal(dynamic_array_size(arr), 1);
    assert_int_equal(dynamic_array_capacity(arr), 1);

    int* val2 = malloc(sizeof(int)); *val2 = 2;
    dynamic_array_push_back(arr, val2); // 这应该触发调整大小
    assert_int_equal(dynamic_array_size(arr), 2);
    assert_true(dynamic_array_capacity(arr) > 1); // 容量应该增加了

    int* val3 = malloc(sizeof(int)); *val3 = 3;
    dynamic_array_push_back(arr, val3); // 可能触发另一次调整大小
    assert_int_equal(dynamic_array_size(arr), 3);
    assert_true(dynamic_array_capacity(arr) >= 3);

    // 验证调整大小后元素仍然正确
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 1);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 1)), 2);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 2)), 3);

    dynamic_array_destroy_with_free(arr);
}

// 设置元素测试用例
static void test_dynamic_array_set(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(5);
    int* val1 = malloc(sizeof(int)); *val1 = 1;
    int* val2 = malloc(sizeof(int)); *val2 = 2;
    dynamic_array_push_back(arr, val1);
    dynamic_array_push_back(arr, val2);

    int* new_val = malloc(sizeof(int)); *new_val = 99;

    // 保存旧值，因为 dynamic_array_set 不会释放它
    void* old_val = dynamic_array_get(arr, 1);
    dsa_result_t result = dynamic_array_set(arr, 1, new_val);
    // 根据函数文档，该函数不释放旧元素，由调用者负责
    // 手动释放旧值
    free(old_val);

    assert_int_equal(result, DSA_SUCCESS); // 验证操作成功

    assert_int_equal(dynamic_array_size(arr), 2);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 1);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 1)), 99);

    dynamic_array_destroy_with_free(arr);
}

// 大小和容量测试用例
static void test_dynamic_array_size_capacity(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(3);
    assert_int_equal(dynamic_array_size(arr), 0);
    assert_int_equal(dynamic_array_capacity(arr), 3);

    int* val1 = malloc(sizeof(int)); *val1 = 1;
    dynamic_array_push_back(arr, val1);
    assert_int_equal(dynamic_array_size(arr), 1);
    assert_int_equal(dynamic_array_capacity(arr), 3);

    int* val2 = malloc(sizeof(int)); *val2 = 2;
    dynamic_array_push_back(arr, val2);
    int* val3 = malloc(sizeof(int)); *val3 = 3;
    dynamic_array_push_back(arr, val3);
    assert_int_equal(dynamic_array_size(arr), 3);
    assert_int_equal(dynamic_array_capacity(arr), 3);

    int* val4 = malloc(sizeof(int)); *val4 = 4;
    dynamic_array_push_back(arr, val4); // 调整大小
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

    // 测试在空数组上设置元素
    int* val = malloc(sizeof(int)); *val = 42;
    assert_int_equal(dynamic_array_set(arr, 0, val), DSA_ERROR_INDEX_OUT_OF_BOUNDS); // 在空数组上设置应返回索引越界错误
    free(val); // 释放未设置的元素

    dynamic_array_destroy(arr);
}

// 无效索引访问测试用例
static void test_dynamic_array_invalid_index(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(5);
    int* val1 = malloc(sizeof(int)); *val1 = 1;
    dynamic_array_push_back(arr, val1);

    assert_null(dynamic_array_get(arr, 1)); // 索引越界（大小为 1）
    assert_null(dynamic_array_get(arr, -1)); // 负索引

    // 测试 set 函数的索引越界
    int* val_test = malloc(sizeof(int)); *val_test = 100;
    assert_int_equal(dynamic_array_set(arr, 1, val_test), DSA_ERROR_INDEX_OUT_OF_BOUNDS); // 索引越界
    free(val_test); // 需要释放未成功设置的元素

    // 测试负索引
    int* val_neg = malloc(sizeof(int)); *val_neg = 200;
    assert_int_equal(dynamic_array_set(arr, (size_t)-1, val_neg), DSA_ERROR_INDEX_OUT_OF_BOUNDS); // 负索引
    free(val_neg); // 需要释放未成功设置的元素

    dynamic_array_destroy_with_free(arr);
}

// 插入元素测试用例
static void test_dynamic_array_insert(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(2);
    assert_non_null(arr);

    // 1. 在末尾插入 (等同于 add)
    int* val1 = malloc(sizeof(int)); *val1 = 10;
    assert_int_equal(dynamic_array_insert(arr, 0, val1), DSA_SUCCESS); // 插入到空数组索引 0
    assert_int_equal(dynamic_array_size(arr), 1);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 10);

    int* val2 = malloc(sizeof(int)); *val2 = 20;
    assert_int_equal(dynamic_array_insert(arr, 1, val2), DSA_SUCCESS); // 插入到末尾索引 1
    assert_int_equal(dynamic_array_size(arr), 2);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 10);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 1)), 20);

    // 2. 在开头插入
    int* val0 = malloc(sizeof(int)); *val0 = 5;
    assert_int_equal(dynamic_array_insert(arr, 0, val0), DSA_SUCCESS); // 插入到开头索引 0
    assert_int_equal(dynamic_array_size(arr), 3);
    assert_true(dynamic_array_capacity(arr) >= 3); // 可能触发扩容
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 5);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 1)), 10);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 2)), 20);

    // 3. 在中间插入
    int* val1_5 = malloc(sizeof(int)); *val1_5 = 15;
    assert_int_equal(dynamic_array_insert(arr, 2, val1_5), DSA_SUCCESS); // 插入到中间索引 2
    assert_int_equal(dynamic_array_size(arr), 4);
    assert_true(dynamic_array_capacity(arr) >= 4);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 5);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 1)), 10);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 2)), 15);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 3)), 20);

    // 4. 插入无效索引
    int* val_invalid = malloc(sizeof(int)); *val_invalid = 100;
    assert_int_equal(dynamic_array_insert(arr, 5, val_invalid), DSA_ERROR_INDEX_OUT_OF_BOUNDS); // 索引 5 越界 (size 是 4)
    assert_int_equal(dynamic_array_size(arr), 4); // 大小不变
    free(val_invalid); // 释放未插入的数据

    // 5. 插入触发扩容 (已在上面测试过)

    dynamic_array_destroy_with_free(arr);
}

// 移除元素测试用例
static void test_dynamic_array_remove(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(5);
    assert_non_null(arr);

    // 准备数据
    int* val1 = malloc(sizeof(int)); *val1 = 10;
    int* val2 = malloc(sizeof(int)); *val2 = 20;
    int* val3 = malloc(sizeof(int)); *val3 = 30;
    int* val4 = malloc(sizeof(int)); *val4 = 40;
    dynamic_array_push_back(arr, val1);
    dynamic_array_push_back(arr, val2);
    dynamic_array_push_back(arr, val3);
    dynamic_array_push_back(arr, val4);
    // 当前数组: [10, 20, 30, 40], size = 4

    // 1. 从中间移除
    void* removed = dynamic_array_remove(arr, 1); // 移除 20
    assert_non_null(removed);
    assert_int_equal(ELEMENT_VALUE(int, removed), 20);
    free(removed); // 释放被移除元素的数据
    assert_int_equal(dynamic_array_size(arr), 3);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 10);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 1)), 30); // 30 移到索引 1
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 2)), 40);
    // 当前数组: [10, 30, 40], size = 3

    // 2. 从末尾移除
    removed = dynamic_array_remove(arr, 2); // 移除 40
    assert_non_null(removed);
    assert_int_equal(ELEMENT_VALUE(int, removed), 40);
    free(removed);
    assert_int_equal(dynamic_array_size(arr), 2);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 10);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 1)), 30);
    // 当前数组: [10, 30], size = 2

    // 3. 从开头移除
    removed = dynamic_array_remove(arr, 0); // 移除 10
    assert_non_null(removed);
    assert_int_equal(ELEMENT_VALUE(int, removed), 10);
    free(removed);
    assert_int_equal(dynamic_array_size(arr), 1);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 30); // 30 移到索引 0
    // 当前数组: [30], size = 1

    // 4. 移除最后一个元素
    removed = dynamic_array_remove(arr, 0); // 移除 30
    assert_non_null(removed);
    assert_int_equal(ELEMENT_VALUE(int, removed), 30);
    free(removed);
    assert_int_equal(dynamic_array_size(arr), 0);
    // 当前数组: [], size = 0

    // 5. 从空数组移除
    removed = dynamic_array_remove(arr, 0);
    assert_null(removed);
    assert_int_equal(dynamic_array_size(arr), 0);

    // 6. 移除无效索引
    // 先添加一个元素
    int* val5 = malloc(sizeof(int)); *val5 = 50;
    dynamic_array_push_back(arr, val5);
    assert_int_equal(dynamic_array_size(arr), 1);
    removed = dynamic_array_remove(arr, 1); // 索引 1 越界
    assert_null(removed);
    assert_int_equal(dynamic_array_size(arr), 1); // 大小不变
    removed = dynamic_array_remove(arr, -1); // 负索引
    assert_null(removed);
    assert_int_equal(dynamic_array_size(arr), 1);

    dynamic_array_destroy_with_free(arr); // 释放 val5
}

// 测试动态数组清空操作
static void test_dynamic_array_clear(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(3);
    assert_non_null(arr);

    // 添加一些元素
    int* val1 = malloc(sizeof(int)); *val1 = 10;
    int* val2 = malloc(sizeof(int)); *val2 = 20;
    int* val3 = malloc(sizeof(int)); *val3 = 30;
    dynamic_array_push_back(arr, val1);
    dynamic_array_push_back(arr, val2);
    dynamic_array_push_back(arr, val3);
    assert_int_equal(dynamic_array_size(arr), 3);

    // 测试普通clear
    dynamic_array_clear(arr);
    assert_int_equal(dynamic_array_size(arr), 0);
    assert_true(dynamic_array_is_empty(arr));

    // 验证容量保持不变
    assert_int_equal(dynamic_array_capacity(arr), 3);

    // 验证数组仍然可用
    int* val4 = malloc(sizeof(int)); *val4 = 40;
    dynamic_array_push_back(arr, val4);
    assert_int_equal(dynamic_array_size(arr), 1);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 40);

    // 清理：注意val1, val2, val3的内存没有被释放，这里手动释放
    free(val1);
    free(val2);
    free(val3);
    dynamic_array_destroy_with_free(arr); // 释放val4
}

// 测试动态数组清空并释放元素操作
static void test_dynamic_array_clear_with_free(void **state) {
    (void) state; // 未使用
    DynamicArray* arr = dynamic_array_create(3);
    assert_non_null(arr);

    // 添加一些元素
    int* val1 = malloc(sizeof(int)); *val1 = 10;
    int* val2 = malloc(sizeof(int)); *val2 = 20;
    int* val3 = malloc(sizeof(int)); *val3 = 30;
    dynamic_array_push_back(arr, val1);
    dynamic_array_push_back(arr, val2);
    dynamic_array_push_back(arr, val3);
    assert_int_equal(dynamic_array_size(arr), 3);

    // 测试clear_with_free
    dynamic_array_clear_with_free(arr);
    assert_int_equal(dynamic_array_size(arr), 0);
    assert_true(dynamic_array_is_empty(arr));

    // 验证容量保持不变
    assert_int_equal(dynamic_array_capacity(arr), 3);

    // 验证数组仍然可用
    int* val4 = malloc(sizeof(int)); *val4 = 40;
    dynamic_array_push_back(arr, val4);
    assert_int_equal(dynamic_array_size(arr), 1);
    assert_int_equal(ELEMENT_VALUE(int, dynamic_array_get(arr, 0)), 40);

    dynamic_array_destroy_with_free(arr); // 释放val4

    // 测试空数组的clear_with_free
    DynamicArray* empty_arr = dynamic_array_create(2);
    assert_non_null(empty_arr);

    dynamic_array_clear_with_free(empty_arr);
    assert_int_equal(dynamic_array_size(empty_arr), 0);
    assert_true(dynamic_array_is_empty(empty_arr));

    dynamic_array_destroy(empty_arr);

    // 测试NULL指针
    dynamic_array_clear_with_free(NULL); // 应该安全处理
    dynamic_array_clear(NULL); // 应该安全处理
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dynamic_array_creation),
        cmocka_unit_test(test_dynamic_array_push_back_get),
        cmocka_unit_test(test_dynamic_array_push_back_resize),
        cmocka_unit_test(test_dynamic_array_set),
        cmocka_unit_test(test_dynamic_array_size_capacity),
        cmocka_unit_test(test_dynamic_array_empty),
        cmocka_unit_test(test_dynamic_array_invalid_index),
        cmocka_unit_test(test_dynamic_array_insert),
        cmocka_unit_test(test_dynamic_array_remove),
        cmocka_unit_test(test_dynamic_array_clear),
        cmocka_unit_test(test_dynamic_array_clear_with_free),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}