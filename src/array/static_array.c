#include "internal/static_array.h"
#include "../../include/common.h"
#include <string.h> // For memcpy and memmove
#include <assert.h> // For assert

// 辅助宏，用于获取给定索引处元素的地址
#define ELEMENT_ADDR(arr, index) ((char*)((arr)->data) + (index) * (arr)->element_size)

bool static_array_init(StaticArray *arr, void *data_buffer, size_t capacity, size_t element_size) {
    if (!arr || !data_buffer || capacity == 0 || element_size == 0) {
        return false;
    }
    arr->data = data_buffer;
    arr->size = 0;
    arr->capacity = capacity;
    arr->element_size = element_size;
    return true;
}

void static_array_destroy(StaticArray *arr) {
    // 对于内存由外部管理的静态数组（例如栈上的 VLA），
    // destroy 不需要释放内存。它只是重置结构体。
    if (arr) {
        arr->data = NULL;
        arr->size = 0;
        arr->capacity = 0;
        arr->element_size = 0;
    }
}

dsa_element_pt static_array_get(const StaticArray *arr, size_t index) {
    if (arr == NULL) {
        return NULL;
    }
    if (index >= arr->size) {
        return NULL; // 索引越界
    }
    return ELEMENT_ADDR(arr, index);
}

dsa_result_t static_array_set(StaticArray *arr, size_t index, dsa_element_pt value) {
    if (arr == NULL || value == NULL) {
        return DSA_ERROR_NULL_POINTER;
    }
    if (index >= arr->size) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS; // 索引越界
    }
    memcpy(ELEMENT_ADDR(arr, index), value, arr->element_size);
    return DSA_SUCCESS;
}

size_t static_array_size(const StaticArray *arr) {
    if (arr == NULL) {
        return 0;
    }
    return arr->size;
}

size_t static_array_capacity(const StaticArray *arr) {
    if (arr == NULL) {
        return 0;
    }
    return arr->capacity;
}

bool static_array_is_empty(const StaticArray *arr) {
    if (arr == NULL) {
        return true;
    }
    return arr->size == 0;
}

bool static_array_is_full(const StaticArray *arr) {
    if (arr == NULL) {
        return false;
    }
    return arr->size == arr->capacity;
}

dsa_result_t static_array_push_back(StaticArray *arr, dsa_element_pt value) {
    // 通过调用insert函数在末尾插入元素
    return static_array_insert(arr, arr->size, value);
}

dsa_element_pt static_array_pop_back(StaticArray *arr) {
    if (arr == NULL || static_array_is_empty(arr)) {
        return NULL;
    }

    // 删除并返回最后一个元素
    return static_array_delete(arr, arr->size - 1);
}

dsa_result_t static_array_insert(StaticArray *arr, size_t index, dsa_element_pt value) {
    if (arr == NULL || value == NULL) {
        return DSA_ERROR_NULL_POINTER;
    }
    if (index > arr->size) { // 允许在末尾插入 (index == size)
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS; // 索引越界
    }
    if (static_array_is_full(arr)) {
        return DSA_ERROR_CAPACITY_FULL; // 数组已满
    }

    // 向右移动元素，为新元素腾出空间
    if (index < arr->size) {
        memmove(ELEMENT_ADDR(arr, index + 1),
                ELEMENT_ADDR(arr, index),
                (arr->size - index) * arr->element_size);
    }

    // 插入新元素
    memcpy(ELEMENT_ADDR(arr, index), value, arr->element_size);
    arr->size++;
    return DSA_SUCCESS;
}

dsa_element_pt static_array_delete(StaticArray *arr, size_t index) {
    if (arr == NULL) {
        return NULL;
    }
    if (static_array_is_empty(arr) || index >= arr->size) {
        return NULL; // 数组为空或索引越界
    }

    // 获取要删除元素的指针
    dsa_element_pt element_to_delete = ELEMENT_ADDR(arr, index);

    // 向左移动元素以填补空缺
    if (index < arr->size - 1) {
        memmove(ELEMENT_ADDR(arr, index),
                ELEMENT_ADDR(arr, index + 1),
                (arr->size - index - 1) * arr->element_size);
    }

    arr->size--;

    // 返回被删除元素的指针
    return element_to_delete;
}

void static_array_clear(StaticArray *arr) {
    if (arr == NULL) {
        return;
    }
    arr->size = 0;
    // 可选：清除内存（非严格必要）
    // memset(arr->data, 0, arr->capacity * arr->element_size);
}