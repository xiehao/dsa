/**
 * @file test_array_stack.c
 * @brief 数组栈专用单元测试
 * @details 专门测试基于动态数组实现的栈的各种功能和边界条件
 * @author oaheixiehao
 * @date 2025/7/3
 */

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
} array_stack_fixture_t;

// 辅助函数，用于创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

static int setup_array_stack(void **state) {
    array_stack_fixture_t *fixture = malloc(sizeof(array_stack_fixture_t));
    assert_non_null(fixture);
    
    fixture->stack = stack_create(STACK_TYPE_ARRAY_LIST);
    assert_non_null(fixture->stack);
    
    *state = fixture;
    return 0;
}

static int teardown_array_stack(void **state) {
    array_stack_fixture_t *fixture = (array_stack_fixture_t *) *state;
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
// 数组栈特定测试
// ============================================================================

/**
 * @brief 测试数组栈的类型信息
 */
static void test_array_stack_type_info(void **state) {
    array_stack_fixture_t *fixture = (array_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    assert_int_equal(stack_get_type(stack), STACK_TYPE_ARRAY_LIST);
    assert_string_equal(stack_get_type_name(stack), "数组列表栈(Array List Stack)");
}

/**
 * @brief 测试数组栈的自动扩容特性
 */
static void test_array_stack_auto_expansion(void **state) {
    array_stack_fixture_t *fixture = (array_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    // 压入大量元素测试自动扩容
    const int count = 100;
    for (int i = 0; i < count; i++) {
        int* data = create_int(i);
        dsa_result_t result = stack_push(stack, data);
        assert_int_equal(result, DSA_SUCCESS);
        assert_int_equal(stack_size(stack), i + 1);
        
        // 验证栈顶元素
        dsa_element_pt top = stack_peek(stack);
        assert_non_null(top);
        assert_int_equal(ELEMENT_VALUE(int, top), i);
    }
    
    // 验证所有元素都正确存储
    assert_int_equal(stack_size(stack), count);
    assert_false(stack_is_empty(stack));
}

/**
 * @brief 测试数组栈的LIFO特性
 */
static void test_array_stack_lifo_behavior(void **state) {
    array_stack_fixture_t *fixture = (array_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    // 压入一系列元素
    int values[] = {10, 20, 30, 40, 50};
    int count = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < count; i++) {
        int* data = create_int(values[i]);
        stack_push(stack, data);
    }
    
    // 验证LIFO顺序弹出
    for (int i = count - 1; i >= 0; i--) {
        dsa_element_pt popped = stack_pop(stack);
        assert_non_null(popped);
        assert_int_equal(ELEMENT_VALUE(int, popped), values[i]);
        free(popped);
    }
    
    assert_true(stack_is_empty(stack));
}

/**
 * @brief 测试数组栈的内存管理
 */
static void test_array_stack_memory_management(void **state) {
    array_stack_fixture_t *fixture = (array_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    // 测试多次压入和弹出
    for (int cycle = 0; cycle < 10; cycle++) {
        // 压入元素
        for (int i = 0; i < 10; i++) {
            int* data = create_int(cycle * 10 + i);
            stack_push(stack, data);
        }
        
        // 弹出一半元素
        for (int i = 0; i < 5; i++) {
            dsa_element_pt popped = stack_pop(stack);
            assert_non_null(popped);
            free(popped);
        }
        
        assert_int_equal(stack_size(stack), (cycle + 1) * 5);
    }
    
    // 清理剩余元素
    while (!stack_is_empty(stack)) {
        dsa_element_pt element = stack_pop(stack);
        if (element) {
            free(element);
        }
    }
}

/**
 * @brief 测试数组栈的边界条件
 */
static void test_array_stack_boundary_conditions(void **state) {
    array_stack_fixture_t *fixture = (array_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    // 测试空栈操作
    assert_null(stack_peek(stack));
    assert_null(stack_pop(stack));
    assert_true(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 0);
    
    // 压入一个元素
    int* data = create_int(42);
    stack_push(stack, data);
    
    // 测试单元素栈
    assert_false(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 1);
    assert_ptr_equal(stack_peek(stack), data);
    
    // 弹出唯一元素
    dsa_element_pt popped = stack_pop(stack);
    assert_ptr_equal(popped, data);
    free(popped);
    
    // 验证回到空栈状态
    assert_true(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 0);
    assert_null(stack_peek(stack));
}

/**
 * @brief 测试数组栈的性能特性
 */
static void test_array_stack_performance(void **state) {
    array_stack_fixture_t *fixture = (array_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    const int large_count = 10000;
    
    // 大量压入操作
    for (int i = 0; i < large_count; i++) {
        int* data = create_int(i);
        dsa_result_t result = stack_push(stack, data);
        assert_int_equal(result, DSA_SUCCESS);
    }
    
    assert_int_equal(stack_size(stack), large_count);
    
    // 验证栈顶元素
    dsa_element_pt top = stack_peek(stack);
    assert_non_null(top);
    assert_int_equal(ELEMENT_VALUE(int, top), large_count - 1);
    
    // 大量弹出操作
    for (int i = large_count - 1; i >= 0; i--) {
        dsa_element_pt popped = stack_pop(stack);
        assert_non_null(popped);
        assert_int_equal(ELEMENT_VALUE(int, popped), i);
        free(popped);
    }
    
    assert_true(stack_is_empty(stack));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_array_stack_type_info, 
                                       setup_array_stack, teardown_array_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_auto_expansion, 
                                       setup_array_stack, teardown_array_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_lifo_behavior, 
                                       setup_array_stack, teardown_array_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_memory_management, 
                                       setup_array_stack, teardown_array_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_boundary_conditions, 
                                       setup_array_stack, teardown_array_stack),
        cmocka_unit_test_setup_teardown(test_array_stack_performance, 
                                       setup_array_stack, teardown_array_stack),
    };

    printf("Running Array Stack Specific Tests...\n");
    return cmocka_run_group_tests(tests, NULL, NULL);
}
