/**
 * @file insertion_sort.c
 * @brief 插入类排序算法实现
 * @details 实现直接插入排序和二分插入排序算法
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <a/sorting.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 插入位置查找策略函数类型
 * @details 用于在已排序序列中查找插入位置的函数指针类型
 */
typedef dsa_iterator_t *(*insertion_position_func_t)(dsa_iterator_t *begin,
                                                     dsa_iterator_t *end,
                                                     dsa_element_pt value,
                                                     compare_func_t compare,
                                                     dsa_sort_stats_t *stats);

/**
 * @brief 线性查找插入位置
 * @details 在已排序序列中线性查找合适的插入位置
 */
static dsa_iterator_t *find_position_linear(dsa_iterator_t *begin, dsa_iterator_t *end,
                                           dsa_element_pt value, compare_func_t compare,
                                           dsa_sort_stats_t *stats) {
    if (!begin || !end || !value || !compare) {
        return NULL;
    }

    dsa_iterator_t *current = iterator_clone(begin);
    if (!current) {
        return NULL;
    }

    while (iterator_is_valid(current) && !iterator_equals(current, end)) {
        dsa_element_pt current_value = iterator_get_value(current);

        if (stats) {
            stats->comparisons++;
        }

        // 如果当前位置的值大于要插入的值，则这里就是插入位置
        if (compare(current_value, value) > 0) {
            return current;
        }

        iterator_next(current);
    }

    // 如果没有找到合适位置，插入到end位置
    return current;
}

/**
 * @brief 二分查找插入位置
 * @details 在已排序序列中使用二分查找确定插入位置
 */
static dsa_iterator_t *find_position_binary(dsa_iterator_t *begin, dsa_iterator_t *end,
                                           dsa_element_pt value, compare_func_t compare,
                                           dsa_sort_stats_t *stats) {
    if (!begin || !end || !value || !compare) {
        return NULL;
    }

    size_t left = 0;
    size_t right = iterator_distance(begin, end);

    while (left < right) {
        size_t mid = left + (right - left) / 2;

        // 移动到中间位置
        dsa_iterator_t *mid_iter = iterator_clone(begin);
        iterator_advance(mid_iter, mid);

        dsa_element_pt mid_value = iterator_get_value(mid_iter);

        if (stats) {
            stats->comparisons++;
        }

        if (compare(mid_value, value) <= 0) {
            left = mid + 1;
        } else {
            right = mid;
        }

        iterator_destroy(mid_iter);
    }

    // 返回插入位置
    dsa_iterator_t *insert_pos = iterator_clone(begin);
    iterator_advance(insert_pos, left);
    return insert_pos;
}

/**
 * @brief 插入策略映射表
 */
static const insertion_position_func_t insertion_strategies[] = {
    [INSERTION_STRATEGY_LINEAR] = find_position_linear,
    [INSERTION_STRATEGY_BINARY] = find_position_binary,
    [INSERTION_STRATEGY_SHELL] = NULL  // 预留给希尔排序
};

/**
 * @brief 统一的插入排序框架
 */
dsa_result_t dsa_insertion_sort(dsa_iterator_t *begin, dsa_iterator_t *end,
                               compare_func_t compare,
                               dsa_insertion_strategy_t strategy,
                               dsa_sort_stats_t *stats) {
    if (!begin || !end || !compare) {
        return DSA_ERROR_NULL_POINTER;
    }

    if (strategy >= INSERTION_STRATEGY_COUNT || !insertion_strategies[strategy]) {
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

    insertion_position_func_t find_position = insertion_strategies[strategy];
    dsa_iterator_t *current = iterator_clone(begin);
    if (!current) {
        return DSA_ERROR_MEMORY_ALLOCATION;
    }

    iterator_next(current); // 从第二个元素开始

    while (iterator_is_valid(current) && !iterator_equals(current, end)) {
        dsa_element_pt current_value = iterator_get_value(current);
        if (!current_value) {
            iterator_destroy(current);
            return DSA_ERROR_NULL_POINTER;
        }

        // 使用策略函数找到插入位置
        dsa_iterator_t *insert_pos = find_position(begin, current, current_value, compare, stats);
        if (!insert_pos) {
            iterator_destroy(current);
            return DSA_ERROR_MEMORY_ALLOCATION;
        }

        // 如果需要移动元素（插入位置不是当前位置）
        if (!iterator_equals(insert_pos, current)) {
            // 简化的插入实现：直接交换到正确位置
            // 这不是最优的插入排序实现，但能保证正确性

            // 从current位置开始，向前冒泡到正确位置
            dsa_iterator_t *bubble_pos = iterator_clone(current);

            while (!iterator_equals(bubble_pos, insert_pos)) {
                dsa_iterator_t *prev_pos = iterator_clone(bubble_pos);
                iterator_retreat(prev_pos, 1);

                // 交换相邻元素
                dsa_result_t swap_result = dsa_iterator_swap(bubble_pos, prev_pos);
                if (swap_result == DSA_SUCCESS && stats) {
                    stats->swaps++;
                }

                iterator_destroy(bubble_pos);
                bubble_pos = prev_pos;
            }

            iterator_destroy(bubble_pos);
        }

        iterator_destroy(insert_pos);
        iterator_next(current);
    }

    iterator_destroy(current);
    return DSA_SUCCESS;
}

/**
 * @brief 直接插入排序实现
 * @details 调用统一插入排序框架，使用线性查找策略
 */
dsa_result_t dsa_insertion_sort_direct(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats) {
    return dsa_insertion_sort(begin, end, compare, INSERTION_STRATEGY_LINEAR, stats);
}

/**
 * @brief 二分插入排序实现
 * @details 调用统一插入排序框架，使用二分查找策略
 */
dsa_result_t dsa_insertion_sort_binary(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats) {
    return dsa_insertion_sort(begin, end, compare, INSERTION_STRATEGY_BINARY, stats);
}
