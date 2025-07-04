#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <adt/stack.h>
#include <common.h>

// ============================================================================
// 测试数据结构和工具函数
// ============================================================================
typedef struct {
    dsa_stack_t *stack;
    dsa_stack_type_t type;
} test_fixture_t;

// 辅助函数，用于创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

static int setup_array_stack(void **state) {
    test_fixture_t *fixture = malloc(sizeof(test_fixture_t));
    assert_non_null(fixture);

    fixture->type = STACK_TYPE_ARRAY_LIST;
    fixture->stack = stack_create(fixture->type);
    assert_non_null(fixture->stack);

    *state = fixture;
    return 0;
}

static int teardown_stack(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    if (fixture) {
        if (fixture->stack) {
            // 清理栈中剩余的元素
            while (!stack_is_empty(fixture->stack)) {
                dsa_element_pt element = stack_pop(fixture->stack);
                if (element) {
                    free(element);
                }
            }
            stack_destroy(fixture->stack);
        }
        free(fixture);
    }
    return 0;
}

// ============================================================================
// Basic Interface Tests (基本栈接口测试)
// ============================================================================

/**
 * @brief 测试数组栈创建和基本属性
 * @details 测试栈初始化、大小、空状态检查
 */
static void test_array_stack_creation(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;

    assert_non_null(stack);
    assert_true(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 0);
    assert_int_equal(stack_get_type(stack), STACK_TYPE_ARRAY_LIST);
    assert_string_equal(stack_get_type_name(stack), "数组列表栈(Array List Stack)");
}

/**
 * @brief 测试向栈中压入元素
 * @details 测试单个和多个元素的压入操作
 */
static void test_array_stack_push(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;

    int* data1 = create_int(10);
    int* data2 = create_int(20);
    int* data3 = create_int(30);

    // 测试第一个元素压入
    dsa_result_t result = stack_push(stack, data1);
    assert_int_equal(result, DSA_SUCCESS);
    assert_false(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 1);
    assert_ptr_equal(stack_peek(stack), data1);
    assert_int_equal(ELEMENT_VALUE(int, stack_peek(stack)), 10);

    // 测试第二个元素压入
    result = stack_push(stack, data2);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(stack_size(stack), 2);
    assert_ptr_equal(stack_peek(stack), data2); // 栈顶元素应为 data2
    assert_int_equal(ELEMENT_VALUE(int, stack_peek(stack)), 20);

    // 测试第三个元素压入
    result = stack_push(stack, data3);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(stack_size(stack), 3);
    assert_ptr_equal(stack_peek(stack), data3);
    assert_int_equal(ELEMENT_VALUE(int, stack_peek(stack)), 30);
}

/**
 * @brief 测试从栈中弹出元素
 * @details 测试LIFO顺序和边界条件
 */
static void test_array_stack_pop(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;

    int* data1 = create_int(10);
    int* data2 = create_int(20);
    int* data3 = create_int(30);

    // 先压入三个元素
    stack_push(stack, data1);
    stack_push(stack, data2);
    stack_push(stack, data3);
    assert_int_equal(stack_size(stack), 3);

    // 测试LIFO顺序弹出
    dsa_element_pt popped_data = stack_pop(stack);
    assert_ptr_equal(popped_data, data3);
    assert_int_equal(ELEMENT_VALUE(int, popped_data), 30);
    assert_int_equal(stack_size(stack), 2);
    assert_ptr_equal(stack_peek(stack), data2);
    free(popped_data);

    popped_data = stack_pop(stack);
    assert_ptr_equal(popped_data, data2);
    assert_int_equal(ELEMENT_VALUE(int, popped_data), 20);
    assert_int_equal(stack_size(stack), 1);
    assert_ptr_equal(stack_peek(stack), data1);
    free(popped_data);

    popped_data = stack_pop(stack);
    assert_ptr_equal(popped_data, data1);
    assert_int_equal(ELEMENT_VALUE(int, popped_data), 10);
    assert_int_equal(stack_size(stack), 0);
    assert_true(stack_is_empty(stack));
    assert_null(stack_peek(stack));
    free(popped_data);

    // 测试从空栈弹出
    popped_data = stack_pop(stack);
    assert_null(popped_data);
    assert_int_equal(stack_size(stack), 0);
    assert_true(stack_is_empty(stack));
}

/**
 * @brief 测试查看栈顶元素
 * @details 测试peek操作不改变栈状态
 */
static void test_array_stack_peek(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;

    // 在空栈上查看
    assert_null(stack_peek(stack));

    int* data1 = create_int(10);
    stack_push(stack, data1);
    assert_ptr_equal(stack_peek(stack), data1);
    assert_int_equal(ELEMENT_VALUE(int, stack_peek(stack)), 10);
    assert_int_equal(stack_size(stack), 1); // peek不改变大小

    int* data2 = create_int(20);
    stack_push(stack, data2);
    assert_ptr_equal(stack_peek(stack), data2);
    assert_int_equal(ELEMENT_VALUE(int, stack_peek(stack)), 20);
    assert_int_equal(stack_size(stack), 2); // peek不改变大小

    // 多次peek应该返回相同结果
    assert_ptr_equal(stack_peek(stack), data2);
    assert_ptr_equal(stack_peek(stack), data2);
    assert_int_equal(stack_size(stack), 2);

    // 弹出一个元素后peek应该返回新的栈顶
    dsa_element_pt popped_data = stack_pop(stack);
    assert_ptr_equal(stack_peek(stack), data1);
    assert_int_equal(ELEMENT_VALUE(int, stack_peek(stack)), 10);
    free(popped_data);
}

/**
 * @brief 测试栈大小和空状态
 * @details 测试size和is_empty函数的正确性
 */
static void test_array_stack_size_empty(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;

    // 初始状态
    assert_true(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 0);

    // 添加元素
    int* data1 = create_int(1);
    stack_push(stack, data1);
    assert_false(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 1);

    int* data2 = create_int(2);
    stack_push(stack, data2);
    assert_false(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 2);

    int* data3 = create_int(3);
    stack_push(stack, data3);
    assert_false(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 3);

    // 移除元素
    dsa_element_pt popped_data = stack_pop(stack);
    assert_false(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 2);
    free(popped_data);

    popped_data = stack_pop(stack);
    assert_false(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 1);
    free(popped_data);

    popped_data = stack_pop(stack);
    assert_true(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 0);
    free(popped_data);
}

/**
 * @brief 测试空栈上的操作
 * @details 测试边界条件和错误处理
 */
static void test_array_stack_empty_operations(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;

    assert_true(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 0);
    assert_null(stack_peek(stack));
    assert_null(stack_pop(stack));

    // 多次操作空栈应该保持一致
    assert_null(stack_pop(stack));
    assert_null(stack_peek(stack));
    assert_true(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 0);
}

/**
 * @brief 测试栈的清空操作
 * @details 测试clear函数的正确性
 */
static void test_array_stack_clear(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;

    // 添加一些元素
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    int* data3 = create_int(30);

    stack_push(stack, data1);
    stack_push(stack, data2);
    stack_push(stack, data3);
    assert_int_equal(stack_size(stack), 3);
    assert_false(stack_is_empty(stack));

    // 清空栈
    dsa_result_t result = stack_clear(stack);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(stack_size(stack), 0);
    assert_true(stack_is_empty(stack));
    assert_null(stack_peek(stack));

    // 清空后应该可以继续使用
    int* new_data = create_int(100);
    result = stack_push(stack, new_data);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(stack_size(stack), 1);
    assert_ptr_equal(stack_peek(stack), new_data);
}

/**
 * @brief 测试NULL指针处理
 * @details 测试各种函数对NULL指针的处理
 */
static void test_array_stack_null_handling(void **state) {
    (void) state; // 不使用fixture

    // 测试NULL栈指针
    assert_int_equal(stack_size(NULL), 0);
    assert_true(stack_is_empty(NULL));
    assert_null(stack_peek(NULL));
    assert_null(stack_pop(NULL));
    assert_int_equal(stack_clear(NULL), DSA_ERROR_NULL_POINTER);
    assert_int_equal(stack_destroy(NULL), DSA_ERROR_NULL_POINTER);
    assert_int_equal(stack_push(NULL, create_int(10)), DSA_ERROR_NULL_POINTER);

    // 清理测试数据
    int* test_data = create_int(10);
    free(test_data);
}

/**
 * @brief 测试大量数据操作
 * @details 测试栈在处理大量数据时的性能和正确性
 */
static void test_array_stack_large_data(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;

    const int count = 1000;
    int** data_array = malloc(count * sizeof(int*));
    assert_non_null(data_array);

    // 压入大量数据
    for (int i = 0; i < count; i++) {
        data_array[i] = create_int(i);
        dsa_result_t result = stack_push(stack, data_array[i]);
        assert_int_equal(result, DSA_SUCCESS);
        assert_int_equal(stack_size(stack), i + 1);
    }

    assert_int_equal(stack_size(stack), count);
    assert_false(stack_is_empty(stack));

    // 验证栈顶元素
    dsa_element_pt top = stack_peek(stack);
    assert_non_null(top);
    assert_int_equal(ELEMENT_VALUE(int, top), count - 1);

    // 弹出所有数据并验证LIFO顺序
    for (int i = count - 1; i >= 0; i--) {
        dsa_element_pt popped = stack_pop(stack);
        assert_non_null(popped);
        assert_int_equal(ELEMENT_VALUE(int, popped), i);
        assert_int_equal(stack_size(stack), i);
        free(popped);
    }

    assert_true(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 0);

    free(data_array);
}

/**
 * @brief 测试两种栈类型的一致性
 * @details 验证数组栈和链表栈在相同操作下的行为一致性
 */
static void test_stack_type_consistency(void **state) {
    (void) state; // 不使用fixture

    // 创建两种类型的栈
    dsa_stack_t *array_stack = stack_create(STACK_TYPE_ARRAY_LIST);
    dsa_stack_t *linked_stack = stack_create(STACK_TYPE_LINKED_LIST);

    assert_non_null(array_stack);
    assert_non_null(linked_stack);

    // 验证类型信息
    assert_int_equal(stack_get_type(array_stack), STACK_TYPE_ARRAY_LIST);
    assert_int_equal(stack_get_type(linked_stack), STACK_TYPE_LINKED_LIST);

    // 测试相同的操作序列
    int values[] = {10, 20, 30, 40, 50};
    int count = sizeof(values) / sizeof(values[0]);

    // 压入相同的元素
    for (int i = 0; i < count; i++) {
        int *array_data = create_int(values[i]);
        int *linked_data = create_int(values[i]);

        dsa_result_t array_result = stack_push(array_stack, array_data);
        dsa_result_t linked_result = stack_push(linked_stack, linked_data);

        assert_int_equal(array_result, linked_result);
        assert_int_equal(stack_size(array_stack), stack_size(linked_stack));
        assert_int_equal(stack_is_empty(array_stack), stack_is_empty(linked_stack));
    }

    // 验证栈顶元素
    dsa_element_pt array_top = stack_peek(array_stack);
    dsa_element_pt linked_top = stack_peek(linked_stack);
    assert_int_equal(ELEMENT_VALUE(int, array_top), ELEMENT_VALUE(int, linked_top));

    // 弹出元素并验证一致性
    for (int i = count - 1; i >= 0; i--) {
        dsa_element_pt array_popped = stack_pop(array_stack);
        dsa_element_pt linked_popped = stack_pop(linked_stack);

        assert_int_equal(ELEMENT_VALUE(int, array_popped), ELEMENT_VALUE(int, linked_popped));
        assert_int_equal(ELEMENT_VALUE(int, array_popped), values[i]);
        assert_int_equal(stack_size(array_stack), stack_size(linked_stack));

        free(array_popped);
        free(linked_popped);
    }

    // 验证最终状态
    assert_true(stack_is_empty(array_stack));
    assert_true(stack_is_empty(linked_stack));
    assert_int_equal(stack_size(array_stack), 0);
    assert_int_equal(stack_size(linked_stack), 0);

    // 清理
    stack_destroy(array_stack);
    stack_destroy(linked_stack);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_array_stack_creation,
                                       setup_array_stack, teardown_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_push,
                                       setup_array_stack, teardown_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_pop,
                                       setup_array_stack, teardown_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_peek,
                                       setup_array_stack, teardown_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_size_empty,
                                       setup_array_stack, teardown_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_empty_operations,
                                       setup_array_stack, teardown_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_clear,
                                       setup_array_stack, teardown_stack),
        cmocka_unit_test(test_array_stack_null_handling),
        cmocka_unit_test_setup_teardown(test_array_stack_large_data,
                                       setup_array_stack, teardown_stack),
        cmocka_unit_test(test_stack_type_consistency),
    };

    printf("Running Array Stack Tests...\n");
    return cmocka_run_group_tests(tests, NULL, NULL);
}