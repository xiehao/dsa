#include <dsa/dynamic_array.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_CAPACITY 10

// 定义动态数组的结构（实现细节）
struct DynamicArray {
    void** data;        // 指向void指针数组的指针
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

    void** new_data = realloc(array->data, new_capacity * sizeof(void*));
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

    array->data = (void**)malloc(array->capacity * sizeof(void*));
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


bool dynamic_array_add(DynamicArray* array, void* element) {
    if (!array) {
        return false;
    }

    // 检查是否需要调整大小
    if (array->size >= array->capacity) {
        size_t new_capacity = (array->capacity == 0) ? DEFAULT_CAPACITY : array->capacity * 2;
        if (!dynamic_array_resize(array, new_capacity)) {
            return false; // 调整大小失败
        }
    }

    array->data[array->size] = element;
    array->size++;
    return true;
}

void* dynamic_array_get(const DynamicArray* array, size_t index) {
    if (!array || index >= array->size) {
        // 索引越界或数组为空
        if (array) {
             fprintf(stderr, "错误：索引 %zu 超出动态数组大小 %zu 的范围。\n", index, array->size);
        }
        return NULL;
    }
    return array->data[index];
}

void* dynamic_array_set(DynamicArray* array, size_t index, void* element) {
    if (!array || index >= array->size) {
        // 索引越界或数组为空
        if (array) {
             fprintf(stderr, "错误：索引 %zu 超出动态数组大小 %zu 的范围。\n", index, array->size);
        }
        return NULL; // 错误/越界时返回 NULL
    }
    // 存储旧元素
    void* old_element = array->data[index];
    // 设置新元素
    array->data[index] = element;
    // 返回旧元素
    return old_element;
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
void* dynamic_array_pop_back(DynamicArray* array) {
    if (!array || array->size == 0) {
        return NULL; // 数组无效或为空
    }

    // 获取最后一个元素
    void* last_element = array->data[array->size - 1];

    // 减少大小
    array->size--;

    // 可选：如果大小远小于容量，考虑缩小数组
    // if (array->size > 0 && array->size <= array->capacity / 4 && array->capacity > DEFAULT_CAPACITY) {
    //     dynamic_array_resize(array, array->capacity / 2);
    //     // 忽略弹出操作的调整大小失败，因为元素在逻辑上已经被移除
    // }

    // 将指针槽位置为 NULL（可选，有助于捕获悬挂指针问题）
    // array->data[array->size] = NULL;

    return last_element;
}
bool dynamic_array_is_empty(const DynamicArray* array) {
    return array ? (array->size == 0) : true;
}