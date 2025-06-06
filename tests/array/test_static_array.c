#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/array/internal/static_array.h"
#include "dsa/common.h" // 包含通用定义

// Test case for static_array_init and basic properties
static void test_static_array_init(void **state) {
    (void)state; // Unused

    const size_t capacity = 10;
    const size_t element_size = sizeof(int);
    int buffer[capacity];
    StaticArray arr;

    bool result = static_array_init(&arr, buffer, capacity, element_size);
    assert_true(result);
    assert_non_null(arr.data);
    assert_int_equal(static_array_size(&arr), 0);
    assert_int_equal(static_array_capacity(&arr), capacity);
    assert_int_equal(arr.element_size, element_size);
    assert_true(static_array_is_empty(&arr));
    assert_false(static_array_is_full(&arr));

    static_array_destroy(&arr);
    assert_null(arr.data);
    assert_int_equal(arr.size, 0);
    assert_int_equal(arr.capacity, 0);
}

// Test case for push_back, get, set, size, is_full
static void test_static_array_push_get_set(void **state) {
    (void)state; // Unused

    const size_t capacity = 5;
    const size_t element_size = sizeof(int);
    int buffer[capacity];
    StaticArray arr;
    static_array_init(&arr, buffer, capacity, element_size);

    // Push elements
    for (int i = 0; i < capacity; ++i) {
        int value = i * 10;
        bool pushed = static_array_push_back(&arr, &value);
        assert_true(pushed);
        assert_int_equal(static_array_size(&arr), i + 1);
    }

    // Check if full
    assert_true(static_array_is_full(&arr));

    // Try pushing when full
    int extra_value = 99;
    bool pushed_full = static_array_push_back(&arr, &extra_value);
    assert_false(pushed_full);
    assert_int_equal(static_array_size(&arr), capacity); // Size should not change

    // Get and verify elements
    for (int i = 0; i < capacity; ++i) {
        int *retrieved_value = (int*)static_array_get(&arr, i);
        assert_non_null(retrieved_value);
        assert_int_equal(ELEMENT_VALUE(int, retrieved_value), i * 10);
    }

    // Test get out of bounds
    assert_null(static_array_get(&arr, capacity));

    // Set elements
    for (int i = 0; i < capacity; ++i) {
        int new_value = i * 100;
        dsa_result_t set_result = static_array_set(&arr, i, &new_value);
        assert_true(set_result == DSA_SUCCESS);
    }

    // Verify set elements
    for (int i = 0; i < capacity; ++i) {
        int *retrieved_value = (int*)static_array_get(&arr, i);
        assert_non_null(retrieved_value);
        assert_int_equal(ELEMENT_VALUE(int, retrieved_value), i * 100);
    }

    // Test set out of bounds
    int value_out = 1000;
    assert_true(static_array_set(&arr, capacity, &value_out) == DSA_ERROR_INDEX_OUT_OF_BOUNDS);

    static_array_destroy(&arr);
}

// Test case for pop_back
static void test_static_array_pop_back(void **state) {
    (void)state; // Unused

    const size_t capacity = 3;
    const size_t element_size = sizeof(char);
    char buffer[capacity];
    StaticArray arr;
    static_array_init(&arr, buffer, capacity, element_size);

    char v1 = 'a', v2 = 'b', v3 = 'c';
    static_array_push_back(&arr, &v1);
    static_array_push_back(&arr, &v2);
    static_array_push_back(&arr, &v3);

    assert_int_equal(static_array_size(&arr), 3);

    // Pop elements
    bool popped = static_array_pop_back(&arr);
    assert_true(popped);
    assert_int_equal(static_array_size(&arr), 2);
    // Verify remaining elements (optional check, depends on whether pop clears)
    char *val = (char*)static_array_get(&arr, 1);
    assert_non_null(val);
    assert_int_equal(ELEMENT_VALUE(char, val), 'b');

    popped = static_array_pop_back(&arr);
    assert_true(popped);
    assert_int_equal(static_array_size(&arr), 1);

    popped = static_array_pop_back(&arr);
    assert_true(popped);
    assert_int_equal(static_array_size(&arr), 0);
    assert_true(static_array_is_empty(&arr));

    // Try popping when empty
    popped = static_array_pop_back(&arr);
    assert_false(popped);
    assert_int_equal(static_array_size(&arr), 0);

    static_array_destroy(&arr);
}

// Test case for insert
static void test_static_array_insert(void **state) {
    (void)state; // Unused

    const size_t capacity = 5;
    const size_t element_size = sizeof(double);
    double buffer[capacity];
    StaticArray arr;
    static_array_init(&arr, buffer, capacity, element_size);

    double v1 = 1.1, v2 = 2.2, v3 = 3.3;
    static_array_push_back(&arr, &v1);
    static_array_push_back(&arr, &v2);

    // Insert at the beginning
    double v_insert_begin = 0.5;
    bool inserted = static_array_insert(&arr, 0, &v_insert_begin);
    assert_true(inserted);
    assert_int_equal(static_array_size(&arr), 3);
    assert_double_equal(ELEMENT_VALUE(double, static_array_get(&arr, 0)), 0.5, 1e-9);
    assert_double_equal(ELEMENT_VALUE(double, static_array_get(&arr, 1)), 1.1, 1e-9);
    assert_double_equal(ELEMENT_VALUE(double, static_array_get(&arr, 2)), 2.2, 1e-9);

    // Insert in the middle
    double v_insert_mid = 1.5;
    inserted = static_array_insert(&arr, 2, &v_insert_mid);
    assert_true(inserted);
    assert_int_equal(static_array_size(&arr), 4);
    assert_double_equal(ELEMENT_VALUE(double, static_array_get(&arr, 0)), 0.5, 1e-9);
    assert_double_equal(ELEMENT_VALUE(double, static_array_get(&arr, 1)), 1.1, 1e-9);
    assert_double_equal(ELEMENT_VALUE(double, static_array_get(&arr, 2)), 1.5, 1e-9);
    assert_double_equal(ELEMENT_VALUE(double, static_array_get(&arr, 3)), 2.2, 1e-9);

    // Insert at the end (equivalent to push_back)
    double v_insert_end = 4.4;
    inserted = static_array_insert(&arr, 4, &v_insert_end);
    assert_true(inserted);
    assert_int_equal(static_array_size(&arr), 5);
    assert_double_equal(ELEMENT_VALUE(double, static_array_get(&arr, 4)), 4.4, 1e-9);

    // Try inserting when full
    double v_extra = 5.5;
    inserted = static_array_insert(&arr, 2, &v_extra);
    assert_false(inserted);
    assert_int_equal(static_array_size(&arr), 5);

    // Try inserting out of bounds
    inserted = static_array_insert(&arr, 6, &v_extra);
    assert_false(inserted);

    static_array_destroy(&arr);
}

// Test case for delete
static void test_static_array_delete(void **state) {
    (void)state; // Unused

    const size_t capacity = 4;
    const size_t element_size = sizeof(int);
    int buffer[capacity];
    StaticArray arr;
    static_array_init(&arr, buffer, capacity, element_size);

    int v1 = 10, v2 = 20, v3 = 30, v4 = 40;
    static_array_push_back(&arr, &v1);
    static_array_push_back(&arr, &v2);
    static_array_push_back(&arr, &v3);
    static_array_push_back(&arr, &v4);

    // Delete from the middle
    bool deleted = static_array_delete(&arr, 1); // Delete 20
    assert_true(deleted);
    assert_int_equal(static_array_size(&arr), 3);
    assert_int_equal(ELEMENT_VALUE(int, static_array_get(&arr, 0)), 10);
    assert_int_equal(ELEMENT_VALUE(int, static_array_get(&arr, 1)), 30);
    assert_int_equal(ELEMENT_VALUE(int, static_array_get(&arr, 2)), 40);

    // Delete from the beginning
    deleted = static_array_delete(&arr, 0); // Delete 10
    assert_true(deleted);
    assert_int_equal(static_array_size(&arr), 2);
    assert_int_equal(ELEMENT_VALUE(int, static_array_get(&arr, 0)), 30);
    assert_int_equal(ELEMENT_VALUE(int, static_array_get(&arr, 1)), 40);

    // Delete from the end
    deleted = static_array_delete(&arr, 1); // Delete 40
    assert_true(deleted);
    assert_int_equal(static_array_size(&arr), 1);
    assert_int_equal(ELEMENT_VALUE(int, static_array_get(&arr, 0)), 30);

    // Delete the last element
    deleted = static_array_delete(&arr, 0); // Delete 30
    assert_true(deleted);
    assert_int_equal(static_array_size(&arr), 0);
    assert_true(static_array_is_empty(&arr));

    // Try deleting when empty
    deleted = static_array_delete(&arr, 0);
    assert_false(deleted);

    // Try deleting out of bounds
    static_array_push_back(&arr, &v1); // Add one element back
    deleted = static_array_delete(&arr, 1);
    assert_false(deleted);
    assert_int_equal(static_array_size(&arr), 1);

    static_array_destroy(&arr);
}

// Test case for clear
static void test_static_array_clear(void **state) {
    (void)state; // Unused

    const size_t capacity = 5;
    const size_t element_size = sizeof(int);
    int buffer[capacity];
    StaticArray arr;
    static_array_init(&arr, buffer, capacity, element_size);

    int v = 1;
    static_array_push_back(&arr, &v); v++;
    static_array_push_back(&arr, &v); v++;
    static_array_push_back(&arr, &v);

    assert_int_equal(static_array_size(&arr), 3);
    assert_false(static_array_is_empty(&arr));

    static_array_clear(&arr);

    assert_int_equal(static_array_size(&arr), 0);
    assert_true(static_array_is_empty(&arr));
    assert_int_equal(static_array_capacity(&arr), capacity); // Capacity remains
    // Check if elements are still accessible (they shouldn't be via get)
    assert_null(static_array_get(&arr, 0));

    // Can push again after clear
    v = 10;
    bool pushed = static_array_push_back(&arr, &v);
    assert_true(pushed);
    assert_int_equal(static_array_size(&arr), 1);
    assert_int_equal(ELEMENT_VALUE(int, static_array_get(&arr, 0)), 10);

    static_array_destroy(&arr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_static_array_init),
        cmocka_unit_test(test_static_array_push_get_set),
        cmocka_unit_test(test_static_array_pop_back),
        cmocka_unit_test(test_static_array_insert),
        cmocka_unit_test(test_static_array_delete),
        cmocka_unit_test(test_static_array_clear),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}