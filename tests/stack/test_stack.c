#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h> // Include for malloc/free

#include "dsa/stack.h"

// 辅助函数，用于创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

// 栈创建测试用例
static void test_stack_creation(void **state) {
    (void) state; // 未使用
    Stack *s = stack_create();
    assert_non_null(s);
    assert_true(stack_is_empty(s));
    assert_int_equal(stack_size(s), 0);
    stack_destroy(s); // 尚无数据需要释放
}

// 测试向栈中压入元素
static void test_stack_push(void **state) {
    (void) state;
    Stack *s = stack_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);

    stack_push(s, data1);
    assert_false(stack_is_empty(s));
    assert_int_equal(stack_size(s), 1);
    assert_ptr_equal(stack_peek(s), data1);

    stack_push(s, data2);
    assert_int_equal(stack_size(s), 2);
    assert_ptr_equal(stack_peek(s), data2); // 栈顶元素应为 data2

    // stack_destroy 不释放数据，在销毁前手动释放
    // 或者，如果栈拥有数据，则弹出并释放直到为空
    free(stack_pop(s)); // 释放 data2
    free(stack_pop(s)); // 释放 data1
    stack_destroy(s);
}

// 测试从栈中弹出元素
static void test_stack_pop(void **state) {
    (void) state;
    Stack *s = stack_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    stack_push(s, data1);
    stack_push(s, data2);

    void* popped_data = stack_pop(s);
    assert_ptr_equal(popped_data, data2);
    assert_int_equal(stack_size(s), 1);
    assert_ptr_equal(stack_peek(s), data1);
    free(popped_data); // 释放弹出的数据

    popped_data = stack_pop(s);
    assert_ptr_equal(popped_data, data1);
    assert_int_equal(stack_size(s), 0);
    assert_true(stack_is_empty(s));
    assert_null(stack_peek(s)); // 在空栈上查看
    free(popped_data);

    // 测试从空栈弹出
    popped_data = stack_pop(s);
    assert_null(popped_data);
    assert_int_equal(stack_size(s), 0);

    stack_destroy(s); // 使用 stack_destroy，没有剩余数据需要释放
}

// 测试查看栈顶元素
static void test_stack_peek(void **state) {
    (void) state;
    Stack *s = stack_create();

    // 在空栈上查看
    assert_null(stack_peek(s));

    int* data1 = create_int(10);
    stack_push(s, data1);
    assert_ptr_equal(stack_peek(s), data1);

    int* data2 = create_int(20);
    stack_push(s, data2);
    assert_ptr_equal(stack_peek(s), data2);

    void* popped_data = stack_pop(s);
    assert_ptr_equal(stack_peek(s), data1);
    free(popped_data); // 释放 data2

    // 销毁前释放剩余数据
    free(stack_pop(s)); // 释放 data1
    stack_destroy(s);
}

// 测试栈大小和空状态
static void test_stack_size_empty(void **state) {
    (void) state;
    Stack *s = stack_create();
    assert_true(stack_is_empty(s));
    assert_int_equal(stack_size(s), 0);

    int* data1 = create_int(1);
    stack_push(s, data1);
    assert_false(stack_is_empty(s));
    assert_int_equal(stack_size(s), 1);

    int* data2 = create_int(2);
    stack_push(s, data2);
    assert_false(stack_is_empty(s));
    assert_int_equal(stack_size(s), 2);

    void* popped_data = stack_pop(s);
    assert_false(stack_is_empty(s));
    assert_int_equal(stack_size(s), 1);
    free(popped_data); // 释放 data2

    popped_data = stack_pop(s);
    assert_true(stack_is_empty(s));
    assert_int_equal(stack_size(s), 0);
    free(popped_data); // 释放 data1

    stack_destroy(s);
}

// 测试空栈上的操作
static void test_stack_empty_operations(void **state) {
    (void) state;
    Stack *s = stack_create();
    assert_true(stack_is_empty(s));
    assert_int_equal(stack_size(s), 0);
    assert_null(stack_peek(s));
    assert_null(stack_pop(s));
    stack_destroy(s);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_stack_creation),
        cmocka_unit_test(test_stack_push),
        cmocka_unit_test(test_stack_pop),
        cmocka_unit_test(test_stack_peek),
        cmocka_unit_test(test_stack_size_empty),
        cmocka_unit_test(test_stack_empty_operations),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}