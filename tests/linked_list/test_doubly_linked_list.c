//
// Created by oaheixiehao on 2025/7/1.
//

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

#include "ds/linked_list.h"

// 辅助函数，用于创建整数数据
static int *create_int(int value) {
    int *data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

// 辅助函数，用于创建字符串数据
static char *create_string(const char *str) {
    char *data = malloc(strlen(str) + 1);
    assert_non_null(data);
    strcpy(data, str);
    return data;
}

// 测试双链表创建
static void test_doubly_linked_list_creation(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    assert_non_null(list);
    assert_true(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 0);
    linked_list_destroy(list);
}

// 测试空链表操作
static void test_doubly_linked_list_empty_operations(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);

    // 测试空链表的基本操作
    assert_true(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 0);

    // 测试从空链表获取元素
    assert_null(linked_list_get(list, 0));
    assert_null(linked_list_get(list, 1));

    // 测试从空链表删除元素
    assert_null(linked_list_remove_at(list, 0));
    assert_null(linked_list_remove_at(list, 1));

    // 测试清空空链表
    assert_int_equal(linked_list_clear(list), DSA_SUCCESS);
    assert_true(linked_list_is_empty(list));

    linked_list_destroy(list);
}

// 测试在头部插入元素
static void test_doubly_linked_list_insert_at_head(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 在索引0处插入第一个元素
    assert_int_equal(linked_list_insert_at(list, 0, data1), DSA_SUCCESS);
    assert_false(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    // 在索引0处插入第二个元素（头部插入）
    assert_int_equal(linked_list_insert_at(list, 0, data2), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data2);
    assert_ptr_equal(linked_list_get(list, 1), data1);

    // 在索引0处插入第三个元素
    assert_int_equal(linked_list_insert_at(list, 0, data3), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 3);
    assert_ptr_equal(linked_list_get(list, 0), data3);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data1);

    free(data1);
    free(data2);
    free(data3);
    linked_list_destroy(list);
}

// 测试在尾部插入元素
static void test_doubly_linked_list_insert_at_tail(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 在尾部插入元素
    assert_int_equal(linked_list_insert_at(list, linked_list_size(list), data1), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    assert_int_equal(linked_list_insert_at(list, linked_list_size(list), data2), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);

    assert_int_equal(linked_list_insert_at(list, linked_list_size(list), data3), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 3);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data3);

    free(data1);
    free(data2);
    free(data3);
    linked_list_destroy(list);
}

// 测试在中间位置插入元素
static void test_doubly_linked_list_insert_at_middle(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);
    int *data4 = create_int(40);

    // 先插入两个元素
    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data3);

    // 在中间插入元素
    assert_int_equal(linked_list_insert_at(list, 1, data2), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 3);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data3);

    // 在另一个中间位置插入
    assert_int_equal(linked_list_insert_at(list, 2, data4), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 4);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data4);
    assert_ptr_equal(linked_list_get(list, 3), data3);

    free(data1);
    free(data2);
    free(data3);
    free(data4);
    linked_list_destroy(list);
}

// 测试插入边界情况
static void test_doubly_linked_list_insert_boundary(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data = create_int(10);

    // 测试越界插入
    assert_int_equal(linked_list_insert_at(list, 1, data), DSA_ERROR_INDEX_OUT_OF_BOUNDS);
    assert_int_equal(linked_list_insert_at(list, 100, data), DSA_ERROR_INDEX_OUT_OF_BOUNDS);

    // 测试插入NULL数据
    assert_int_equal(linked_list_insert_at(list, 0, NULL), DSA_ERROR_NULL_POINTER);

    free(data);
    linked_list_destroy(list);
}

// 测试获取元素
static void test_doubly_linked_list_get(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 插入元素
    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    linked_list_insert_at(list, 2, data3);

    // 测试正常获取
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data3);

    // 测试越界获取
    assert_null(linked_list_get(list, 3));
    assert_null(linked_list_get(list, 100));

    free(data1);
    free(data2);
    free(data3);
    linked_list_destroy(list);
}

// 测试设置元素
static void test_doubly_linked_list_set(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *new_data = create_int(99);

    // 插入元素
    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);

    // 测试正常设置
    assert_int_equal(linked_list_set(list, 0, new_data), DSA_SUCCESS);
    assert_ptr_equal(linked_list_get(list, 0), new_data);
    assert_ptr_equal(linked_list_get(list, 1), data2);

    // 测试越界设置
    assert_int_equal(linked_list_set(list, 2, new_data), DSA_ERROR_INDEX_OUT_OF_BOUNDS);
    assert_int_equal(linked_list_set(list, 100, new_data), DSA_ERROR_INDEX_OUT_OF_BOUNDS);

    // 测试设置NULL数据
    assert_int_equal(linked_list_set(list, 0, NULL), DSA_ERROR_NULL_POINTER);

    free(data1);
    free(data2);
    free(new_data);
    linked_list_destroy(list);
}

// 测试从头部删除元素
static void test_doubly_linked_list_remove_from_head(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 插入元素
    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    linked_list_insert_at(list, 2, data3);

    // 从头部删除
    void *removed = linked_list_remove_at(list, 0);
    assert_ptr_equal(removed, data1);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data2);
    assert_ptr_equal(linked_list_get(list, 1), data3);
    free(removed);

    // 继续从头部删除
    removed = linked_list_remove_at(list, 0);
    assert_ptr_equal(removed, data2);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data3);
    free(removed);

    // 删除最后一个元素
    removed = linked_list_remove_at(list, 0);
    assert_ptr_equal(removed, data3);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));
    free(removed);

    linked_list_destroy(list);
}

// 测试从尾部删除元素
static void test_doubly_linked_list_remove_from_tail(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 插入元素
    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    linked_list_insert_at(list, 2, data3);

    // 从尾部删除
    void *removed = linked_list_remove_at(list, linked_list_size(list) - 1);
    assert_ptr_equal(removed, data3);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    free(removed);

    // 继续从尾部删除
    removed = linked_list_remove_at(list, linked_list_size(list) - 1);
    assert_ptr_equal(removed, data2);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    free(removed);

    // 删除最后一个元素
    removed = linked_list_remove_at(list, linked_list_size(list) - 1);
    assert_ptr_equal(removed, data1);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));
    free(removed);

    linked_list_destroy(list);
}

// 测试从中间删除元素
static void test_doubly_linked_list_remove_from_middle(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);
    int *data4 = create_int(40);
    int *data5 = create_int(50);

    // 插入元素
    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    linked_list_insert_at(list, 2, data3);
    linked_list_insert_at(list, 3, data4);
    linked_list_insert_at(list, 4, data5);

    // 从中间删除
    void *removed = linked_list_remove_at(list, 2);
    assert_ptr_equal(removed, data3);
    assert_int_equal(linked_list_size(list), 4);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data4);
    assert_ptr_equal(linked_list_get(list, 3), data5);
    free(removed);

    // 从另一个中间位置删除
    removed = linked_list_remove_at(list, 1);
    assert_ptr_equal(removed, data2);
    assert_int_equal(linked_list_size(list), 3);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data4);
    assert_ptr_equal(linked_list_get(list, 2), data5);
    free(removed);

    free(data1);
    free(data4);
    free(data5);
    linked_list_destroy(list);
}

// 测试删除边界情况
static void test_doubly_linked_list_remove_boundary(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data = create_int(10);

    // 测试从空链表删除
    assert_null(linked_list_remove_at(list, 0));
    assert_null(linked_list_remove_at(list, 1));

    // 插入一个元素后测试越界删除
    linked_list_insert_at(list, 0, data);
    assert_null(linked_list_remove_at(list, 1));
    assert_null(linked_list_remove_at(list, 100));

    // 删除唯一元素
    void *removed = linked_list_remove_at(list, 0);
    assert_ptr_equal(removed, data);
    assert_true(linked_list_is_empty(list));
    free(removed);

    linked_list_destroy(list);
}

// 测试清空链表
static void test_doubly_linked_list_clear(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 插入元素
    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    linked_list_insert_at(list, 2, data3);
    assert_int_equal(linked_list_size(list), 3);
    assert_false(linked_list_is_empty(list));

    // 清空链表
    assert_int_equal(linked_list_clear(list), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));

    // 测试从已清空的链表获取元素
    assert_null(linked_list_get(list, 0));
    assert_null(linked_list_remove_at(list, 0));

    // 测试在已清空的链表中再次插入
    int *new_data = create_int(100);
    assert_int_equal(linked_list_insert_at(list, 0, new_data), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), new_data);

    free(data1);
    free(data2);
    free(data3);
    free(new_data);
    linked_list_destroy(list);
}

// 测试大量元素操作
static void test_doubly_linked_list_large_operations(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    const int count = 1000;
    int **data_array = malloc(count * sizeof(int*));

    // 插入大量元素
    for (int i = 0; i < count; i++) {
        data_array[i] = create_int(i);
        assert_int_equal(linked_list_insert_at(list, i, data_array[i]), DSA_SUCCESS);
    }

    assert_int_equal(linked_list_size(list), count);
    assert_false(linked_list_is_empty(list));

    // 验证所有元素
    for (int i = 0; i < count; i++) {
        assert_ptr_equal(linked_list_get(list, i), data_array[i]);
        assert_int_equal(*(int*)linked_list_get(list, i), i);
    }

    // 删除一半元素
    for (int i = 0; i < count / 2; i++) {
        void *removed = linked_list_remove_at(list, 0);
        assert_ptr_equal(removed, data_array[i]);
        free(removed);
    }

    assert_int_equal(linked_list_size(list), count / 2);

    // 验证剩余元素
    for (int i = 0; i < count / 2; i++) {
        assert_ptr_equal(linked_list_get(list, i), data_array[i + count / 2]);
    }

    // 清理剩余元素
    for (int i = count / 2; i < count; i++) {
        free(data_array[i]);
    }
    free(data_array);
    linked_list_destroy(list);
}

// 测试混合操作
static void test_doubly_linked_list_mixed_operations(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);

    // 混合插入、删除、设置操作
    int *data1 = create_int(1);
    int *data2 = create_int(2);
    int *data3 = create_int(3);
    int *data4 = create_int(4);
    int *data5 = create_int(5);

    // 插入、删除、再插入
    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    void *removed = linked_list_remove_at(list, 0);
    assert_ptr_equal(removed, data1);
    free(removed);

    linked_list_insert_at(list, 0, data3);
    linked_list_insert_at(list, 1, data4);
    assert_int_equal(linked_list_size(list), 3);

    // 验证顺序
    assert_ptr_equal(linked_list_get(list, 0), data3);
    assert_ptr_equal(linked_list_get(list, 1), data4);
    assert_ptr_equal(linked_list_get(list, 2), data2);

    // 设置元素
    assert_int_equal(linked_list_set(list, 1, data5), DSA_SUCCESS);
    assert_ptr_equal(linked_list_get(list, 1), data5);

    free(data2);
    free(data3);
    free(data4);
    free(data5);
    linked_list_destroy(list);
}

// 测试NULL指针处理
static void test_doubly_linked_list_null_pointer_handling(void **state) {
    (void) state;

    // 测试NULL链表的各种操作
    assert_int_equal(linked_list_size(NULL), 0);
    assert_true(linked_list_is_empty(NULL));
    assert_null(linked_list_get(NULL, 0));
    assert_int_equal(linked_list_clear(NULL), DSA_ERROR_NULL_POINTER);
    assert_int_equal(linked_list_destroy(NULL), DSA_ERROR_NULL_POINTER);
    assert_null(linked_list_remove_at(NULL, 0));

    // 测试有效链表的NULL指针操作
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    assert_int_equal(linked_list_insert_at(list, 0, NULL), DSA_ERROR_NULL_POINTER);
    assert_int_equal(linked_list_set(list, 0, NULL), DSA_ERROR_NULL_POINTER);

    linked_list_destroy(list);
}

// 测试不同数据类型
static void test_doubly_linked_list_different_data_types(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);

    // 测试字符串数据
    char *str1 = create_string("Hello");
    char *str2 = create_string("World");
    char *str3 = create_string("Test");

    linked_list_insert_at(list, 0, str1);
    linked_list_insert_at(list, 1, str2);
    linked_list_insert_at(list, 2, str3);

    assert_string_equal((char*)linked_list_get(list, 0), "Hello");
    assert_string_equal((char*)linked_list_get(list, 1), "World");
    assert_string_equal((char*)linked_list_get(list, 2), "Test");

    // 测试整数数据
    int *int1 = create_int(100);
    int *int2 = create_int(200);

    linked_list_set(list, 0, int1);
    linked_list_set(list, 1, int2);

    assert_int_equal(*(int*)linked_list_get(list, 0), 100);
    assert_int_equal(*(int*)linked_list_get(list, 1), 200);
    assert_string_equal((char*)linked_list_get(list, 2), "Test");

    free(str1);
    free(str2);
    free(str3);
    free(int1);
    free(int2);
    linked_list_destroy(list);
}

// 测试单元素链表操作
static void test_doubly_linked_list_single_element(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data = create_int(42);

    // 插入单个元素
    assert_int_equal(linked_list_insert_at(list, 0, data), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 1);
    assert_false(linked_list_is_empty(list));
    assert_ptr_equal(linked_list_get(list, 0), data);

    // 测试单元素链表的各种操作
    int *new_data = create_int(99);
    assert_int_equal(linked_list_set(list, 0, new_data), DSA_SUCCESS);
    assert_ptr_equal(linked_list_get(list, 0), new_data);

    // 删除单个元素
    void *removed = linked_list_remove_at(list, 0);
    assert_ptr_equal(removed, new_data);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));

    free(data);
    free(removed);
    linked_list_destroy(list);
}

// 测试双链表的push_front操作
static void test_doubly_linked_list_push_front(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 测试push_front操作
    assert_int_equal(linked_list_push_front(list, data1), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    assert_int_equal(linked_list_push_front(list, data2), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data2); // 新元素在前面
    assert_ptr_equal(linked_list_get(list, 1), data1);

    assert_int_equal(linked_list_push_front(list, data3), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 3);
    assert_ptr_equal(linked_list_get(list, 0), data3); // 最新元素在最前面
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data1);

    // 测试NULL指针
    assert_int_equal(linked_list_push_front(NULL, data1), DSA_ERROR_NULL_POINTER);

    free(data1);
    free(data2);
    free(data3);
    linked_list_destroy(list);
}

// 测试双链表的push_back操作
static void test_doubly_linked_list_push_back(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 测试push_back操作
    assert_int_equal(linked_list_push_back(list, data1), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    assert_int_equal(linked_list_push_back(list, data2), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2); // 新元素在后面

    assert_int_equal(linked_list_push_back(list, data3), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 3);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data3); // 最新元素在最后面

    // 测试NULL指针
    assert_int_equal(linked_list_push_back(NULL, data1), DSA_ERROR_NULL_POINTER);

    free(data1);
    free(data2);
    free(data3);
    linked_list_destroy(list);
}

// 测试双链表的pop_front操作
static void test_doubly_linked_list_pop_front(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 先添加一些元素
    linked_list_push_back(list, data1);
    linked_list_push_back(list, data2);
    linked_list_push_back(list, data3);

    // 测试pop_front操作
    void *popped = linked_list_pop_front(list);
    assert_ptr_equal(popped, data1);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data2);
    free(popped);

    popped = linked_list_pop_front(list);
    assert_ptr_equal(popped, data2);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data3);
    free(popped);

    popped = linked_list_pop_front(list);
    assert_ptr_equal(popped, data3);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));
    free(popped);

    // 测试空链表pop_front
    popped = linked_list_pop_front(list);
    assert_null(popped);

    // 测试NULL指针
    assert_null(linked_list_pop_front(NULL));

    linked_list_destroy(list);
}

// 测试双链表的pop_back操作
static void test_doubly_linked_list_pop_back(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 先添加一些元素
    linked_list_push_back(list, data1);
    linked_list_push_back(list, data2);
    linked_list_push_back(list, data3);

    // 测试pop_back操作（双链表的优势：O(1)时间复杂度）
    void *popped = linked_list_pop_back(list);
    assert_ptr_equal(popped, data3);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    free(popped);

    popped = linked_list_pop_back(list);
    assert_ptr_equal(popped, data2);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    free(popped);

    popped = linked_list_pop_back(list);
    assert_ptr_equal(popped, data1);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));
    free(popped);

    // 测试空链表pop_back
    popped = linked_list_pop_back(list);
    assert_null(popped);

    // 测试NULL指针
    assert_null(linked_list_pop_back(NULL));

    linked_list_destroy(list);
}

// 测试双链表的高效双端操作
static void test_doubly_linked_list_efficient_double_ended_operations(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);
    int *data4 = create_int(40);

    // 测试双链表的双端操作优势：所有操作都是O(1)
    linked_list_push_front(list, data2);  // [20]
    linked_list_push_back(list, data3);   // [20, 30]
    linked_list_push_front(list, data1);  // [10, 20, 30]
    linked_list_push_back(list, data4);   // [10, 20, 30, 40]

    assert_int_equal(linked_list_size(list), 4);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data3);
    assert_ptr_equal(linked_list_get(list, 3), data4);

    // 高效的双端删除操作
    void *popped_back = linked_list_pop_back(list);    // 移除40，剩余[10, 20, 30]
    assert_ptr_equal(popped_back, data4);
    free(popped_back);

    void *popped_front = linked_list_pop_front(list);  // 移除10，剩余[20, 30]
    assert_ptr_equal(popped_front, data1);
    free(popped_front);

    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data2);
    assert_ptr_equal(linked_list_get(list, 1), data3);

    free(data2);
    free(data3);
    linked_list_destroy(list);
}

// 主测试函数
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_doubly_linked_list_creation),
        cmocka_unit_test(test_doubly_linked_list_empty_operations),
        cmocka_unit_test(test_doubly_linked_list_insert_at_head),
        cmocka_unit_test(test_doubly_linked_list_insert_at_tail),
        cmocka_unit_test(test_doubly_linked_list_insert_at_middle),
        cmocka_unit_test(test_doubly_linked_list_insert_boundary),
        cmocka_unit_test(test_doubly_linked_list_get),
        cmocka_unit_test(test_doubly_linked_list_set),
        cmocka_unit_test(test_doubly_linked_list_remove_from_head),
        cmocka_unit_test(test_doubly_linked_list_remove_from_tail),
        cmocka_unit_test(test_doubly_linked_list_remove_from_middle),
        cmocka_unit_test(test_doubly_linked_list_remove_boundary),
        cmocka_unit_test(test_doubly_linked_list_clear),
        cmocka_unit_test(test_doubly_linked_list_large_operations),
        cmocka_unit_test(test_doubly_linked_list_mixed_operations),
        cmocka_unit_test(test_doubly_linked_list_null_pointer_handling),
        cmocka_unit_test(test_doubly_linked_list_different_data_types),
        cmocka_unit_test(test_doubly_linked_list_single_element),
        // Linear trait tests
        cmocka_unit_test(test_doubly_linked_list_push_front),
        cmocka_unit_test(test_doubly_linked_list_push_back),
        cmocka_unit_test(test_doubly_linked_list_pop_front),
        cmocka_unit_test(test_doubly_linked_list_pop_back),
        cmocka_unit_test(test_doubly_linked_list_efficient_double_ended_operations),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}