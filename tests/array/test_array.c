#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include "dsa/array.h"

// 测试静态数组的统一接口
static void test_static_array_unified_interface(void **state) {
    (void)state; // 未使用

    int buffer[5];
    dsa_array_t* arr = array_create_static(buffer, 5, sizeof(int));
    assert_non_null(arr);

    // 测试初始状态
    assert_int_equal(array_size(arr), 0);
    assert_int_equal(array_capacity(arr), 5);
    assert_true(array_is_empty(arr));
    assert_false(array_is_full(arr));
    assert_int_equal(array_get_type(arr), ARRAY_TYPE_STATIC);

    // 测试类型安全的添加操作
    assert_int_equal(array_push_back_int(arr, 10), DSA_SUCCESS);
    assert_int_equal(array_push_back_int(arr, 20), DSA_SUCCESS);
    assert_int_equal(array_push_back_int(arr, 30), DSA_SUCCESS);

    assert_int_equal(array_size(arr), 3);
    assert_false(array_is_empty(arr));
    assert_false(array_is_full(arr));

    // 测试类型安全的获取操作
    int value;
    assert_int_equal(array_get_int(arr, 0, &value), DSA_SUCCESS);
    assert_int_equal(value, 10);

    assert_int_equal(array_get_int(arr, 1, &value), DSA_SUCCESS);
    assert_int_equal(value, 20);

    assert_int_equal(array_get_int(arr, 2, &value), DSA_SUCCESS);
    assert_int_equal(value, 30);

    // 测试越界访问
    assert_int_equal(array_get_int(arr, 3, &value), DSA_ERROR_INDEX_OUT_OF_BOUNDS);

    // 测试类型安全的设置操作
    assert_int_equal(array_set_int(arr, 1, 25), DSA_SUCCESS);
    assert_int_equal(array_get_int(arr, 1, &value), DSA_SUCCESS);
    assert_int_equal(value, 25);

    // 测试插入操作 - 静态数组需要使用栈上变量
    int insert_value = 15;
    assert_int_equal(array_insert(arr, 1, &insert_value), DSA_SUCCESS);
    assert_int_equal(array_size(arr), 4);

    assert_int_equal(array_get_int(arr, 1, &value), DSA_SUCCESS);
    assert_int_equal(value, 15);
    assert_int_equal(array_get_int(arr, 2, &value), DSA_SUCCESS);
    assert_int_equal(value, 25);

    // 测试移除操作 - 静态数组返回的是元素副本，需要释放
    dsa_element_pt removed = array_remove(arr, 1);
    assert_non_null(removed);
    assert_int_equal(ELEMENT_VALUE(int, removed), 15);
    free(removed); // 静态数组的remove返回malloc分配的元素副本，需要释放

    assert_int_equal(array_size(arr), 3);
    assert_int_equal(array_get_int(arr, 1, &value), DSA_SUCCESS);
    assert_int_equal(value, 25);

    // 测试弹出操作 - 静态数组返回的是元素副本，需要释放
    dsa_element_pt popped = array_pop_back(arr);
    assert_non_null(popped);
    assert_int_equal(ELEMENT_VALUE(int, popped), 30);
    free(popped); // 静态数组的pop_back返回malloc分配的元素副本，需要释放

    assert_int_equal(array_size(arr), 2);

    // 测试容量限制
    array_push_back_int(arr, 40);
    array_push_back_int(arr, 50);
    array_push_back_int(arr, 60);
    assert_true(array_is_full(arr));

    // 尝试添加到已满的数组
    assert_int_equal(array_push_back_int(arr, 70), DSA_ERROR_CAPACITY_FULL);

    array_destroy(arr);
}

// 测试动态数组的统一接口
static void test_dynamic_array_unified_interface(void **state) {
    (void)state; // 未使用

    dsa_array_t* arr = array_create_dynamic(2);
    assert_non_null(arr);

    // 测试初始状态
    assert_int_equal(array_size(arr), 0);
    assert_int_equal(array_capacity(arr), 2);
    assert_true(array_is_empty(arr));
    assert_false(array_is_full(arr)); // 动态数组永远不会满
    assert_int_equal(array_get_type(arr), ARRAY_TYPE_DYNAMIC);

    // 测试类型安全的添加操作（触发扩容）
    assert_int_equal(array_push_back_int(arr, 100), DSA_SUCCESS);
    assert_int_equal(array_push_back_int(arr, 200), DSA_SUCCESS);
    assert_int_equal(array_push_back_int(arr, 300), DSA_SUCCESS); // 应该触发扩容

    assert_int_equal(array_size(arr), 3);
    assert_true(array_capacity(arr) >= 3); // 容量应该已经扩展
    assert_false(array_is_full(arr)); // 动态数组永远不会满

    // 测试类型安全的获取操作
    int value;
    assert_int_equal(array_get_int(arr, 0, &value), DSA_SUCCESS);
    assert_int_equal(value, 100);

    assert_int_equal(array_get_int(arr, 2, &value), DSA_SUCCESS);
    assert_int_equal(value, 300);

    // 测试类型安全的设置操作
    // 对于动态数组，array_set_int函数不会释放旧元素
    // 但我们可以手动获取旧元素并释放它
    int old_value;
    array_get_int(arr, 1, &old_value); // 获取旧值用于验证
    assert_int_equal(old_value, 200);

    assert_int_equal(array_set_int(arr, 1, 250), DSA_SUCCESS);
    assert_int_equal(array_get_int(arr, 1, &value), DSA_SUCCESS);
    assert_int_equal(value, 250);

    // 测试插入操作
    int* insert_value = malloc(sizeof(int));
    *insert_value = 150;
    assert_int_equal(array_insert(arr, 1, insert_value), DSA_SUCCESS);
    assert_int_equal(array_size(arr), 4);

    assert_int_equal(array_get_int(arr, 1, &value), DSA_SUCCESS);
    assert_int_equal(value, 150);

    // 清理所有元素
    while (!array_is_empty(arr)) {
        dsa_element_pt elem = array_pop_back(arr);
        if (elem) free(elem);
    }

    array_destroy(arr);
}

// 测试双精度浮点数组操作
static void test_double_array_operations(void **state) {
    (void)state; // 未使用

    dsa_array_t* arr = array_create_dynamic(3);
    assert_non_null(arr);

    // 添加双精度浮点数
    assert_int_equal(array_push_back_double(arr, 3.14), DSA_SUCCESS);
    assert_int_equal(array_push_back_double(arr, 2.71), DSA_SUCCESS);
    assert_int_equal(array_push_back_double(arr, 1.41), DSA_SUCCESS);

    assert_int_equal(array_size(arr), 3);

    // 测试获取操作
    double value;
    assert_int_equal(array_get_double(arr, 0, &value), DSA_SUCCESS);
    assert_double_equal(value, 3.14, 0.001);

    assert_int_equal(array_get_double(arr, 1, &value), DSA_SUCCESS);
    assert_double_equal(value, 2.71, 0.001);

    // 测试设置操作
    assert_int_equal(array_set_double(arr, 1, 2.72), DSA_SUCCESS);
    assert_int_equal(array_get_double(arr, 1, &value), DSA_SUCCESS);
    assert_double_equal(value, 2.72, 0.001);

    // 清理
    while (!array_is_empty(arr)) {
        dsa_element_pt elem = array_pop_back(arr);
        if (elem) free(elem);
    }

    array_destroy(arr);
}

// 测试错误处理
static void test_error_handling(void **state) {
    (void)state; // 未使用

    // 测试无效参数
    assert_null(array_create_static(NULL, 5, sizeof(int)));

    // 测试无效容量 - 需要释放分配的内存
    void* buffer1 = malloc(20);
    assert_null(array_create_static(buffer1, 0, sizeof(int)));
    free(buffer1);

    // 测试无效元素大小 - 需要释放分配的内存
    void* buffer2 = malloc(20);
    assert_null(array_create_static(buffer2, 5, 0));
    free(buffer2);

    // 测试空指针操作
    assert_int_equal(array_size(NULL), 0);
    assert_int_equal(array_capacity(NULL), 0);
    assert_true(array_is_empty(NULL));
    assert_false(array_is_full(NULL));
    assert_null(array_get(NULL, 0));
    assert_int_equal(array_set(NULL, 0, &(int){42}), DSA_ERROR_NULL_POINTER);
    assert_int_equal(array_push_back(NULL, &(int){42}), DSA_ERROR_NULL_POINTER);
    assert_null(array_pop_back(NULL));

    // 测试类型安全函数的错误处理
    int value;
    assert_int_equal(array_get_int(NULL, 0, &value), DSA_ERROR_NULL_POINTER);
    assert_int_equal(array_set_int(NULL, 0, 42), DSA_ERROR_NULL_POINTER);
    assert_int_equal(array_push_back_int(NULL, 42), DSA_ERROR_NULL_POINTER);

    // 测试有效数组的越界访问
    int buffer[2];
    dsa_array_t* arr = array_create_static(buffer, 2, sizeof(int));
    assert_non_null(arr);

    // 空数组的越界访问
    assert_int_equal(array_get_int(arr, 0, &value), DSA_ERROR_INDEX_OUT_OF_BOUNDS);
    assert_int_equal(array_set_int(arr, 0, 42), DSA_ERROR_INDEX_OUT_OF_BOUNDS);

    // 添加一个元素后测试越界
    assert_int_equal(array_push_back_int(arr, 42), DSA_SUCCESS);
    assert_int_equal(array_get_int(arr, 1, &value), DSA_ERROR_INDEX_OUT_OF_BOUNDS);

    array_destroy(arr);
}

// 测试数组清空操作
static void test_array_clear(void **state) {
    (void)state; // 未使用

    // 测试静态数组清空
    int buffer[5];
    dsa_array_t* static_arr = array_create_static(buffer, 5, sizeof(int));
    assert_non_null(static_arr);

    array_push_back_int(static_arr, 1);
    array_push_back_int(static_arr, 2);
    array_push_back_int(static_arr, 3);
    assert_int_equal(array_size(static_arr), 3);

    array_clear(static_arr);
    assert_int_equal(array_size(static_arr), 0);
    assert_true(array_is_empty(static_arr));

    array_destroy(static_arr);

    // 测试动态数组清空
    dsa_array_t* dynamic_arr = array_create_dynamic(3);
    assert_non_null(dynamic_arr);

    array_push_back_int(dynamic_arr, 10);
    array_push_back_int(dynamic_arr, 20);
    assert_int_equal(array_size(dynamic_arr), 2);

    array_clear(dynamic_arr);
    assert_int_equal(array_size(dynamic_arr), 0);
    assert_true(array_is_empty(dynamic_arr));

    array_destroy(dynamic_arr);
}

// 测试数组清空并释放元素操作
static void test_array_clear_with_free(void **state) {
    (void)state; // 未使用

    // 测试静态数组clear_with_free（应该降级为普通clear）
    int buffer[3];
    dsa_array_t* static_arr = array_create_static(buffer, 3, sizeof(int));
    assert_non_null(static_arr);

    array_push_back_int(static_arr, 1);
    array_push_back_int(static_arr, 2);
    assert_int_equal(array_size(static_arr), 2);

    // 对静态数组调用clear_with_free应该静默降级为普通clear
    array_clear_with_free(static_arr);
    assert_int_equal(array_size(static_arr), 0);
    assert_true(array_is_empty(static_arr));

    array_destroy(static_arr);

    // 测试动态数组clear_with_free
    dsa_array_t* dynamic_arr = array_create_dynamic(3);
    assert_non_null(dynamic_arr);

    // 添加动态分配的元素
    int* val1 = malloc(sizeof(int)); *val1 = 10;
    int* val2 = malloc(sizeof(int)); *val2 = 20;
    int* val3 = malloc(sizeof(int)); *val3 = 30;

    assert_int_equal(array_push_back(dynamic_arr, val1), DSA_SUCCESS);
    assert_int_equal(array_push_back(dynamic_arr, val2), DSA_SUCCESS);
    assert_int_equal(array_push_back(dynamic_arr, val3), DSA_SUCCESS);
    assert_int_equal(array_size(dynamic_arr), 3);

    // 清空并释放所有元素
    array_clear_with_free(dynamic_arr);
    assert_int_equal(array_size(dynamic_arr), 0);
    assert_true(array_is_empty(dynamic_arr));

    // 验证数组仍然可用
    array_push_back_int(dynamic_arr, 100);
    assert_int_equal(array_size(dynamic_arr), 1);

    // 清理
    while (!array_is_empty(dynamic_arr)) {
        dsa_element_pt elem = array_pop_back(dynamic_arr);
        if (elem) free(elem);
    }

    array_destroy(dynamic_arr);

    // 测试空数组的clear_with_free
    dsa_array_t* empty_arr = array_create_dynamic(2);
    assert_non_null(empty_arr);

    array_clear_with_free(empty_arr);
    assert_int_equal(array_size(empty_arr), 0);
    assert_true(array_is_empty(empty_arr));

    array_destroy(empty_arr);

    // 测试NULL指针
    array_clear_with_free(NULL); // 应该安全处理
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_static_array_unified_interface),
        cmocka_unit_test(test_dynamic_array_unified_interface),
        cmocka_unit_test(test_double_array_operations),
        cmocka_unit_test(test_error_handling),
        cmocka_unit_test(test_array_clear),
        cmocka_unit_test(test_array_clear_with_free),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
