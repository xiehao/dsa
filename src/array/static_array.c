/**
 * @file static_array.c
 * @brief 静态数组实现
 * @details 提供基于预分配缓冲区的静态数组容器实现，支持基本的数组操作
 * @author DSA项目组
 * @date 2025-06-16
 */

#include <internal/static_array.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <internal/array_list_traits.h>

/**
 * @struct static_array_t
 * @brief 静态数组结构体
 * @details 基于预分配缓冲区的静态数组容器，容量固定不可变
 */
typedef struct {
    trait_array_list_t const *trait;       ///< 数组接口
    dsa_element_pt data;                   ///< 指向实际数据存储的指针 (通常是 VLA)
    size_t size;                           ///< 当前元素数量
    size_t capacity;                       ///< 数组的总容量 (固定)
    size_t element_size;                   ///< 单个元素的大小（字节）
} static_array_t;

/**
 * @def ELEMENT_ADDR(arr, index)
 * @brief 获取指定索引处元素的地址
 * @param arr 数组指针
 * @param index 元素索引
 * @return 指向指定索引元素的指针
 */
#define ELEMENT_ADDR(arr, index) ((char*)((arr)->data) + (index) * (arr)->element_size)

/**
 * @brief 检查索引是否越界
 * @param array 静态数组指针
 * @param index 要检查的索引
 * @return true 如果索引越界或数组指针无效，false 否则
 */
static bool is_index_out_of_bounds(const static_array_t *array, size_t index) {
    if (!array) {
        // 检查数组指针是否有效
        fprintf(stderr, "错误：静态数组指针无效。\n");
        return true; // 无效指针视为越界
    }
    if (index >= array->size) {
        fprintf(stderr, "错误：索引 %zu 超出静态数组大小 %zu 的范围 [0, %zu)。\n", index, array->size, array->size);
        return true;
    }
    return false;
}

/**
 * @brief 获取数组中元素的数量
 * @param array 数组容器指针
 * @return 数组中元素的数量，如果数组为空则返回0
 */
static size_t static_array_size(dsa_const_container_pt array) {
    static_array_t const *this = array;
    return this ? this->size : 0;
}

/**
 * @brief 获取数组的容量
 * @param array 数组容器指针
 * @return 数组的最大容量，如果数组为空则返回0
 */
static size_t static_array_capacity(dsa_const_container_pt array) {
    static_array_t const *this = array;
    return this ? this->capacity : 0;
}

/**
 * @brief 检查数组是否为空
 * @param array 数组容器指针
 * @return true 如果数组为空或指针无效，false 否则
 */
static bool static_array_is_empty(dsa_const_container_pt array) {
    static_array_t const *this = array;
    return this ? this->size == 0 : true;
}

/**
 * @brief 检查数组是否已满
 * @param array 数组容器指针
 * @return true 如果数组已满或指针无效，false 否则
 */
static bool static_array_is_full(dsa_const_container_pt array) {
    static_array_t const *this = array;
    return this ? this->size == this->capacity : true;
}

/**
 * @brief 清空数组
 * @param array 数组容器指针
 * @details 将数组大小重置为0，但不释放内存
 */
static void static_array_clear(dsa_container_pt array) {
    if (array == NULL) {
        return;
    }
    static_array_t *this = array;
    this->size = 0;
    // 可选：清除内存（非严格必要）
    // memset(arr->data, 0, arr->capacity * arr->element_size);
}

/**
 * @brief 销毁数组
 * @param array 数组容器指针
 * @details 重置数组结构体，但不释放外部管理的内存
 */
static void static_array_destroy(dsa_container_pt array) {
    static_array_t *this = array;
    // 对于内存由外部管理的静态数组（例如栈上的 VLA），
    // destroy 不需要释放内存。它只是重置结构体。
    if (this) {
        this->data = NULL;
        this->size = 0;
        this->capacity = 0;
        this->element_size = 0;
    }
}

/**
 * @brief 基本容器接口实现
 */
static trait_basic_t const basic_trait = {
    .get_size = static_array_size,
    .get_capacity = static_array_capacity,
    .is_empty = static_array_is_empty,
    .is_full = static_array_is_full,
    .clear = static_array_clear,
    .clear_with_free = static_array_clear,
    .destroy = static_array_destroy,
    .destroy_with_free = static_array_destroy,
};

/**
 * @brief 获取指定索引处的元素
 * @param array 数组容器指针
 * @param index 元素索引
 * @return 指向元素的指针，如果索引越界或数组为空则返回NULL
 */
static dsa_element_pt static_array_get(dsa_const_container_pt array, size_t index) {
    if (array == NULL) {
        return NULL;
    }
    static_array_t const *this = array;
    if (index >= this->size) {
        return NULL; // 索引越界
    }
    return ELEMENT_ADDR(this, index);
}

/**
 * @brief 设置指定索引处的元素值
 * @param array 数组容器指针
 * @param index 元素索引
 * @param value 要设置的值
 * @return DSA_SUCCESS 成功，DSA_ERROR_NULL_POINTER 空指针，DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引越界
 */
static dsa_result_t static_array_set(dsa_container_pt array, size_t index, dsa_element_pt value) {
    if (array == NULL || value == NULL) {
        return DSA_ERROR_NULL_POINTER;
    }
    static_array_t *this = array;
    if (index >= this->size) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS; // 索引越界
    }
    memcpy(ELEMENT_ADDR(this, index), value, this->element_size);
    return DSA_SUCCESS;
}

/**
 * @brief 在指定位置插入元素
 * @param array 数组容器指针
 * @param index 插入位置索引
 * @param value 要插入的值
 * @return DSA_SUCCESS 成功，DSA_ERROR_NULL_POINTER 空指针，
 *         DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引越界，DSA_ERROR_CAPACITY_FULL 数组已满
 */
static dsa_result_t static_array_insert(dsa_container_pt array, size_t index, dsa_element_pt value) {
    static_array_t *this = array;
    if (this == NULL || value == NULL) {
        return DSA_ERROR_NULL_POINTER;
    }
    if (index > this->size) {
        // 允许在末尾插入 (index == size)
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS; // 索引越界
    }
    if (static_array_is_full(this)) {
        return DSA_ERROR_CAPACITY_FULL; // 数组已满
    }

    // 向右移动元素，为新元素腾出空间
    if (index < this->size) {
        memmove(ELEMENT_ADDR(this, index + 1),
                ELEMENT_ADDR(this, index),
                (this->size - index) * this->element_size);
    }

    // 插入新元素
    memcpy(ELEMENT_ADDR(this, index), value, this->element_size);
    this->size++;
    return DSA_SUCCESS;
}

/**
 * @brief 删除指定位置的元素
 * @param array 数组容器指针
 * @param index 要删除的元素索引
 * @return 指向被删除元素的指针，如果索引越界或数组为空则返回NULL
 * @warning 返回的指针需要调用者使用free()释放内存
 */
static dsa_element_pt static_array_remove(dsa_container_pt array, size_t index) {
    if (is_index_out_of_bounds(array, index)) {
        return NULL; // 数组为空或索引越界
    }
    static_array_t *this = array;
    
    // 动态分配内存保存要删除的元素数据
    dsa_element_pt removed_element = malloc(this->element_size);
    if (!removed_element) {
        fprintf(stderr, "错误：无法为删除的元素分配内存\n");
        return NULL;
    }
    
    // 获取要删除元素的指针并复制数据
    dsa_element_pt element_to_delete = ELEMENT_ADDR(this, index);
    memcpy(removed_element, element_to_delete, this->element_size);

    // 向左移动元素以填补空缺
    if (index < this->size - 1) {
        memmove(ELEMENT_ADDR(this, index),
                ELEMENT_ADDR(this, index + 1),
                (this->size - index - 1) * this->element_size);
    }

    this->size--;

    // 返回动态分配的内存指针
    return removed_element;
}

/**
 * @brief 随机访问接口实现
 */
static trait_random_access_t const random_access_trait = {
    .get_at = static_array_get,
    .set_at = static_array_set,
    .insert_at = static_array_insert,
    .remove_at = static_array_remove,
};

/**
 * @brief 在数组末尾添加元素
 * @param array 数组容器指针
 * @param element 要添加的元素
 * @return DSA_SUCCESS 成功，其他值表示错误
 */
static dsa_result_t static_array_push_back(dsa_container_pt array, dsa_element_pt element) {
    // 通过调用insert函数在末尾插入元素
    return static_array_insert(array, static_array_size(array), element);
}

/**
 * @brief 删除并返回数组末尾的元素
 * @param array 数组容器指针
 * @return 指向被删除元素的指针，如果数组为空则返回NULL
 * @warning 返回的指针可能在后续操作中失效，应立即使用或复制数据
 */
static dsa_element_pt static_array_pop_back(dsa_container_pt array) {
    size_t const size = static_array_size(array);
    // 删除并返回最后一个元素
    return 0 == size ? NULL : static_array_remove(array, size - 1);
}

/**
 * @brief 尾部操作接口实现
 */
static trait_linear_t const linear_trait = {
    .push_back = static_array_push_back,
    .pop_back = static_array_pop_back,
};

/**
 * @brief 获取数组类型
 * @return 数组类型枚举值
 */
static dsa_array_list_type_t static_array_get_type() {
    return ARRAY_LIST_TYPE_STATIC;
}

/**
 * @brief 获取数组类型名称
 * @return 数组类型名称字符串
 */
static char const *static_array_get_type_name() {
    return "静态数组(Static Array)";
}

/**
 * @brief 数组接口实现
 */
static trait_array_list_t const array_trait = {
    .basic = &basic_trait,
    .random_access = &random_access_trait,
    .linear = &linear_trait,
    .get_type = static_array_get_type,
    .get_type_name = static_array_get_type_name,
};

/**
 * @brief 创建静态数组
 * @param data_buffer 预分配的数据缓冲区
 * @param capacity 数组容量
 * @param element_size 单个元素大小（字节）
 * @return 指向新创建的静态数组的指针，失败时返回NULL
 * @details 创建基于外部缓冲区的静态数组实例。缓冲区生命周期由调用者管理。
 * @note 数组结构体本身需要通过malloc分配，但数据缓冲区由外部提供
 */
dsa_array_list_t *static_array_create(void *data_buffer, size_t capacity, size_t element_size) {
    if (!data_buffer || capacity == 0 || element_size == 0) {
        return NULL;
    }
    static_array_t *array = malloc(sizeof(static_array_t));
    if (!array) {
        return NULL;
    }
    array->trait = &array_trait;
    array->data = data_buffer;
    array->size = 0;
    array->capacity = capacity;
    array->element_size = element_size;
    return (dsa_array_list_t *) array;
}