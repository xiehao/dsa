/**
 * @file linked_stack.c
 * @brief 基于单链表的栈实现
 * @details 实现了基于单链表的栈数据结构，提供LIFO（后进先出）访问模式。
 *          底层使用单链表存储数据，具有动态内存分配的优势，无需预分配固定大小的内存。
 *          相比数组栈，链表栈在频繁插入删除时具有更好的性能，但内存局部性较差。
 * @author oaheixiehao
 * @date 2025/7/4
 * @version 1.0
 */

#include <internal/linked_stack.h>
#include <internal/stack_traits.h>
#include <ds/linked_list.h>

/**
 * @brief 链表栈的内部结构体
 * @details 包含栈的特征表和底层单链表数据结构
 */
typedef struct {
    trait_stack_t const *traits;  ///< 栈操作特征表，必须与dsa_stack_t结构匹配
    dsa_linked_list_t *data;      ///< 底层单链表，存储栈元素
} linked_stack_t;

/**
 * @brief 获取链表栈中元素的数量
 * @param stack 栈的常量指针
 * @return 栈中元素的数量，如果栈为NULL则返回0
 * @note 时间复杂度：O(n)，其中n为栈中元素数量（需要遍历链表）
 */
static size_t linked_stack_size(dsa_const_container_pt stack) {
    linked_stack_t const *this = stack;
    return this ? linked_list_size(this->data) : 0;
}

/**
 * @brief 检查链表栈是否为空
 * @param stack 栈的常量指针
 * @return 如果栈为空或NULL则返回true，否则返回false
 * @note 时间复杂度：O(1)
 */
static bool linked_stack_is_empty(dsa_const_container_pt stack) {
    linked_stack_t const *this = stack;
    return this ? linked_list_is_empty(this->data) : true;
}

/**
 * @brief 清空链表栈中的所有元素
 * @param stack 栈的指针
 * @return 操作结果码：DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示栈为NULL
 * @note 时间复杂度：O(n)，其中n为栈中元素数量
 * @warning 此函数不会释放元素本身的内存，调用者需要自行管理元素内存
 */
static dsa_result_t linked_stack_clear(dsa_container_pt stack) {
    linked_stack_t *this = stack;
    return this ? linked_list_clear(this->data) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 销毁链表栈并释放相关内存
 * @param stack 栈的指针
 * @return 操作结果码：DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示栈为NULL
 * @note 时间复杂度：O(n)，其中n为栈中元素数量
 * @warning 此函数不会释放栈中存储的元素内存，调用者需要在销毁前清理元素
 */
static dsa_result_t linked_stack_destroy(dsa_container_pt stack) {
    linked_stack_t *this = stack;
    return this ? linked_list_destroy(this->data) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 链表栈的基本操作特征表
 * @details 定义了栈的基本容器操作函数指针
 */
static trait_basic_t const basic_trait = {
    .get_size = linked_stack_size,
    .is_empty = linked_stack_is_empty,
    .clear = linked_stack_clear,
    .destroy = linked_stack_destroy,
};

/**
 * @brief 向链表栈顶压入一个元素
 * @param stack 栈的指针
 * @param element 要压入的元素指针
 * @return 操作结果码：DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示栈为NULL
 * @note 时间复杂度：O(1)
 * @details 元素被插入到底层单链表的头部（索引0），实现LIFO访问模式。
 *          相比数组栈，链表栈的压入操作始终是O(1)时间复杂度。
 */
static dsa_result_t linked_stack_push(dsa_stack_t *stack, dsa_element_pt element) {
    linked_stack_t *this = (linked_stack_t *) stack;
    return this ? linked_list_insert_at(this->data, 0, element) : DSA_ERROR_NULL_POINTER;
}

/**
 * @brief 从链表栈顶弹出一个元素
 * @param stack 栈的指针
 * @return 弹出的元素指针，如果栈为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 * @details 从底层单链表的头部（索引0）移除元素，实现LIFO访问模式
 */
static dsa_element_pt linked_stack_pop(dsa_stack_t *stack) {
    linked_stack_t *this = (linked_stack_t *) stack;
    return this ? linked_list_remove_at(this->data, 0) : NULL;
}

/**
 * @brief 查看链表栈顶元素但不移除
 * @param stack 栈的常量指针
 * @return 栈顶元素指针，如果栈为空或NULL则返回NULL
 * @note 时间复杂度：O(1)
 * @details 返回底层单链表头部元素（索引0）的指针，不改变栈的状态
 */
static dsa_element_pt linked_stack_peek(dsa_stack_t const *stack) {
    linked_stack_t const *this = (linked_stack_t *) stack;
    return this ? linked_list_get(this->data, 0) : NULL;
}

/**
 * @brief 获取链表栈的类型标识
 * @return 栈类型枚举值STACK_TYPE_LINKED_LIST
 * @note 时间复杂度：O(1)
 */
static dsa_stack_type_t linked_stack_get_type(void) {
    return STACK_TYPE_LINKED_LIST;
}

/**
 * @brief 获取链表栈的类型名称字符串
 * @return 栈类型的中文描述字符串
 * @note 时间复杂度：O(1)
 */
static char const *linked_stack_get_type_name(void) {
    return "链式列表栈(链栈，Linked List Stack)";
}

/**
 * @brief 链表栈的完整操作特征表
 * @details 包含了链表栈的所有操作函数指针，用于实现多态性
 */
static trait_stack_t const stack_traits = {
    .basic = &basic_trait,
    .push = linked_stack_push,
    .pop = linked_stack_pop,
    .peek = linked_stack_peek,
    .get_type = linked_stack_get_type,
    .get_type_name = linked_stack_get_type_name,
};

/**
 * @brief 创建一个新的链表栈实例
 * @return 新创建的栈指针，如果内存分配失败则返回NULL
 * @note 时间复杂度：O(1)
 * @details 创建一个基于单链表的栈，无需预分配固定大小的内存。
 *          相比数组栈，链表栈在内存使用上更加灵活，但可能有更多的内存碎片。
 *          返回的栈指针需要调用stack_destroy()来释放内存。
 * @warning 调用者负责管理返回的栈指针的生命周期
 */
dsa_stack_t *linked_stack_create(void) {
    linked_stack_t *stack = malloc(sizeof(linked_stack_t));
    if (!stack) { return NULL; }
    stack->data = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    if (!stack->data) {
        free(stack);
        return NULL;
    }
    stack->traits = &stack_traits;
    return (dsa_stack_t *) stack;
}
