#include "internal/dynamic_array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_CAPACITY 10

// 定义动态数组的结构（实现细节）
struct DynamicArray {
    dsa_element_pt* data;   // 指向元素指针数组的指针
    size_t size;        // 当前元素数量
    size_t capacity;    // 当前分配的容量
};

// 内部辅助函数，用于调整数组大小
static bool dynamic_array_resize(DynamicArray* array, size_t new_capacity) {
    if (new_capacity <= 0) { // 避免容量非正
        new_capacity = DEFAULT_CAPACITY;
    }
    if (new_capacity < array->size) {
        // 不能将大小调整为小于当前元素数量
        fprintf(stderr, "错误：无法将动态数组调整为小于其当前大小。\n");
        return false;
    }

    dsa_element_pt* new_data = realloc(array->data, new_capacity * sizeof(dsa_element_pt));
    if (!new_data) {
        fprintf(stderr, "错误：无法为动态数组重新分配内存。\n");
        return false; // 分配失败
    }

    array->data = new_data;
    array->capacity = new_capacity;
    return true;
}

DynamicArray* dynamic_array_create(size_t initial_capacity) {
    DynamicArray* array = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (!array) {
        perror("为 DynamicArray 结构分配内存失败");
        return NULL;
    }

    array->size = 0;
    array->capacity = (initial_capacity > 0) ? initial_capacity : DEFAULT_CAPACITY;

    array->data = (dsa_element_pt*)malloc(array->capacity * sizeof(dsa_element_pt));
    if (!array->data) {
        perror("为动态数组数据分配内存失败");
        free(array); // 清理结构分配
        return NULL;
    }

    return array;
}

void dynamic_array_destroy(DynamicArray* array) {
    if (!array) {
        return;
    }
    // 注意：此函数不会释放数组指向的元素，
    // 因为数组只存储指针。调用者负责管理
    // 实际元素的生命周期。
    free(array->data);
    free(array);
}

/**
 * @brief 销毁动态数组并释放其所有元素
 *
 * 此函数首先释放数组中每个元素指向的内存，
 * 然后释放数组本身使用的内存。
 * 只有当数组存储的是堆分配的指针时才应使用此函数。
 *
 * @param array 指向要销毁的DynamicArray的指针
 */
void dynamic_array_destroy_with_free(DynamicArray* array) {
    if (!array) {
        return;
    }
    // 释放每个元素指向的内存
    for (size_t i = 0; i < array->size; ++i) {
        free(array->data[i]); // 假设元素是指向堆内存的指针
        array->data[i] = NULL; // 可选：将指针置为 NULL
    }
    // 释放存储指针的数组
    free(array->data);
    // 释放动态数组结构本身
    free(array);
}


bool dynamic_array_push_back(DynamicArray* array, dsa_element_pt element) {
    // 添加元素等同于在末尾插入
    return dynamic_array_insert(array, dynamic_array_size(array), element);
}

// 内部辅助函数，用于检查索引是否越界 (适用于 get/set 操作)
static bool is_index_out_of_bounds(const DynamicArray* array, size_t index) {
    if (!array) { // 检查数组指针是否有效
        fprintf(stderr, "错误：动态数组指针无效。\n");
        return true; // 无效指针视为越界
    }
    if (index >= array->size) {
        fprintf(stderr, "错误：索引 %zu 超出动态数组大小 %zu 的范围 [0, %zu)。\n", index, array->size, array->size);
        return true;
    }
    return false;
}

// 内部辅助函数，用于检查插入索引是否无效 (适用于 insert 操作)
static bool is_insert_index_invalid(const DynamicArray* array, size_t index) {
    if (!array) { // 检查数组指针是否有效
        fprintf(stderr, "错误：动态数组指针无效。\n");
        return true; // 无效指针视为无效索引
    }
    // 插入索引允许等于 size
    if (index > array->size) {
        fprintf(stderr, "错误：插入索引 %zu 超出动态数组大小 %zu 的允许范围 [0, %zu]。\n", index, array->size, array->size);
        return true;
    }
    return false;
}

dsa_element_pt dynamic_array_get(const DynamicArray* array, size_t index) {
    if (is_index_out_of_bounds(array, index)) {
        return NULL;
    }
    return array->data[index];
}

dsa_element_pt dynamic_array_set(DynamicArray* array, size_t index, dsa_element_pt element) {
    if (is_index_out_of_bounds(array, index)) {
        return NULL; // 错误/越界时返回 NULL
    }
    // 存储旧元素
    dsa_element_pt old_element = array->data[index];
    // 设置新元素
    array->data[index] = element;
    // 返回旧元素
    return old_element;
}

bool dynamic_array_insert(DynamicArray* array, size_t index, dsa_element_pt element) {
    // 使用新的辅助函数检查索引
    if (is_insert_index_invalid(array, index)) {
        return false;
    }

    // 检查是否需要调整大小
    if (array->size >= array->capacity) {
        size_t new_capacity = (array->capacity == 0) ? DEFAULT_CAPACITY : array->capacity * 2;
        if (!dynamic_array_resize(array, new_capacity)) {
            return false; // 调整大小失败
        }
    }

    // 如果插入位置不是末尾，则需要移动元素
    if (index < array->size) {
        // 将从 index 开始的元素向后移动一个位置
        memmove(&array->data[index + 1], &array->data[index],
                (array->size - index) * sizeof(dsa_element_pt));
    }

    // 在指定位置插入新元素
    array->data[index] = element;
    // 增加数组大小
    array->size++;

    return true;
}

/**
 * @brief 从动态数组的指定索引处移除一个元素
 *
 * 索引 `index` 必须在 `[0, size - 1]` 范围内。
 * 后续元素将向前移动以填补空缺。
 *
 * @param array 指向DynamicArray的指针
 * @param index 要移除元素的索引
 * @return 指向被移除元素的指针，如果索引无效或数组为空则返回NULL
 */
dsa_element_pt dynamic_array_remove(DynamicArray* array, size_t index) {
    // 使用现有的辅助函数检查索引是否越界
    if (is_index_out_of_bounds(array, index)) {
        return NULL; // 索引无效或数组为空
    }

    // 存储要移除的元素
    dsa_element_pt removed_element = array->data[index];

    // 如果移除的不是最后一个元素，则需要移动后续元素
    if (index < array->size - 1) {
        // 将从 index + 1 开始的元素向前移动一个位置
        memmove(&array->data[index], &array->data[index + 1],
                (array->size - 1 - index) * sizeof(dsa_element_pt));
    }

    // 减少数组大小
    array->size--;

    // 可选：将最后一个（现在未使用的）槽位置为 NULL
    // array->data[array->size] = NULL;

    // 可选：如果大小远小于容量，考虑缩小数组
    // if (array->size > 0 && array->size <= array->capacity / 4 && array->capacity > DEFAULT_CAPACITY) {
    //     dynamic_array_resize(array, array->capacity / 2);
    //     // 忽略移除操作的调整大小失败，因为元素在逻辑上已经被移除
    // }

    return removed_element;
}

size_t dynamic_array_size(const DynamicArray* array) {
    return array ? array->size : 0;
}

size_t dynamic_array_capacity(const DynamicArray* array) {
    return array ? array->capacity : 0;
}

/**
 * @brief 移除并返回动态数组的最后一个元素
 *
 * @param array 指向DynamicArray的指针
 * @return 指向被移除元素的指针，如果数组为空或无效则返回NULL
 */
dsa_element_pt dynamic_array_pop_back(DynamicArray* array) {
    // 检查数组是否为空或无效
    if (!array || dynamic_array_is_empty(array)) {
        return NULL;
    }
    // 调用 remove 函数移除最后一个元素
    // 此时 size > 0，所以 size - 1 是有效索引
    return dynamic_array_remove(array, dynamic_array_size(array) - 1);
}
bool dynamic_array_is_empty(const DynamicArray* array) {
    return array ? (array->size == 0) : true;
}