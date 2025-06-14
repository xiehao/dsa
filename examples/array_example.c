#include <stdio.h>
#include <stdlib.h>
#include <ds/array.h>

/**
 * @brief 演示统一数组接口的使用
 *
 * 这个函数接受任意类型的数组（静态或动态），并执行相同的操作
 */
void demonstrate_unified_array_interface(dsa_array_t* arr, const char* description) {
    printf("\n=== %s ===\n", description);

    // 显示数组初始状态
    printf("初始状态:\n");
    array_print_info(arr);

    // 使用类型安全的函数添加整数
    printf("\n添加整数: 10, 20, 30\n");
    dsa_result_t result;

    result = array_push_back_int(arr, 10);
    if (result != DSA_SUCCESS) {
        printf("添加 10 失败: %d\n", result);
        return;
    }

    result = array_push_back_int(arr, 20);
    if (result != DSA_SUCCESS) {
        printf("添加 20 失败: %d\n", result);
        return;
    }

    result = array_push_back_int(arr, 30);
    if (result != DSA_SUCCESS) {
        printf("添加 30 失败: %d\n", result);
        return;
    }

    // 显示添加后的状态
    printf("添加后状态:\n");
    array_print_info(arr);

    // 显示所有元素
    printf("数组元素: ");
    for (size_t i = 0; i < array_size(arr); i++) {
        int value;
        if (array_get_int(arr, i, &value) == DSA_SUCCESS) {
            printf("%d ", value);
        }
    }
    printf("\n");

    // 在索引1处插入元素15
    printf("\n在索引1处插入元素15\n");
    if (array_get_type(arr) == ARRAY_TYPE_STATIC) {
        int insert_value = 15;
        result = array_insert(arr, 1, &insert_value);
    } else {
        // 动态数组需要堆分配
        int* insert_value = malloc(sizeof(int));
        *insert_value = 15;
        result = array_insert(arr, 1, insert_value);
        if (result != DSA_SUCCESS) {
            free(insert_value);
        }
    }
    if (result == DSA_SUCCESS) {
        printf("插入成功，当前数组: ");
        for (size_t i = 0; i < array_size(arr); i++) {
            int value;
            if (array_get_int(arr, i, &value) == DSA_SUCCESS) {
                printf("%d ", value);
            }
        }
        printf("\n");
    } else {
        printf("插入失败: %d\n", result);
    }

    // 修改索引2处的元素
    printf("\n将索引2处的元素修改为99\n");
    result = array_set_int(arr, 2, 99);
    if (result == DSA_SUCCESS) {
        printf("修改成功，当前数组: ");
        for (size_t i = 0; i < array_size(arr); i++) {
            int value;
            if (array_get_int(arr, i, &value) == DSA_SUCCESS) {
                printf("%d ", value);
            }
        }
        printf("\n");
    } else {
        printf("修改失败: %d\n", result);
    }

    // 移除索引1处的元素
    printf("\n移除索引1处的元素\n");
    dsa_element_pt removed = array_remove(arr, 1);
    if (removed) {
        printf("移除的元素: %d\n", ELEMENT_VALUE(int, removed));
        free(removed);  // 释放移除的元素

        printf("移除后数组: ");
        for (size_t i = 0; i < array_size(arr); i++) {
            int value;
            if (array_get_int(arr, i, &value) == DSA_SUCCESS) {
                printf("%d ", value);
            }
        }
        printf("\n");
    } else {
        printf("移除失败\n");
    }

    // 弹出最后一个元素
    printf("\n弹出最后一个元素\n");
    dsa_element_pt popped = array_pop_back(arr);
    if (popped) {
        printf("弹出的元素: %d\n", ELEMENT_VALUE(int, popped));
        free(popped);  // 释放弹出的元素

        printf("弹出后数组: ");
        for (size_t i = 0; i < array_size(arr); i++) {
            int value;
            if (array_get_int(arr, i, &value) == DSA_SUCCESS) {
                printf("%d ", value);
            }
        }
        printf("\n");
    } else {
        printf("弹出失败\n");
    }

    printf("最终状态:\n");
    array_print_info(arr);
}

/**
 * @brief 清理数组中的所有元素
 *
 * 使用统一的清理接口，自动根据数组类型选择合适的清理方式：
 * - 动态数组：释放所有元素内存并清空
 * - 静态数组：只清空，不释放元素内存
 */
void cleanup_array_elements(dsa_array_t* arr) {
    array_clear_with_free(arr);
}

/**
 * @brief 演示双精度浮点数组操作
 */
void demonstrate_double_array(dsa_array_t* arr) {
    printf("\n=== 双精度浮点数组演示 ===\n");

    // 添加一些双精度浮点数
    array_push_back_double(arr, 3.14);
    array_push_back_double(arr, 2.71);
    array_push_back_double(arr, 1.41);

    printf("双精度数组元素: ");
    for (size_t i = 0; i < array_size(arr); i++) {
        double value;
        if (array_get_double(arr, i, &value) == DSA_SUCCESS) {
            printf("%.2f ", value);
        }
    }
    printf("\n");

    // 修改第二个元素
    array_set_double(arr, 1, 2.72);
    printf("修改后的数组: ");
    for (size_t i = 0; i < array_size(arr); i++) {
        double value;
        if (array_get_double(arr, i, &value) == DSA_SUCCESS) {
            printf("%.2f ", value);
        }
    }
    printf("\n");

    // 清理
    cleanup_array_elements(arr);
}

int main() {
    printf("=== 统一数组接口演示程序 ===\n");
    printf("本程序演示如何使用统一的接口操作静态数组和动态数组\n");

    // 1. 创建并测试静态数组
    printf("\n1. 测试静态数组\n");
    int static_buffer[10];  // 静态缓冲区
    dsa_array_t* static_arr = array_create_static(static_buffer, 10, sizeof(int));

    if (static_arr) {
        demonstrate_unified_array_interface(static_arr, "静态数组演示");
        array_destroy(static_arr);
    } else {
        printf("创建静态数组失败\n");
    }

    // 2. 创建并测试动态数组
    printf("\n\n2. 测试动态数组\n");
    dsa_array_t* dynamic_arr = array_create_dynamic(5);

    if (dynamic_arr) {
        demonstrate_unified_array_interface(dynamic_arr, "动态数组演示");
        cleanup_array_elements(dynamic_arr);
        array_destroy(dynamic_arr);
    } else {
        printf("创建动态数组失败\n");
    }

    // 3. 演示双精度浮点数组
    printf("\n\n3. 测试双精度浮点数组\n");
    dsa_array_t* double_arr = array_create_dynamic(3);
    if (double_arr) {
        demonstrate_double_array(double_arr);
        array_destroy(double_arr);
    }

    // 4. 演示容量限制（静态数组）
    printf("\n\n4. 测试静态数组容量限制\n");
    int small_buffer[3];
    dsa_array_t* small_arr = array_create_static(small_buffer, 3, sizeof(int));
    if (small_arr) {
        printf("创建容量为3的静态数组\n");

        // 添加3个元素
        for (int i = 1; i <= 3; i++) {
            dsa_result_t result = array_push_back_int(small_arr, i * 10);
            printf("添加 %d: %s\n", i * 10,
                   result == DSA_SUCCESS ? "成功" : "失败");
        }

        // 尝试添加第4个元素（应该失败）
        dsa_result_t result = array_push_back_int(small_arr, 40);
        printf("尝试添加第4个元素 40: %s\n",
               result == DSA_SUCCESS ? "成功" : "失败（预期）");

        array_print_info(small_arr);
        array_destroy(small_arr);
    }

    printf("\n=== 演示完成 ===\n");
    return 0;
}
