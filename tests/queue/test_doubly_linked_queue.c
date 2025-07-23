/**
 * @file test_doubly_linked_queue.c
 * @brief 双向链表队列专用单元测试
 * @details 测试双向链表队列的特定功能和性能特点
 * @author DSA Team
 * @date 2025-07-22
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <adt/queue.h>

// 测试夹具结构体
typedef struct {
    dsa_queue_t *queue;
} doubly_linked_queue_fixture_t;

// 辅助函数：创建整数数据
static int *create_int(int value) {
    int *data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

// 设置函数
static int setup_doubly_linked_queue(void **state) {
    doubly_linked_queue_fixture_t *fixture = malloc(sizeof(doubly_linked_queue_fixture_t));
    assert_non_null(fixture);

    fixture->queue = queue_create(QUEUE_TYPE_DOUBLY_LINKED_LIST);
    assert_non_null(fixture->queue);

    *state = fixture;
    return 0;
}

// 清理函数
static int teardown_doubly_linked_queue(void **state) {
    doubly_linked_queue_fixture_t *fixture = (doubly_linked_queue_fixture_t *) *state;
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
// Doubly Linked Queue Specific Tests (双向链表队列专用测试)
// ============================================================================

/**
 * @brief 测试双向链表队列的类型信息
 */
static void test_doubly_linked_queue_type_info(void **state) {
    doubly_linked_queue_fixture_t *fixture = (doubly_linked_queue_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;
    
    assert_int_equal(queue_get_type(queue), QUEUE_TYPE_DOUBLY_LINKED_LIST);
    assert_string_equal(queue_get_type_name(queue), "双链队列 (Doubly Linked Queue)");
}

/**
 * @brief 测试双向链表队列的基本操作
 */
static void test_doubly_linked_queue_basic_operations(void **state) {
    doubly_linked_queue_fixture_t *fixture = (doubly_linked_queue_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;
    
    // 初始状态
    assert_true(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 0);
    
    // 入队元素
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    int* data3 = create_int(30);
    
    dsa_result_t result = queue_enqueue(queue, data1);
    assert_int_equal(result, DSA_SUCCESS);
    assert_false(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 1);
    
    result = queue_enqueue(queue, data2);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(queue_size(queue), 2);
    
    result = queue_enqueue(queue, data3);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(queue_size(queue), 3);
    
    // 出队元素（FIFO顺序）
    dsa_element_pt dequeued = queue_dequeue(queue);
    assert_ptr_equal(dequeued, data1);
    assert_int_equal(*(int*)dequeued, 10);
    assert_int_equal(queue_size(queue), 2);
    free(dequeued);
    
    dequeued = queue_dequeue(queue);
    assert_ptr_equal(dequeued, data2);
    assert_int_equal(*(int*)dequeued, 20);
    assert_int_equal(queue_size(queue), 1);
    free(dequeued);
    
    dequeued = queue_dequeue(queue);
    assert_ptr_equal(dequeued, data3);
    assert_int_equal(*(int*)dequeued, 30);
    assert_int_equal(queue_size(queue), 0);
    assert_true(queue_is_empty(queue));
    free(dequeued);
}

/**
 * @brief 测试双向链表队列的FIFO行为
 */
static void test_doubly_linked_queue_fifo_behavior(void **state) {
    doubly_linked_queue_fixture_t *fixture = (doubly_linked_queue_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;
    
    // 入队一系列元素
    int values[] = {1, 2, 3, 4, 5};
    int count = sizeof(values) / sizeof(values[0]);
    
    for (int i = 0; i < count; i++) {
        int* data = create_int(values[i]);
        queue_enqueue(queue, data);
    }
    
    // 验证FIFO顺序出队
    for (int i = 0; i < count; i++) {
        dsa_element_pt dequeued = queue_dequeue(queue);
        assert_non_null(dequeued);
        assert_int_equal(*(int*)dequeued, values[i]);
        free(dequeued);
    }
    
    assert_true(queue_is_empty(queue));
}

/**
 * @brief 测试双向链表队列的动态特性
 */
static void test_doubly_linked_queue_dynamic_nature(void **state) {
    doubly_linked_queue_fixture_t *fixture = (doubly_linked_queue_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;
    
    // 添加大量元素（无需预设容量）
    const int large_count = 1000;
    
    for (int i = 0; i < large_count; i++) {
        int* data = create_int(i);
        dsa_result_t result = queue_enqueue(queue, data);
        assert_int_equal(result, DSA_SUCCESS);
    }
    
    assert_int_equal(queue_size(queue), large_count);
    assert_false(queue_is_empty(queue));
    
    // 验证所有元素都能正确出队
    for (int i = 0; i < large_count; i++) {
        dsa_element_pt dequeued = queue_dequeue(queue);
        assert_non_null(dequeued);
        assert_int_equal(*(int*)dequeued, i);
        free(dequeued);
    }
    
    assert_true(queue_is_empty(queue));
}

/**
 * @brief 测试双向链表队列的内存管理
 */
static void test_doubly_linked_queue_memory_management(void **state) {
    doubly_linked_queue_fixture_t *fixture = (doubly_linked_queue_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;
    
    // 添加元素
    int* data1 = create_int(100);
    int* data2 = create_int(200);
    
    queue_enqueue(queue, data1);
    queue_enqueue(queue, data2);
    
    // 清空队列
    dsa_result_t result = queue_clear(queue);
    assert_int_equal(result, DSA_SUCCESS);
    assert_true(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 0);
    
    // 清空后应该可以继续使用
    int* data3 = create_int(300);
    result = queue_enqueue(queue, data3);
    assert_int_equal(result, DSA_SUCCESS);
    assert_int_equal(queue_size(queue), 1);
    
    dsa_element_pt dequeued = queue_dequeue(queue);
    assert_ptr_equal(dequeued, data3);
    assert_int_equal(*(int*)dequeued, 300);
    free(dequeued);
}

/**
 * @brief 测试双向链表队列的边界条件
 */
static void test_doubly_linked_queue_boundary_conditions(void **state) {
    doubly_linked_queue_fixture_t *fixture = (doubly_linked_queue_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;
    
    // 测试空队列操作
    assert_null(queue_dequeue(queue));
    assert_true(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 0);
    
    // 入队一个元素
    int* data = create_int(42);
    queue_enqueue(queue, data);
    
    // 测试单元素队列
    assert_false(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 1);
    
    // 出队唯一元素
    dsa_element_pt dequeued = queue_dequeue(queue);
    assert_ptr_equal(dequeued, data);
    free(dequeued);
    
    // 验证回到空队列状态
    assert_true(queue_is_empty(queue));
    assert_int_equal(queue_size(queue), 0);
    assert_null(queue_dequeue(queue));
}

/**
 * @brief 测试双向链表队列的链表特性
 */
static void test_doubly_linked_queue_linked_characteristics(void **state) {
    doubly_linked_queue_fixture_t *fixture = (doubly_linked_queue_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;
    
    // 测试频繁的入队出队操作（模拟链表的动态分配特性）
    const int operation_count = 100;

    for (int i = 0; i < operation_count; i++) {
        // 入队一个元素
        int* data = create_int(i);
        queue_enqueue(queue, data);

        // 出队一个元素
        dsa_element_pt dequeued = queue_dequeue(queue);
        assert_non_null(dequeued);
        assert_int_equal(*(int*)dequeued, i);
        free(dequeued);

        // 队列大小应该保持为0（每次入队一个出队一个）
        assert_int_equal(queue_size(queue), 0);
    }
    
    // 清理剩余元素
    while (!queue_is_empty(queue)) {
        dsa_element_pt dequeued = queue_dequeue(queue);
        free(dequeued);
    }
    
    assert_true(queue_is_empty(queue));
}

/**
 * @brief 测试双向链表队列的性能特点
 */
static void test_doubly_linked_queue_performance(void **state) {
    doubly_linked_queue_fixture_t *fixture = (doubly_linked_queue_fixture_t *) *state;
    dsa_queue_t *queue = fixture->queue;
    
    // 测试大量入队出队操作
    const int operation_count = 500;
    
    // 入队操作
    for (int i = 0; i < operation_count; i++) {
        int* data = create_int(i);
        dsa_result_t result = queue_enqueue(queue, data);
        assert_int_equal(result, DSA_SUCCESS);
    }
    
    assert_int_equal(queue_size(queue), operation_count);
    
    // 出队操作
    for (int i = 0; i < operation_count; i++) {
        dsa_element_pt dequeued = queue_dequeue(queue);
        assert_non_null(dequeued);
        assert_int_equal(*(int*)dequeued, i);
        free(dequeued);
    }
    
    assert_true(queue_is_empty(queue));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_doubly_linked_queue_type_info, 
                                       setup_doubly_linked_queue, teardown_doubly_linked_queue),
        cmocka_unit_test_setup_teardown(test_doubly_linked_queue_basic_operations, 
                                       setup_doubly_linked_queue, teardown_doubly_linked_queue),
        cmocka_unit_test_setup_teardown(test_doubly_linked_queue_fifo_behavior, 
                                       setup_doubly_linked_queue, teardown_doubly_linked_queue),
        cmocka_unit_test_setup_teardown(test_doubly_linked_queue_dynamic_nature, 
                                       setup_doubly_linked_queue, teardown_doubly_linked_queue),
        cmocka_unit_test_setup_teardown(test_doubly_linked_queue_memory_management, 
                                       setup_doubly_linked_queue, teardown_doubly_linked_queue),
        cmocka_unit_test_setup_teardown(test_doubly_linked_queue_boundary_conditions, 
                                       setup_doubly_linked_queue, teardown_doubly_linked_queue),
        cmocka_unit_test_setup_teardown(test_doubly_linked_queue_linked_characteristics, 
                                       setup_doubly_linked_queue, teardown_doubly_linked_queue),
        cmocka_unit_test_setup_teardown(test_doubly_linked_queue_performance, 
                                       setup_doubly_linked_queue, teardown_doubly_linked_queue),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
