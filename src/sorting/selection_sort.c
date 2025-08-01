/**
 * @file selection_sort.c
 * @brief 选择类排序算法实现
 * @details 实现统一的选择排序框架，支持多种选择策略：线性选择和冒泡选择
 * @author DSA项目组
 * @date 2025-07-27
 * @version 1.1
 */

#include <a/sorting.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 选择策略函数类型
 * @details 用于在指定范围内选择最值元素的函数指针类型
 */
typedef dsa_iterator_t *(*selection_func_t)(dsa_iterator_t *begin,
                                           dsa_iterator_t *end,
                                           compare_func_t compare,
                                           dsa_sort_stats_t *stats);

/**
 * @brief 线性查找最小元素
 * @details 在指定范围内线性查找最小元素的位置
 */
static dsa_iterator_t *find_min_linear(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats) {
    if (!begin || !end || !compare) {
        return NULL;
    }

    if (!iterator_is_valid(begin) || iterator_equals(begin, end)) {
        return NULL;
    }

    dsa_iterator_t *min_iter = iterator_clone(begin);
    dsa_iterator_t *current = iterator_clone(begin);
    if (!min_iter || !current) {
        iterator_destroy(min_iter);
        iterator_destroy(current);
        return NULL;
    }

    iterator_next(current);

    while (iterator_is_valid(current) && !iterator_equals(current, end)) {
        dsa_element_pt min_value = iterator_get_value(min_iter);
        dsa_element_pt current_value = iterator_get_value(current);

        if (stats) {
            stats->comparisons++;
        }

        if (compare(current_value, min_value) < 0) {
            iterator_destroy(min_iter);
            min_iter = iterator_clone(current);
        }

        iterator_next(current);
    }

    iterator_destroy(current);
    return min_iter;
}

/**
 * @brief 冒泡方式选择最小元素
 * @details 通过一轮冒泡将最小元素移动到begin位置
 */
static dsa_iterator_t *find_min_bubble(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats) {
    if (!begin || !end || !compare) {
        return NULL;
    }

    if (!iterator_is_valid(begin) || iterator_equals(begin, end)) {
        return NULL;
    }

    // 一轮冒泡：从后往前，将最小元素冒泡到begin位置
    dsa_iterator_t *current = iterator_clone(begin);
    if (!current) {
        return NULL;
    }

    while (iterator_is_valid(current)) {
        dsa_iterator_t *next = iterator_clone(current);
        iterator_next(next);

        if (!iterator_is_valid(next) || iterator_equals(next, end)) {
            iterator_destroy(next);
            break;
        }

        dsa_element_pt current_value = iterator_get_value(current);
        dsa_element_pt next_value = iterator_get_value(next);

        if (stats) {
            stats->comparisons++;
        }

        // 如果当前元素大于下一个元素，则交换
        if (compare(current_value, next_value) > 0) {
            dsa_result_t swap_result = dsa_iterator_swap(current, next);
            if (swap_result == DSA_SUCCESS && stats) {
                stats->swaps++;
            }
        }

        iterator_destroy(next);
        iterator_next(current);
    }

    iterator_destroy(current);
    return iterator_clone(begin); // 返回begin位置
}



/**
 * @brief 选择策略映射表
 */
static const selection_func_t selection_strategies[] = {
    [SELECTION_STRATEGY_LINEAR] = find_min_linear,
    [SELECTION_STRATEGY_BUBBLE] = find_min_bubble,
    [SELECTION_STRATEGY_HEAP] = NULL  // 预留给堆排序
};

/**
 * @brief 统一的选择排序框架
 */
dsa_result_t dsa_selection_sort(dsa_iterator_t *begin, dsa_iterator_t *end,
                               compare_func_t compare,
                               dsa_selection_strategy_t strategy,
                               dsa_sort_stats_t *stats) {
    if (!begin || !end || !compare) {
        return DSA_ERROR_NULL_POINTER;
    }

    if (strategy >= SELECTION_STRATEGY_COUNT || !selection_strategies[strategy]) {
        return DSA_ERROR_INVALID_PARAMETER;
    }

    // 初始化统计信息
    if (stats) {
        dsa_sort_stats_init(stats);
        stats->element_count = iterator_distance(begin, end);
    }

    // 空序列或单元素序列无需排序
    if (!iterator_is_valid(begin) || iterator_equals(begin, end)) {
        return DSA_SUCCESS;
    }

    // 统一的选择策略流程
    selection_func_t select_func = selection_strategies[strategy];
    dsa_iterator_t *current = iterator_clone(begin);
    if (!current) {
        return DSA_ERROR_MEMORY_ALLOCATION;
    }

    while (iterator_is_valid(current) && !iterator_equals(current, end)) {
        // 在未排序部分找到最小元素
        dsa_iterator_t *min_iter = select_func(current, end, compare, stats);
        if (!min_iter) {
            iterator_destroy(current);
            return DSA_ERROR_MEMORY_ALLOCATION;
        }

        // 如果最小元素不是当前位置，则交换
        if (!iterator_equals(current, min_iter)) {
            dsa_result_t swap_result = dsa_iterator_swap(current, min_iter);
            if (swap_result != DSA_SUCCESS) {
                iterator_destroy(current);
                iterator_destroy(min_iter);
                return swap_result;
            }

            if (stats) {
                stats->swaps++;
            }
        }

        iterator_destroy(min_iter);
        iterator_next(current);
    }

    iterator_destroy(current);
    return DSA_SUCCESS;
}

/**
 * @brief 直接选择排序实现
 * @details 调用统一选择排序框架，使用线性查找策略
 */
dsa_result_t dsa_selection_sort_direct(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats) {
    return dsa_selection_sort(begin, end, compare, SELECTION_STRATEGY_LINEAR, stats);
}

/**
 * @brief 冒泡排序实现
 * @details 调用统一选择排序框架，使用冒泡策略选择最值
 * @note 通过冒泡方式逐个选择最小元素，体现了选择排序的本质
 */
dsa_result_t dsa_selection_sort_bubble(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats) {
    return dsa_selection_sort(begin, end, compare, SELECTION_STRATEGY_BUBBLE, stats);
}
