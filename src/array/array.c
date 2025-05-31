#include <dsa/array.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

// 包含内部头文件
#include "internal/static_array.h"
#include "internal/dynamic_array.h"

/**
 * @brief 统一数组结构体
 *
 * 使用联合体来节省内存，同时包含类型信息和虚函数表
 */
struct array_t {
    dsa_array_type_t type;                 ///< 数组类型
    union {
        StaticArray static_array;   ///< 静态数组实例
        DynamicArray* dynamic_array;///< 动态数组指针
    } impl;                         ///< 具体实现
};

// ============================================================================
// 内部辅助函数
// ============================================================================

/**
 * @brief 检查数组指针是否有效
 */
static bool is_valid_array(const dsa_array_t* array) {
    return array != NULL;
}

/**
 * @brief 获取错误信息字符串
 */
static const char* get_error_string(dsa_array_result_t result) {
    switch (result) {
        case ARRAY_SUCCESS: return "成功";
        case ARRAY_ERROR_NULL_POINTER: return "空指针错误";
        case ARRAY_ERROR_INDEX_OUT_OF_BOUNDS: return "索引越界";
        case ARRAY_ERROR_CAPACITY_FULL: return "容量已满";
        case ARRAY_ERROR_EMPTY: return "数组为空";
        case ARRAY_ERROR_MEMORY_ALLOCATION: return "内存分配失败";
        case ARRAY_ERROR_INVALID_PARAMETER: return "无效参数";
        default: return "未知错误";
    }
}

// ============================================================================
// 创建和销毁函数
// ============================================================================

dsa_array_t* array_create_static(void* buffer, size_t capacity, size_t element_size) {
    if (!buffer || capacity == 0 || element_size == 0) {
        return NULL;
    }

    dsa_array_t* array = malloc(sizeof(dsa_array_t));
    if (!array) {
        return NULL;
    }

    array->type = ARRAY_TYPE_STATIC;
    if (!static_array_init(&array->impl.static_array, buffer, capacity, element_size)) {
        free(array);
        return NULL;
    }

    return array;
}

dsa_array_t* array_create_dynamic(size_t initial_capacity) {
    dsa_array_t* array = malloc(sizeof(dsa_array_t));
    if (!array) {
        return NULL;
    }

    array->type = ARRAY_TYPE_DYNAMIC;
    array->impl.dynamic_array = dynamic_array_create(initial_capacity);
    if (!array->impl.dynamic_array) {
        free(array);
        return NULL;
    }

    return array;
}

void array_destroy(dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            static_array_destroy(&array->impl.static_array);
            break;
        case ARRAY_TYPE_DYNAMIC:
            dynamic_array_destroy(array->impl.dynamic_array);
            break;
    }

    free(array);
}

void array_destroy_with_free(dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            // 静态数组不支持自动释放元素
            static_array_destroy(&array->impl.static_array);
            break;
        case ARRAY_TYPE_DYNAMIC:
            dynamic_array_destroy_with_free(array->impl.dynamic_array);
            break;
    }

    free(array);
}

// ============================================================================
// 基本操作函数
// ============================================================================

dsa_element_pt array_get(const dsa_array_t* array, size_t index) {
    if (!is_valid_array(array)) {
        return NULL;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            return static_array_get(&array->impl.static_array, index);
        case ARRAY_TYPE_DYNAMIC:
            return dynamic_array_get(array->impl.dynamic_array, index);
        default:
            return NULL;
    }
}

dsa_array_result_t array_set(dsa_array_t* array, size_t index, dsa_element_pt element) {
    if (!is_valid_array(array)) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            bool result = static_array_set(&array->impl.static_array, index, element);
            return result ? ARRAY_SUCCESS : ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
        }
        case ARRAY_TYPE_DYNAMIC: {
            dsa_element_pt old_element = dynamic_array_set(array->impl.dynamic_array, index, element);
            if (old_element) {
                free(old_element);  // 释放旧元素，保持与类型安全函数的一致性
                return ARRAY_SUCCESS;
            } else {
                return ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
            }
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}

dsa_array_result_t array_push_back(dsa_array_t* array, dsa_element_pt element) {
    if (!is_valid_array(array)) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            bool result = static_array_push_back(&array->impl.static_array, element);
            if (!result) {
                if (static_array_is_full(&array->impl.static_array)) {
                    return ARRAY_ERROR_CAPACITY_FULL;
                }
                return ARRAY_ERROR_INVALID_PARAMETER;
            }
            return ARRAY_SUCCESS;
        }
        case ARRAY_TYPE_DYNAMIC: {
            bool result = dynamic_array_push_back(array->impl.dynamic_array, element);
            return result ? ARRAY_SUCCESS : ARRAY_ERROR_MEMORY_ALLOCATION;
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}

dsa_element_pt array_pop_back(dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return NULL;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            if (static_array_is_empty(&array->impl.static_array)) {
                return NULL;
            }

            // 获取最后一个元素的副本
            dsa_element_pt last_element = static_array_get(&array->impl.static_array,
                                                      array->impl.static_array.size - 1);
            if (!last_element) {
                return NULL;
            }

            // 创建元素的副本
            void* element_copy = malloc(array->impl.static_array.element_size);
            if (!element_copy) {
                return NULL;
            }
            memcpy(element_copy, last_element, array->impl.static_array.element_size);

            // 移除最后一个元素
            static_array_pop_back(&array->impl.static_array);

            return element_copy;
        }
        case ARRAY_TYPE_DYNAMIC:
            return dynamic_array_pop_back(array->impl.dynamic_array);
        default:
            return NULL;
    }
}

dsa_array_result_t array_insert(dsa_array_t* array, size_t index, dsa_element_pt element) {
    if (!is_valid_array(array)) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            bool result = static_array_insert(&array->impl.static_array, index, element);
            if (!result) {
                if (static_array_is_full(&array->impl.static_array)) {
                    return ARRAY_ERROR_CAPACITY_FULL;
                }
                if (index > array->impl.static_array.size) {
                    return ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
                }
                return ARRAY_ERROR_INVALID_PARAMETER;
            }
            return ARRAY_SUCCESS;
        }
        case ARRAY_TYPE_DYNAMIC: {
            bool result = dynamic_array_insert(array->impl.dynamic_array, index, element);
            if (!result) {
                if (index > dynamic_array_size(array->impl.dynamic_array)) {
                    return ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
                }
                return ARRAY_ERROR_MEMORY_ALLOCATION;
            }
            return ARRAY_SUCCESS;
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}

dsa_element_pt array_remove(dsa_array_t* array, size_t index) {
    if (!is_valid_array(array)) {
        return NULL;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            if (index >= array->impl.static_array.size) {
                return NULL;
            }

            // 获取要删除元素的副本
            dsa_element_pt element_to_remove = static_array_get(&array->impl.static_array, index);
            if (!element_to_remove) {
                return NULL;
            }

            // 创建元素的副本
            void* element_copy = malloc(array->impl.static_array.element_size);
            if (!element_copy) {
                return NULL;
            }
            memcpy(element_copy, element_to_remove, array->impl.static_array.element_size);

            // 删除元素
            static_array_delete(&array->impl.static_array, index);

            return element_copy;
        }
        case ARRAY_TYPE_DYNAMIC:
            return dynamic_array_remove(array->impl.dynamic_array, index);
        default:
            return NULL;
    }
}

// ============================================================================
// 查询函数
// ============================================================================

size_t array_size(const dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return 0;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            return static_array_size(&array->impl.static_array);
        case ARRAY_TYPE_DYNAMIC:
            return dynamic_array_size(array->impl.dynamic_array);
        default:
            return 0;
    }
}

size_t array_capacity(const dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return 0;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            return static_array_capacity(&array->impl.static_array);
        case ARRAY_TYPE_DYNAMIC:
            return dynamic_array_capacity(array->impl.dynamic_array);
        default:
            return 0;
    }
}

bool array_is_empty(const dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return true;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            return static_array_is_empty(&array->impl.static_array);
        case ARRAY_TYPE_DYNAMIC:
            return dynamic_array_is_empty(array->impl.dynamic_array);
        default:
            return true;
    }
}

bool array_is_full(const dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return false;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            return static_array_is_full(&array->impl.static_array);
        case ARRAY_TYPE_DYNAMIC:
            return false; // 动态数组理论上永远不会满
        default:
            return false;
    }
}

dsa_array_type_t array_get_type(const dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return ARRAY_TYPE_STATIC; // 默认返回静态类型
    }

    return array->type;
}

const char* array_get_type_name(const dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return "无效数组";
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            return "静态数组";
        case ARRAY_TYPE_DYNAMIC:
            return "动态数组";
        default:
            return "未知类型";
    }
}

// ============================================================================
// 其他操作函数
// ============================================================================

void array_clear(dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            static_array_clear(&array->impl.static_array);
            break;
        case ARRAY_TYPE_DYNAMIC:
            dynamic_array_clear(array->impl.dynamic_array);
            break;
    }
}

void array_clear_with_free(dsa_array_t* array) {
    if (!is_valid_array(array)) {
        return;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            // 静态数组不支持释放元素，静默降级为普通clear
            static_array_clear(&array->impl.static_array);
            break;
        case ARRAY_TYPE_DYNAMIC:
            dynamic_array_clear_with_free(array->impl.dynamic_array);
            break;
    }
}

void array_print_info(const dsa_array_t* array) {
    if (!is_valid_array(array)) {
        printf("数组: 无效 (空指针)\n");
        return;
    }

    printf("数组信息:\n");
    printf("  类型: %s\n", array_get_type_name(array));
    printf("  大小: %zu\n", array_size(array));
    printf("  容量: %zu\n", array_capacity(array));
    printf("  是否为空: %s\n", array_is_empty(array) ? "是" : "否");
    printf("  是否已满: %s\n", array_is_full(array) ? "是" : "否");

    if (array->type == ARRAY_TYPE_STATIC) {
        printf("  元素大小: %zu 字节\n", array->impl.static_array.element_size);
    }
}

// ============================================================================
// 类型安全的便利函数
// ============================================================================

dsa_array_result_t array_push_back_int(dsa_array_t* array, int value) {
    if (!is_valid_array(array)) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            // 静态数组：直接传递值的地址
            return array_push_back(array, &value);
        }
        case ARRAY_TYPE_DYNAMIC: {
            // 动态数组：需要在堆上分配内存
            int* heap_value = malloc(sizeof(int));
            if (!heap_value) {
                return ARRAY_ERROR_MEMORY_ALLOCATION;
            }
            *heap_value = value;

            dsa_array_result_t result = array_push_back(array, heap_value);
            if (result != ARRAY_SUCCESS) {
                free(heap_value);
            }
            return result;
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}

dsa_array_result_t array_get_int(const dsa_array_t* array, size_t index, int* value) {
    if (!is_valid_array(array) || !value) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    dsa_element_pt element = array_get(array, index);
    if (!element) {
        return ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    *value = ELEMENT_VALUE(int, element);
    return ARRAY_SUCCESS;
}

dsa_array_result_t array_set_int(dsa_array_t* array, size_t index, int value) {
    if (!is_valid_array(array)) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            // 静态数组：直接传递值的地址
            return array_set(array, index, &value);
        }
        case ARRAY_TYPE_DYNAMIC: {
            // 动态数组：需要在堆上分配内存
            int* heap_value = malloc(sizeof(int));
            if (!heap_value) {
                return ARRAY_ERROR_MEMORY_ALLOCATION;
            }
            *heap_value = value;

            // 获取旧值并释放
            dsa_element_pt old_element = dynamic_array_set(array->impl.dynamic_array, index, heap_value);
            if (old_element) {
                free(old_element);  // 释放旧值
                return ARRAY_SUCCESS;
            } else {
                free(heap_value);   // 设置失败，释放新分配的内存
                return ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
            }
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}

dsa_array_result_t array_push_back_double(dsa_array_t* array, double value) {
    if (!is_valid_array(array)) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            // 静态数组：直接传递值的地址
            return array_push_back(array, &value);
        }
        case ARRAY_TYPE_DYNAMIC: {
            // 动态数组：需要在堆上分配内存
            double* heap_value = malloc(sizeof(double));
            if (!heap_value) {
                return ARRAY_ERROR_MEMORY_ALLOCATION;
            }
            *heap_value = value;

            dsa_array_result_t result = array_push_back(array, heap_value);
            if (result != ARRAY_SUCCESS) {
                free(heap_value);
            }
            return result;
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}

dsa_array_result_t array_get_double(const dsa_array_t* array, size_t index, double* value) {
    if (!is_valid_array(array) || !value) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    dsa_element_pt element = array_get(array, index);
    if (!element) {
        return ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    *value = ELEMENT_VALUE(double, element);
    return ARRAY_SUCCESS;
}

dsa_array_result_t array_set_double(dsa_array_t* array, size_t index, double value) {
    if (!is_valid_array(array)) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            // 静态数组：直接传递值的地址
            return array_set(array, index, &value);
        }
        case ARRAY_TYPE_DYNAMIC: {
            // 动态数组：需要在堆上分配内存
            double* heap_value = malloc(sizeof(double));
            if (!heap_value) {
                return ARRAY_ERROR_MEMORY_ALLOCATION;
            }
            *heap_value = value;

            // 获取旧值并释放
            dsa_element_pt old_element = dynamic_array_set(array->impl.dynamic_array, index, heap_value);
            if (old_element) {
                free(old_element);  // 释放旧值
                return ARRAY_SUCCESS;
            } else {
                free(heap_value);   // 设置失败，释放新分配的内存
                return ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
            }
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}