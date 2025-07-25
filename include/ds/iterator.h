/**
 * @file iterator.h
 * @brief 数据结构迭代器的统一接口定义
 * @details 提供了统一的迭代器操作接口，支持各种数据结构的遍历访问
 * @author DSA项目组
 * @date 2025-07-24
 * @version 1.0
 */

#ifndef DSA_ITERATOR_H
#define DSA_ITERATOR_H

#include <stddef.h>
#include <stdbool.h>
#include <common.h>

/**
 * @brief 迭代器的不透明指针类型
 * 
 * 使用不透明指针隐藏迭代器的内部实现细节，提供更好的封装性。
 * 用户只能通过提供的API函数来操作迭代器，无法直接访问内部结构。
 */
typedef struct dsa_iterator_t dsa_iterator_t;

/**
 * @defgroup IteratorOperations 通用迭代器操作
 * @brief 适用于所有迭代器类型的通用操作接口
 * @{
 */

/**
 * @brief 将迭代器移动到下一个元素
 *
 * @param iter 当前迭代器指针
 * @return 指向下一个元素的迭代器，如果已到达末尾则返回等同于end()的迭代器
 * 
 * @pre iter != NULL
 * @note 如果迭代器已经指向末尾，行为是未定义的
 */
dsa_iterator_t *iterator_next(dsa_iterator_t *iter);

/**
 * @brief 将迭代器移动到前一个元素
 *
 * @param iter 当前迭代器指针
 * @return 指向前一个元素的迭代器，如果已到达开头则返回NULL
 * 
 * @pre iter != NULL
 * @note 如果迭代器已经指向开头，行为是未定义的
 * @note 并非所有迭代器类型都支持反向遍历（如单向链表迭代器）
 */
dsa_iterator_t *iterator_prev(dsa_iterator_t *iter);

/**
 * @brief 获取迭代器当前指向的元素
 *
 * @param iter 迭代器指针
 * @return 当前元素的指针，如果迭代器无效或指向末尾则返回NULL
 *
 * @pre iter != NULL
 */
dsa_element_pt iterator_get_value(dsa_iterator_t *iter);

/**
 * @brief 设置迭代器当前指向的元素值
 *
 * @param iter 迭代器指针
 * @param value 要设置的新值指针
 * @return 操作结果，成功返回DSA_SUCCESS
 *
 * @pre iter != NULL
 * @pre value != NULL
 * @note 只有当迭代器指向有效元素时才能设置值
 * @note 对于只读容器，此操作可能失败
 */
dsa_result_t iterator_set_value(dsa_iterator_t *iter, dsa_element_pt value);

/**
 * @brief 检查迭代器是否有效
 *
 * @param iter 要检查的迭代器指针
 * @return 如果迭代器有效且指向有效元素返回true，否则返回false
 * 
 * @note 指向末尾位置的迭代器被认为是无效的（不指向实际元素）
 */
bool iterator_is_valid(dsa_iterator_t *iter);

/**
 * @brief 比较两个迭代器是否相等
 *
 * @param iter1 第一个迭代器指针
 * @param iter2 第二个迭代器指针
 * @return 如果两个迭代器指向同一位置返回true，否则返回false
 * 
 * @note 只有来自同一容器的迭代器才能进行有意义的比较
 */
bool iterator_equals(dsa_iterator_t *iter1, dsa_iterator_t *iter2);

/**
 * @brief 销毁迭代器并释放相关内存
 *
 * @param iter 要销毁的迭代器指针
 *
 * @note 销毁后的迭代器指针不能再使用
 * @warning 调用此函数后，iter指针将失效，不能再使用
 */
void iterator_destroy(dsa_iterator_t *iter);

/** @} */ // IteratorOperations

/**
 * @defgroup IteratorAdvancedOperations 高级迭代器操作
 * @brief 支持排序算法等复杂操作的高级迭代器接口
 * @{
 */

/**
 * @brief 克隆迭代器
 * @details 创建一个指向相同位置的新迭代器，用于需要多个独立迭代器的场景
 *
 * @param iter 要克隆的迭代器指针
 * @return 新的迭代器指针，失败时返回NULL
 *
 * @pre iter != NULL
 * @note 返回的迭代器需要调用iterator_destroy()释放
 * @note 克隆的迭代器与原迭代器相互独立，可以分别移动
 */
dsa_iterator_t *iterator_clone(const dsa_iterator_t *iter);

/**
 * @brief 计算两个迭代器之间的距离
 * @details 计算从begin到end需要多少次iterator_next()操作
 *
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @return 距离（元素个数），如果end在begin之前则返回0
 *
 * @pre begin != NULL && end != NULL
 * @pre begin和end来自同一个容器
 * @note 时间复杂度：O(n)，其中n是距离
 */
size_t iterator_distance(dsa_iterator_t *begin, dsa_iterator_t *end);

/**
 * @brief 将迭代器向前移动指定步数
 * @details 相当于连续调用n次iterator_next()
 *
 * @param iter 要移动的迭代器
 * @param n 移动的步数
 * @return 移动后的迭代器指针（与输入相同）
 *
 * @pre iter != NULL
 * @note 如果移动超出容器范围，迭代器将指向end位置
 * @note 时间复杂度：O(n)
 */
dsa_iterator_t *iterator_advance(dsa_iterator_t *iter, size_t n);

/**
 * @brief 将迭代器向后移动指定步数
 * @details 相当于连续调用n次iterator_prev()
 *
 * @param iter 要移动的迭代器
 * @param n 移动的步数
 * @return 移动后的迭代器指针（与输入相同）
 *
 * @pre iter != NULL
 * @note 如果移动超出容器范围，迭代器将指向begin位置
 * @note 时间复杂度：O(n)
 * @note 并非所有迭代器类型都支持反向移动
 */
dsa_iterator_t *iterator_retreat(dsa_iterator_t *iter, size_t n);

/**
 * @brief 在指定范围内查找第一个满足条件的元素
 * @details 从begin开始向end方向查找，直到找到第一个使predicate返回true的元素
 *
 * @param begin 起始迭代器
 * @param end 结束迭代器
 * @param predicate 判断函数，接受元素指针，返回bool
 * @param context 传递给predicate的上下文参数
 * @return 指向找到元素的迭代器，未找到时返回end的克隆
 *
 * @pre begin != NULL && end != NULL && predicate != NULL
 * @note 返回的迭代器需要调用iterator_destroy()释放
 */
typedef bool (*iterator_predicate_t)(dsa_const_element_pt element, void *context);
dsa_iterator_t *iterator_find_if(dsa_iterator_t *begin, dsa_iterator_t *end,
                                iterator_predicate_t predicate, void *context);

/** @} */ // IteratorAdvancedOperations

#endif // DSA_ITERATOR_H
