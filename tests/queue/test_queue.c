/**
 * @file test_queue.c
 * @brief 队列通用接口单元测试
 * @details 测试队列ADT的通用接口，验证不同实现的一致性
 * @author DSA Team
 * @date 2025-07-22
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <adt/queue.h>

// 测试夹具结构体
typedef struct {
    dsa_queue_type_t type;
    dsa_queue_t *queue;
} test_fixture_t;

// 辅助函数：创建整数数据
static int *create_int(int value) {
    int *data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

// 设置函数：创建循环数组队列
static int setup_circular_array_queue(void **state) {
    test_fixture_t *fixture = malloc(sizeof(test_fixture_t));
    assert_non_null(fixture);

    fixture->type = QUEUE_TYPE_CIRCULAR_ARRAY_LIST;
    fixture->queue = queue_create(fixture->type);
    assert_non_null(fixture->queue);

    *state = fixture;
    return 0;
}

// 设置函数：创建双向链表队列
static int setup_doubly_linked_queue(void **state) {
    test_fixture_t *fixture = malloc(sizeof(test_fixture_t));
    assert_non_null(fixture);

    fixture->type = QUEUE_TYPE_DOUBLY_LINKED_LIST;
    fixture->queue = queue_create(fixture->type);
    assert_non_null(fixture->queue);

    *state = fixture;
    return 0;
}

// 清理函数
static int teardown_queue(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    if (fixture) {
        if (fixture->queue) {
            // 清理队列中剩余的元素
            while (!queue_is_empty(fixture->queue)) {
                dsa_element_pt element = queue_dequeue(fixture->queue);
                if (element) {
                    free(element);
                }
            }
            queue_destroy(fixture->queue);
        }
        free(fixture);
    }
    return 0;
}

// ============================================================================
// Basic Interface Tests (基本队列接口测试)
// ============================================================================

/**
 * @brief 测试队列创建和基本属性
 * @details 测试队列初始化、大小、空状态检查
 */
static void test_queue_creation(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;

    assert_non_null(queue);
    assert_true(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 0);
    assert_int_equal(queue_get_type(queue), fixture->type);
    
    // 验证类型名称不为空
    const char *type_name = queue_get_type_name(queue);
    assert_non_null(type_name);
    assert_true(strlen(type_name) > 0);
}

/**
 * @brief 测试向队列中入队元素
 * @details 测试单个和多个元素的入队操作
 */
static void test_queue_enqueue(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;

    int* data1 = create_int(10);
    int* data2 = create_int(20);
    int* data3 = create_int(30);

    // 测试第一个元素入队
    dsa_result_t result = queue_enqueue(queue, data1);
    assert_int_equal(result, DSA_SUCCESS);
    assert_false(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 1);

    // 测试多个元素入队
    result = queue_enqueue(queue, data2);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(queue_size(queue), 2);

    result = queue_enqueue(queue, data3);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(queue_size(queue), 3);
    assert_false(queue_is_empty(queue));
}

/**
 * @brief 测试从队列中出队元素
 * @details 测试FIFO顺序和边界条件
 */
static void test_queue_dequeue(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;

    int* data1 = create_int(10);
    int* data2 = create_int(20);
    int* data3 = create_int(30);

    // 先入队三个元素
    queue_enqueue(queue, data1);
    queue_enqueue(queue, data2);
    queue_enqueue(queue, data3);
    assert_int_equal(queue_size(queue), 3);

    // 测试FIFO顺序出队
    dsa_element_pt dequeued_data = queue_dequeue(queue);
    assert_ptr_equal(dequeued_data, data1);
    assert_int_equal(*(int*)dequeued_data, 10);
    assert_int_equal(queue_size(queue), 2);
    free(dequeued_data);

    dequeued_data = queue_dequeue(queue);
    assert_ptr_equal(dequeued_data, data2);
    assert_int_equal(*(int*)dequeued_data, 20);
    assert_int_equal(queue_size(queue), 1);
    free(dequeued_data);

    dequeued_data = queue_dequeue(queue);
    assert_ptr_equal(dequeued_data, data3);
    assert_int_equal(*(int*)dequeued_data, 30);
    assert_int_equal(queue_size(queue), 0);
    assert_true(queue_is_empty(queue));
    free(dequeued_data);
}

/**
 * @brief 测试队列大小和空状态
 * @details 测试大小计算和空状态检查的正确性
 */
static void test_queue_size_empty(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;

    // 初始状态
    assert_true(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 0);

    // 添加元素后
    int* data = create_int(42);
    queue_enqueue(queue, data);
    assert_false(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 1);

    // 移除元素后
    dsa_element_pt removed = queue_dequeue(queue);
    free(removed);
    assert_true(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 0);
}

/**
 * @brief 测试空队列操作
 * @details 测试在空队列上进行操作的边界条件
 */
static void test_empty_queue_operations(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;

    // 空队列出队应返回NULL
    assert_null(queue_dequeue(queue));
    assert_true(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 0);

    // 清空空队列应该成功
    dsa_result_t result = queue_clear(queue);
    assert_int_equal(result, DSA_SUCCESS);
    assert_true(queue_is_empty(queue));
}

/**
 * @brief 测试队列清空操作
 * @details 测试清空队列的功能
 */
static void test_queue_clear(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;

    // 添加一些元素
    for (int i = 1; i <= 5; i++) {
        int* data = create_int(i * 10);
        queue_enqueue(queue, data);
    }
    assert_int_equal(queue_size(queue), 5);
    assert_false(queue_is_empty(queue));

    // 清空队列
    dsa_result_t result = queue_clear(queue);
    assert_int_equal(result, DSA_SUCCESS);
    assert_true(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 0);

    // 清空后应该可以继续使用
    int* data = create_int(100);
    result = queue_enqueue(queue, data);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(queue_size(queue), 1);
    
    dsa_element_pt removed = queue_dequeue(queue);
    assert_ptr_equal(removed, data);
    free(removed);
}

/**
 * @brief 测试NULL指针处理
 * @details 测试函数对NULL指针的处理
 */
static void test_null_handling(void **state) {
    (void) state; // 不使用fixture

    // 测试NULL队列的各种操作
    assert_int_equal(queue_size(NULL), 0);
    assert_true(queue_is_empty(NULL));
    assert_int_equal(queue_clear(NULL), DSA_SUCCESS);
    assert_int_equal(queue_destroy(NULL), DSA_SUCCESS);
    assert_int_equal(queue_enqueue(NULL, create_int(1)), DSA_ERROR_NULL_POINTER);
    assert_null(queue_dequeue(NULL));
    assert_int_equal(queue_get_type(NULL), QUEUE_TYPE_CIRCULAR_ARRAY_LIST);
    assert_string_equal(queue_get_type_name(NULL), "未知队列类型");
}

int main(void) {
    const struct CMUnitTest circular_array_tests[] = {
        cmocka_unit_test_setup_teardown(test_queue_creation,
                                       setup_circular_array_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_queue_enqueue,
                                       setup_circular_array_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_queue_dequeue,
                                       setup_circular_array_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_queue_size_empty,
                                       setup_circular_array_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_empty_queue_operations,
                                       setup_circular_array_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_queue_clear,
                                       setup_circular_array_queue, teardown_queue),
    };

    const struct CMUnitTest doubly_linked_tests[] = {
        cmocka_unit_test_setup_teardown(test_queue_creation,
                                       setup_doubly_linked_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_queue_enqueue,
                                       setup_doubly_linked_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_queue_dequeue,
                                       setup_doubly_linked_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_queue_size_empty,
                                       setup_doubly_linked_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_empty_queue_operations,
                                       setup_doubly_linked_queue, teardown_queue),
        cmocka_unit_test_setup_teardown(test_queue_clear,
                                       setup_doubly_linked_queue, teardown_queue),
    };

    const struct CMUnitTest null_tests[] = {
        cmocka_unit_test(test_null_handling),
    };

    printf("Running Circular Array Queue Tests...\n");
    int result1 = cmocka_run_group_tests(circular_array_tests, NULL, NULL);

    printf("Running Doubly Linked Queue Tests...\n");
    int result2 = cmocka_run_group_tests(doubly_linked_tests, NULL, NULL);

    printf("Running NULL Handling Tests...\n");
    int result3 = cmocka_run_group_tests(null_tests, NULL, NULL);

    return result1 + result2 + result3;
}
