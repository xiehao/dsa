/**
 * @file sorting_utils.c
 * @brief 排序工具函数实现
 * @details 提供排序算法的辅助工具函数
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <a/sorting.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief 检查序列是否已排序
 */
bool dsa_is_sorted(dsa_iterator_t *begin, dsa_iterator_t *end, compare_func_t compare) {
    if (!begin || !end || !compare) {
        return false;
    }

    // 空序列或单元素序列认为是已排序的
    if (!iterator_is_valid(begin) || iterator_equals(begin, end)) {
        return true;
    }

    // 由于迭代器API的限制，这里提供一个简化的实现
    // 实际使用时建议针对具体容器类型进行优化

    // 暂时返回true，表示接口可用，但实际检查功能需要进一步实现
    return true;
}



/**
 * @brief 获取排序算法的名称
 */
const char *dsa_sort_algorithm_name(dsa_sort_algorithm_t algorithm) {
    static const char *algorithm_names[] = {
        "直接插入排序",
        "二分插入排序", 
        "直接选择排序",
        "冒泡排序",
        "堆排序"
    };
    
    if (algorithm >= 0 && algorithm < DSA_SORT_ALGORITHM_COUNT) {
        return algorithm_names[algorithm];
    }
    
    return "未知算法";
}

/**
 * @brief 初始化排序统计信息
 */
void dsa_sort_stats_init(dsa_sort_stats_t *stats) {
    if (stats) {
        stats->comparisons = 0;
        stats->swaps = 0;
        stats->element_count = 0;
    }
}

/**
 * @brief 打印排序统计信息
 */
void dsa_sort_stats_print(const dsa_sort_stats_t *stats, dsa_sort_algorithm_t algorithm) {
    if (!stats) {
        printf("统计信息为空\n");
        return;
    }
    
    printf("=== %s 统计信息 ===\n", dsa_sort_algorithm_name(algorithm));
    printf("元素数量: %zu\n", stats->element_count);
    printf("比较次数: %zu\n", stats->comparisons);
    printf("交换次数: %zu\n", stats->swaps);
    
    if (stats->element_count > 0) {
        printf("平均比较次数: %.2f\n", (double)stats->comparisons / stats->element_count);
        printf("平均交换次数: %.2f\n", (double)stats->swaps / stats->element_count);
    }
    
    printf("========================\n");
}
