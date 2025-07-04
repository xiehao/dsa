/**
 * @file array_stack.c
 * @brief 基于动态数组的栈实现
 * @details 实现了基于动态数组的栈数据结构，提供LIFO（后进先出）访问模式。
 *          底层使用动态数组存储数据，支持自动扩容，具有良好的内存局部性。
 * @author oaheixiehao
 * @date 2025/7/3
 * @version 1.0
 */

#include <internal/array_stack.h>
#include <internal/stack_traits.h>
#include <ds/array_list.h>

/**
 * @brief 数组栈的内部结构体
 * @details 包含栈的特征表和底层动态数组数据结构
 */
typedef struct {
    trait_stack_t const *traits;  ///< 栈操作特征表，必须与dsa_stack_t结构匹配
    dsa_array_list_t *data;       ///< 底层动态数组，存储栈元素
} array_stack_t;

/**
 * @brief 获取数组栈中元素的数量
 * @param stack 栈的常量指针
 * @return 栈中元素的数量，如果栈为NULL则返回0
 * @note 时间复杂度：O(1)
 */
static size_t array_stack_size(dsa_const_container_pt stack) {
    array_stack_t const *this = stack;
    return this ? array_list_size(this->data) : 0;
}

/**
 * @brief 检查数组栈是否为空
 * @param stack 栈的常量指针
 * @return 如果栈为空或NULL则返回true，否则返回false
 * @note 时间复杂度：O(1)
 */
static bool array_stack_is_empty(dsa_const_container_pt stack) {
    array_stack_t const *this = stack;
    return this ? array_list_is_empty(this->data) : true;
}

/**
 * @brief 清空数组栈中的所有元素
 * @param stack 栈的指针
 * @return 操作结果码：DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示栈为NULL
 * @note 时间复杂度：O(n)，其中n为栈中元素数量
 * @warning 此函数不会释放元素本身的内存，调用者需要自行管理元素内存
 */
static dsa_result_t array_stack_clear(dsa_container_pt stack) {
    array_stack_t *this = stack;
    return this ? array_list_clear(this->data) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 销毁数组栈并释放相关内存
 * @param stack 栈的指针
 * @return 操作结果码：DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示栈为NULL
 * @note 时间复杂度：O(1)
 * @warning 此函数不会释放栈中存储的元素内存，调用者需要在销毁前清理元素
 */
static dsa_result_t array_stack_destroy(dsa_container_pt stack) {
    array_stack_t *this = stack;
    return this ? array_list_destroy(this->data) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 数组栈的基本操作特征表
 * @details 定义了栈的基本容器操作函数指针
 */
static trait_basic_t const basic_trait = {
    .get_size = array_stack_size,
    .is_empty = array_stack_is_empty,
    .clear = array_stack_clear,
    .destroy = array_stack_destroy,
};

/**
 * @brief 向数组栈顶压入一个元素
 * @param stack 栈的指针
 * @param element 要压入的元素指针
 * @return 操作结果码：DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示栈为NULL
 * @note 时间复杂度：平摊O(1)，最坏情况O(n)（当需要扩容时）
 * @details 元素被添加到底层动态数组的末尾，实现LIFO访问模式
 */
static dsa_result_t array_stack_push(dsa_stack_t *stack, dsa_element_pt element) {
    array_stack_t *this = (array_stack_t *)stack;
    return this ? array_list_push_back(this->data, element) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 从数组栈顶弹出一个元素
 * @param stack 栈的指针
 * @return 弹出的元素指针，如果栈为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 * @details 从底层动态数组的末尾移除元素，实现LIFO访问模式
 */
static dsa_element_pt array_stack_pop(dsa_stack_t *stack) {
    array_stack_t *this = (array_stack_t *)stack;
    return this ? array_list_pop_back(this->data) : NULL;
}

/**
 * @brief 查看数组栈顶元素但不移除
 * @param stack 栈的常量指针
 * @return 栈顶元素指针，如果栈为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 * @details 返回底层动态数组末尾元素的指针，不改变栈的状态
 */
static dsa_element_pt array_stack_peek(dsa_stack_t const *stack) {
    array_stack_t const *this = (array_stack_t const *)stack;
    if (!this || array_list_is_empty(this->data)) {
        return NULL;
    }
    return array_list_get(this->data, array_list_size(this->data) - 1);
}

/**
 * @brief 获取数组栈的类型标识
 * @return 栈类型枚举值STACK_TYPE_ARRAY_LIST
 * @note 时间复杂度：O(1)
 */
static dsa_stack_type_t array_stack_get_type(void) {
    return STACK_TYPE_ARRAY_LIST;
}

/**
 * @brief 获取数组栈的类型名称字符串
 * @return 栈类型的中文描述字符串
 * @note 时间复杂度：O(1)
 */
static char const *array_stack_get_type_name(void) {
    return "数组列表栈(Array List Stack)";
}

/**
 * @brief 数组栈的完整操作特征表
 * @details 包含了数组栈的所有操作函数指针，用于实现多态性
 */
static trait_stack_t const stack_traits = {
    .basic = &basic_trait,
    .push = array_stack_push,
    .pop = array_stack_pop,
    .peek = array_stack_peek,
    .get_type = array_stack_get_type,
    .get_type_name = array_stack_get_type_name,
};

/**
 * @brief 创建一个新的数组栈实例
 * @return 新创建的栈指针，如果内存分配失败则返回NULL
 * @note 时间复杂度：O(1)
 * @details 创建一个基于动态数组的栈，初始容量为0，支持自动扩容。
 *          返回的栈指针需要调用stack_destroy()来释放内存。
 * @warning 调用者负责管理返回的栈指针的生命周期
 */
dsa_stack_t *array_stack_create(void) {
    array_stack_t *stack = malloc(sizeof(array_stack_t));
    if (!stack) { return NULL; }

    stack->data = array_list_create_dynamic(0);
    if (!stack->data) {
        free(stack);
        return NULL;
    }

    // 设置traits
    stack->traits = &stack_traits;
    return (dsa_stack_t *)stack;
}
