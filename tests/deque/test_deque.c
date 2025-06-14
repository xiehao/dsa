#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h> // Include for malloc/free

#include "../../include/adt/deque.h"

// 辅助函数，用于创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

// 双端队列创建测试用例
static void test_deque_creation(void **state) {
    (void) state; // 未使用
    Deque *d = deque_create();
    assert_non_null(d);
    assert_true(deque_is_empty(d));
    assert_int_equal(deque_size(d), 0);
    deque_destroy(d, NULL); // 尚无数据需要释放
}

// 测试向队首添加元素
static void test_deque_add_first(void **state) {
    (void) state;
    Deque *d = deque_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);

    deque_add_first(d, data1);
    assert_false(deque_is_empty(d));
    assert_int_equal(deque_size(d), 1);
    assert_ptr_equal(deque_peek_first(d), data1);
    assert_ptr_equal(deque_peek_last(d), data1);

    deque_add_first(d, data2);
    assert_int_equal(deque_size(d), 2);
    assert_ptr_equal(deque_peek_first(d), data2); // 新的队首
    assert_ptr_equal(deque_peek_last(d), data1); // 队尾保持不变

    deque_destroy(d, free);
}

// 测试向队尾添加元素
static void test_deque_add_last(void **state) {
    (void) state;
    Deque *d = deque_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);

    deque_add_last(d, data1);
    assert_false(deque_is_empty(d));
    assert_int_equal(deque_size(d), 1);
    assert_ptr_equal(deque_peek_first(d), data1);
    assert_ptr_equal(deque_peek_last(d), data1);

    deque_add_last(d, data2);
    assert_int_equal(deque_size(d), 2);
    assert_ptr_equal(deque_peek_first(d), data1); // 队首保持不变
    assert_ptr_equal(deque_peek_last(d), data2); // 新的队尾

    deque_destroy(d, free);
}

// 测试从队首移除元素
static void test_deque_remove_first(void **state) {
    (void) state;
    Deque *d = deque_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    deque_add_last(d, data1);
    deque_add_last(d, data2);

    void* removed_data = deque_remove_first(d);
    assert_ptr_equal(removed_data, data1);
    assert_int_equal(deque_size(d), 1);
    assert_ptr_equal(deque_peek_first(d), data2);
    assert_ptr_equal(deque_peek_last(d), data2);
    assert_int_equal(*(int*)removed_data, 10); // 验证移除的元素内容
    free(removed_data);

    removed_data = deque_remove_first(d);
    assert_ptr_equal(removed_data, data2);
    assert_int_equal(deque_size(d), 0);
    assert_true(deque_is_empty(d));
    assert_null(deque_peek_first(d));
    assert_null(deque_peek_last(d));
    assert_int_equal(*(int*)removed_data, 20); // 验证移除的元素内容
    free(removed_data);

    // 测试从空双端队列移除
    removed_data = deque_remove_first(d);
    assert_null(removed_data);
    assert_int_equal(deque_size(d), 0);

    deque_destroy(d, free);
}

// 测试从队尾移除元素
static void test_deque_remove_last(void **state) {
    (void) state;
    Deque *d = deque_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    deque_add_last(d, data1);
    deque_add_last(d, data2);

    void* removed_data = deque_remove_last(d);
    assert_ptr_equal(removed_data, data2);
    assert_int_equal(deque_size(d), 1);
    assert_ptr_equal(deque_peek_first(d), data1);
    assert_ptr_equal(deque_peek_last(d), data1);
    assert_int_equal(*(int*)removed_data, 20); // 验证移除的元素内容
    free(removed_data);

    removed_data = deque_remove_last(d);
    assert_ptr_equal(removed_data, data1);
    assert_int_equal(deque_size(d), 0);
    assert_true(deque_is_empty(d));
    assert_null(deque_peek_first(d));
    assert_null(deque_peek_last(d));
    assert_int_equal(*(int*)removed_data, 10); // 验证移除的元素内容
    free(removed_data);

    // 测试从空双端队列移除
    removed_data = deque_remove_last(d);
    assert_null(removed_data);
    assert_int_equal(deque_size(d), 0);

    deque_destroy(d, free);
}

// 测试查看元素
static void test_deque_peek(void **state) {
    (void) state;
    Deque *d = deque_create();

    // 在空双端队列上查看
    assert_null(deque_peek_first(d));
    assert_null(deque_peek_last(d));

    int* data1 = create_int(10);
    deque_add_first(d, data1);
    assert_ptr_equal(deque_peek_first(d), data1);
    assert_ptr_equal(deque_peek_last(d), data1);

    int* data2 = create_int(20);
    deque_add_last(d, data2);
    assert_ptr_equal(deque_peek_first(d), data1);
    assert_ptr_equal(deque_peek_last(d), data2);

    int* data3 = create_int(5);
    deque_add_first(d, data3);
    assert_ptr_equal(deque_peek_first(d), data3);
    assert_ptr_equal(deque_peek_last(d), data2);

    deque_destroy(d, free);
}

// 测试双端队列大小和空状态
static void test_deque_size_empty(void **state) {
    (void) state;
    Deque *d = deque_create();
    assert_true(deque_is_empty(d));
    assert_int_equal(deque_size(d), 0);

    int* data1 = create_int(1);
    deque_add_first(d, data1);
    assert_false(deque_is_empty(d));
    assert_int_equal(deque_size(d), 1);

    int* data2 = create_int(2);
    deque_add_last(d, data2);
    assert_false(deque_is_empty(d));
    assert_int_equal(deque_size(d), 2);

    void* removed = deque_remove_first(d);
    assert_false(deque_is_empty(d));
    assert_int_equal(deque_size(d), 1);
    free(removed);

    removed = deque_remove_last(d);
    assert_true(deque_is_empty(d));
    assert_int_equal(deque_size(d), 0);
    free(removed);

    deque_destroy(d, free);
}

// 测试空双端队列上的操作
static void test_deque_empty_operations(void **state) {
    (void) state;
    Deque *d = deque_create();
    assert_true(deque_is_empty(d));
    assert_int_equal(deque_size(d), 0);
    assert_null(deque_peek_first(d));
    assert_null(deque_peek_last(d));
    assert_null(deque_remove_first(d));
    assert_null(deque_remove_last(d));
    deque_destroy(d, NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_deque_creation),
        cmocka_unit_test(test_deque_add_first),
        cmocka_unit_test(test_deque_add_last),
        cmocka_unit_test(test_deque_remove_first),
        cmocka_unit_test(test_deque_remove_last),
        cmocka_unit_test(test_deque_peek),
        cmocka_unit_test(test_deque_size_empty),
        cmocka_unit_test(test_deque_empty_operations),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}