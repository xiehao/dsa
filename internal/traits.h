/**
 * @file traits.h
 * @brief 数据结构算法库（DSA）容器特性定义
 * @details 定义了各种容器类型的通用接口，提供统一的容器操作抽象层。
 *          包括基础操作、随机访问、迭代器、批量操作、排序变换等接口。
 * @author DSA Team
 * @date 2025-06-17
 * @version 1.0
 */

#ifndef DSA_TRAITS_H
#define DSA_TRAITS_H

#include <stddef.h>  // for size_t
#include <stdbool.h> // for bool (if using C)
#include <common.h>

/**
 * @defgroup ContainerTypes 容器类型定义
 * @brief 容器相关的类型别名定义
 * @{
 */

/**
 * @typedef dsa_container_pt
 * @brief 容器指针类型别名
 * @details 提高代码可读性的通用容器指针类型，指向可修改的容器实例
 */
typedef void *dsa_container_pt;

/**
 * @typedef dsa_const_container_pt
 * @brief 常量容器指针类型别名
 * @details 提高代码可读性的常量容器指针类型，指向不可修改的容器实例
 */
typedef const void *dsa_const_container_pt;

/** @} */ // ContainerTypes

/**
 * @defgroup BasicInterface 基础容器接口
 * @brief 所有容器都应实现的基础操作接口
 * @{
 */

/**
 * @struct trait_basic_t
 * @brief 基础容器操作接口
 * @details 定义了所有容器类型都应该实现的基础操作，包括状态查询、清理和内存管理操作
 */
typedef struct {
    /**
     * @brief 获取容器中的元素数量
     * @param container 容器的常量指针
     * @return 容器中的元素数量
     */
    size_t (*get_size)(dsa_const_container_pt container);

    /**
     * @brief 获取容器的最大容量
     * @param container 容器的常量指针
     * @return 容器的最大容量
     */
    size_t (*get_capacity)(dsa_const_container_pt container);

    /**
     * @brief 检查容器是否为空
     * @param container 容器的常量指针
     * @return 如果容器为空返回true，否则返回false
     */
    bool (*is_empty)(dsa_const_container_pt container);

    /**
     * @brief 检查容器是否已满
     * @param container 容器的常量指针
     * @return 如果容器已满返回true，否则返回false
     */
    bool (*is_full)(dsa_const_container_pt container);

    /**
     * @brief 清空容器中的所有元素但不释放它们的内存
     * @param container 容器指针
     * @return dsa_result_t 操作结果状态码
     * @note 此操作将容器重置为空状态，但不释放元素占用的内存
     */
    dsa_result_t (*clear)(dsa_container_pt container);

    /**
     * @brief 清空容器并释放所有元素的内存
     * @param container 容器指针
     * @return dsa_result_t 操作结果状态码
     * @note 此操作会释放所有元素占用的内存，然后重置容器为空状态
     */
    dsa_result_t (*clear_with_free)(dsa_container_pt container);

    /**
     * @brief 销毁容器并释放相关资源
     * @param container 要销毁的容器指针
     * @return dsa_result_t 操作结果状态码
     * @note 此函数不会释放容器元素的内存，仅释放容器结构本身的内存
     */
    dsa_result_t (*destroy)(dsa_container_pt container);

    /**
     * @brief 销毁容器并释放所有元素的内存
     * @param container 要销毁的容器指针
     * @return dsa_result_t 操作结果状态码
     * @note 此函数会先释放容器中所有元素的内存，然后释放容器结构的内存
     */
    dsa_result_t (*destroy_with_free)(dsa_container_pt container);
} trait_basic_t;

/** @} */ // BasicInterface

/**
 * @defgroup LinearInterface 线性容器接口
 * @brief 线性容器的前端和后端操作接口
 * @{
 */

/**
 * @struct trait_linear_t
 * @brief 线性容器双端操作接口
 * @details 定义了支持双端操作的线性容器（如双端队列、列表）应实现的接口，
 *          包括在容器前端和后端进行元素插入和删除操作
 */
typedef struct {
    /**
     * @brief 在容器前端添加元素
     * @param container 目标容器指针
     * @param element 要添加的元素指针
     * @return dsa_result_t 操作结果状态码
     * @retval DSA_SUCCESS 成功添加元素
     * @retval DSA_ERROR_INVALID_PARAM 参数无效
     * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
     * @note 该操作会在容器的开头位置插入新元素
     */
    dsa_result_t (*push_front)(dsa_container_pt container, dsa_element_pt element);

    /**
     * @brief 从容器前端移除并返回元素
     * @param container 目标容器指针
     * @return dsa_element_pt 被移除的元素指针，如果容器为空则返回NULL
     * @note 调用者负责释放返回元素的内存
     * @warning 在空容器上调用此函数会返回NULL
     */
    dsa_element_pt (*pop_front)(dsa_container_pt container);

    dsa_element_pt (*peek_front)(dsa_const_container_pt container);

    /**
     * @brief 在容器后端添加元素
     * @param container 目标容器指针
     * @param element 要添加的元素指针
     * @return dsa_result_t 操作结果状态码
     * @retval DSA_SUCCESS 成功添加元素
     * @retval DSA_ERROR_INVALID_PARAM 参数无效
     * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
     * @note 该操作会在容器的末尾位置插入新元素
     */
    dsa_result_t (*push_back)(dsa_container_pt container, dsa_element_pt element);

    /**
     * @brief 从容器后端移除并返回元素
     * @param container 目标容器指针
     * @return dsa_element_pt 被移除的元素指针，如果容器为空则返回NULL
     * @note 调用者负责释放返回元素的内存
     * @warning 在空容器上调用此函数会返回NULL
     */
    dsa_element_pt (*pop_back)(dsa_container_pt container);

    dsa_element_pt (*peek_back)(dsa_const_container_pt container);
} trait_linear_t;

/** @} */ // LinearInterface

/**
 * @defgroup RandomAccessInterface 随机访问接口
 * @brief 支持随机访问的容器操作接口
 * @{
 */

/**
 * @struct trait_random_access_t
 * @brief 随机访问容器操作接口
 * @details 定义了支持通过索引随机访问的容器（如向量、数组）应实现的接口
 */
typedef struct {
    /**
     * @brief 获取指定索引位置的元素
     * @param container 容器的常量指针
     * @param index 要访问的元素索引
     * @return 元素指针，如果索引越界则返回NULL
     * @note 不会改变元素所有权，返回的指针指向容器内的元素
     */
    dsa_element_pt (*get_at)(dsa_const_container_pt container, size_t index);

    /**
     * @brief 设置指定索引位置的元素
     * @param container 容器指针
     * @param index 要设置的元素索引
     * @param element 新元素的指针
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 此函数不会释放被替换元素的内存，调用者负责管理原有元素的内存
     */
    dsa_result_t (*set_at)(dsa_container_pt container, size_t index, dsa_element_pt element);

    /**
     * @brief 在指定索引位置插入元素
     * @param container 容器指针
     * @param index 要插入的位置索引
     * @param element 要插入的元素指针
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 插入后，该位置及其后的所有元素将向后移动一个位置
     */
    dsa_result_t (*insert_at)(dsa_container_pt container, size_t index, dsa_element_pt element);

    /**
     * @brief 移除指定索引位置的元素
     * @param container 容器指针
     * @param index 要移除的元素索引
     * @return 被移除元素的指针，如果索引无效则返回NULL
     * @note 调用者负责释放返回元素的内存；移除后，后续元素将前移一个位置
     */
    dsa_element_pt (*remove_at)(dsa_container_pt container, size_t index);
} trait_random_access_t;

/** @} */ // RandomAccessInterface

/**
 * @defgroup SearchInterface 容器查找接口
 * @brief 容器元素查找操作接口
 * @{
 */

/**
 * @struct trait_search_t
 * @brief 容器查找操作接口
 * @details 定义了容器中查找元素的各种操作
 */
typedef struct {
    /**
     * @brief 在容器中查找匹配的元素
     * @param container 容器的常量指针
     * @param element 要查找的元素指针
     * @param compare 比较函数，签名为int (*)(const void*, const void*)，
     *               相等返回0，小于返回负值，大于返回正值
     * @return 找到的元素指针，如果未找到则返回NULL
     * @note 如果多个元素匹配，返回第一个匹配的元素
     */
    dsa_element_pt (*find)(dsa_const_container_pt container, dsa_element_pt element,
                           compare_func_t compare);

    /**
     * @brief 查找元素在容器中的索引
     * @param container 容器的常量指针
     * @param element 要查找的元素指针
     * @param compare 比较函数，签名为int (*)(const void*, const void*)
     * @return 元素的索引，如果未找到则返回容器大小
     */
    size_t (*find_index)(dsa_const_container_pt container, dsa_element_pt element,
                         compare_func_t compare);

    /**
     * @brief 检查容器是否包含指定元素
     * @param container 容器的常量指针
     * @param element 要检查的元素指针
     * @param compare 比较函数，签名为int (*)(const void*, const void*)
     * @return 如果容器包含该元素则返回true，否则返回false
     */
    bool (*contains)(dsa_const_container_pt container, dsa_element_pt element,
                     compare_func_t compare);

    /**
     * @brief 计算容器中匹配指定元素的数量
     * @param container 容器的常量指针
     * @param element 要计数的元素指针
     * @param compare 比较函数，签名为int (*)(const void*, const void*)
     * @return 匹配元素的数量
     */
    size_t (*count)(dsa_const_container_pt container, dsa_element_pt element,
                    compare_func_t compare);
} trait_search_t;

/** @} */ // SearchInterface

/**
 * @defgroup IteratorInterface 迭代器接口
 * @brief 容器迭代器操作接口
 *
 * 本模块定义了数据结构算法库中迭代器的基本类型和接口。
 * 迭代器提供了统一的容器元素访问方式，支持顺序遍历和随机访问等操作。
 *
 * @details
 * 迭代器接口包含以下主要功能：
 * - 元素访问和修改
 * - 位置移动（前进、后退）
 * - 迭代器比较
 * - 距离计算
 *
 * @note 所有迭代器操作都应该通过相应的trait接口进行，以确保类型安全
 *
 * @see trait_iterator_t
 * @since 1.0
 * @author DSA Library Team
 *
 * @{
 */

/**
 * @typedef dsa_iterator_pt
 * @brief 可变迭代器指针类型
 *
 * 指向dsa_iterator_t结构体的指针，用于表示可变迭代器。
 * 通过此类型的迭代器可以读取和修改容器中的元素。
 *
 * @details
 * 可变迭代器支持以下操作：
 * - 读取当前位置的元素值
 * - 修改当前位置的元素值
 * - 移动到下一个或上一个位置
 * - 与其他迭代器进行比较
 *
 * @note 使用前需要确保迭代器已正确初始化且指向有效位置
 *
 * @see dsa_const_iterator_pt
 * @see trait_iterator_t
 */
typedef struct dsa_iterator_t *dsa_iterator_pt;

/**
 * @typedef dsa_const_iterator_pt
 * @brief 常量迭代器指针类型
 *
 * 指向dsa_iterator_t结构体的常量指针，用于表示只读迭代器。
 * 通过此类型的迭代器只能读取容器中的元素，不能修改。
 *
 * @details
 * 常量迭代器支持以下操作：
 * - 读取当前位置的元素值（只读）
 * - 移动到下一个或上一个位置
 * - 与其他迭代器进行比较
 *
 * @note
 * - 常量迭代器不支持元素修改操作
 * - 使用前需要确保迭代器已正确初始化且指向有效位置
 *
 * @see dsa_iterator_pt
 * @see trait_iterator_t
 */
typedef const struct dsa_iterator_t *dsa_const_iterator_pt;

/**
 * @struct trait_iterator_t
 * @brief 容器迭代器接口
 * @details 定义了容器迭代访问的标准接口，支持双向迭代
 */
typedef struct {
    /**
     * @brief 获取指向容器第一个元素的迭代器
     * @param container 容器的常量指针
     * @return 指向容器第一个元素的迭代器，如果容器为空则返回等同于end()的迭代器
     */
    void * (*begin)(dsa_const_container_pt container);

    /**
     * @brief 获取指向容器尾部（最后一个元素之后）的迭代器
     * @param container 容器的常量指针
     * @return 指向容器尾部的迭代器
     */
    void * (*end)(dsa_const_container_pt container);

    /**
     * @brief 将迭代器移动到下一个元素
     * @param iterator 当前迭代器
     * @return 指向下一个元素的迭代器，如果没有下一个元素则返回等同于end()的迭代器
     */
    void * (*next)(void *iterator);

    /**
     * @brief 将迭代器移动到前一个元素
     * @param iterator 当前迭代器
     * @return 指向前一个元素的迭代器，如果没有前一个元素则返回NULL
     */
    void * (*prev)(void *iterator);

    /**
     * @brief 获取迭代器当前指向的元素
     * @param iterator 当前迭代器
     * @return 当前元素的指针，如果迭代器无效则返回NULL
     */
    dsa_element_pt (*get_value)(void *iterator);

    /**
     * @brief 检查迭代器是否有效
     * @param iterator 要检查的迭代器
     * @return 如果迭代器有效返回true，否则返回false
     */
    bool (*is_valid)(void *iterator);
} trait_iterator_t;

/** @} */ // IteratorInterface

/**
 * @defgroup BatchInterface 批量操作接口
 * @brief 容器批量操作接口
 * @{
 */

/**
 * @struct trait_batch_t
 * @brief 容器批量操作接口
 * @details 定义了容器的批量操作，如批量插入、删除、复制等
 */
typedef struct {
    /**
     * @brief 在指定位置插入一组元素
     * @param container 容器指针
     * @param index 要插入的位置索引
     * @param elements 要插入的元素指针数组
     * @param count 要插入的元素数量
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 插入后，index位置及其后的所有原有元素将向后移动count个位置
     */
    dsa_result_t (*insert_range)(dsa_container_pt container, size_t index,
                                 dsa_element_pt *elements, size_t count);

    /**
     * @brief 移除一段连续的元素
     * @param container 容器指针
     * @param start_index 要移除的起始索引
     * @param count 要移除的元素数量
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 此函数不负责释放被移除元素的内存，调用者需要事先保存并管理这些元素
     */
    dsa_result_t (*remove_range)(dsa_container_pt container, size_t start_index, size_t count);

    /**
     * @brief 复制容器中一段连续的元素到目标数组
     * @param container 容器的常量指针
     * @param start_index 要复制的起始索引
     * @param count 要复制的元素数量
     * @param dest 目标数组，用于存储复制的元素指针
     * @param dest_size 目标数组的大小
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 调用者需要确保dest数组有足够的空间存储count个元素指针
     */
    dsa_result_t (*copy_range)(dsa_const_container_pt container, size_t start_index,
                               size_t count, dsa_element_pt *dest, size_t dest_size);

    /**
     * @brief 将另一个容器的所有元素追加到当前容器末尾
     * @param container 目标容器指针
     * @param source 源容器的常量指针
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 此操作只复制元素指针，不会复制元素内容，两个容器将共享相同的元素
     */
    dsa_result_t (*append_all)(dsa_container_pt container, dsa_const_container_pt source);

    /**
     * @brief 从容器中移除所有匹配的元素
     * @param container 容器指针
     * @param element 要移除的元素指针
     * @param compare 比较函数，签名为int (*)(const void*, const void*)
     * @return 被移除的元素数量
     * @note 此函数不负责释放被移除元素的内存，调用者需要管理这些元素
     */
    size_t (*remove_all)(dsa_container_pt container, dsa_element_pt element,
                         int (*compare)(const void *a, const void *b));

    /**
     * @brief 保留所有匹配的元素，移除其他元素
     * @param container 容器指针
     * @param element 要保留的元素指针
     * @param compare 比较函数，签名为int (*)(const void*, const void*)
     * @return 被移除的元素数量
     * @note 此函数不负责释放被移除元素的内存，调用者需要管理这些元素
     */
    size_t (*retain_all)(dsa_container_pt container, dsa_element_pt element,
                         int (*compare)(const void *a, const void *b));

    /**
     * @brief 调整容器的容量
     * @param container 容器指针
     * @param new_capacity 新的容量大小
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 如果新容量小于当前元素数量，可能会导致数据丢失
     */
    dsa_result_t (*resize)(dsa_container_pt container, size_t new_capacity);

    /**
     * @brief 预留容器空间以避免频繁重新分配
     * @param container 容器指针
     * @param min_capacity 最小所需容量
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 此操作只增加容量，不会减少现有容量
     */
    dsa_result_t (*reserve)(dsa_container_pt container, size_t min_capacity);
} trait_batch_t;

/** @} */ // BatchInterface

/**
 * @defgroup TransformInterface 排序和变换接口
 * @brief 容器排序和变换操作接口
 * @{
 */

/**
 * @struct trait_transform_t
 * @brief 容器排序和变换接口
 * @details 定义了容器的排序、反转、打乱以及函数式编程风格的操作
 */
typedef struct {
    /**
     * @brief 对容器元素进行排序
     * @param container 容器指针
     * @param compare 比较函数，签名为int (*)(const void*, const void*)，
     *               小于返回负值，等于返回0，大于返回正值
     */
    void (*sort)(dsa_container_pt container, compare_func_t compare);

    /**
     * @brief 反转容器中元素的顺序
     * @param container 容器指针
     */
    void (*reverse)(dsa_container_pt container);

    /**
     * @brief 随机打乱容器中元素的顺序
     * @param container 容器指针
     */
    void (*shuffle)(dsa_container_pt container);

    /**
     * @brief 对容器中的每个元素执行指定函数
     * @param container 容器的常量指针
     * @param func 要执行的函数，签名为void (*)(dsa_element_pt)
     */
    void (*for_each)(dsa_const_container_pt container, void (*func)(dsa_element_pt element));

    /**
     * @brief 创建一个新容器，包含原容器中满足条件的元素
     * @param container 容器的常量指针
     * @param predicate 判断函数，签名为bool (*)(dsa_element_pt)，返回true表示元素满足条件
     * @return 新创建的容器指针，如果创建失败则返回NULL
     * @note 返回的新容器需要由调用者负责销毁
     */
    dsa_container_pt (*filter)(dsa_const_container_pt container,
                               bool (*predicate)(dsa_element_pt element));

    /**
     * @brief 创建一个新容器，包含对原容器中每个元素应用转换函数后的结果
     * @param container 容器的常量指针
     * @param transform 转换函数，签名为dsa_element_pt (*)(dsa_element_pt)
     * @return 新创建的容器指针，如果创建失败则返回NULL
     * @note 返回的新容器需要由调用者负责销毁
     */
    dsa_container_pt (*map)(dsa_const_container_pt container,
                            dsa_element_pt (*transform)(dsa_element_pt element));
} trait_transform_t;

/** @} */ // TransformInterface

/**
 * @defgroup UtilityInterface 容器工具接口
 * @brief 容器比较和复制操作接口
 * @{
 */

/**
 * @struct trait_utility_t
 * @brief 容器比较和复制接口
 * @details 定义了容器间的比较、相等性检查和复制操作
 */
typedef struct {
    /**
     * @brief 比较两个容器是否相等
     * @param lhs 第一个容器的常量指针
     * @param rhs 第二个容器的常量指针
     * @param compare 元素比较函数，签名为int (*)(const void*, const void*)
     * @return 如果两个容器大小相等且所有对应元素都相等则返回true，否则返回false
     */
    bool (*equals)(dsa_const_container_pt lhs, dsa_const_container_pt rhs,
                   compare_func_t compare);

    /**
     * @brief 比较两个容器的顺序
     * @param lhs 第一个容器的常量指针
     * @param rhs 第二个容器的常量指针
     * @param compare 元素比较函数，签名为int (*)(const void*, const void*)
     * @return 负值表示container1小于container2，0表示相等，正值表示大于
     * @note 比较方式类似于字典序：先比较第一对不相等的元素，如果所有对应元素都相等，则较短的容器较小
     */
    int (*compare_containers)(dsa_const_container_pt lhs,
                              dsa_const_container_pt rhs,
                              compare_func_t compare);

    /**
     * @brief 创建容器的完整副本
     * @param container 要复制的容器的常量指针
     * @return 新创建的容器副本指针，如果创建失败则返回NULL
     * @note 返回的新容器需要由调用者负责销毁；此函数仅复制元素指针，不复制元素内容
     */
    dsa_container_pt (*clone)(dsa_const_container_pt container);

    /**
     * @brief 将源容器的内容复制到目标容器
     * @param dest 目标容器指针
     * @param src 源容器的常量指针
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 目标容器会先被清空，然后添加源容器的所有元素
     */
    dsa_result_t (*copy_from)(dsa_container_pt dest, dsa_const_container_pt src);
} trait_utility_t;

/** @} */ // UtilityInterface

/**
 * @defgroup StackQueueInterface 栈和队列接口
 * @brief 栈和队列特定操作接口
 * @{
 */

/**
 * @struct trait_queue_t
 * @brief 队列容器特定接口
 * @details 定义了队列数据结构的FIFO（先进先出）操作
 */
typedef struct {
    /**
     * @brief 将元素加入队列尾部
     * @param container 队列容器指针
     * @param element 要入队的元素指针
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     */
    dsa_result_t (*enqueue)(dsa_container_pt container, dsa_element_pt element);

    /**
     * @brief 移除并返回队列头部元素
     * @param container 队列容器指针
     * @return 队列头部元素指针，如果队列为空则返回NULL
     * @note 调用者负责释放返回元素的内存（如果需要）
     */
    dsa_element_pt (*dequeue)(dsa_container_pt container);

    /**
     * @brief 查看队列头部元素但不移除
     * @param container 队列容器的常量指针
     * @return 队列头部元素指针，如果队列为空则返回NULL
     */
    dsa_element_pt (*front)(dsa_const_container_pt container);

    /**
     * @brief 查看队列尾部元素但不移除
     * @param container 队列容器的常量指针
     * @return 队列尾部元素指针，如果队列为空则返回NULL
     */
    dsa_element_pt (*rear)(dsa_const_container_pt container);
} trait_queue_t;

/** @} */ // StackQueueInterface

/**
 * @defgroup PriorityQueueInterface 优先队列接口
 * @brief 优先队列特定操作接口
 * @{
 */

/**
 * @struct trait_priority_queue_t
 * @brief 优先队列接口
 * @details 定义了优先队列数据结构的基于优先级的操作
 */
typedef struct {
    /**
     * @brief 以指定优先级插入元素
     * @param container 优先队列容器指针
     * @param element 要插入的元素指针
     * @param priority 元素的优先级值
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     */
    dsa_result_t (*insert_with_priority)(dsa_container_pt container,
                                         dsa_element_pt element, int priority);

    /**
     * @brief 移除并返回优先级最高的元素
     * @param container 优先队列容器指针
     * @return 优先级最高的元素指针，如果队列为空则返回NULL
     */
    dsa_element_pt (*extract_max)(dsa_container_pt container);

    /**
     * @brief 移除并返回优先级最低的元素
     * @param container 优先队列容器指针
     * @return 优先级最低的元素指针，如果队列为空则返回NULL
     */
    dsa_element_pt (*extract_min)(dsa_container_pt container);

    /**
     * @brief 查看优先级最高的元素但不移除
     * @param container 优先队列容器的常量指针
     * @return 优先级最高的元素指针，如果队列为空则返回NULL
     */
    dsa_element_pt (*peek_max)(dsa_const_container_pt container);

    /**
     * @brief 查看优先级最低的元素但不移除
     * @param container 优先队列容器的常量指针
     * @return 优先级最低的元素指针，如果队列为空则返回NULL
     */
    dsa_element_pt (*peek_min)(dsa_const_container_pt container);

    /**
     * @brief 更改队列中元素的优先级
     * @param container 优先队列容器指针
     * @param element 要更改优先级的元素指针
     * @param new_priority 新的优先级值
     * @note 更改优先级后，队列会重新调整以维持优先级顺序
     */
    void (*change_priority)(dsa_container_pt container, dsa_element_pt element, int new_priority);
} trait_priority_queue_t;

/** @} */ // PriorityQueueInterface

/**
 * @defgroup SetInterface 集合接口
 * @brief 集合特定操作接口
 * @{
 */

/**
 * @struct trait_set_t
 * @brief 集合特定接口
 * @details 定义了集合数据结构的操作，包括基本的集合运算
 */
typedef struct {
    /**
     * @brief 向集合中添加元素（如果不存在）
     * @param container 集合容器指针
     * @param element 要添加的元素指针
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 如果元素已存在，不会重复添加
     */
    dsa_result_t (*add)(dsa_container_pt container, dsa_element_pt element);

    /**
     * @brief 从集合中移除元素
     * @param container 集合容器指针
     * @param element 要移除的元素指针
     * @return 如果元素存在并被成功移除则返回true，否则返回false
     * @note 此函数不负责释放被移除元素的内存
     */
    bool (*remove)(dsa_container_pt container, dsa_element_pt element);

    /**
     * @brief 创建包含两个集合所有元素的并集
     * @param set1 第一个集合的常量指针
     * @param set2 第二个集合的常量指针
     * @return 新创建的并集集合指针，如果创建失败则返回NULL
     * @note 返回的新集合需要由调用者负责销毁
     */
    dsa_container_pt (*union_sets)(dsa_const_container_pt set1, dsa_const_container_pt set2);

    /**
     * @brief 创建包含两个集合共有元素的交集
     * @param set1 第一个集合的常量指针
     * @param set2 第二个集合的常量指针
     * @return 新创建的交集集合指针，如果创建失败则返回NULL
     * @note 返回的新集合需要由调用者负责销毁
     */
    dsa_container_pt (*intersect)(dsa_const_container_pt set1, dsa_const_container_pt set2);

    /**
     * @brief 创建包含第一个集合中不在第二个集合中的元素的差集
     * @param set1 第一个集合的常量指针
     * @param set2 第二个集合的常量指针
     * @return 新创建的差集集合指针，如果创建失败则返回NULL
     * @note 返回的新集合需要由调用者负责销毁
     */
    dsa_container_pt (*difference)(dsa_const_container_pt set1, dsa_const_container_pt set2);

    /**
     * @brief 检查一个集合是否是另一个集合的子集
     * @param subset 可能的子集的常量指针
     * @param superset 可能的超集的常量指针
     * @return 如果subset是superset的子集则返回true，否则返回false
     */
    bool (*is_subset)(dsa_const_container_pt subset, dsa_const_container_pt superset);
} trait_set_t;

/** @} */ // SetInterface

/**
 * @defgroup MapInterface 映射接口
 * @brief 映射/字典特定操作接口
 * @{
 */

/**
 * @struct trait_map_t
 * @brief 映射/字典特定接口
 * @details 定义了键值对映射数据结构的操作
 */
typedef struct {
    /**
     * @brief 在映射中添加或更新键值对
     * @param container 映射容器指针
     * @param key 键指针
     * @param value 值指针
     * @return 成功返回DSA_SUCCESS，失败返回相应的错误码
     * @note 如果键已存在，将更新对应的值
     */
    dsa_result_t (*put)(dsa_container_pt container, dsa_element_pt key, dsa_element_pt value);

    /**
     * @brief 获取映射中指定键对应的值
     * @param container 映射容器的常量指针
     * @param key 要查找的键指针
     * @return 对应的值指针，如果键不存在则返回NULL
     */
    dsa_element_pt (*get)(dsa_const_container_pt container, dsa_element_pt key);

    /**
     * @brief 通过键移除映射中的键值对
     * @param container 映射容器指针
     * @param key 要移除的键指针
     * @return 如果键存在并成功移除则返回true，否则返回false
     * @note 此函数不负责释放被移除的键和值的内存
     */
    bool (*remove_by_key)(dsa_container_pt container, dsa_element_pt key);

    /**
     * @brief 检查映射是否包含指定的键
     * @param container 映射容器的常量指针
     * @param key 要检查的键指针
     * @return 如果键存在则返回true，否则返回false
     */
    bool (*contains_key)(dsa_const_container_pt container, dsa_element_pt key);

    /**
     * @brief 检查映射是否包含指定的值
     * @param container 映射容器的常量指针
     * @param value 要检查的值指针
     * @return 如果值存在则返回true，否则返回false
     */
    bool (*contains_value)(dsa_const_container_pt container, dsa_element_pt value);

    /**
     * @brief 获取映射中所有键的集合
     * @param container 映射容器的常量指针
     * @return 包含所有键的新容器指针，如果创建失败则返回NULL
     * @note 返回的新容器需要由调用者负责销毁
     */
    dsa_container_pt (*get_keys)(dsa_const_container_pt container);

    /**
     * @brief 获取映射中所有值的集合
     * @param container 映射容器的常量指针
     * @return 包含所有值的新容器指针，如果创建失败则返回NULL
     * @note 返回的新容器需要由调用者负责销毁
     */
    dsa_container_pt (*get_values)(dsa_const_container_pt container);
} trait_map_t;

/** @} */ // MapInterface

#endif // DSA_TRAITS_H
