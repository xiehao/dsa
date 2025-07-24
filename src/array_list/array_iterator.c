/**
 * @file array_iterator.c
 * @brief 数组迭代器的实现
 * @details 提供了数组列表的迭代器功能，支持静态数组和动态数组
 * @author DSA项目组
 * @date 2025-07-24
 * @version 1.0
 */

#include <ds/iterator.h>
#include <ds/array_list.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <iterator_internal.h>
#include <internal/array_iterator_internal.h>
#include <internal/static_array_list.h>
#include <internal/dynamic_array_list.h>



/**
 * @brief 将迭代器移动到下一个元素
 * @param iterator 当前迭代器
 * @return 指向下一个元素的迭代器，如果没有下一个元素则返回等同于end()的迭代器
 */
static void *array_iterator_next(void *iterator) {
    if (!iterator) {
        return NULL;
    }
    
    array_iterator_t *iter = (array_iterator_t *)iterator;
    if (iter->current_index < iter->container_size) {
        iter->current_index++;
    }
    return iterator;
}

/**
 * @brief 将迭代器移动到前一个元素
 * @param iterator 当前迭代器
 * @return 指向前一个元素的迭代器，如果没有前一个元素则返回NULL
 */
static void *array_iterator_prev(void *iterator) {
    if (!iterator) {
        return NULL;
    }

    array_iterator_t *iter = (array_iterator_t *)iterator;
    if (iter->current_index > 0) {
        iter->current_index--;
    } else {
        // 如果已经在开始位置，设置为一个无效的索引值
        // 使用SIZE_MAX表示超出开始位置
        iter->current_index = SIZE_MAX;
    }
    return iterator;
}

/**
 * @brief 获取迭代器当前指向的元素
 * @param iterator 当前迭代器
 * @return 当前元素的指针，如果迭代器无效则返回NULL
 */
static dsa_element_pt array_iterator_get_value(void *iterator) {
    if (!iterator) {
        return NULL;
    }

    array_iterator_t *iter = (array_iterator_t *)iterator;

    // 检查迭代器是否指向有效位置
    if (iter->current_index >= iter->container_size) {
        return NULL;
    }

    // 直接访问内部结构以提高效率
    if (iter->iterator_type == ARRAY_ITERATOR_TYPE_STATIC) {
        const static_array_t *static_arr = (const static_array_t *)iter->container;
        return (char *)static_arr->data + (iter->current_index * static_arr->element_size);
    } else if (iter->iterator_type == ARRAY_ITERATOR_TYPE_DYNAMIC) {
        const dynamic_array_t *dynamic_arr = (const dynamic_array_t *)iter->container;
        return dynamic_arr->data[iter->current_index];
    }

    return NULL;
}

/**
 * @brief 检查迭代器是否有效
 * @param iterator 要检查的迭代器
 * @return 如果迭代器有效返回true，否则返回false
 */
static bool array_iterator_is_valid(void *iterator) {
    if (!iterator) {
        return false;
    }

    array_iterator_t *iter = (array_iterator_t *)iterator;
    // SIZE_MAX表示超出开始位置，也是无效的
    return iter->current_index < iter->container_size && iter->current_index != SIZE_MAX;
}

/**
 * @brief 数组迭代器接口实现
 */
static trait_iterator_t const array_iterator_trait = {
    .begin = NULL,  // 不再需要，因为begin/end函数直接实现
    .end = NULL,    // 不再需要，因为begin/end函数直接实现
    .next = array_iterator_next,
    .prev = array_iterator_prev,
    .get_value = array_iterator_get_value,
    .is_valid = array_iterator_is_valid,
};

/**
 * @brief 创建数组迭代器的通用函数
 * @param array 数组指针
 * @param index 初始索引位置
 * @param iterator_type 迭代器类型
 * @return 新创建的迭代器指针，失败时返回NULL
 */
static dsa_iterator_t *create_array_iterator(const dsa_array_list_t *array, size_t index, dsa_array_iterator_type_t iterator_type) {
    if (!array) {
        return NULL;
    }

    // 分配迭代器内存
    array_iterator_t *iter = malloc(sizeof(array_iterator_t));
    if (!iter) {
        return NULL;
    }

    // 初始化迭代器
    iter->trait = &array_iterator_trait;
    iter->container = array;
    iter->current_index = index;
    iter->container_size = array_list_size(array);
    iter->iterator_type = iterator_type;

    return (dsa_iterator_t *)iter;
}

/**
 * @brief 创建指向数组列表开始位置的迭代器
 */
dsa_iterator_t *array_list_begin(const dsa_array_list_t *array) {
    if (!array) {
        return NULL;
    }

    // 根据数组类型创建相应的迭代器
    dsa_array_list_type_t type = array_list_get_type(array);
    switch (type) {
        case ARRAY_LIST_TYPE_STATIC:
            return create_array_iterator(array, 0, ARRAY_ITERATOR_TYPE_STATIC);
        case ARRAY_LIST_TYPE_DYNAMIC:
            return create_array_iterator(array, 0, ARRAY_ITERATOR_TYPE_DYNAMIC);
        default:
            return NULL;
    }
}

/**
 * @brief 创建指向数组列表结束位置的迭代器
 */
dsa_iterator_t *array_list_end(const dsa_array_list_t *array) {
    if (!array) {
        return NULL;
    }

    size_t size = array_list_size(array);

    // 根据数组类型创建相应的迭代器
    dsa_array_list_type_t type = array_list_get_type(array);
    switch (type) {
        case ARRAY_LIST_TYPE_STATIC:
            return create_array_iterator(array, size, ARRAY_ITERATOR_TYPE_STATIC);
        case ARRAY_LIST_TYPE_DYNAMIC:
            return create_array_iterator(array, size, ARRAY_ITERATOR_TYPE_DYNAMIC);
        default:
            return NULL;
    }
}


