/**
 * @file array_list.c
 * @brief 数组数据结构的实现
 * @details 提供静态数组和动态数组的统一接口实现，支持基本的数组操作
 * @author DSA项目组
 * @date 2025-06-15
 * @version 1.0
 */

#include <ds/array_list.h>
#include <stdlib.h>
#include <stdio.h>
#include <traits.h>
#include <internal/array_list_traits.h>

// 包含内部头文件
#include <internal/static_array_list.h>
#include <internal/dynamic_array_list.h>

/**
 * @brief 数组结构体定义
 * @details 包含各种接口指针，用于实现数组的多态行为
 */
struct array_list_t {
    trait_array_list_t const *trait;       /**< 数组专用接口 */
};

// ============================================================================
// 内部辅助函数
// ============================================================================

/**
 * @brief 检查数组指针是否有效
 * @param array 要检查的数组指针
 * @return 如果数组指针有效返回 true，否则返回 false
 * @note 这是一个内部辅助函数，用于统一的空指针检查
 */
static bool is_valid_array(const dsa_array_list_t *array) {
    return array != NULL;
}

/**
 * @brief 获取错误信息字符串
 * @param result 错误代码
 * @return 对应的错误信息字符串
 * @note 将错误代码转换为可读的中文错误信息
 */
static const char *get_error_string(dsa_result_t result) {
    switch (result) {
        case DSA_SUCCESS: return "成功";
        case DSA_ERROR_NULL_POINTER: return "空指针错误";
        case DSA_ERROR_INDEX_OUT_OF_BOUNDS: return "索引越界";
        case DSA_ERROR_CAPACITY_FULL: return "容量已满";
        case DSA_ERROR_EMPTY: return "数组为空";
        case DSA_ERROR_MEMORY_ALLOCATION: return "内存分配失败";
        case DSA_ERROR_INVALID_PARAMETER: return "无效参数";
        default: return "未知错误";
    }
}

// ============================================================================
// 创建函数
// ============================================================================

/**
 * @brief 创建静态数组
 * @param buffer 用于存储数组元素的缓冲区
 * @param capacity 数组容量（最大元素数量）
 * @param element_size 单个元素的大小（字节）
 * @return 成功时返回数组指针，失败时返回 NULL
 * @note 静态数组使用用户提供的缓冲区，不会动态分配内存来存储元素
 * @warning 用户必须确保缓冲区在数组生命周期内保持有效
 */
dsa_array_list_t *array_list_create_static(void *buffer, size_t capacity, size_t element_size) {
    if (!buffer || capacity == 0 || element_size == 0) {
        return NULL;
    }

    // 使用新的static_array_create函数创建静态数组
    return static_array_create(buffer, capacity, element_size);
}

/**
 * @brief 创建动态数组
 * @param initial_capacity 初始容量
 * @return 成功时返回数组指针，失败时返回 NULL
 * @note 动态数组会根据需要自动扩展容量
 */
dsa_array_list_t *array_list_create_dynamic(size_t initial_capacity) {
    return dynamic_array_create(initial_capacity);
}

// ============================================================================
// 基本函数
// ============================================================================

/**
 * @brief 获取数组当前大小
 * @param array_list 数组指针
 * @return 数组中元素的数量
 * @note 如果数组指针无效，返回 0
 */
size_t array_list_size(const dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return 0;
    }
    return array_list->trait->basic->get_size(array_list);
}

/**
 * @brief 获取数组容量
 * @param array_list 数组指针
 * @return 数组的最大容量
 * @note 如果数组指针无效，返回 0
 */
size_t array_list_capacity(const dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return 0;
    }
    return array_list->trait->basic->get_capacity(array_list);
}

/**
 * @brief 检查数组是否为空
 * @param array_list 数组指针
 * @return 如果数组为空返回 true，否则返回 false
 * @note 如果数组指针无效，返回 true
 */
bool array_list_is_empty(const dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return true;
    }
    return array_list->trait->basic->is_empty(array_list);
}

/**
 * @brief 检查数组是否已满
 * @param array_list 数组指针
 * @return 如果数组已满返回 true，否则返回 false
 * @note 如果数组指针无效，返回 false
 */
bool array_list_is_full(const dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return false;
    }
    return array_list->trait->basic->is_full(array_list);
}

/**
 * @brief 清空数组
 * @param array_list 数组指针
 * @return 操作结果代码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 空指针错误
 * @note 移除所有元素但不释放元素内存
 */
dsa_result_t array_list_clear(dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return DSA_ERROR_NULL_POINTER;
    }
    return array_list->trait->basic->clear(array_list);
}

/**
 * @brief 清空数组并释放所有元素内存
 * @param array_list 数组指针
 * @return 操作结果代码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 空指针错误
 * @note 移除所有元素并释放它们占用的内存
 */
dsa_result_t array_list_clear_with_free(dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return DSA_ERROR_NULL_POINTER;
    }
    return array_list->trait->basic->clear_with_free(array_list);
}

/**
 * @brief 销毁数组
 * @param array_list 要销毁的数组
 * @return 操作结果代码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 空指针错误
 * @note 释放数组结构体占用的内存，但不释放元素内存
 */
dsa_result_t array_list_destroy(dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return DSA_ERROR_NULL_POINTER;
    }
    return array_list->trait->basic->destroy(array_list);
}

/**
 * @brief 销毁数组并释放所有内存
 * @param array_list 要销毁的数组
 * @return 操作结果代码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 空指针错误
 * @note 释放数组结构体和所有元素占用的内存
 */
dsa_result_t array_list_destroy_with_free(dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return DSA_ERROR_NULL_POINTER;
    }
    return array_list->trait->basic->destroy_with_free(array_list);
}

// ============================================================================
// 基本操作函数
// ============================================================================

/**
 * @brief 获取指定索引处的元素
 * @param array_list 数组指针
 * @param index 元素索引
 * @return 成功时返回元素指针，失败时返回 NULL
 * @note 返回的是元素的指针，而不是元素的副本
 */
dsa_element_pt array_list_get(const dsa_array_list_t *array_list, size_t index) {
    if (!is_valid_array(array_list)) {
        return NULL;
    }
    return array_list->trait->random_access->get_at(array_list, index);
}

/**
 * @brief 设置指定索引处的元素
 * @param array_list 数组指针
 * @param index 元素索引
 * @param element 要设置的元素
 * @return 操作结果代码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 空指针错误
 * @retval DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引越界
 */
dsa_result_t array_list_set(dsa_array_list_t *array_list, size_t index, dsa_element_pt element) {
    if (!is_valid_array(array_list)) {
        return DSA_ERROR_NULL_POINTER;
    }
    return array_list->trait->random_access->set_at(array_list, index, element);
}

/**
 * @brief 在数组末尾添加元素
 * @param array_list 数组指针
 * @param element 要添加的元素
 * @return 操作结果代码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 空指针错误
 * @retval DSA_ERROR_CAPACITY_FULL 容量已满
 */
dsa_result_t array_list_push_back(dsa_array_list_t *array_list, dsa_element_pt element) {
    if (!is_valid_array(array_list)) {
        return DSA_ERROR_NULL_POINTER;
    }
    return array_list->trait->linear->push_back(array_list, element);
}

/**
 * @brief 移除并返回数组末尾的元素
 * @param array_list 数组指针
 * @return 成功时返回移除的元素指针，失败时返回 NULL
 * @note 调用者负责释放返回的元素内存（如果需要）
 */
dsa_element_pt array_list_pop_back(dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return NULL;
    }
    return array_list->trait->linear->pop_back(array_list);
}

/**
 * @brief 在指定位置插入元素
 * @param array_list 数组指针
 * @param index 插入位置的索引
 * @param element 要插入的元素
 * @return 操作结果代码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER 空指针错误
 * @retval DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引越界
 * @retval DSA_ERROR_CAPACITY_FULL 容量已满
 */
dsa_result_t array_list_insert(dsa_array_list_t *array_list, size_t index, dsa_element_pt element) {
    if (!is_valid_array(array_list)) {
        return DSA_ERROR_NULL_POINTER;
    }
    return array_list->trait->random_access->insert_at(array_list, index, element);
}

/**
 * @brief 移除指定位置的元素
 * @param array_list 数组指针
 * @param index 要移除元素的索引
 * @return 成功时返回移除的元素指针，失败时返回 NULL
 * @note 调用者负责释放返回的元素内存（如果需要）
 */
dsa_element_pt array_list_remove(dsa_array_list_t *array_list, size_t index) {
    if (!is_valid_array(array_list)) {
        return NULL;
    }
    return array_list->trait->random_access->remove_at(array_list, index);
}

/**
 * @brief 获取数组类型
 * @param array_list 数组指针
 * @return 数组类型枚举值
 * @note 如果数组指针无效，返回默认的静态类型
 */
dsa_array_list_type_t array_list_get_type(const dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return ARRAY_LIST_TYPE_STATIC; // 默认返回静态类型
    }
    return array_list->trait->get_type();
}

/**
 * @brief 获取数组类型名称
 * @param array_list 数组指针
 * @return 数组类型的字符串描述
 * @note 如果数组指针无效，返回"无效数组"
 */
const char *array_list_get_type_name(const dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        return "无效数组";
    }
    return array_list->trait->get_type_name();
}

// ============================================================================
// 其他操作函数
// ============================================================================

/**
 * @brief 打印数组信息
 * @param array_list 数组指针
 * @note 在标准输出中打印数组的详细信息，包括类型、大小、容量等
 */
void array_list_print_info(const dsa_array_list_t *array_list) {
    if (!is_valid_array(array_list)) {
        printf("数组: 无效 (空指针)\n");
        return;
    }

    printf("数组信息:\n");
    printf("  类型: %s\n", array_list_get_type_name(array_list));
    printf("  大小: %zu\n", array_list_size(array_list));
    printf("  容量: %zu\n", array_list_capacity(array_list));
    printf("  是否为空: %s\n", array_list_is_empty(array_list) ? "是" : "否");
    printf("  是否已满: %s\n", array_list_is_full(array_list) ? "是" : "否");

    // if (trait->type == ARRAY_LIST_TYPE_STATIC) {
    //     printf("  元素大小: %zu 字节\n", trait->impl.static_array.element_size);
    // }
}

// ============================================================================
// 类型安全的便利函数
// ============================================================================

/**
 * @note 以下被注释的函数提供了类型安全的便利接口
 * @note 这些函数专门用于处理特定类型（如 int、double）的元素
 * @note 它们处理了静态数组和动态数组之间内存管理的差异
 */

// 注释掉的函数包含了类型安全的便利函数，用于处理 int 和 double 类型的数据
// 这些函数在实际使用时需要根据具体的数据结构定义进行调整