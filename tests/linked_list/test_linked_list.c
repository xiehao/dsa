#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h> // Include for malloc/free

#include "dsa/linked_list.h"

// 辅助函数，用于创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

// 链表创建测试用例
static void test_linked_list_creation(void **state) {
    (void) state; // 未使用
    LinkedList *list = linked_list_create();
    assert_non_null(list);
    assert_true(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 0);
    linked_list_destroy(list, NULL); // 尚无数据需要释放
}

// 测试向链表头部添加元素
static void test_linked_list_add_first(void **state) {
    (void) state;
    LinkedList *list = linked_list_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);

    linked_list_add_first(list, data1);
    assert_false(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    linked_list_add_first(list, data2);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data2); // 新的第一个元素
    assert_ptr_equal(linked_list_get(list, 1), data1);

    linked_list_destroy(list, free); // 释放整数数据
}

// 测试向链表尾部添加元素
static void test_linked_list_add_last(void **state) {
    (void) state;
    LinkedList *list = linked_list_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);

    linked_list_add_last(list, data1);
    assert_false(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    linked_list_add_last(list, data2);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2); // 新的最后一个元素

    linked_list_destroy(list, free);
}

// 测试从链表头部移除元素
static void test_linked_list_remove_first(void **state) {
    (void) state;
    LinkedList *list = linked_list_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    linked_list_add_last(list, data1);
    linked_list_add_last(list, data2);

    void* removed_data = linked_list_remove_first(list);
    assert_ptr_equal(removed_data, data1);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data2);
    free(removed_data); // 释放被移除的数据

    removed_data = linked_list_remove_first(list);
    assert_ptr_equal(removed_data, data2);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));
    free(removed_data);

    // 测试从空列表移除
    removed_data = linked_list_remove_first(list);
    assert_null(removed_data);
    assert_int_equal(linked_list_size(list), 0);

    linked_list_destroy(list, free); // 应该为空，但以防万一调用 free
}

// 测试从链表尾部移除元素
static void test_linked_list_remove_last(void **state) {
    (void) state;
    LinkedList *list = linked_list_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    linked_list_add_last(list, data1);
    linked_list_add_last(list, data2);

    void* removed_data = linked_list_remove_last(list);
    assert_ptr_equal(removed_data, data2);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    free(removed_data);

    removed_data = linked_list_remove_last(list);
    assert_ptr_equal(removed_data, data1);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));
    free(removed_data);

    // 测试从空列表移除
    removed_data = linked_list_remove_last(list);
    assert_null(removed_data);
    assert_int_equal(linked_list_size(list), 0);

    linked_list_destroy(list, free);
}

// 测试按索引获取元素
static void test_linked_list_get(void **state) {
    (void) state;
    LinkedList *list = linked_list_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    linked_list_add_last(list, data1);
    linked_list_add_last(list, data2);

    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);

    // 测试越界访问
    assert_null(linked_list_get(list, 2));
    assert_null(linked_list_get(list, -1));

    // 测试在空列表上获取
    LinkedList *empty_list = linked_list_create();
    assert_null(linked_list_get(empty_list, 0));
    linked_list_destroy(empty_list, NULL);

    linked_list_destroy(list, free);
}

// 测试按索引设置元素
static void test_linked_list_set(void **state) {
    (void) state;
    LinkedList *list = linked_list_create();
    int* data1 = create_int(10);
    int* data2 = create_int(20);
    linked_list_add_last(list, data1);
    linked_list_add_last(list, data2);

    int* new_data = create_int(99);
    // linked_list_set 返回布尔值，而不是旧数据指针
    // 如果我们想检查/释放旧数据，需要在设置之前获取它
    void* old_data_ptr = linked_list_get(list, 1);
    bool set_success = linked_list_set(list, 1, new_data);

    assert_true(set_success); // 检查设置是否成功
    assert_ptr_equal(old_data_ptr, data2); // 验证设置前数据确实是 data2
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), new_data); // 验证新数据已设置
    if (old_data_ptr) free(old_data_ptr); // 释放原始的 data2

    // 测试越界设置 - 应返回 false
    int* another_new_data = create_int(100);
    set_success = linked_list_set(list, 2, another_new_data);
    assert_false(set_success); // 预期越界时返回 false
    assert_int_equal(linked_list_size(list), 2); // 大小不应改变
    assert_ptr_equal(linked_list_get(list, 1), new_data); // 确保索引 1 处的数据未改变
    free(another_new_data); // 释放未插入的数据

    // 测试使用负索引设置 - 应返回 false（size_t 索引意味着负数会变成大的正数）
    // 根据实现，这可能被视为越界。
    set_success = linked_list_set(list, (size_t)-1, NULL);
    assert_false(set_success); // 预期无效索引时返回 false

    linked_list_destroy(list, free); // 释放 data1 和 new_data
}

// 测试空列表上的操作
static void test_linked_list_empty_operations(void **state) {
    (void) state;
    LinkedList *list = linked_list_create();
    assert_true(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 0);
    assert_null(linked_list_get(list, 0));
    assert_null(linked_list_remove_first(list));
    assert_null(linked_list_remove_last(list));
    // 测试在空列表上设置 - 应返回 false
    bool empty_set_success = linked_list_set(list, 0, NULL);
    assert_false(empty_set_success);
    linked_list_destroy(list, NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_linked_list_creation),
        cmocka_unit_test(test_linked_list_add_first),
        cmocka_unit_test(test_linked_list_add_last),
        cmocka_unit_test(test_linked_list_remove_first),
        cmocka_unit_test(test_linked_list_remove_last),
        cmocka_unit_test(test_linked_list_get),
        cmocka_unit_test(test_linked_list_set),
        cmocka_unit_test(test_linked_list_empty_operations),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}