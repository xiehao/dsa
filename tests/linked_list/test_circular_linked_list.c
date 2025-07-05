/**
 * @file test_circular_linked_list.c
 * @brief 循环链表单元测试
 * @details 使用CMocka框架对循环链表进行全面测试
 * @author oaheixiehao
 * @date 2025/7/2
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include "ds/linked_list.h"

// 辅助函数，用于创建整数数据
static int *create_int(int value) {
    int *data = malloc(sizeof(int));
    assert_non_null(data);
    *data = value;
    return data;
}

// 循环链表创建测试用例
static void test_circular_linked_list_creation(void **state) {
    (void) state; // 未使用
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    assert_non_null(list);
    assert_true(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 0);
    
    linked_list_destroy(list);
}

// 测试向循环链表头部添加元素
static void test_circular_linked_list_add_first(void **state) {
    (void) state;
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);

    // 在索引0处添加（头部添加）
    assert_int_equal(linked_list_insert_at(list, 0, data1), DSA_SUCCESS);
    assert_false(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    assert_int_equal(linked_list_insert_at(list, 0, data2), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 2);
    assert_ptr_equal(linked_list_get(list, 0), data2); // 新的第一个元素
    assert_ptr_equal(linked_list_get(list, 1), data1);

    // 手动释放内存
    free(data1);
    free(data2);
    linked_list_destroy(list);
}

// 测试向循环链表尾部添加元素
static void test_circular_linked_list_add_last(void **state) {
    (void) state;
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 在尾部添加元素
    assert_int_equal(linked_list_insert_at(list, 0, data1), DSA_SUCCESS);
    assert_int_equal(linked_list_insert_at(list, 1, data2), DSA_SUCCESS);
    assert_int_equal(linked_list_insert_at(list, 2, data3), DSA_SUCCESS);
    
    assert_int_equal(linked_list_size(list), 3);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data3);

    free(data1);
    free(data2);
    free(data3);
    linked_list_destroy(list);
}

// 测试从循环链表头部删除元素
static void test_circular_linked_list_remove_first(void **state) {
    (void) state;
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);

    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);

    // 删除第一个元素
    int *removed = (int*)linked_list_remove_at(list, 0);
    assert_ptr_equal(removed, data1);
    assert_int_equal(*removed, 10);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data2);

    free(removed);
    free(data2);
    linked_list_destroy(list);
}

// 测试从循环链表尾部删除元素
static void test_circular_linked_list_remove_last(void **state) {
    (void) state;
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);

    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);

    // 删除最后一个元素
    int *removed = (int*)linked_list_remove_at(list, 1);
    assert_ptr_equal(removed, data2);
    assert_int_equal(*removed, 20);
    assert_int_equal(linked_list_size(list), 1);
    assert_ptr_equal(linked_list_get(list, 0), data1);

    free(removed);
    free(data1);
    linked_list_destroy(list);
}

// 测试循环链表的get操作
static void test_circular_linked_list_get(void **state) {
    (void) state;
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    linked_list_insert_at(list, 2, data3);

    // 测试正常索引
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data3);

    // 测试越界索引
    assert_null(linked_list_get(list, 3));
    assert_null(linked_list_get(list, SIZE_MAX));

    free(data1);
    free(data2);
    free(data3);
    linked_list_destroy(list);
}

// 测试循环链表的set操作
static void test_circular_linked_list_set(void **state) {
    (void) state;
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *new_data = create_int(99);

    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);

    // 测试正常设置
    assert_int_equal(linked_list_set(list, 1, new_data), DSA_SUCCESS);
    assert_ptr_equal(linked_list_get(list, 1), new_data);
    assert_int_equal(*(int*)linked_list_get(list, 1), 99);

    // 测试越界设置
    int *extra_data = create_int(88);
    assert_int_equal(linked_list_set(list, 3, extra_data), DSA_ERROR_INDEX_OUT_OF_BOUNDS);

    // 测试NULL指针设置
    assert_int_equal(linked_list_set(list, 0, NULL), DSA_ERROR_NULL_POINTER);

    free(data1);
    free(data2); // 被替换的数据
    free(new_data);
    free(extra_data);
    linked_list_destroy(list);
}

// 测试循环链表的中间插入操作
static void test_circular_linked_list_insert_middle(void **state) {
    (void) state;
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(15);

    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    
    // 在中间插入
    assert_int_equal(linked_list_insert_at(list, 1, data3), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 3);
    
    // 验证顺序：10, 15, 20
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data3);
    assert_ptr_equal(linked_list_get(list, 2), data2);

    free(data1);
    free(data2);
    free(data3);
    linked_list_destroy(list);
}

// 测试循环链表的清空操作
static void test_circular_linked_list_clear(void **state) {
    (void) state;
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);

    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    
    assert_int_equal(linked_list_size(list), 2);
    assert_false(linked_list_is_empty(list));

    // 清空链表
    assert_int_equal(linked_list_clear(list), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 0);
    assert_true(linked_list_is_empty(list));

    // 清空后应该可以继续使用
    int *new_data = create_int(99);
    assert_int_equal(linked_list_insert_at(list, 0, new_data), DSA_SUCCESS);
    assert_int_equal(linked_list_size(list), 1);

    free(data1);
    free(data2);
    free(new_data);
    linked_list_destroy(list);
}

// 测试循环链表的空操作
static void test_circular_linked_list_empty_operations(void **state) {
    (void) state;
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    
    // 测试空链表的各种操作
    assert_true(linked_list_is_empty(list));
    assert_int_equal(linked_list_size(list), 0);
    assert_null(linked_list_get(list, 0));
    assert_null(linked_list_remove_at(list, 0));
    assert_null(linked_list_remove_at(list, SIZE_MAX));
    
    // 测试在空列表上设置 - 应返回错误
    dsa_result_t empty_set_result = linked_list_set(list, 0, NULL);
    assert_int_equal(DSA_ERROR_NULL_POINTER, empty_set_result);
    
    // 测试越界插入
    int *data = create_int(10);
    assert_int_equal(linked_list_insert_at(list, 1, data), DSA_ERROR_INDEX_OUT_OF_BOUNDS);
    
    free(data);
    linked_list_destroy(list);
}

// 测试循环链表的push_front操作
static void test_circular_linked_list_push_front(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
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

// 测试循环链表的push_back操作
static void test_circular_linked_list_push_back(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
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

// 测试循环链表的pop_front操作
static void test_circular_linked_list_pop_front(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
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

// 测试循环链表的pop_back操作
static void test_circular_linked_list_pop_back(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 先添加一些元素
    linked_list_push_back(list, data1);
    linked_list_push_back(list, data2);
    linked_list_push_back(list, data3);

    // 测试pop_back操作
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

// 测试循环链表的循环特性与linear_trait操作的结合
static void test_circular_linked_list_circular_nature_with_linear_operations(void **state) {
    (void) state;
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);

    // 使用linear_trait操作构建循环链表
    linked_list_push_front(list, data2);  // [20]
    linked_list_push_back(list, data3);   // [20, 30]
    linked_list_push_front(list, data1);  // [10, 20, 30]

    assert_int_equal(linked_list_size(list), 3);
    assert_ptr_equal(linked_list_get(list, 0), data1);
    assert_ptr_equal(linked_list_get(list, 1), data2);
    assert_ptr_equal(linked_list_get(list, 2), data3);

    // 测试循环链表的特性：虽然内部是循环的，但通过标准接口访问时表现为线性
    // 验证边界访问
    assert_ptr_equal(linked_list_get(list, 0), data1);  // 第一个元素
    assert_ptr_equal(linked_list_get(list, 2), data3);  // 最后一个元素
    assert_null(linked_list_get(list, 3));              // 越界访问返回NULL

    // 使用pop操作清空链表
    void *popped1 = linked_list_pop_front(list);  // 移除10
    void *popped2 = linked_list_pop_back(list);   // 移除30
    void *popped3 = linked_list_pop_front(list);  // 移除20

    assert_ptr_equal(popped1, data1);
    assert_ptr_equal(popped2, data3);
    assert_ptr_equal(popped3, data2);
    assert_true(linked_list_is_empty(list));

    free(popped1);
    free(popped2);
    free(popped3);
    linked_list_destroy(list);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_circular_linked_list_creation),
        cmocka_unit_test(test_circular_linked_list_add_first),
        cmocka_unit_test(test_circular_linked_list_add_last),
        cmocka_unit_test(test_circular_linked_list_remove_first),
        cmocka_unit_test(test_circular_linked_list_remove_last),
        cmocka_unit_test(test_circular_linked_list_get),
        cmocka_unit_test(test_circular_linked_list_set),
        cmocka_unit_test(test_circular_linked_list_insert_middle),
        cmocka_unit_test(test_circular_linked_list_clear),
        cmocka_unit_test(test_circular_linked_list_empty_operations),
        // Linear trait tests
        cmocka_unit_test(test_circular_linked_list_push_front),
        cmocka_unit_test(test_circular_linked_list_push_back),
        cmocka_unit_test(test_circular_linked_list_pop_front),
        cmocka_unit_test(test_circular_linked_list_pop_back),
        cmocka_unit_test(test_circular_linked_list_circular_nature_with_linear_operations),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
