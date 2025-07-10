/**
 * @file doubly_linked_deque.c
 * @brief 基于双向链表的双端队列实现
 * @author DSA Team
 * @date 2025-07-10
 * @version 1.0
 */

#include <internal/doubly_linked_deque.h>
#include <internal/deque_traits.h>
#include <ds/linked_list.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/**
 * @brief 双向链表双端队列结构体
 * @details 使用双向链表实现的双端队列，支持高效的双端插入和删除操作
 */
typedef struct {
    trait_deque_t const *trait;    ///< deque接口指针
    dsa_linked_list_t *data;       ///< 底层双向链表数据结构
} doubly_linked_deque_t;

/**
 * @brief 获取双端队列中的元素数量
 * @param deque 双端队列的常量指针
 * @return 队列中的元素数量，如果deque为NULL则返回0
 */
static size_t doubly_linked_deque_size(dsa_const_container_pt deque) {
    doubly_linked_deque_t const *this = deque;
    return this ? linked_list_size(this->data) : 0;
}

/**
 * @brief 获取双端队列的容量
 * @param deque 双端队列的常量指针
 * @return 对于链表实现，返回SIZE_MAX表示理论上无限容量
 * @note 实际容量受系统内存限制
 */
static size_t doubly_linked_deque_capacity(dsa_const_container_pt deque) {
    doubly_linked_deque_t const *this = deque;
    return this ? SIZE_MAX : 0;  // 链表理论上无容量限制
}

/**
 * @brief 检查双端队列是否为空
 * @param deque 双端队列的常量指针
 * @return 如果队列为空返回true，否则返回false；如果deque为NULL也返回true
 */
static bool doubly_linked_deque_is_empty(dsa_const_container_pt deque) {
    doubly_linked_deque_t const *this = deque;
    return this ? linked_list_is_empty(this->data) : true;
}

/**
 * @brief 检查双端队列是否已满
 * @param deque 双端队列的常量指针
 * @return 对于链表实现，总是返回false（除非deque为NULL）
 * @note 链表实现理论上不会满，除非系统内存不足
 */
static bool doubly_linked_deque_is_full(dsa_const_container_pt deque) {
    doubly_linked_deque_t const *this = deque;
    return this ? false : true;  // 链表不会满，除非deque为NULL
}

/**
 * @brief 清空双端队列
 * @param deque 双端队列指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER deque为NULL
 */
static dsa_result_t doubly_linked_deque_clear(dsa_container_pt deque) {
    doubly_linked_deque_t *this = deque;
    return this ? linked_list_clear(this->data) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 清空双端队列并释放所有元素
 * @param deque 双端队列指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER deque为NULL
 * @note 由于linked_list模块没有clear_with_free函数，此函数等同于clear
 */
static dsa_result_t doubly_linked_deque_clear_with_free(dsa_container_pt deque) {
    doubly_linked_deque_t *this = deque;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 手动释放所有元素
    size_t size = linked_list_size(this->data);
    for (size_t i = 0; i < size; i++) {
        dsa_element_pt element = linked_list_remove_at(this->data, 0);
        if (element) {
            free(element);
        }
    }

    return DSA_SUCCESS;
}

/**
 * @brief 销毁双端队列
 * @param deque 双端队列指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER deque为NULL
 */
static dsa_result_t doubly_linked_deque_destroy(dsa_container_pt deque) {
    doubly_linked_deque_t *this = deque;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    dsa_result_t result = linked_list_destroy(this->data);
    free(this);
    return result;
}

/**
 * @brief 销毁双端队列并释放所有元素
 * @param deque 双端队列指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER deque为NULL
 * @note 先释放所有元素，再销毁链表和deque结构
 */
static dsa_result_t doubly_linked_deque_destroy_with_free(dsa_container_pt deque) {
    doubly_linked_deque_t *this = deque;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 先释放所有元素
    doubly_linked_deque_clear_with_free(deque);

    // 再销毁链表和deque结构
    dsa_result_t result = linked_list_destroy(this->data);
    free(this);
    return result;
}

/// 基本容器接口实现
static trait_basic_t const basic_trait = {
    .get_size = doubly_linked_deque_size,
    .get_capacity = doubly_linked_deque_capacity,
    .is_empty = doubly_linked_deque_is_empty,
    .is_full = doubly_linked_deque_is_full,
    .clear = doubly_linked_deque_clear,
    .clear_with_free = doubly_linked_deque_clear_with_free,
    .destroy = doubly_linked_deque_destroy,
    .destroy_with_free = doubly_linked_deque_destroy_with_free,
};

/**
 * @brief 在双端队列前端添加元素
 * @param deque 双端队列指针
 * @param element 要添加的元素指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER deque或element为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 */
static dsa_result_t doubly_linked_deque_push_front(dsa_container_pt deque, dsa_element_pt element) {
    doubly_linked_deque_t *this = deque;
    if (!this || !element) {
        return DSA_ERROR_NULL_POINTER;
    }
    return linked_list_push_front(this->data, element);
}

/**
 * @brief 在双端队列后端添加元素
 * @param deque 双端队列指针
 * @param element 要添加的元素指针
 * @return 操作结果码
 * @retval DSA_SUCCESS 操作成功
 * @retval DSA_ERROR_NULL_POINTER deque或element为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 */
static dsa_result_t doubly_linked_deque_push_back(dsa_container_pt deque, dsa_element_pt element) {
    doubly_linked_deque_t *this = deque;
    if (!this || !element) {
        return DSA_ERROR_NULL_POINTER;
    }
    return linked_list_push_back(this->data, element);
}

/**
 * @brief 从双端队列前端移除元素
 * @param deque 双端队列指针
 * @return 被移除的元素指针，如果队列为空或deque为NULL则返回NULL
 * @note 调用者负责释放返回的元素内存
 */
static dsa_element_pt doubly_linked_deque_pop_front(dsa_container_pt deque) {
    doubly_linked_deque_t *this = deque;
    return this ? linked_list_pop_front(this->data) : NULL;
}

/**
 * @brief 从双端队列后端移除元素
 * @param deque 双端队列指针
 * @return 被移除的元素指针，如果队列为空或deque为NULL则返回NULL
 * @note 调用者负责释放返回的元素内存
 */
static dsa_element_pt doubly_linked_deque_pop_back(dsa_container_pt deque) {
    doubly_linked_deque_t *this = deque;
    return this ? linked_list_pop_back(this->data) : NULL;
}

/**
 * @brief 查看双端队列前端元素但不移除
 * @param deque 双端队列的常量指针
 * @return 前端元素指针，如果队列为空或deque为NULL则返回NULL
 */
static dsa_element_pt doubly_linked_deque_peek_front(dsa_const_container_pt deque) {
    doubly_linked_deque_t const *this = deque;
    return this ? linked_list_get(this->data, 0) : NULL;
}

/**
 * @brief 查看双端队列后端元素但不移除
 * @param deque 双端队列的常量指针
 * @return 后端元素指针，如果队列为空或deque为NULL则返回NULL
 */
static dsa_element_pt doubly_linked_deque_peek_back(dsa_const_container_pt deque) {
    doubly_linked_deque_t const *this = deque;
    if (!this || linked_list_is_empty(this->data)) {
        return NULL;
    }
    size_t last_index = linked_list_size(this->data) - 1;
    return linked_list_get(this->data, last_index);
}

/// 线性容器接口实现
static trait_linear_t const linear_trait = {
    .push_front = doubly_linked_deque_push_front,
    .pop_front = doubly_linked_deque_pop_front,
    .peek_front = doubly_linked_deque_peek_front,
    .push_back = doubly_linked_deque_push_back,
    .pop_back = doubly_linked_deque_pop_back,
    .peek_back = doubly_linked_deque_peek_back,
};

/**
 * @brief 获取双端队列类型
 * @return 双端队列类型枚举值
 */
static dsa_deque_type_t doubly_linked_deque_get_type(void) {
    return DEQUE_TYPE_DOUBLY_LINKED_LIST;
}

/**
 * @brief 获取双端队列类型名称
 * @return 类型名称字符串
 */
static char const *doubly_linked_deque_get_type_name(void) {
    return "双向链表双端队列 (Doubly Linked List Deque)";
}

/// 双端队列接口实现
static trait_deque_t const deque_trait = {
    .basic = &basic_trait,
    .linear = &linear_trait,
    .get_type = doubly_linked_deque_get_type,
    .get_type_name = doubly_linked_deque_get_type_name,
};

/**
 * @brief 创建双向链表双端队列
 * @return 成功返回双端队列指针，失败返回NULL
 * @retval NULL 内存分配失败
 * @note 创建的双端队列需要调用deque_destroy()释放
 */
dsa_deque_t *doubly_linked_deque_create(void) {
    doubly_linked_deque_t *deque = malloc(sizeof(doubly_linked_deque_t));
    if (!deque) {
        fprintf(stderr, "错误：无法为双向链表双端队列分配内存\n");
        return NULL;
    }

    deque->data = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    if (!deque->data) {
        fprintf(stderr, "错误：无法为双向链表双端队列的底层链表分配内存\n");
        free(deque);
        return NULL;
    }

    deque->trait = &deque_trait;
    return (dsa_deque_t *)deque;
}
