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
struct Array {
    ArrayType type;                 ///< 数组类型
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
static bool is_valid_array(const Array* array) {
    return array != NULL;
}

/**
 * @brief 获取错误信息字符串
 */
static const char* get_error_string(ArrayResult result) {
    switch (result) {
        case ARRAY_SUCCESS: return "Success";
        case ARRAY_ERROR_NULL_POINTER: return "Null pointer";
        case ARRAY_ERROR_INDEX_OUT_OF_BOUNDS: return "Index out of bounds";
        case ARRAY_ERROR_CAPACITY_FULL: return "Capacity full";
        case ARRAY_ERROR_EMPTY: return "Array empty";
        case ARRAY_ERROR_MEMORY_ALLOCATION: return "Memory allocation failed";
        case ARRAY_ERROR_INVALID_PARAMETER: return "Invalid parameter";
        default: return "Unknown error";
    }
}

// ============================================================================
// 创建和销毁函数
// ============================================================================

Array* array_create_static(void* buffer, size_t capacity, size_t element_size) {
    if (!buffer || capacity == 0 || element_size == 0) {
        return NULL;
    }

    Array* array = malloc(sizeof(Array));
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

Array* array_create_dynamic(size_t initial_capacity) {
    Array* array = malloc(sizeof(Array));
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

void array_destroy(Array* array) {
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

void array_destroy_with_free(Array* array) {
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

ElementPtr array_get(const Array* array, size_t index) {
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

ArrayResult array_set(Array* array, size_t index, ElementPtr element) {
    if (!is_valid_array(array)) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            bool result = static_array_set(&array->impl.static_array, index, element);
            return result ? ARRAY_SUCCESS : ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
        }
        case ARRAY_TYPE_DYNAMIC: {
            ElementPtr old_element = dynamic_array_set(array->impl.dynamic_array, index, element);
            return old_element ? ARRAY_SUCCESS : ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}

ArrayResult array_push_back(Array* array, ElementPtr element) {
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

ElementPtr array_pop_back(Array* array) {
    if (!is_valid_array(array)) {
        return NULL;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            if (static_array_is_empty(&array->impl.static_array)) {
                return NULL;
            }

            // 获取最后一个元素的副本
            ElementPtr last_element = static_array_get(&array->impl.static_array,
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

ArrayResult array_insert(Array* array, size_t index, ElementPtr element) {
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

ElementPtr array_remove(Array* array, size_t index) {
    if (!is_valid_array(array)) {
        return NULL;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC: {
            if (index >= array->impl.static_array.size) {
                return NULL;
            }

            // 获取要删除元素的副本
            ElementPtr element_to_remove = static_array_get(&array->impl.static_array, index);
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

size_t array_size(const Array* array) {
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

size_t array_capacity(const Array* array) {
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

bool array_is_empty(const Array* array) {
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

bool array_is_full(const Array* array) {
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

ArrayType array_get_type(const Array* array) {
    if (!is_valid_array(array)) {
        return ARRAY_TYPE_STATIC; // 默认返回静态类型
    }

    return array->type;
}

const char* array_get_type_name(const Array* array) {
    if (!is_valid_array(array)) {
        return "Invalid";
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            return "StaticArray";
        case ARRAY_TYPE_DYNAMIC:
            return "DynamicArray";
        default:
            return "Unknown";
    }
}

// ============================================================================
// 其他操作函数
// ============================================================================

void array_clear(Array* array) {
    if (!is_valid_array(array)) {
        return;
    }

    switch (array->type) {
        case ARRAY_TYPE_STATIC:
            static_array_clear(&array->impl.static_array);
            break;
        case ARRAY_TYPE_DYNAMIC:
            // 动态数组没有clear函数，手动实现
            while (!dynamic_array_is_empty(array->impl.dynamic_array)) {
                dynamic_array_pop_back(array->impl.dynamic_array);
            }
            break;
    }
}

void array_print_info(const Array* array) {
    if (!is_valid_array(array)) {
        printf("Array: Invalid (NULL pointer)\n");
        return;
    }

    printf("Array Information:\n");
    printf("  Type: %s\n", array_get_type_name(array));
    printf("  Size: %zu\n", array_size(array));
    printf("  Capacity: %zu\n", array_capacity(array));
    printf("  Is Empty: %s\n", array_is_empty(array) ? "Yes" : "No");
    printf("  Is Full: %s\n", array_is_full(array) ? "Yes" : "No");

    if (array->type == ARRAY_TYPE_STATIC) {
        printf("  Element Size: %zu bytes\n", array->impl.static_array.element_size);
    }
}

// ============================================================================
// 类型安全的便利函数
// ============================================================================

ArrayResult array_push_back_int(Array* array, int value) {
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

            ArrayResult result = array_push_back(array, heap_value);
            if (result != ARRAY_SUCCESS) {
                free(heap_value);
            }
            return result;
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}

ArrayResult array_get_int(const Array* array, size_t index, int* value) {
    if (!is_valid_array(array) || !value) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    ElementPtr element = array_get(array, index);
    if (!element) {
        return ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    *value = ELEMENT_VALUE(int, element);
    return ARRAY_SUCCESS;
}

ArrayResult array_set_int(Array* array, size_t index, int value) {
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
            ElementPtr old_element = dynamic_array_set(array->impl.dynamic_array, index, heap_value);
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

ArrayResult array_push_back_double(Array* array, double value) {
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

            ArrayResult result = array_push_back(array, heap_value);
            if (result != ARRAY_SUCCESS) {
                free(heap_value);
            }
            return result;
        }
        default:
            return ARRAY_ERROR_INVALID_PARAMETER;
    }
}

ArrayResult array_get_double(const Array* array, size_t index, double* value) {
    if (!is_valid_array(array) || !value) {
        return ARRAY_ERROR_NULL_POINTER;
    }

    ElementPtr element = array_get(array, index);
    if (!element) {
        return ARRAY_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    *value = ELEMENT_VALUE(double, element);
    return ARRAY_SUCCESS;
}

ArrayResult array_set_double(Array* array, size_t index, double value) {
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
            ElementPtr old_element = dynamic_array_set(array->impl.dynamic_array, index, heap_value);
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