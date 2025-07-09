/**
 * @file circular_array_deque.c
 * @brief 基于循环数组的双端队列实现
 * @author DSA Team
 * @date 2025-07-09
 * @version 1.0
 */

#include <internal/circular_array_deque.h>
#include <internal/deque_traits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/// 默认初始容量
#define DEFAULT_CAPACITY 8

/**
 * @brief 循环数组双端队列结构体
 * @details 使用循环数组实现的双端队列，支持高效的双端插入和删除操作
 */
typedef struct {
    trait_deque_t const *trait; ///< deque接口指针
    dsa_element_pt *data; ///< 元素指针数组
    size_t capacity; ///< 数组容量
    size_t size; ///< 当前元素数量
    size_t front; ///< 队首索引
    size_t rear; ///< 队尾索引（指向最后一个元素的下一个位置）
} circular_array_deque_t;

/**
 * @brief 扩容循环数组双端队列
 * @param deque 双端队列指针
 * @param new_capacity
 * @return 成功返回true，失败返回false
 */
static dsa_result_t circular_deque_resize(circular_array_deque_t *deque, size_t new_capacity) {
    assert(deque != NULL);
    if (new_capacity < DEFAULT_CAPACITY) { new_capacity = DEFAULT_CAPACITY; }
    if (new_capacity < deque->size) { return DSA_ERROR_INVALID_PARAMETER; }

    dsa_element_pt *new_data = malloc(new_capacity * sizeof(dsa_element_pt));
    if (!new_data) { return DSA_ERROR_MEMORY_ALLOCATION; }

    // 将元素按顺序复制到新数组
    if (deque->front < deque->rear) {
        // 情况1：元素连续存储 [_,_,a,b,c,d,_,_]
        memcpy(new_data, deque->data + deque->front, deque->size * sizeof(dsa_element_pt));
    } else {
        // 情况2：元素跨越数组边界 [c,d,_,_,_,_,a,b]
        size_t first_part = deque->capacity - deque->front;
        size_t second_part = deque->rear;
        memcpy(new_data, deque->data + deque->front, first_part * sizeof(dsa_element_pt));
        memcpy(new_data + first_part, deque->data, second_part * sizeof(dsa_element_pt));
    }

    // 释放旧数组，更新指针和索引
    free(deque->data);
    deque->data = new_data;
    deque->capacity = new_capacity;
    deque->front = 0;
    deque->rear = deque->size;

    return DSA_SUCCESS;
}

/**
 * @brief 获取双端队列中的元素数量
 */
static size_t circular_deque_size(dsa_const_container_pt deque) {
    circular_array_deque_t const *this = deque;
    return this ? this->size : 0;
}

/**
 * @brief 检查双端队列是否为空
 */
static bool circular_deque_is_empty(dsa_const_container_pt deque) {
    circular_array_deque_t const *this = deque;
    return this ? (this->size == 0) : true;
}

/**
 * @brief 检查双端队列是否已满
 */
static bool circular_deque_is_full(dsa_const_container_pt deque) {
    circular_array_deque_t const *this = deque;
    return this ? (this->size == this->capacity) : true;
}

/**
 * @brief 获取双端队列容量
 */
static size_t circular_deque_capacity(dsa_const_container_pt deque) {
    circular_array_deque_t const *this = deque;
    return this ? this->capacity : 0;
}

/**
 * @brief 清空双端队列
 */
static dsa_result_t circular_deque_clear(dsa_container_pt deque) {
    circular_array_deque_t *this = deque;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    this->size = 0;
    this->front = 0;
    this->rear = 0;
    return DSA_SUCCESS;
}

/**
 * @brief 清空双端队列并释放所有元素
 */
static dsa_result_t circular_deque_clear_with_free(dsa_container_pt deque) {
    circular_array_deque_t *this = deque;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 释放所有元素
    for (size_t i = 0; i < this->size; i++) {
        size_t index = (this->front + i) % this->capacity;
        free(this->data[index]);
        this->data[index] = NULL;
    }

    this->size = 0;
    this->front = 0;
    this->rear = 0;
    return DSA_SUCCESS;
}

/**
 * @brief 销毁双端队列
 */
static dsa_result_t circular_deque_destroy(dsa_container_pt deque) {
    circular_array_deque_t *this = deque;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    free(this->data);
    free(this);
    return DSA_SUCCESS;
}

/**
 * @brief 销毁双端队列并释放所有元素
 */
static dsa_result_t circular_deque_destroy_with_free(dsa_container_pt deque) {
    circular_array_deque_t *this = deque;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 先释放所有元素
    circular_deque_clear_with_free(deque);

    // 再释放数组和结构体
    free(this->data);
    free(this);
    return DSA_SUCCESS;
}

/// 基本容器接口实现
static trait_basic_t const basic_trait = {
    .get_size = circular_deque_size,
    .get_capacity = circular_deque_capacity,
    .is_empty = circular_deque_is_empty,
    .is_full = circular_deque_is_full,
    .clear = circular_deque_clear,
    .clear_with_free = circular_deque_clear_with_free,
    .destroy = circular_deque_destroy,
    .destroy_with_free = circular_deque_destroy_with_free,
};

/**
 * @brief 在双端队列前端添加元素
 */
static dsa_result_t circular_deque_push_front(dsa_container_pt deque, dsa_element_pt element) {
    circular_array_deque_t *this = deque;
    if (!this || !element) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 检查是否需要扩容
    if (this->size == this->capacity) {
        dsa_result_t result = circular_deque_resize(this, this->capacity * 2);
        if (DSA_SUCCESS != result) {
            return result;
        }
    }

    // 计算新的front位置
    this->front = (this->front - 1 + this->capacity) % this->capacity;
    this->data[this->front] = element;
    this->size++;

    return DSA_SUCCESS;
}

/**
 * @brief 从双端队列前端移除元素
 */
static dsa_element_pt circular_deque_pop_front(dsa_container_pt deque) {
    circular_array_deque_t *this = deque;
    if (!this || this->size == 0) {
        return NULL;
    }

    dsa_element_pt element = this->data[this->front];
    this->data[this->front] = NULL; // 清理指针
    this->front = (this->front + 1) % this->capacity;
    this->size--;

    // 检查是否需要缩容
    if (this->capacity >= 2 * DEFAULT_CAPACITY && 4 * this->size < this->capacity) {
        circular_deque_resize(this, this->capacity / 2);
    }

    return element;
}

/**
 * @brief 查看双端队列前端元素但不移除
 */
static dsa_element_pt circular_deque_peek_front(dsa_const_container_pt deque) {
    circular_array_deque_t const *this = deque;
    if (!this || this->size == 0) {
        return NULL;
    }

    return this->data[this->front];
}

/**
 * @brief 在双端队列后端添加元素
 */
static dsa_result_t circular_deque_push_back(dsa_container_pt deque, dsa_element_pt element) {
    circular_array_deque_t *this = deque;
    if (!this || !element) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 检查是否需要扩容
    if (this->size == this->capacity) {
        dsa_result_t result = circular_deque_resize(this, this->capacity * 2);
        if (DSA_SUCCESS != result) {
            return result;
        }
    }

    this->data[this->rear] = element;
    this->rear = (this->rear + 1) % this->capacity;
    this->size++;

    return DSA_SUCCESS;
}

/**
 * @brief 从双端队列后端移除元素
 */
static dsa_element_pt circular_deque_pop_back(dsa_container_pt deque) {
    circular_array_deque_t *this = deque;
    if (!this || this->size == 0) {
        return NULL;
    }

    this->rear = (this->rear - 1 + this->capacity) % this->capacity;
    dsa_element_pt element = this->data[this->rear];
    this->data[this->rear] = NULL; // 清理指针
    this->size--;

    // 检查是否需要缩容
    if (this->capacity >= 2 * DEFAULT_CAPACITY && 4 * this->size < this->capacity) {
        circular_deque_resize(this, this->capacity / 2);
    }

    return element;
}

/**
 * @brief 查看双端队列后端元素但不移除
 */
static dsa_element_pt circular_deque_peek_back(dsa_const_container_pt deque) {
    circular_array_deque_t const *this = deque;
    if (!this || this->size == 0) {
        return NULL;
    }

    size_t last_index = (this->rear - 1 + this->capacity) % this->capacity;
    return this->data[last_index];
}

/// 线性容器接口实现
static trait_linear_t const linear_trait = {
    .push_front = circular_deque_push_front,
    .pop_front = circular_deque_pop_front,
    .peek_front = circular_deque_peek_front,
    .push_back = circular_deque_push_back,
    .pop_back = circular_deque_pop_back,
    .peek_back = circular_deque_peek_back,
};

/**
 * @brief 获取双端队列类型
 */
static dsa_deque_type_t circular_deque_get_type(void) {
    return DEQUE_TYPE_CIRCULAR_ARRAY_LIST;
}

/**
 * @brief 获取双端队列类型名称
 */
static char const *circular_deque_get_type_name(void) {
    return "循环数组双端队列 (Circular Array Deque)";
}

/// 双端队列接口实现
static trait_deque_t const deque_trait = {
    .basic = &basic_trait,
    .linear = &linear_trait,
    .get_type = circular_deque_get_type,
    .get_type_name = circular_deque_get_type_name,
};

/**
 * @brief 创建循环数组双端队列
 * @return 成功返回双端队列指针，失败返回NULL
 */
dsa_deque_t *circular_array_deque_create(void) {
    circular_array_deque_t *deque = malloc(sizeof(circular_array_deque_t));
    if (!deque) {
        fprintf(stderr, "错误：无法为循环数组双端队列分配内存\n");
        return NULL;
    }

    deque->trait = &deque_trait;
    deque->capacity = DEFAULT_CAPACITY;
    deque->size = 0;
    deque->front = 0;
    deque->rear = 0;

    deque->data = malloc(deque->capacity * sizeof(dsa_element_pt));
    if (!deque->data) {
        fprintf(stderr, "错误：无法为循环数组双端队列数据分配内存\n");
        free(deque);
        return NULL;
    }

    return (dsa_deque_t *) deque;
}
