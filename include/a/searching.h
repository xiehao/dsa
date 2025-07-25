/**
 * @file searching.h
 * @brief 查找算法的统一接口定义
 * @details 提供了基于迭代器的查找算法实现，支持线性查找、二分查找等多种查找策略
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#ifndef DSA_SEARCHING_H
#define DSA_SEARCHING_H

#include <stddef.h>
#include <stdbool.h>
#include <common.h>
#include <a/iterator.h>

/**
 * @defgroup SearchingAlgorithms 查找算法
 * @brief 基于迭代器的查找算法实现
 * @{
 */

/**
 * @brief 查找策略枚举
 * @details 定义了支持的查找算法类型
 */
typedef enum {
    DSA_SEARCH_STRATEGY_LINEAR = 0,    ///< 线性查找
    DSA_SEARCH_STRATEGY_BINARY,        ///< 二分查找
    DSA_SEARCH_STRATEGY_COUNT          ///< 策略总数（用于边界检查）
} dsa_search_strategy_t;

/**
 * @brief 查找统计信息
 * @details 记录查找过程中的比较次数等统计数据
 */
typedef struct {
    size_t comparisons;     ///< 比较次数
    size_t element_count;   ///< 搜索范围内的元素总数
} dsa_search_stats_t;

/**
 * @defgroup LinearSearch 线性查找
 * @brief 线性查找算法实现
 * @{
 */

/**
 * @brief 线性查找指定值
 * @details 在指定范围内线性查找第一个等于目标值的元素
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param value 要查找的值
 * @param compare 比较函数
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 指向找到元素的迭代器，未找到时返回end的克隆
 * @note 时间复杂度：O(n)，空间复杂度：O(1)
 * @note 返回的迭代器需要调用iterator_destroy()释放
 */
dsa_iterator_t *dsa_linear_search(dsa_iterator_t *begin, dsa_iterator_t *end,
                                 dsa_element_pt value, compare_func_t compare,
                                 dsa_search_stats_t *stats);

/**
 * @brief 线性查找插入位置
 * @details 在已排序序列中线性查找合适的插入位置
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param value 要插入的值
 * @param compare 比较函数
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 指向插入位置的迭代器，失败时返回NULL
 * @note 时间复杂度：O(n)，空间复杂度：O(1)
 * @note 返回的迭代器需要调用iterator_destroy()释放
 * @note 要求输入序列已按compare函数定义的顺序排序
 */
dsa_iterator_t *dsa_linear_search_insert_position(dsa_iterator_t *begin, dsa_iterator_t *end,
                                                 dsa_element_pt value, compare_func_t compare,
                                                 dsa_search_stats_t *stats);

/**
 * @brief 线性查找最小元素
 * @details 在指定范围内线性查找最小元素的位置
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param compare 比较函数
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 指向最小元素的迭代器，失败时返回NULL
 * @note 时间复杂度：O(n)，空间复杂度：O(1)
 * @note 返回的迭代器需要调用iterator_destroy()释放
 */
dsa_iterator_t *dsa_linear_search_min(dsa_iterator_t *begin, dsa_iterator_t *end,
                                     compare_func_t compare, dsa_search_stats_t *stats);

/** @} */ // LinearSearch

/**
 * @defgroup BinarySearch 二分查找
 * @brief 二分查找算法实现
 * @{
 */

/**
 * @brief 二分查找指定值
 * @details 在已排序序列中使用二分查找算法查找指定值
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param value 要查找的值
 * @param compare 比较函数
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 指向找到元素的迭代器，未找到时返回end的克隆
 * @note 时间复杂度：O(log n)，空间复杂度：O(1)
 * @note 返回的迭代器需要调用iterator_destroy()释放
 * @note 要求输入序列已按compare函数定义的顺序排序
 * @note 仅适用于支持随机访问的容器（如数组列表）
 */
dsa_iterator_t *dsa_binary_search(dsa_iterator_t *begin, dsa_iterator_t *end,
                                 dsa_element_pt value, compare_func_t compare,
                                 dsa_search_stats_t *stats);

/**
 * @brief 二分查找插入位置
 * @details 在已排序序列中使用二分查找确定插入位置
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param value 要插入的值
 * @param compare 比较函数
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 指向插入位置的迭代器，失败时返回NULL
 * @note 时间复杂度：O(log n)，空间复杂度：O(1)
 * @note 返回的迭代器需要调用iterator_destroy()释放
 * @note 要求输入序列已按compare函数定义的顺序排序
 * @note 仅适用于支持随机访问的容器（如数组列表）
 */
dsa_iterator_t *dsa_binary_search_insert_position(dsa_iterator_t *begin, dsa_iterator_t *end,
                                                 dsa_element_pt value, compare_func_t compare,
                                                 dsa_search_stats_t *stats);

/** @} */ // BinarySearch

/**
 * @defgroup UnifiedSearchFramework 统一查找框架
 * @brief 支持多种查找策略的统一接口
 * @{
 */

/**
 * @brief 统一的查找框架
 * @details 根据指定的策略执行查找，支持多种查找算法
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param value 要查找的值
 * @param compare 比较函数
 * @param strategy 查找策略
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 指向找到元素的迭代器，未找到时返回end的克隆
 * @note 时间复杂度取决于具体策略
 * @note 返回的迭代器需要调用iterator_destroy()释放
 */
dsa_iterator_t *dsa_search(dsa_iterator_t *begin, dsa_iterator_t *end,
                          dsa_element_pt value, compare_func_t compare,
                          dsa_search_strategy_t strategy, dsa_search_stats_t *stats);

/**
 * @brief 统一的插入位置查找框架
 * @details 根据指定的策略查找插入位置，支持多种查找算法
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param value 要插入的值
 * @param compare 比较函数
 * @param strategy 查找策略
 * @param stats 统计信息（可选，传NULL则不统计）
 * @return 指向插入位置的迭代器，失败时返回NULL
 * @note 时间复杂度取决于具体策略
 * @note 返回的迭代器需要调用iterator_destroy()释放
 * @note 要求输入序列已按compare函数定义的顺序排序（对于二分查找）
 */
dsa_iterator_t *dsa_search_insert_position(dsa_iterator_t *begin, dsa_iterator_t *end,
                                          dsa_element_pt value, compare_func_t compare,
                                          dsa_search_strategy_t strategy, dsa_search_stats_t *stats);

/** @} */ // UnifiedSearchFramework

/**
 * @defgroup SearchingUtilities 查找工具函数
 * @brief 查找相关的工具函数
 * @{
 */

/**
 * @brief 初始化查找统计信息
 * @param stats 要初始化的统计信息结构体
 */
void dsa_search_stats_init(dsa_search_stats_t *stats);

/**
 * @brief 打印查找统计信息
 * @param stats 统计信息结构体
 * @param strategy 使用的查找策略
 */
void dsa_search_stats_print(const dsa_search_stats_t *stats, dsa_search_strategy_t strategy);

/**
 * @brief 获取查找算法的名称
 * @param strategy 查找策略
 * @return 算法名称字符串
 */
const char *dsa_search_strategy_name(dsa_search_strategy_t strategy);

/** @} */ // SearchingUtilities

/** @} */ // SearchingAlgorithms

#endif // DSA_SEARCHING_H
