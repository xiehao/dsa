#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h> // Include for malloc/free

#include "ds/linked_list.h"

// 辅助函数，用于创建整数数据
static int *create_int(int value) {
    int *data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

// 单链表创建测试用例
static void test_singly_linked_list_creation(void **state) {
    (void) state; // 未使用
    // 根据错误信息，linked_list_create需要1个参数（可能是释放函数）
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    assert_non_null(list);
    assert_true(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 0);
    // 根据错误信息，linked_list_destroy只需要1个参数
    linked_list_destroy(list);
}

// 测试向单链表头部添加元素 - 使用 linked_list_add 在索引0处添加
static void test_singly_linked_list_add_first(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY); // 使用NULL以便手动管理内存
    int *data1 = create_int(10);
    int *data2 = create_int(20);

    // 在索引0处添加（头部添加）
    linked_list_insert_at(list, 0, data1);
    assert_false(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    linked_list_insert_at(list, 0, data2);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data2); // 新的第一个元素
    assert_ptr_equal(linked_list_get(list, 1), data1);

    // 手动释放内存
    free(data1);
    free(data2);
    linked_list_destroy(list);
}

// 测试向单链表尾部添加元素 - 使用 linked_list_add 在末尾添加
static void test_singly_linked_list_add_last(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);

    // 在末尾添加
    linked_list_insert_at(list, linked_list_size(list), data1);
    assert_false(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    linked_list_insert_at(list, linked_list_size(list), data2);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2); // 新的最后一个元素

    free(data1);
    free(data2);
    linked_list_destroy(list);
}

// 测试从单链表头部移除元素 - 使用 linked_list_remove 从索引0移除
static void test_singly_linked_list_remove_first(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    linked_list_insert_at(list, linked_list_size(list), data1);
    linked_list_insert_at(list, linked_list_size(list), data2);

    void *removed_data = linked_list_remove_at(list, 0);
    assert_ptr_equal(removed_data, data1);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data2);
    free(removed_data); // 释放被移除的数据

    removed_data = linked_list_remove_at(list, 0);
    assert_ptr_equal(removed_data, data2);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));
    free(removed_data);

    // 测试从空列表移除
    removed_data = linked_list_remove_at(list, 0);
    assert_null(removed_data);
    assert_int_equal(linked_list_size(list), 0);

    linked_list_destroy(list);
}

// 测试从单链表尾部移除元素 - 使用 linked_list_remove 从最后索引移除
static void test_singly_linked_list_remove_last(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    linked_list_insert_at(list, linked_list_size(list), data1);
    linked_list_insert_at(list, linked_list_size(list), data2);

    void *removed_data = linked_list_remove_at(list, linked_list_size(list) - 1);
    assert_ptr_equal(removed_data, data2);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    free(removed_data);

    removed_data = linked_list_remove_at(list, linked_list_size(list) - 1);
    assert_ptr_equal(removed_data, data1);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));
    free(removed_data);

    // 测试从空列表移除
    removed_data = linked_list_remove_at(list, 0);
    assert_null(removed_data);
    assert_int_equal(linked_list_size(list), 0);

    linked_list_destroy(list);
}

// 测试按索引获取单链表元素
static void test_singly_linked_list_get(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    linked_list_insert_at(list, linked_list_size(list), data1);
    linked_list_insert_at(list, linked_list_size(list), data2);

    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);

    // 测试越界访问
    assert_null(linked_list_get(list, 2));
    assert_null(linked_list_get(list, SIZE_MAX)); // 测试大数值（类似负数）

    // 测试在空列表上获取
    dsa_linked_list_t *empty_list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    assert_null(linked_list_get(empty_list, 0));
    linked_list_destroy(empty_list);

    free(data1);
    free(data2);
    linked_list_destroy(list);
}

// 测试按索引设置单链表元素
static void test_singly_linked_list_set(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    linked_list_insert_at(list, linked_list_size(list), data1);
    linked_list_insert_at(list, linked_list_size(list), data2);

    int *new_data = create_int(99);
    // linked_list_set 返回布尔值，而不是旧数据指针
    // 如果我们想检查/释放旧数据，需要在设置之前获取它
    void *old_data_ptr = linked_list_get(list, 1);
    dsa_result_t set_result = linked_list_set(list, 1, new_data);

    assert_int_equal(DSA_SUCCESS, set_result); // 检查设置是否成功
    assert_ptr_equal(old_data_ptr, data2); // 验证设置前数据确实是 data2
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), new_data); // 验证新数据已设置
    if (old_data_ptr) free(old_data_ptr); // 释放原始的 data2

    // 测试越界设置 - 应返回 false
    int *another_new_data = create_int(100);
    set_result = linked_list_set(list, 2, another_new_data);
    assert_int_equal(DSA_ERROR_INDEX_OUT_OF_BOUNDS, set_result); // 预期越界时返回 false
    assert_int_equal(linked_list_size(list), 2); // 大小不应改变
    assert_ptr_equal(linked_list_get(list, 1), new_data); // 确保索引 1 处的数据未改变
    free(another_new_data); // 释放未插入的数据

    free(data1);
    free(new_data);
    linked_list_destroy(list);
}

// 测试单链表空列表上的操作
static void test_singly_linked_list_empty_operations(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    assert_true(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 0);
    assert_null(linked_list_get(list, 0));
    assert_null(linked_list_remove_at(list, 0));
    assert_null(linked_list_remove_at(list, SIZE_MAX));
    // 测试在空列表上设置 - 应返回 false
    dsa_result_t empty_set_result = linked_list_set(list, 0, NULL);
    assert_int_equal(DSA_ERROR_NULL_POINTER, empty_set_result);
    linked_list_destroy(list);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_singly_linked_list_creation),
        cmocka_unit_test(test_singly_linked_list_add_first),
        cmocka_unit_test(test_singly_linked_list_add_last),
        cmocka_unit_test(test_singly_linked_list_remove_first),
        cmocka_unit_test(test_singly_linked_list_remove_last),
        cmocka_unit_test(test_singly_linked_list_get),
        cmocka_unit_test(test_singly_linked_list_set),
        cmocka_unit_test(test_singly_linked_list_empty_operations),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}