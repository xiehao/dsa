#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdio.h>
#include <ds/array_list.h>
#include <common.h>

// ============================================================================
// 测试数据结构和工具函数
// ============================================================================
typedef struct {
    dsa_array_list_t *array;
    const size_t capacity;
    const size_t element_size;
} test_fixture_t;

static int setup_test_array(void **state) {
    test_fixture_t *fixture = malloc(sizeof(test_fixture_t));
    assert_non_null(fixture);

    // 使用常量初始化
    *((size_t *) &fixture->capacity) = 10;
    *((size_t *) &fixture->element_size) = sizeof(int);

    fixture->array = array_list_create_dynamic(fixture->capacity);
    assert_non_null(fixture->array);

    *state = fixture;
    return 0; // 返回0表示成功
}

static int teardown_test_array(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    if (fixture) {
        if (fixture->array) {
            array_list_destroy(fixture->array);
        }
        free(fixture);
    }
    return 0; // 返回0表示成功
}

// ============================================================================
// Basic Interface Tests (基本容器接口测试)
// ============================================================================
/**
 * @brief 测试数组创建和基本属性
 * @details 测试数组初始化、大小、容量、空状态和满状态检查
 */
static void test_basic_interface_creation_and_properties(void **state) {
    const size_t capacity = 5;
    const size_t element_size = sizeof(int);

    dsa_array_list_t *arr = array_list_create_dynamic(capacity);
    assert_non_null(arr);

    // 测试基本属性
    assert_int_equal(array_list_size(arr), 0);
    assert_int_equal(array_list_capacity(arr), capacity);
    assert_true(array_list_is_empty(arr));
    assert_false(array_list_is_full(arr));

    array_list_destroy(arr);
}

/**
 * @brief 测试数组清空功能
 * @details 测试clear操作对数组状态的影响
 */
static void test_basic_interface_clear(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 添加一些元素
    for (int i = 0; i < 3; i++) {
        int value = i * 10;
        assert_int_equal(array_list_push_back(arr, &value), DSA_SUCCESS);
    }
    assert_int_equal(array_list_size(arr), 3);
    assert_false(array_list_is_empty(arr));

    // 清空数组
    array_list_clear(arr);
    assert_int_equal(array_list_size(arr), 0);
    assert_true(array_list_is_empty(arr));
    assert_int_equal(array_list_capacity(arr), fixture->capacity); // 容量不变
}

/**
 * @brief 测试数组满状态检查
 * @details 测试数组填满后的状态检查
 */
static void test_basic_interface_full_state(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 填满数组
    for (size_t i = 0; i < fixture->capacity; i++) {
        int value = (int) i;
        assert_int_equal(array_list_push_back(arr, &value), DSA_SUCCESS);
    }

    assert_true(array_list_is_full(arr));
    assert_false(array_list_is_empty(arr));
    assert_int_equal(array_list_size(arr), fixture->capacity);
}

// ============================================================================
// Random Access Interface Tests (随机访问接口测试)
// ============================================================================
/**
 * @brief 测试随机访问 - get和set操作
 * @details 测试通过索引获取和设置元素
 */
static void test_random_access_get_set(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 先添加一些元素
    int values[5]; // 创建一个数组来存储值
    for (int i = 0; i < 5; i++) {
        values[i] = i * 10;
        assert_int_equal(array_list_push_back(arr, &values[i]), DSA_SUCCESS);
    }

    // 测试get操作
    for (int i = 0; i < 5; i++) {
        int *retrieved = (int *) array_list_get(arr, i);
        assert_non_null(retrieved);
        assert_int_equal(*retrieved, i * 10);
    }

    // 测试越界访问
    assert_null(array_list_get(arr, 5));
    assert_null(array_list_get(arr, SIZE_MAX));

    // 测试set操作
    int new_values[5]; // 创建一个数组来存储新值
    for (int i = 0; i < 5; i++) {
        new_values[i] = i * 100;
        assert_int_equal(array_list_set(arr, i, &new_values[i]), DSA_SUCCESS);
    }

    // 验证set操作结果
    for (int i = 0; i < 5; i++) {
        int *retrieved = (int *) array_list_get(arr, i);
        assert_non_null(retrieved);
        assert_int_equal(*retrieved, i * 100);
    }

    // 测试set越界
    int value = 999;
    assert_int_equal(array_list_set(arr, 5, &value), DSA_ERROR_INDEX_OUT_OF_BOUNDS);
}

/**
 * @brief 测试随机访问 - insert操作
 * @details 测试在指定位置插入元素
 */
static void test_random_access_insert(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 初始添加两个元素
    int v1 = 10, v2 = 30;
    array_list_push_back(arr, &v1);
    array_list_push_back(arr, &v2);

    // 在开头插入
    int v_begin = 5;
    assert_int_equal(array_list_insert(arr, 0, &v_begin), DSA_SUCCESS);
    assert_int_equal(array_list_size(arr), 3);
    assert_int_equal(*(int*)array_list_get(arr, 0), 5);
    assert_int_equal(*(int*)array_list_get(arr, 1), 10);
    assert_int_equal(*(int*)array_list_get(arr, 2), 30);

    // 在中间插入
    int v_mid = 20;
    assert_int_equal(array_list_insert(arr, 2, &v_mid), DSA_SUCCESS);
    assert_int_equal(array_list_size(arr), 4);
    assert_int_equal(*(int*)array_list_get(arr, 0), 5);
    assert_int_equal(*(int*)array_list_get(arr, 1), 10);
    assert_int_equal(*(int*)array_list_get(arr, 2), 20);
    assert_int_equal(*(int*)array_list_get(arr, 3), 30);

    // 在末尾插入
    int v_end = 40;
    assert_int_equal(array_list_insert(arr, 4, &v_end), DSA_SUCCESS);
    assert_int_equal(array_list_size(arr), 5);
    assert_int_equal(*(int*)array_list_get(arr, 4), 40);

    // 测试越界插入
    int v_invalid = 50;
    assert_int_equal(array_list_insert(arr, 6, &v_invalid), DSA_ERROR_INDEX_OUT_OF_BOUNDS);
}

/**
 * @brief 测试随机访问 - remove操作
 * @details 测试移除指定位置的元素
 */
static void test_random_access_remove(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 添加测试数据
    int values[] = {10, 20, 30, 40};
    for (int i = 0; i < 4; i++) {
        array_list_push_back(arr, &values[i]);
    }

    // 从中间移除
    int *removed = (int *) array_list_remove(arr, 1); // 移除20
    assert_non_null(removed);
    assert_int_equal(*removed, 20);
    assert_int_equal(array_list_size(arr), 3);
    assert_int_equal(*(int*)array_list_get(arr, 0), 10);
    assert_int_equal(*(int*)array_list_get(arr, 1), 30);
    assert_int_equal(*(int*)array_list_get(arr, 2), 40);

    // 从开头移除
    removed = (int *) array_list_remove(arr, 0); // 移除10
    assert_non_null(removed);
    assert_int_equal(*removed, 10);
    assert_int_equal(array_list_size(arr), 2);

    // 从末尾移除
    removed = (int *) array_list_remove(arr, 1); // 移除40
    assert_non_null(removed);
    assert_int_equal(*removed, 40);
    assert_int_equal(array_list_size(arr), 1);

    // 测试越界移除
    assert_null(array_list_remove(arr, 1));
}

// ============================================================================
// Back Interface Tests (后端操作接口测试)
// ============================================================================
/**
 * @brief 测试后端操作 - push_back
 * @details 测试向数组末尾添加元素
 */
static void test_back_interface_push_back(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 逐个添加元素
    for (size_t i = 0; i < fixture->capacity; i++) {
        int value = (int) (i * 5);
        assert_int_equal(array_list_push_back(arr, &value), DSA_SUCCESS);
        assert_int_equal(array_list_size(arr), i + 1);
        // 验证添加的元素
        int *retrieved = (int *) array_list_get(arr, i);
        assert_non_null(retrieved);
        assert_int_equal(*retrieved, (int)(i * 5));
    }

    // 测试满容量时的push_back - 动态数组应该自动扩容
    int extra_value = 999;
    assert_int_equal(array_list_push_back(arr, &extra_value), DSA_SUCCESS);
    assert_int_equal(array_list_size(arr), fixture->capacity + 1);
    // 验证容量确实增加了
    assert_true(array_list_capacity(arr) > fixture->capacity);
    // 验证新添加的元素
    int *retrieved = (int *) array_list_get(arr, fixture->capacity);
    assert_non_null(retrieved);
    assert_int_equal(*retrieved, 999);
}

/**
 * @brief 测试后端操作 - pop_back
 * @details 测试从数组末尾移除元素
 */
static void test_back_interface_pop_back(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 添加测试数据
    char char_values[] = {'a', 'b', 'c'};
    int int_values[3]; // 创建独立的int数组来存储转换后的值

    // 先转换并存储到独立的数组中
    for (int i = 0; i < 3; i++) {
        int_values[i] = (int) char_values[i];
        array_list_push_back(arr, &int_values[i]);
    }
    assert_int_equal(array_list_size(arr), 3);

    // 逐个弹出元素
    int *popped = (int *) array_list_pop_back(arr);
    assert_non_null(popped);
    assert_int_equal(*popped, (int)'c');
    assert_int_equal(array_list_size(arr), 2);

    popped = (int *) array_list_pop_back(arr);
    assert_non_null(popped);
    assert_int_equal(*popped, (int)'b');
    assert_int_equal(array_list_size(arr), 1);

    popped = (int *) array_list_pop_back(arr);
    assert_non_null(popped);
    assert_int_equal(*popped, (int)'a');
    assert_int_equal(array_list_size(arr), 0);
    assert_true(array_list_is_empty(arr));

    // 测试空数组pop_back
    popped = (int *) array_list_pop_back(arr);
    assert_null(popped);
    assert_int_equal(array_list_size(arr), 0);
}

// ============================================================================
// Array Interface Tests (数组专用接口测试)
// ============================================================================
/**
 * @brief 测试数组类型相关功能
 * @details 测试获取数组类型和类型名称
 */
static void test_array_interface_type_info(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 测试类型获取
    dsa_array_list_type_t type = array_list_get_type(arr);
    assert_int_equal(type, ARRAY_LIST_TYPE_DYNAMIC);

    // 测试类型名称获取
    const char *type_name = array_list_get_type_name(arr);
    assert_non_null(type_name);
    assert_string_not_equal(type_name, "无效数组");
}

/**
 * @brief 测试数组信息打印
 * @details 测试打印数组详细信息的功能
 */
static void test_array_interface_print_info(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 添加一些数据
    for (int i = 0; i < 3; i++) {
        int value = i;
        array_list_push_back(arr, &value);
    }

    // 这个测试主要是确保函数不会崩溃
    // 实际的输出验证在单元测试中比较困难
    printf("\n=== 数组信息打印测试 ===\n");
    array_list_print_info(arr);
    printf("=== 数组信息打印测试结束 ===\n");

    // 基本验证 - 确保函数调用成功
    assert_true(true); // 如果到达这里说明没有崩溃
}

// ============================================================================
// 综合测试
// ============================================================================
/**
 * @brief 综合测试 - 混合操作
 * @details 测试各种操作的组合使用
 */
static void test_comprehensive_mixed_operations(void **state) {
    test_fixture_t *fixture = (test_fixture_t *) *state;
    dsa_array_list_t *arr = fixture->array;

    // 1. 基本操作组合
    assert_true(array_list_is_empty(arr));

    // 2. 添加元素
    for (int i = 0; i < 5; i++) {
        int value = i * 2;
        assert_int_equal(array_list_push_back(arr, &value), DSA_SUCCESS);
    }
    assert_int_equal(array_list_size(arr), 5);

    // 3. 随机访问修改
    int new_value = 100;
    assert_int_equal(array_list_set(arr, 2, &new_value), DSA_SUCCESS);
    assert_int_equal(*(int*)array_list_get(arr, 2), 100);

    // 4. 插入操作
    int insert_value = 50;
    assert_int_equal(array_list_insert(arr, 1, &insert_value), DSA_SUCCESS);
    assert_int_equal(array_list_size(arr), 6);

    // 5. 移除操作
    int *removed = (int *) array_list_remove(arr, 3);
    assert_non_null(removed);
    assert_int_equal(array_list_size(arr), 5);

    // 6. 后端操作
    int *popped = (int *) array_list_pop_back(arr);
    assert_non_null(popped);
    assert_int_equal(array_list_size(arr), 4);

    // 7. 验证最终状态
    assert_false(array_list_is_empty(arr));
    assert_false(array_list_is_full(arr));

    // 8. 清空测试
    array_list_clear(arr);
    assert_true(array_list_is_empty(arr));
    assert_int_equal(array_list_size(arr), 0);
}

// ============================================================================
// 错误处理测试
// ============================================================================
/**
 * @brief 测试空指针处理
 * @details 测试各种函数对空指针的处理
 */
static void test_null_pointer_handling(void **state) {
    (void) state; // 不使用fixture

    // 测试对空数组的操作
    assert_int_equal(array_list_size(NULL), 0);
    assert_int_equal(array_list_capacity(NULL), 0);
    assert_true(array_list_is_empty(NULL));
    assert_false(array_list_is_full(NULL));
    assert_null(array_list_get(NULL, 0));

    int value = 42;
    assert_int_equal(array_list_set(NULL, 0, &value), DSA_ERROR_NULL_POINTER);
    assert_int_equal(array_list_push_back(NULL, &value), DSA_ERROR_NULL_POINTER);
    assert_null(array_list_pop_back(NULL));
    assert_int_equal(array_list_insert(NULL, 0, &value), DSA_ERROR_NULL_POINTER);
    assert_null(array_list_remove(NULL, 0));

    // 这些函数应该安全处理空指针
    array_list_clear(NULL);
    array_list_clear_with_free(NULL);
    array_list_destroy(NULL);
    array_list_destroy_with_free(NULL);
    array_list_print_info(NULL);
}

// ============================================================================
// 测试运行器
// ============================================================================
int main(void) {
    const struct CMUnitTest tests[] = {
        // Basic Interface Tests
        cmocka_unit_test(test_basic_interface_creation_and_properties),
        cmocka_unit_test_setup_teardown(test_basic_interface_clear,
                                        setup_test_array, teardown_test_array),
        cmocka_unit_test_setup_teardown(test_basic_interface_full_state,
                                        setup_test_array, teardown_test_array),

        // Random Access Interface Tests
        cmocka_unit_test_setup_teardown(test_random_access_get_set,
                                        setup_test_array, teardown_test_array),
        cmocka_unit_test_setup_teardown(test_random_access_insert,
                                        setup_test_array, teardown_test_array),
        cmocka_unit_test_setup_teardown(test_random_access_remove,
                                        setup_test_array, teardown_test_array),

        // Back Interface Tests
        cmocka_unit_test_setup_teardown(test_back_interface_push_back,
                                        setup_test_array, teardown_test_array),
        cmocka_unit_test_setup_teardown(test_back_interface_pop_back,
                                        setup_test_array, teardown_test_array),

        // Array Interface Tests
        cmocka_unit_test_setup_teardown(test_array_interface_type_info,
                                        setup_test_array, teardown_test_array),
        cmocka_unit_test_setup_teardown(test_array_interface_print_info,
                                        setup_test_array, teardown_test_array),

        // Comprehensive Tests
        cmocka_unit_test_setup_teardown(test_comprehensive_mixed_operations,
                                        setup_test_array, teardown_test_array),

        // Error Handling Tests
        cmocka_unit_test(test_null_pointer_handling),
    };

    printf("开始运行动态数组测试套件...\n");
    int result = cmocka_run_group_tests(tests, NULL, NULL);
    printf("动态数组测试套件完成.\n");

    return result;
}
