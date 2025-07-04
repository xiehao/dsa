/**
 * @file test_linked_stack.c
 * @brief 链表栈专用单元测试
 * @details 专门测试基于单链表实现的栈的各种功能和边界条件
 * @author oaheixiehao
 * @date 2025/7/4
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
} linked_stack_fixture_t;

// 辅助函数，用于创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

static int setup_linked_stack(void **state) {
    linked_stack_fixture_t *fixture = malloc(sizeof(linked_stack_fixture_t));
    assert_non_null(fixture);
    
    fixture->stack = stack_create(STACK_TYPE_LINKED_LIST);
    assert_non_null(fixture->stack);
    
    *state = fixture;
    return 0;
}

static int teardown_linked_stack(void **state) {
    linked_stack_fixture_t *fixture = (linked_stack_fixture_t *) *state;
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
// 链表栈特定测试
// ============================================================================

/**
 * @brief 测试链表栈的类型信息
 */
static void test_linked_stack_type_info(void **state) {
    linked_stack_fixture_t *fixture = (linked_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    assert_int_equal(stack_get_type(stack), STACK_TYPE_LINKED_LIST);
    assert_string_equal(stack_get_type_name(stack), "链式列表栈(链栈，Linked List Stack)");
}

/**
 * @brief 测试链表栈的基本操作
 */
static void test_linked_stack_basic_operations(void **state) {
    linked_stack_fixture_t *fixture = (linked_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    // 初始状态
    assert_true(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 0);
    assert_null(stack_peek(stack));
    
    // 压入元素
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    int* data3 = create_int(30);
    
    dsa_result_t result = stack_push(stack, data1);
    assert_int_equal(result, DSA_SUCCESS);
    assert_false(stack_is_empty(stack));
    assert_int_equal(stack_size(stack), 1);
    assert_ptr_equal(stack_peek(stack), data1);
    
    result = stack_push(stack, data2);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(stack_size(stack), 2);
    assert_ptr_equal(stack_peek(stack), data2);
    
    result = stack_push(stack, data3);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(stack_size(stack), 3);
    assert_ptr_equal(stack_peek(stack), data3);
}

/**
 * @brief 测试链表栈的LIFO特性
 */
static void test_linked_stack_lifo_behavior(void **state) {
    linked_stack_fixture_t *fixture = (linked_stack_fixture_t *) *state;
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
 * @brief 测试链表栈的动态特性
 */
static void test_linked_stack_dynamic_nature(void **state) {
    linked_stack_fixture_t *fixture = (linked_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    // 测试大量元素的动态添加
    const int count = 1000;
    
    // 压入大量元素
    for (int i = 0; i < count; i++) {
        int* data = create_int(i);
        dsa_result_t result = stack_push(stack, data);
        assert_int_equal(result, DSA_SUCCESS);
        assert_int_equal(stack_size(stack), i + 1);
    }
    
    // 验证栈顶元素
    dsa_element_pt top = stack_peek(stack);
    assert_non_null(top);
    assert_int_equal(ELEMENT_VALUE(int, top), count - 1);
    
    // 弹出所有元素并验证顺序
    for (int i = count - 1; i >= 0; i--) {
        dsa_element_pt popped = stack_pop(stack);
        assert_non_null(popped);
        assert_int_equal(ELEMENT_VALUE(int, popped), i);
        assert_int_equal(stack_size(stack), i);
        free(popped);
    }
    
    assert_true(stack_is_empty(stack));
}

/**
 * @brief 测试链表栈的内存管理
 */
static void test_linked_stack_memory_management(void **state) {
    linked_stack_fixture_t *fixture = (linked_stack_fixture_t *) *state;
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
 * @brief 测试链表栈的边界条件
 */
static void test_linked_stack_boundary_conditions(void **state) {
    linked_stack_fixture_t *fixture = (linked_stack_fixture_t *) *state;
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
 * @brief 测试链表栈的O(1)操作特性
 */
static void test_linked_stack_constant_time_operations(void **state) {
    linked_stack_fixture_t *fixture = (linked_stack_fixture_t *) *state;
    dsa_stack_t *stack = fixture->stack;
    
    // 压入一些元素
    for (int i = 0; i < 100; i++) {
        int* data = create_int(i);
        dsa_result_t result = stack_push(stack, data);
        assert_int_equal(result, DSA_SUCCESS);
    }
    
    // 测试peek操作不改变栈状态
    dsa_element_pt top1 = stack_peek(stack);
    dsa_element_pt top2 = stack_peek(stack);
    assert_ptr_equal(top1, top2);
    assert_int_equal(stack_size(stack), 100);
    
    // 测试pop操作
    dsa_element_pt popped = stack_pop(stack);
    assert_ptr_equal(popped, top1);
    assert_int_equal(ELEMENT_VALUE(int, popped), 99);
    assert_int_equal(stack_size(stack), 99);
    free(popped);
    
    // 验证新的栈顶
    dsa_element_pt new_top = stack_peek(stack);
    assert_int_equal(ELEMENT_VALUE(int, new_top), 98);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_linked_stack_type_info, 
                                       setup_linked_stack, teardown_linked_stack),
        cmocka_unit_test_setup_teardown(test_linked_stack_basic_operations, 
                                       setup_linked_stack, teardown_linked_stack),
        cmocka_unit_test_setup_teardown(test_linked_stack_lifo_behavior, 
                                       setup_linked_stack, teardown_linked_stack),
        cmocka_unit_test_setup_teardown(test_linked_stack_dynamic_nature, 
                                       setup_linked_stack, teardown_linked_stack),
        cmocka_unit_test_setup_teardown(test_linked_stack_memory_management, 
                                       setup_linked_stack, teardown_linked_stack),
        cmocka_unit_test_setup_teardown(test_linked_stack_boundary_conditions, 
                                       setup_linked_stack, teardown_linked_stack),
        cmocka_unit_test_setup_teardown(test_linked_stack_constant_time_operations, 
                                       setup_linked_stack, teardown_linked_stack),
    };

    printf("Running Linked Stack Specific Tests...\n");
    return cmocka_run_group_tests(tests, NULL, NULL);
}
