/**
 * @file iterator_operations.c
 * @brief 通用迭代器操作实现
 * @details 提供了适用于所有迭代器类型的通用操作函数
 * @author DSA项目组
 * @date 2025-07-24
 * @version 1.0
 */

#include <ds/iterator.h>
#include <traits.h>
#include <iterator_internal.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



/**
 * @brief 将迭代器移动到下一个元素
 */
dsa_iterator_t *iterator_next(dsa_iterator_t *iter) {
    if (!iter || !iter->trait || !iter->trait->next) {
        return NULL;
    }
    
    return (dsa_iterator_t *)iter->trait->next(iter);
}

/**
 * @brief 将迭代器移动到前一个元素
 */
dsa_iterator_t *iterator_prev(dsa_iterator_t *iter) {
    if (!iter || !iter->trait || !iter->trait->prev) {
        return NULL;
    }
    
    return (dsa_iterator_t *)iter->trait->prev(iter);
}

/**
 * @brief 获取迭代器当前指向的元素
 */
dsa_element_pt iterator_get_value(dsa_iterator_t *iter) {
    if (!iter || !iter->trait || !iter->trait->get_value) {
        return NULL;
    }
    
    return iter->trait->get_value(iter);
}

/**
 * @brief 检查迭代器是否有效
 */
bool iterator_is_valid(dsa_iterator_t *iter) {
    if (!iter || !iter->trait || !iter->trait->is_valid) {
        return false;
    }
    
    return iter->trait->is_valid(iter);
}

/**
 * @brief 比较两个迭代器是否相等
 */
bool iterator_equals(dsa_iterator_t *iter1, dsa_iterator_t *iter2) {
    if (!iter1 || !iter2) {
        return iter1 == iter2;  // 两个都是NULL时相等
    }

    // 如果trait不同，迭代器肯定不相等
    if (iter1->trait != iter2->trait) {
        return false;
    }

    // 对于数组迭代器，我们需要比较内部状态
    // 这里我们假设所有迭代器都有类似的结构（container + index）
    // 这是一个简化的实现，实际项目中应该使用更好的多态机制

    // 将迭代器转换为通用结构来比较
    typedef struct {
        trait_iterator_t const *trait;
        void *container;
        size_t index;
        // 其他字段会被忽略
    } generic_iterator_t;

    generic_iterator_t *gen_iter1 = (generic_iterator_t *)iter1;
    generic_iterator_t *gen_iter2 = (generic_iterator_t *)iter2;

    return (gen_iter1->container == gen_iter2->container) &&
           (gen_iter1->index == gen_iter2->index);
}

/**
 * @brief 销毁迭代器并释放相关内存
 */
void iterator_destroy(dsa_iterator_t *iter) {
    if (iter) {
        free(iter);
    }
}
