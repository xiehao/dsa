/**
 * @file searching.c
 * @brief 查找算法实现
 * @details 实现基于迭代器的查找算法，包括线性查找和二分查找
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <a/searching.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief 查找策略函数类型
 * @details 用于统一查找框架的函数指针类型
 */
typedef dsa_iterator_t *(*search_func_t)(dsa_iterator_t *begin,
                                         dsa_iterator_t *end,
                                         dsa_element_pt value,
                                         compare_func_t compare,
                                         dsa_search_stats_t *stats);

/**
 * @brief 插入位置查找策略函数类型
 * @details 用于统一插入位置查找框架的函数指针类型
 */
typedef dsa_iterator_t *(*insert_position_func_t)(dsa_iterator_t *begin,
                                                  dsa_iterator_t *end,
                                                  dsa_element_pt value,
                                                  compare_func_t compare,
                                                  dsa_search_stats_t *stats);

/**
 * @brief 线性查找指定值
 */
dsa_iterator_t *dsa_linear_search(dsa_iterator_t *begin, dsa_iterator_t *end,
                                 dsa_element_pt value, compare_func_t compare,
                                 dsa_search_stats_t *stats) {
    if (!begin || !end || !value || !compare) {
        return NULL;
    }

    // 初始化统计信息
    if (stats) {
        dsa_search_stats_init(stats);
        stats->element_count = iterator_distance(begin, end);
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

        // 如果找到匹配的值，返回当前位置
        if (compare(current_value, value) == 0) {
            return current;
        }

        iterator_next(current);
    }

    // 未找到，返回end的克隆
    iterator_destroy(current);
    return iterator_clone(end);
}

/**
 * @brief 线性查找插入位置
 */
dsa_iterator_t *dsa_linear_search_insert_position(dsa_iterator_t *begin, dsa_iterator_t *end,
                                                 dsa_element_pt value, compare_func_t compare,
                                                 dsa_search_stats_t *stats) {
    if (!begin || !end || !value || !compare) {
        return NULL;
    }

    // 初始化统计信息
    if (stats) {
        dsa_search_stats_init(stats);
        stats->element_count = iterator_distance(begin, end);
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
 * @brief 线性查找最小元素
 */
dsa_iterator_t *dsa_linear_search_min(dsa_iterator_t *begin, dsa_iterator_t *end,
                                     compare_func_t compare, dsa_search_stats_t *stats) {
    if (!begin || !end || !compare) {
        return NULL;
    }

    if (!iterator_is_valid(begin) || iterator_equals(begin, end)) {
        return NULL;
    }

    // 初始化统计信息
    if (stats) {
        dsa_search_stats_init(stats);
        stats->element_count = iterator_distance(begin, end);
    }

    dsa_iterator_t *min_iter = iterator_clone(begin);
    dsa_iterator_t *current = iterator_clone(begin);
    if (!min_iter || !current) {
        iterator_destroy(min_iter);
        iterator_destroy(current);
        return NULL;
    }

    iterator_next(current); // 从第二个元素开始比较

    while (iterator_is_valid(current) && !iterator_equals(current, end)) {
        dsa_element_pt current_value = iterator_get_value(current);
        dsa_element_pt min_value = iterator_get_value(min_iter);

        if (stats) {
            stats->comparisons++;
        }

        // 如果当前元素小于最小元素，更新最小元素位置
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
 * @brief 二分查找指定值
 */
dsa_iterator_t *dsa_binary_search(dsa_iterator_t *begin, dsa_iterator_t *end,
                                 dsa_element_pt value, compare_func_t compare,
                                 dsa_search_stats_t *stats) {
    if (!begin || !end || !value || !compare) {
        return NULL;
    }

    // 初始化统计信息
    if (stats) {
        dsa_search_stats_init(stats);
        stats->element_count = iterator_distance(begin, end);
    }

    size_t left = 0;
    size_t right = iterator_distance(begin, end);

    while (left < right) {
        size_t mid = left + (right - left) / 2;

        // 移动到中间位置
        dsa_iterator_t *mid_iter = iterator_clone(begin);
        iterator_next_n(mid_iter, mid);

        dsa_element_pt mid_value = iterator_get_value(mid_iter);

        if (stats) {
            stats->comparisons++;
        }

        int cmp_result = compare(mid_value, value);
        if (cmp_result == 0) {
            // 找到了目标值
            return mid_iter;
        } else if (cmp_result < 0) {
            left = mid + 1;
        } else {
            right = mid;
        }

        iterator_destroy(mid_iter);
    }

    // 未找到，返回end的克隆
    return iterator_clone(end);
}

/**
 * @brief 二分查找插入位置
 */
dsa_iterator_t *dsa_binary_search_insert_position(dsa_iterator_t *begin, dsa_iterator_t *end,
                                                 dsa_element_pt value, compare_func_t compare,
                                                 dsa_search_stats_t *stats) {
    if (!begin || !end || !value || !compare) {
        return NULL;
    }

    // 初始化统计信息
    if (stats) {
        dsa_search_stats_init(stats);
        stats->element_count = iterator_distance(begin, end);
    }

    size_t left = 0;
    size_t right = iterator_distance(begin, end);

    while (left < right) {
        size_t mid = left + (right - left) / 2;

        // 移动到中间位置
        dsa_iterator_t *mid_iter = iterator_clone(begin);
        iterator_next_n(mid_iter, mid);

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
    iterator_next_n(insert_pos, left);
    return insert_pos;
}

/**
 * @brief 查找策略映射表
 */
static const search_func_t search_strategies[] = {
    [DSA_SEARCH_STRATEGY_LINEAR] = dsa_linear_search,
    [DSA_SEARCH_STRATEGY_BINARY] = dsa_binary_search,
};

/**
 * @brief 插入位置查找策略映射表
 */
static const insert_position_func_t insert_position_strategies[] = {
    [DSA_SEARCH_STRATEGY_LINEAR] = dsa_linear_search_insert_position,
    [DSA_SEARCH_STRATEGY_BINARY] = dsa_binary_search_insert_position,
};

/**
 * @brief 统一的查找框架
 */
dsa_iterator_t *dsa_search(dsa_iterator_t *begin, dsa_iterator_t *end,
                          dsa_element_pt value, compare_func_t compare,
                          dsa_search_strategy_t strategy, dsa_search_stats_t *stats) {
    if (!begin || !end || !value || !compare) {
        return NULL;
    }

    if (strategy >= DSA_SEARCH_STRATEGY_COUNT || !search_strategies[strategy]) {
        return NULL;
    }

    return search_strategies[strategy](begin, end, value, compare, stats);
}

/**
 * @brief 统一的插入位置查找框架
 */
dsa_iterator_t *dsa_search_insert_position(dsa_iterator_t *begin, dsa_iterator_t *end,
                                          dsa_element_pt value, compare_func_t compare,
                                          dsa_search_strategy_t strategy, dsa_search_stats_t *stats) {
    if (!begin || !end || !value || !compare) {
        return NULL;
    }

    if (strategy >= DSA_SEARCH_STRATEGY_COUNT || !insert_position_strategies[strategy]) {
        return NULL;
    }

    return insert_position_strategies[strategy](begin, end, value, compare, stats);
}

/**
 * @brief 初始化查找统计信息
 */
void dsa_search_stats_init(dsa_search_stats_t *stats) {
    if (stats) {
        stats->comparisons = 0;
        stats->element_count = 0;
    }
}

/**
 * @brief 获取查找算法的名称
 */
const char *dsa_search_strategy_name(dsa_search_strategy_t strategy) {
    static const char *strategy_names[] = {
        "线性查找",
        "二分查找"
    };

    if (strategy >= 0 && strategy < DSA_SEARCH_STRATEGY_COUNT) {
        return strategy_names[strategy];
    }

    return "未知策略";
}

/**
 * @brief 打印查找统计信息
 */
void dsa_search_stats_print(const dsa_search_stats_t *stats, dsa_search_strategy_t strategy) {
    if (!stats) {
        printf("统计信息为空\n");
        return;
    }

    printf("=== %s 统计信息 ===\n", dsa_search_strategy_name(strategy));
    printf("元素数量: %zu\n", stats->element_count);
    printf("比较次数: %zu\n", stats->comparisons);

    if (stats->element_count > 0) {
        printf("平均比较次数: %.2f\n", (double)stats->comparisons / stats->element_count);
    }

    printf("========================\n");
}
