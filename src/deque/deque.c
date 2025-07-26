/**
 * @file deque.c
 * @brief 双端队列抽象数据类型的统一接口实现
 * @details 本文件提供了双端队列ADT的统一接口，支持多种底层实现（循环数组和双向链表）。
 *          通过工厂模式和特性接口，实现了不同双端队列实现的统一访问。
 * @author DSA项目组
 * @date 2025-07-22
 * @version 1.0
 */

#include <adt/deque.h>
#include <traits.h>
#include <stdlib.h>
#include <internal/deque_traits.h>
#include <internal/circular_array_deque.h>
#include <internal/doubly_linked_deque.h>

/**
 * @struct deque_t
 * @brief 双端队列抽象数据类型结构体
 * @details 封装了双端队列的特性接口指针，实现多态行为
 */
struct deque_t {
    trait_deque_t const *traits;    ///< 双端队列特性接口指针
};

/**
 * @brief 创建指定类型的双端队列
 * @param type 双端队列类型枚举值
 * @return 成功返回新创建的双端队列指针，失败返回NULL
 * @note 时间复杂度：O(1)
 * @details 根据指定类型创建相应的双端队列实现：
 *          - DEQUE_TYPE_CIRCULAR_ARRAY_LIST: 创建循环数组双端队列
 *          - DEQUE_TYPE_DOUBLY_LINKED_LIST: 创建双向链表双端队列
 */
dsa_deque_t *deque_create(dsa_deque_type_t type) {
    switch (type) {
        case DEQUE_TYPE_CIRCULAR_ARRAY_LIST:
            return circular_array_deque_create();
        case DEQUE_TYPE_DOUBLY_LINKED_LIST:
            return doubly_linked_deque_create();
        default:
            return NULL;
    }
}

/**
 * @brief 获取双端队列中元素的数量
 * @param deque 双端队列的常量指针
 * @return 双端队列中元素的数量，如果双端队列为NULL则返回0
 * @note 时间复杂度：O(1)
 */
size_t deque_size(dsa_deque_t const *deque) {
    return deque ? deque->traits->basic->get_size(deque) : 0;
}

/**
 * @brief 检查双端队列是否为空
 * @param deque 双端队列的常量指针
 * @return 如果双端队列为空或为NULL则返回true，否则返回false
 * @note 时间复杂度：O(1)
 */
bool deque_is_empty(dsa_deque_t const *deque) {
    return deque ? deque->traits->basic->is_empty(deque) : true;
}

/**
 * @brief 清空双端队列中的所有元素
 * @param deque 双端队列指针
 * @return 操作结果状态码
 * @note 时间复杂度：O(n)，其中n是元素数量
 */
dsa_result_t deque_clear(dsa_deque_t *deque) {
    return deque ? deque->traits->basic->clear(deque) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 销毁双端队列并释放内存
 * @param deque 双端队列指针
 * @return 操作结果状态码
 * @note 时间复杂度：O(n)，其中n是元素数量
 */
dsa_result_t deque_destroy(dsa_deque_t *deque) {
    return deque ? deque->traits->basic->destroy(deque) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 在双端队列前端添加元素
 * @param deque 双端队列指针
 * @param element 要添加的元素指针
 * @return 操作结果状态码
 * @note 时间复杂度：O(1)
 */
dsa_result_t deque_add_first(dsa_deque_t *deque, dsa_element_pt element) {
    return deque ? deque->traits->linear->push_front(deque, element) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 在双端队列后端添加元素
 * @param deque 双端队列指针
 * @param element 要添加的元素指针
 * @return 操作结果状态码
 * @note 时间复杂度：O(1)
 */
dsa_result_t deque_add_last(dsa_deque_t *deque, dsa_element_pt element) {
    return deque ? deque->traits->linear->push_back(deque, element) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 从双端队列前端移除并返回元素
 * @param deque 双端队列指针
 * @return 被移除的元素指针，如果双端队列为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 */
dsa_element_pt deque_remove_first(dsa_deque_t *deque) {
    return deque ? deque->traits->linear->pop_front(deque) : NULL;
}

/**
 * @brief 从双端队列后端移除并返回元素
 * @param deque 双端队列指针
 * @return 被移除的元素指针，如果双端队列为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 */
dsa_element_pt deque_remove_last(dsa_deque_t *deque) {
    return deque ? deque->traits->linear->pop_back(deque) : NULL;
}

/**
 * @brief 查看双端队列前端元素但不移除
 * @param deque 双端队列的常量指针
 * @return 前端元素指针，如果双端队列为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 */
dsa_element_pt deque_peek_first(dsa_deque_t const *deque) {
    return deque ? deque->traits->linear->peek_front(deque) : NULL;
}

/**
 * @brief 查看双端队列后端元素但不移除
 * @param deque 双端队列的常量指针
 * @return 后端元素指针，如果双端队列为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 */
dsa_element_pt deque_peek_last(dsa_deque_t const *deque) {
    return deque ? deque->traits->linear->peek_back(deque) : NULL;
}

/**
 * @brief 获取双端队列的类型
 * @param deque 双端队列的常量指针
 * @return 双端队列类型枚举值
 * @note 时间复杂度：O(1)
 */
dsa_deque_type_t deque_get_type(dsa_deque_t const *deque) {
    return deque ? deque->traits->get_type() : DEQUE_TYPE_CIRCULAR_ARRAY_LIST;
}

/**
 * @brief 获取双端队列类型的名称
 * @param deque 双端队列的常量指针
 * @return 双端队列类型的字符串描述
 * @note 时间复杂度：O(1)
 */
char const *deque_get_type_name(dsa_deque_t const *deque) {
    return deque ? deque->traits->get_type_name() : "循环双端队列(Circular Array List)";
}