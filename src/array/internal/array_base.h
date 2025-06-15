#ifndef DSA_ARRAY_BASE_H
#define DSA_ARRAY_BASE_H

#include <stddef.h>
#include <stdbool.h>
#include <common.h>
#include <ds/array.h>

/**
 * @brief 数组基类抽象接口
 */
typedef struct array_base_t array_base_t;

/**
 * @brief 数组基类操作函数表
 */
typedef struct {
    // 销毁数组
    void (*destroy)(array_base_t* array);

    // 销毁数组并释放所有元素
    void (*destroy_with_free)(array_base_t* array);

    // 获取元素
    dsa_element_pt (*get)(const array_base_t* array, size_t index);

    // 设置元素
    dsa_result_t (*set)(array_base_t* array, size_t index, dsa_element_pt element);

    // 添加元素到末尾
    dsa_result_t (*push_back)(array_base_t* array, dsa_element_pt element);

    // 移除并返回末尾元素
    dsa_element_pt (*pop_back)(array_base_t* array);

    // 在指定位置插入元素
    dsa_result_t (*insert)(array_base_t* array, size_t index, dsa_element_pt element);

    // 移除指定位置的元素
    dsa_element_pt (*remove)(array_base_t* array, size_t index);

    // 获取数组大小
    size_t (*size)(const array_base_t* array);

    // 获取数组容量
    size_t (*capacity)(const array_base_t* array);

    // 检查数组是否为空
    bool (*is_empty)(const array_base_t* array);

    // 检查数组是否已满
    bool (*is_full)(const array_base_t* array);

    // 清空数组
    void (*clear)(array_base_t* array);

    // 清空数组并释放元素
    void (*clear_with_free)(array_base_t* array);

} array_vtable_t;

/**
 * @brief 数组基类结构
 */
struct array_base_t {
    const array_vtable_t* vtable;  // 虚函数表
    dsa_array_type_t type;         // 数组类型
};

// 基类宏辅助函数
#define ARRAY_BASE(ptr) ((array_base_t*)(ptr))

#endif // DSA_ARRAY_BASE_H
