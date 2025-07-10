/**
 * @file test_doubly_linked_deque.c
 * @brief 双向链表双端队列的单元测试
 * @author DSA Team
 * @date 2025-07-10
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include "../../include/adt/deque.h"

// 辅助函数，用于创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

// 测试双向链表双端队列创建
static void test_doubly_linked_deque_creation(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
    assert_non_null(deque);
    assert_true(deque_is_empty(deque));
    assert_int_equal(deque_size(deque), 0);
    assert_int_equal(deque_get_type(deque), DEQUE_TYPE_DOUBLY_LINKED_LIST);
    
    const char *type_name = deque_get_type_name(deque);
    assert_non_null(type_name);
    
    deque_destroy(deque);
}

// 测试向队首添加元素
static void test_doubly_linked_deque_add_first(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
    int* data1 = create_int(10);
    int* data2 = create_int(20);

    assert_int_equal(deque_add_first(deque, data1), DSA_SUCCESS);
    assert_false(deque_is_empty(deque));
    assert_int_equal(deque_size(deque), 1);
    assert_ptr_equal(deque_peek_first(deque), data1);
    assert_ptr_equal(deque_peek_last(deque), data1);

    assert_int_equal(deque_add_first(deque, data2), DSA_SUCCESS);
    assert_int_equal(deque_size(deque), 2);
    assert_ptr_equal(deque_peek_first(deque), data2); // 新的队首
    assert_ptr_equal(deque_peek_last(deque), data1); // 队尾保持不变

    // 清理内存
    free(deque_remove_first(deque));
    free(deque_remove_first(deque));
    deque_destroy(deque);
}

// 测试向队尾添加元素
static void test_doubly_linked_deque_add_last(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
    int* data1 = create_int(10);
    int* data2 = create_int(20);

    assert_int_equal(deque_add_last(deque, data1), DSA_SUCCESS);
    assert_false(deque_is_empty(deque));
    assert_int_equal(deque_size(deque), 1);
    assert_ptr_equal(deque_peek_first(deque), data1);
    assert_ptr_equal(deque_peek_last(deque), data1);

    assert_int_equal(deque_add_last(deque, data2), DSA_SUCCESS);
    assert_int_equal(deque_size(deque), 2);
    assert_ptr_equal(deque_peek_first(deque), data1); // 队首保持不变
    assert_ptr_equal(deque_peek_last(deque), data2); // 新的队尾

    // 清理内存
    free(deque_remove_first(deque));
    free(deque_remove_first(deque));
    deque_destroy(deque);
}

// 测试从队首移除元素
static void test_doubly_linked_deque_remove_first(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    deque_add_last(deque, data1);
    deque_add_last(deque, data2);

    void* removed_data = deque_remove_first(deque);
    assert_ptr_equal(removed_data, data1);
    assert_int_equal(deque_size(deque), 1);
    assert_ptr_equal(deque_peek_first(deque), data2);
    assert_ptr_equal(deque_peek_last(deque), data2);
    assert_int_equal(*(int*)removed_data, 10); // 验证移除的元素内容
    free(removed_data);

    removed_data = deque_remove_first(deque);
    assert_ptr_equal(removed_data, data2);
    assert_int_equal(deque_size(deque), 0);
    assert_true(deque_is_empty(deque));
    assert_null(deque_peek_first(deque));
    assert_null(deque_peek_last(deque));
    assert_int_equal(*(int*)removed_data, 20); // 验证移除的元素内容
    free(removed_data);

    // 测试从空双端队列移除
    removed_data = deque_remove_first(deque);
    assert_null(removed_data);
    assert_int_equal(deque_size(deque), 0);

    deque_destroy(deque);
}

// 测试从队尾移除元素
static void test_doubly_linked_deque_remove_last(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    deque_add_last(deque, data1);
    deque_add_last(deque, data2);

    void* removed_data = deque_remove_last(deque);
    assert_ptr_equal(removed_data, data2);
    assert_int_equal(deque_size(deque), 1);
    assert_ptr_equal(deque_peek_first(deque), data1);
    assert_ptr_equal(deque_peek_last(deque), data1);
    assert_int_equal(*(int*)removed_data, 20); // 验证移除的元素内容
    free(removed_data);

    removed_data = deque_remove_last(deque);
    assert_ptr_equal(removed_data, data1);
    assert_int_equal(deque_size(deque), 0);
    assert_true(deque_is_empty(deque));
    assert_null(deque_peek_first(deque));
    assert_null(deque_peek_last(deque));
    assert_int_equal(*(int*)removed_data, 10); // 验证移除的元素内容
    free(removed_data);

    // 测试从空双端队列移除
    removed_data = deque_remove_last(deque);
    assert_null(removed_data);
    assert_int_equal(deque_size(deque), 0);

    deque_destroy(deque);
}

// 测试查看元素
static void test_doubly_linked_deque_peek(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);

    // 在空双端队列上查看
    assert_null(deque_peek_first(deque));
    assert_null(deque_peek_last(deque));

    int* data1 = create_int(10);
    deque_add_first(deque, data1);
    assert_ptr_equal(deque_peek_first(deque), data1);
    assert_ptr_equal(deque_peek_last(deque), data1);

    int* data2 = create_int(20);
    deque_add_last(deque, data2);
    assert_ptr_equal(deque_peek_first(deque), data1);
    assert_ptr_equal(deque_peek_last(deque), data2);

    int* data3 = create_int(5);
    deque_add_first(deque, data3);
    assert_ptr_equal(deque_peek_first(deque), data3);
    assert_ptr_equal(deque_peek_last(deque), data2);

    // 清理内存
    free(deque_remove_first(deque));
    free(deque_remove_first(deque));
    free(deque_remove_first(deque));
    deque_destroy(deque);
}

// 测试双端队列大小和空状态
static void test_doubly_linked_deque_size_empty(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
    assert_true(deque_is_empty(deque));
    assert_int_equal(deque_size(deque), 0);

    int* data1 = create_int(1);
    deque_add_first(deque, data1);
    assert_false(deque_is_empty(deque));
    assert_int_equal(deque_size(deque), 1);

    int* data2 = create_int(2);
    deque_add_last(deque, data2);
    assert_false(deque_is_empty(deque));
    assert_int_equal(deque_size(deque), 2);

    void* removed = deque_remove_first(deque);
    assert_false(deque_is_empty(deque));
    assert_int_equal(deque_size(deque), 1);
    free(removed);

    removed = deque_remove_last(deque);
    assert_true(deque_is_empty(deque));
    assert_int_equal(deque_size(deque), 0);
    free(removed);

    deque_destroy(deque);
}

// 测试空双端队列上的操作
static void test_doubly_linked_deque_empty_operations(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
    assert_true(deque_is_empty(deque));
    assert_int_equal(deque_size(deque), 0);
    assert_null(deque_peek_first(deque));
    assert_null(deque_peek_last(deque));
    assert_null(deque_remove_first(deque));
    assert_null(deque_remove_last(deque));
    deque_destroy(deque);
}

// 测试大量元素操作（链表的优势）
static void test_doubly_linked_deque_large_operations(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
    
    // 添加大量元素，测试链表的动态特性
    const int num_elements = 1000;
    int* data[num_elements];
    
    // 从前端和后端交替添加元素
    for (int i = 0; i < num_elements; i++) {
        data[i] = create_int(i);
        if (i % 2 == 0) {
            assert_int_equal(deque_add_first(deque, data[i]), DSA_SUCCESS);
        } else {
            assert_int_equal(deque_add_last(deque, data[i]), DSA_SUCCESS);
        }
    }
    
    assert_int_equal(deque_size(deque), num_elements);
    assert_false(deque_is_empty(deque));
    
    // 清理内存
    for (int i = 0; i < num_elements; i++) {
        void* removed = (i % 2 == 0) ? deque_remove_first(deque) : deque_remove_last(deque);
        free(removed);
    }
    
    assert_true(deque_is_empty(deque));
    deque_destroy(deque);
}

// 测试清空操作
static void test_doubly_linked_deque_clear(void **state) {
    (void) state;
    dsa_deque_t *deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
    
    // 添加一些元素
    for (int i = 0; i < 5; i++) {
        int* data = create_int(i);
        deque_add_last(deque, data);
    }
    
    assert_int_equal(deque_size(deque), 5);
    assert_false(deque_is_empty(deque));
    
    // 清空队列
    assert_int_equal(deque_clear(deque), DSA_SUCCESS);
    assert_int_equal(deque_size(deque), 0);
    assert_true(deque_is_empty(deque));
    assert_null(deque_peek_first(deque));
    assert_null(deque_peek_last(deque));
    
    deque_destroy(deque);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_doubly_linked_deque_creation),
        cmocka_unit_test(test_doubly_linked_deque_add_first),
        cmocka_unit_test(test_doubly_linked_deque_add_last),
        cmocka_unit_test(test_doubly_linked_deque_remove_first),
        cmocka_unit_test(test_doubly_linked_deque_remove_last),
        cmocka_unit_test(test_doubly_linked_deque_peek),
        cmocka_unit_test(test_doubly_linked_deque_size_empty),
        cmocka_unit_test(test_doubly_linked_deque_empty_operations),
        cmocka_unit_test(test_doubly_linked_deque_large_operations),
        cmocka_unit_test(test_doubly_linked_deque_clear),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
