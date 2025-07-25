/**
 * @file sorting.h
 * @brief 排序算法统一接口定义
 * @details 提供了基于迭代器的排序算法接口，支持各种数据结构的排序操作
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#ifndef DSA_SORTING_H
#define DSA_SORTING_H

#include <stddef.h>
#include <stdbool.h>
#include <common.h>
#include <ds/iterator.h>

/**
 * @defgroup SortingAlgorithms 排序算法
 * @brief 基于迭代器的排序算法实现
 * @{
 */

/**
 * @enum dsa_sort_algorithm_t
 * @brief 排序算法类型枚举
 * @details 定义了支持的各种排序算法类型
 */
typedef enum {
    DSA_SORT_INSERTION_DIRECT,      ///< 直接插入排序
    DSA_SORT_INSERTION_BINARY,      ///< 二分插入排序
    DSA_SORT_SELECTION_DIRECT,      ///< 直接选择排序
    DSA_SORT_SELECTION_BUBBLE,      ///< 冒泡排序
    DSA_SORT_SELECTION_HEAP,        ///< 堆排序（预留）
    DSA_SORT_ALGORITHM_COUNT        ///< 算法总数
} dsa_sort_algorithm_t;

/**
 * @enum dsa_selection_strategy_t
 * @brief 选择排序策略枚举
 * @details 定义了选择排序的不同策略
 */
typedef enum {
    SELECTION_STRATEGY_LINEAR,      ///< 线性查找最值（直接选择排序）
    SELECTION_STRATEGY_BUBBLE,      ///< 冒泡方式选择最值（冒泡排序）
    SELECTION_STRATEGY_HEAP,        ///< 堆方式选择最值（堆排序，预留）
    SELECTION_STRATEGY_COUNT        ///< 策略总数
} dsa_selection_strategy_t;

/**
 * @enum dsa_insertion_strategy_t
 * @brief 插入排序策略枚举
 * @details 定义了插入排序的不同策略
 */
typedef enum {
    INSERTION_STRATEGY_LINEAR,      ///< 线性查找插入位置（直接插入）
    INSERTION_STRATEGY_BINARY,      ///< 二分查找插入位置（二分插入）
    INSERTION_STRATEGY_SHELL,       ///< Shell增量序列（希尔排序，预留）
    INSERTION_STRATEGY_COUNT        ///< 策略总数
} dsa_insertion_strategy_t;

/**
 * @struct dsa_sort_stats_t
 * @brief 排序统计信息
 * @details 记录排序过程中的比较次数和交换次数
 */
typedef struct {
    size_t comparisons;     ///< 比较次数
    size_t swaps;          ///< 交换次数
    size_t element_count;  ///< 元素总数
} dsa_sort_stats_t;

/**
 * @defgroup InsertionSort 插入类排序算法
 * @brief 插入类排序算法实现
 * @{
 */

/**
 * @brief 直接插入排序
 * @details 通过构建有序序列，对于未排序数据，在已排序序列中从后向前扫描，找到相应位置并插入
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param compare 比较函数
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 操作结果
 * @note 时间复杂度：O(n²)，空间复杂度：O(1)
 * @note 稳定排序算法
 * @note 对于小规模数据或基本有序的数据效率较高
 * @note 这是便利函数，内部调用统一框架
 */
dsa_result_t dsa_insertion_sort_direct(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats);

/**
 * @brief 二分插入排序
 * @details 在直接插入排序的基础上，使用二分查找来确定插入位置，减少比较次数
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param compare 比较函数
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 操作结果
 * @note 时间复杂度：O(n²)，空间复杂度：O(1)
 * @note 稳定排序算法
 * @note 比较次数为O(n log n)，但移动次数仍为O(n²)
 * @note 这是便利函数，内部调用统一框架
 */
dsa_result_t dsa_insertion_sort_binary(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats);

/** @} */ // InsertionSort

/**
 * @defgroup UnifiedSortingFramework 统一排序框架
 * @brief 基于策略模式的统一排序框架
 * @{
 */

/**
 * @brief 统一的选择排序框架
 * @details 根据指定的策略执行选择排序，支持多种选择策略
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param compare 比较函数
 * @param strategy 选择策略
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 操作结果
 * @note 时间复杂度：O(n²)，空间复杂度：O(1)
 * @note 稳定性取决于具体策略
 */
dsa_result_t dsa_selection_sort(dsa_iterator_t *begin, dsa_iterator_t *end,
                               compare_func_t compare,
                               dsa_selection_strategy_t strategy,
                               dsa_sort_stats_t *stats);

/**
 * @brief 统一的插入排序框架
 * @details 根据指定的策略执行插入排序，支持多种插入策略
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param compare 比较函数
 * @param strategy 插入策略
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 操作结果
 * @note 时间复杂度：O(n²)，空间复杂度：O(1)
 * @note 稳定排序算法
 */
dsa_result_t dsa_insertion_sort(dsa_iterator_t *begin, dsa_iterator_t *end,
                               compare_func_t compare,
                               dsa_insertion_strategy_t strategy,
                               dsa_sort_stats_t *stats);

/** @} */ // UnifiedSortingFramework

/**
 * @defgroup SelectionSort 选择类排序算法
 * @brief 选择类排序算法实现
 * @{
 */

/**
 * @brief 直接选择排序
 * @details 每次从未排序序列中找到最小（大）元素，放到排序序列的起始位置
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param compare 比较函数
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 操作结果
 * @note 时间复杂度：O(n²)，空间复杂度：O(1)
 * @note 不稳定排序算法
 * @note 交换次数最少，为O(n)
 * @note 这是便利函数，内部调用统一框架
 */
dsa_result_t dsa_selection_sort_direct(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats);

/**
 * @brief 冒泡排序
 * @details 重复地走访过要排序的数列，一次比较两个元素，如果顺序错误就把它们交换过来
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param compare 比较函数
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 操作结果
 * @note 时间复杂度：O(n²)，空间复杂度：O(1)
 * @note 稳定排序算法
 * @note 对于基本有序的数据可以提前结束
 * @note 这是便利函数，内部调用统一框架
 */
dsa_result_t dsa_selection_sort_bubble(dsa_iterator_t *begin, dsa_iterator_t *end,
                                      compare_func_t compare, dsa_sort_stats_t *stats);

/** @} */ // SelectionSort

/**
 * @defgroup SortingUtilities 排序工具函数
 * @brief 排序相关的工具函数
 * @{
 */

/**
 * @brief 检查序列是否已排序
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param compare 比较函数
 * @return 如果序列已排序返回true，否则返回false
 * @note 时间复杂度：O(n)
 */
bool dsa_is_sorted(dsa_iterator_t *begin, dsa_iterator_t *end, compare_func_t compare);

/**
 * @brief 交换两个迭代器指向的元素
 * @param iter1 第一个迭代器
 * @param iter2 第二个迭代器
 * @return 操作结果
 * @note 内部使用临时缓冲区进行交换
 */
dsa_result_t dsa_iterator_swap(dsa_iterator_t *iter1, dsa_iterator_t *iter2);

/**
 * @brief 获取排序算法的名称
 * @param algorithm 排序算法类型
 * @return 算法名称字符串
 */
const char *dsa_sort_algorithm_name(dsa_sort_algorithm_t algorithm);

/**
 * @brief 初始化排序统计信息
 * @param stats 统计信息结构体指针
 */
void dsa_sort_stats_init(dsa_sort_stats_t *stats);

/**
 * @brief 打印排序统计信息
 * @param stats 统计信息结构体指针
 * @param algorithm 排序算法类型
 */
void dsa_sort_stats_print(const dsa_sort_stats_t *stats, dsa_sort_algorithm_t algorithm);

/** @} */ // SortingUtilities

/** @} */ // SortingAlgorithms

#endif // DSA_SORTING_H
