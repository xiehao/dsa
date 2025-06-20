/**
 * @file dynamic_array.c
 * @brief 动态数组的实现
 * @author dsa项目团队
 * @date 2025-06-15
 */

#include <internal/dynamic_array.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <internal/array_list_traits.h>

/// 默认初始容量
#define DEFAULT_CAPACITY 10

/**
 * @brief 动态数组结构定义（实现细节）
 * 
 * 该结构体包含了动态数组的所有内部状态信息以及接口指针。
 * 使用组合模式来实现多种容器接口。
 */
typedef struct {
    container_basic_interface_t const *basic;           ///< 基本容器接口
    container_random_access_interface_t const *random_access; ///< 随机访问接口
    container_back_interface_t const *back;             ///< 尾部操作接口
    array_list_interface_t const *array;                ///< 数组接口
    dsa_element_pt *data;                               ///< 指向元素指针数组的指针
    size_t size;                                        ///< 当前元素数量
    size_t capacity;                                    ///< 当前分配的容量
} dynamic_array_t;

/**
 * @brief 调整动态数组容量的内部辅助函数
 * 
 * @param array 指向动态数组的指针
 * @param new_capacity 新的容量大小
 * @return 成功返回true，失败返回false
 * 
 * @note 如果new_capacity为0或负数，将设置为默认容量
 * @note 新容量不能小于当前元素数量
 */
static bool dynamic_array_resize(dynamic_array_t *array, size_t new_capacity) {
    if (new_capacity == 0) {
        // 避免容量非正
        new_capacity = DEFAULT_CAPACITY;
    }
    if (new_capacity < array->size) {
        // 不能将大小调整为小于当前元素数量
        fprintf(stderr, "错误：无法将动态数组调整为小于其当前大小。\n");
        return false;
    }

    dsa_element_pt *new_data = realloc(array->data, new_capacity * sizeof(dsa_element_pt));
    if (!new_data) {
        fprintf(stderr, "错误：无法为动态数组重新分配内存。\n");
        return false; // 分配失败
    }

    array->data = new_data;
    array->capacity = new_capacity;
    return true;
}

/**
 * @brief 检查索引是否越界的内部辅助函数（用于get/set操作）
 * 
 * @param array 指向动态数组的指针
 * @param index 要检查的索引
 * @return 越界返回true，否则返回false
 */
static bool is_index_out_of_bounds(const dynamic_array_t *array, size_t index) {
    if (!array) {
        // 检查数组指针是否有效
        fprintf(stderr, "错误：动态数组指针无效。\n");
        return true; // 无效指针视为越界
    }
    if (index >= array->size) {
        fprintf(stderr, "错误：索引 %zu 超出动态数组大小 %zu 的范围 [0, %zu)。\n", index, array->size, array->size);
        return true;
    }
    return false;
}

/**
 * @brief 检查插入索引是否无效的内部辅助函数（用于insert操作）
 * 
 * @param array 指向动态数组的指针
 * @param index 要检查的插入索引
 * @return 无效返回true，否则返回false
 * 
 * @note 插入操作允许索引等于size（在末尾插入）
 */
static bool is_insert_index_invalid(const dynamic_array_t *array, size_t index) {
    if (!array) {
        // 检查数组指针是否有效
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

/**
 * @brief 获取动态数组当前元素数量
 * 
 * @param array 指向动态数组的指针
 * @return 数组中的元素数量，如果数组为NULL则返回0
 */
static size_t dynamic_array_size(dsa_const_container_pt array) {
    dynamic_array_t const *this = array;
    return this ? this->size : 0;
}

/**
 * @brief 获取动态数组当前容量
 * 
 * @param array 指向动态数组的指针
 * @return 数组的当前容量，如果数组为NULL则返回0
 */
static size_t dynamic_array_capacity(dsa_const_container_pt array) {
    dynamic_array_t const *this = array;
    return this ? this->capacity : 0;
}

/**
 * @brief 检查动态数组是否为空
 * 
 * @param array 指向动态数组的指针
 * @return 数组为空返回true，否则返回false；如果数组为NULL也返回true
 */
static bool dynamic_array_is_empty(dsa_const_container_pt array) {
    dynamic_array_t const *this = array;
    return this ? (this->size == 0) : true;
}

/**
 * @brief 检查动态数组是否已满
 * 
 * @param array 指向动态数组的指针
 * @return 数组已满返回true，否则返回false；如果数组为NULL返回true
 */
static bool dynamic_array_is_full(dsa_const_container_pt array) {
    dynamic_array_t const *this = array;
    return this ? (this->size == this->capacity) : true;
}

/**
 * @brief 清空动态数组但保留容量
 *
 * 此函数将数组大小重置为0，但不释放元素指向的内存，
 * 也不释放数组的存储空间。数组可以继续使用。
 *
 * @param array 指向要清空的DynamicArray的指针
 */
static void dynamic_array_clear(dsa_container_pt array) {
    if (!array) {
        return;
    }
    dynamic_array_t *this = array;
    // 只重置大小，保留容量和数据指针
    this->size = 0;
}

/**
 * @brief 清空动态数组并释放所有元素
 *
 * 此函数首先释放数组中每个元素指向的内存，
 * 然后将数组大小重置为0。数组结构本身保留，可以继续使用。
 *
 * @param array 指向要清空的DynamicArray的指针
 */
static void dynamic_array_clear_with_free(dsa_container_pt array) {
    if (!array) {
        return;
    }
    dynamic_array_t *this = array;
    // 释放每个元素指向的内存
    for (size_t i = 0; i < this->size; i++) {
        free(this->data[i]);
        this->data[i] = NULL; // 防止悬空指针
    }
    // 重置大小，保留容量
    this->size = 0;
}

/**
 * @brief 销毁动态数组但不释放元素
 *
 * 此函数释放数组本身使用的内存，但不释放数组中存储的元素指针指向的内存。
 * 调用者需要自行管理元素的生命周期。
 *
 * @param array 指向要销毁的动态数组的指针
 */
static void dynamic_array_destroy(dsa_container_pt array) {
    dynamic_array_t const *this = array;
    if (!this) {
        return;
    }
    // 注意：此函数不会释放数组指向的元素，
    // 因为数组只存储指针。调用者负责管理
    // 实际元素的生命周期。
    free(this->data);
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
static void dynamic_array_destroy_with_free(dsa_container_pt array) {
    if (!array) {
        return;
    }
    dynamic_array_t *this = array;
    // 释放每个元素指向的内存
    for (size_t i = 0; i < this->size; ++i) {
        free(this->data[i]); // 假设元素是指向堆内存的指针
        this->data[i] = NULL; // 可选：将指针置为 NULL
    }
    // 释放存储指针的数组
    free(this->data);
    // 释放动态数组结构本身
    free(array);
}

/// 基本容器接口实现
static container_basic_interface_t const basic_trait = {
    .get_size = dynamic_array_size,
    .get_capacity = dynamic_array_capacity,
    .is_empty = dynamic_array_is_empty,
    .is_full = dynamic_array_is_full,
    .clear = dynamic_array_clear,
    .clear_with_free = dynamic_array_clear_with_free,
    .destroy = dynamic_array_destroy,
    .destroy_with_free = dynamic_array_destroy_with_free,
};

/**
 * @brief 获取指定索引处的元素
 * 
 * @param array 指向动态数组的指针
 * @param index 要获取元素的索引
 * @return 指向元素的指针，如果索引无效则返回NULL
 */
static dsa_element_pt dynamic_array_get(dsa_const_container_pt array, size_t index) {
    dynamic_array_t const *this = array;
    if (is_index_out_of_bounds(array, index)) {
        return NULL;
    }
    return this->data[index];
}

/**
 * @brief 设置指定索引处的元素
 * 
 * @param array 指向动态数组的指针
 * @param index 要设置元素的索引
 * @param element 要设置的元素指针
 * @return DSA_SUCCESS表示成功，DSA_ERROR_INDEX_OUT_OF_BOUNDS表示索引越界
 * 
 * @note 此函数不会释放被替换的旧元素
 */
static dsa_result_t dynamic_array_set(dsa_container_pt array, size_t index, dsa_element_pt element) {
    dynamic_array_t *this = array;
    if (is_index_out_of_bounds(array, index)) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS; // 错误/越界时返回错误码
    }
    // 不应该在这里释放旧元素，否则与函数文档不符
    // 根据dynamic_array.h中的说明，该函数不应释放旧元素
    // 设置新元素
    this->data[index] = element;
    return DSA_SUCCESS;
}

/**
 * @brief 在指定位置插入元素
 * 
 * @param array 指向动态数组的指针
 * @param index 插入位置的索引
 * @param element 要插入的元素指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 插入成功
 * @retval DSA_ERROR_NULL_POINTER 参数为空指针
 * @retval DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引超出范围
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 */
static dsa_result_t dynamic_array_insert(dsa_container_pt array, size_t index, dsa_element_pt element) {
    dynamic_array_t *this = array;
    // 检查参数有效性
    if (this == NULL || element == NULL) {
        return DSA_ERROR_NULL_POINTER;
    }
    // 检查插入索引是否有效
    if (index > this->size) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    // 检查是否需要调整大小
    if (this->size >= this->capacity) {
        size_t new_capacity = (this->capacity == 0) ? DEFAULT_CAPACITY : this->capacity * 2;
        if (!dynamic_array_resize(array, new_capacity)) {
            return DSA_ERROR_MEMORY_ALLOCATION; // 调整大小失败
        }
    }

    // 如果插入位置不是末尾，则需要移动元素
    if (index < this->size) {
        // 将从 index 开始的元素向后移动一个位置
        memmove(&this->data[index + 1], &this->data[index],
                (this->size - index) * sizeof(dsa_element_pt));
    }

    // 在指定位置插入新元素
    this->data[index] = element;
    // 增加数组大小
    this->size++;

    return DSA_SUCCESS;
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
static dsa_element_pt dynamic_array_remove(dsa_container_pt array, size_t index) {
    // 使用现有的辅助函数检查索引是否越界
    if (is_index_out_of_bounds(array, index)) {
        return NULL; // 索引无效或数组为空
    }
    dynamic_array_t *this = array;
    // 存储要移除的元素
    dsa_element_pt removed_element = this->data[index];

    // 如果移除的不是最后一个元素，则需要移动后续元素
    if (index < this->size - 1) {
        // 将从 index + 1 开始的元素向前移动一个位置
        memmove(&this->data[index], &this->data[index + 1],
                (this->size - 1 - index) * sizeof(dsa_element_pt));
    }

    // 减少数组大小
    this->size--;

    // 可选：将最后一个（现在未使用的）槽位置为 NULL
    // this->data[array->size] = NULL;

    // 可选：如果大小远小于容量，考虑缩小数组
    if (this->size > 0 && this->size <= this->capacity / 4 && this->capacity > DEFAULT_CAPACITY) {
        dynamic_array_resize(array, this->capacity / 2);
        // 忽略移除操作的调整大小失败，因为元素在逻辑上已经被移除
    }

    return removed_element;
}

/// 随机访问接口实现
static container_random_access_interface_t const random_access_trait = {
    .get_at = dynamic_array_get,
    .set_at = dynamic_array_set,
    .insert_at = dynamic_array_insert,
    .remove_at = dynamic_array_remove,
};

/**
 * @brief 在数组末尾添加元素
 *
 * @param array 指向动态数组的指针
 * @param element 要添加的元素指针
 * @return 操作结果码，同dynamic_array_insert
 */
static dsa_result_t dynamic_array_push_back(dsa_container_pt array, dsa_element_pt element) {
    // 添加元素等同于在末尾插入
    // 直接返回插入函数的结果，现在它返回详细的错误码
    return dynamic_array_insert(array, dynamic_array_size(array), element);
}

/**
 * @brief 移除并返回动态数组的最后一个元素
 *
 * @param array 指向DynamicArray的指针
 * @return 指向被移除元素的指针，如果数组为空或无效则返回NULL
 */
static dsa_element_pt dynamic_array_pop_back(dsa_container_pt array) {
    // 调用 remove 函数移除最后一个元素
    // 此时 size > 0，所以 size - 1 是有效索引
    return dynamic_array_remove(array, dynamic_array_size(array) - 1);
}

/// 尾部操作接口实现
static container_back_interface_t const back_trait = {
    .push_back = dynamic_array_push_back,
    .pop_back = dynamic_array_pop_back,
};

/**
 * @brief 获取动态数组类型
 * 
 * @param array 指向动态数组的指针
 * @return 数组类型枚举值
 */
static dsa_array_list_type_t dynamic_array_get_type() {
    return ARRAY_LIST_TYPE_DYNAMIC;
}

/**
 * @brief 获取动态数组类型名称
 *
 * @return 指向类型名称字符串的指针
 */
static char const *dynamic_array_get_type_name() {
    return "动态数组 (Dynamic Array)";
}

/// 数组接口实现
static array_list_interface_t const array_trait = {
    .get_type = dynamic_array_get_type,
    .get_type_name = dynamic_array_get_type_name,
};

/**
 * @brief 创建新的动态数组
 * 
 * @param initial_capacity 初始容量，如果小于等于0则使用默认容量
 * @return 指向新创建的动态数组的指针，创建失败返回NULL
 * 
 * @note 返回的指针需要通过destroy函数释放
 * @see dynamic_array_destroy
 * @see dynamic_array_destroy_with_free
 */
dsa_array_list_t *dynamic_array_create(size_t initial_capacity) {
    dynamic_array_t *array = malloc(sizeof(dynamic_array_t));
    if (!array) {
        perror("为 dynamic_array_t 结构分配内存失败");
        return NULL;
    }
    array->basic = &basic_trait;
    array->random_access = &random_access_trait;
    array->back = &back_trait;
    array->array = &array_trait;
    array->size = 0;
    array->capacity = (initial_capacity > 0) ? initial_capacity : DEFAULT_CAPACITY;
    array->data = (dsa_element_pt *) malloc(array->capacity * sizeof(dsa_element_pt));
    if (!array->data) {
        perror("为动态数组数据分配内存失败");
        free(array); // 清理结构分配
        return NULL;
    }
    return (dsa_array_list_t *) array;
}