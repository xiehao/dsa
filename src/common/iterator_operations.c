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
 * @brief 设置迭代器当前指向的元素值
 */
dsa_result_t iterator_set_value(dsa_iterator_t *iter, dsa_element_pt value) {
    if (!iter || !iter->trait || !iter->trait->set_value) {
        return DSA_ERROR_INVALID_PARAMETER;
    }

    return iter->trait->set_value(iter, value);
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

/**
 * @brief 克隆迭代器
 */
dsa_iterator_t *iterator_clone(const dsa_iterator_t *iter) {
    if (!iter) {
        return NULL;
    }

    // 为了简化实现，我们假设所有迭代器都有相似的结构
    // 实际项目中应该通过trait提供clone方法

    // 获取迭代器的大小（这里使用一个合理的估计值）
    size_t iter_size = 256; // 足够大以容纳任何迭代器类型

    dsa_iterator_t *cloned = malloc(iter_size);
    if (!cloned) {
        return NULL;
    }

    // 复制整个迭代器结构
    memcpy(cloned, iter, iter_size);

    return cloned;
}

/**
 * @brief 计算两个迭代器之间的距离
 */
size_t iterator_distance(dsa_iterator_t *begin, dsa_iterator_t *end) {
    if (!begin || !end) {
        return 0;
    }

    size_t distance = 0;
    dsa_iterator_t *current = iterator_clone(begin);
    if (!current) {
        return 0;
    }

    while (iterator_is_valid(current) && !iterator_equals(current, end)) {
        distance++;
        iterator_next(current);

        // 防止无限循环（如果end在begin之前）
        if (distance > 100000) { // 合理的上限
            break;
        }
    }

    iterator_destroy(current);
    return distance;
}

/**
 * @brief 将迭代器向前移动指定步数
 */
dsa_iterator_t *iterator_advance(dsa_iterator_t *iter, size_t n) {
    if (!iter) {
        return NULL;
    }

    for (size_t i = 0; i < n && iterator_is_valid(iter); i++) {
        iterator_next(iter);
    }

    return iter;
}

/**
 * @brief 将迭代器向后移动指定步数
 */
dsa_iterator_t *iterator_retreat(dsa_iterator_t *iter, size_t n) {
    if (!iter) {
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        dsa_iterator_t *prev_result = iterator_prev(iter);
        if (!prev_result || !iterator_is_valid(iter)) {
            break;
        }
    }

    return iter;
}

/**
 * @brief 在指定范围内查找第一个满足条件的元素
 */
dsa_iterator_t *iterator_find_if(dsa_iterator_t *begin, dsa_iterator_t *end,
                                iterator_predicate_t predicate, void *context) {
    if (!begin || !end || !predicate) {
        return NULL;
    }

    dsa_iterator_t *current = iterator_clone(begin);
    if (!current) {
        return NULL;
    }

    while (iterator_is_valid(current) && !iterator_equals(current, end)) {
        dsa_element_pt element = iterator_get_value(current);
        if (element && predicate(element, context)) {
            return current; // 找到了，返回当前位置
        }
        iterator_next(current);
    }

    // 未找到，返回end的克隆
    iterator_destroy(current);
    return iterator_clone(end);
}
