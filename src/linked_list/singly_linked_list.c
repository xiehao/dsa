/**
 * @file singly_linked.c
 * @brief 单链表数据结构的实现
 * @details 实现了基于单链表的动态数据结构，提供了基本的增删改查操作
 * @author oaheixiehao
 * @date 2025/6/23
 */

#include <internal/singly_linked_list.h>
#include <internal/linked_list_traits.h>

/**
 * @brief 单链表节点结构
 * @details 定义了单链表的基本节点结构，包含数据指针和下一个节点的指针
 */
typedef struct node_t {
    struct node_t *next;    /**< 指向下一个节点的指针 */
    dsa_element_pt data;    /**< 存储的数据元素指针 */
} node_t;

/**
 * @brief 单链表结构体
 * @details 定义了单链表的主体结构，包含特性指针、头节点和大小信息
 */
typedef struct {
    trait_linked_list_t const *traits;  /**< 链表特性接口指针 */
    node_t *head;                       /**< 头节点指针（哨兵节点） */
    size_t size;                        /**< 链表中元素的数量 */
} singly_linked_t;

/**
 * @brief 获取单链表的大小
 * @param list 链表容器指针
 * @return 链表中元素的数量，失败时返回SIZE_MAX
 * @note 查询类函数：返回数据，特殊值表示错误
 */
static size_t singly_linked_size(dsa_const_container_pt list) {
    singly_linked_t const *this = list;
    return this ? this->size : SIZE_MAX; // SIZE_MAX表示错误
}

/**
 * @brief 检查单链表是否为空
 * @param list 链表容器指针
 * @return true表示链表为空或指针为NULL，false表示链表非空
 */
static bool singly_linked_is_empty(dsa_const_container_pt list) {
    singly_linked_t const *this = list;
    if (!this) return true; // NULL指针视为空
    return this->size == 0;
}

/**
 * @brief 清空单链表中的所有元素
 * @param list 链表容器指针
 * @return DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示空指针错误
 * @note 修改类函数：返回错误码
 * @details 保留头节点，只清除数据节点
 */
static dsa_result_t singly_linked_clear(dsa_container_pt list) {
    singly_linked_t *this = list;
    if (!this || !this->head) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 跳过头结点，从第一个数据节点开始清理
    node_t *current = this->head->next;
    while (current) {
        node_t *next = current->next;
        free(current);
        current = next;
    }
    // 重置头结点的next指针和大小计数器
    this->head->next = NULL;
    this->size = 0;
    return DSA_SUCCESS;
}

/**
 * @brief 销毁单链表，释放所有内存
 * @param list 链表容器指针
 * @return DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示空指针错误
 * @note 析构类函数：返回错误码
 * @details 先清空所有元素，然后释放头节点和链表结构本身
 */
static dsa_result_t singly_linked_destroy(dsa_container_pt list) {
    singly_linked_t *this = list;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    dsa_result_t result = singly_linked_clear(list);
    // 释放头结点
    if (this->head) {
        free(this->head);
    }
    free(this);
    return result;
}

/**
 * @brief 基本特性接口实现
 * @details 定义了单链表的基本操作接口
 */
static trait_basic_t const basic_trait = {
    .get_size = singly_linked_size,
    .is_empty = singly_linked_is_empty,
    .clear = singly_linked_clear,
    .destroy = singly_linked_destroy,
};

/**
 * @brief 根据索引查找前一个节点
 * @param this 链表结构指针
 * @param index 目标索引位置
 * @return 指定索引位置前一个节点的指针，失败时返回NULL
 * @note 内部辅助函数：用于插入和删除操作
 * @details 从头节点开始查找，返回指定索引位置的前一个节点
 */
static node_t *find_node_before(singly_linked_t const *this, size_t index) {
    if (!this || !this->head || index > this->size) {
        return NULL;
    }

    // 从头结点开始查找前一个节点
    node_t *previous = this->head;
    for (size_t i = 0; i < index && previous->next; ++i) {
        previous = previous->next;
    }
    return previous;
}

/**
 * @brief 获取指定索引位置的元素
 * @param list 链表容器指针
 * @param index 要获取元素的索引位置
 * @return 指定位置的元素指针，失败时返回NULL
 * @note 查询类函数：返回数据，NULL表示错误
 */
static dsa_element_pt singly_linked_get(dsa_const_container_pt list, size_t index) {
    singly_linked_t const *this = list;
    node_t const *node = find_node_before(this, index + 1);
    return node ? node->data : NULL;
}

/**
 * @brief 设置指定索引位置的元素
 * @param list 链表容器指针
 * @param index 要设置元素的索引位置
 * @param element 要设置的元素指针
 * @return DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示空指针错误，DSA_ERROR_INDEX_OUT_OF_BOUNDS表示索引越界
 * @note 修改类函数：返回错误码
 */
static dsa_result_t singly_linked_set(dsa_container_pt list, size_t index, dsa_element_pt element) {
    singly_linked_t *this = list;
    if (!this || !element) {
        return DSA_ERROR_NULL_POINTER;
    }

    node_t *node = find_node_before(this, index + 1);
    if (!node) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    node->data = element;
    return DSA_SUCCESS;
}

/**
 * @brief 创建新节点
 * @param element 要存储的元素指针
 * @param out_node 输出参数，用于返回创建的节点指针
 * @return DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示空指针错误，DSA_ERROR_MEMORY_ALLOCATION表示内存分配失败
 * @note 内部辅助函数：创建新节点并初始化
 */
static dsa_result_t create_node(dsa_element_pt element, node_t **out_node) {
    if (!out_node) {
        return DSA_ERROR_NULL_POINTER;
    }

    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node) {
        return DSA_ERROR_MEMORY_ALLOCATION;
    }

    new_node->data = element;
    new_node->next = NULL;
    *out_node = new_node;

    return DSA_SUCCESS;
}

/**
 * @brief 在指定节点后插入新节点
 * @param node 要在其后插入的节点
 * @param new_node 要插入的新节点
 * @return DSA_SUCCESS表示成功，DSA_ERROR_NULL_POINTER表示空指针错误
 * @note 内部辅助函数：维护链表的链接关系
 */
static dsa_result_t attach_node_after(node_t *node, node_t *new_node) {
    if (!node || !new_node) {
        return DSA_ERROR_NULL_POINTER;
    }
    new_node->next = node->next;
    node->next = new_node;
    return DSA_SUCCESS;
}

/**
 * @brief 在指定索引位置插入元素
 * @param list 链表容器指针
 * @param index 要插入的索引位置
 * @param element 要插入的元素指针
 * @return DSA_SUCCESS表示成功，相应错误码表示失败原因
 * @note 修改类函数：返回错误码
 * @details 允许在链表末尾插入元素（index == size）
 */
static dsa_result_t singly_linked_insert_at(dsa_container_pt list, size_t index, dsa_element_pt element) {
    singly_linked_t *this = list;
    if (!this || !this->head || !element) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 允许在末尾插入
    if (index > this->size) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    // 使用统一的查找函数找到插入位置的前一个节点
    node_t *previous_node = find_node_before(this, index);
    if (!previous_node) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    node_t *new_node;
    dsa_result_t result = create_node(element, &new_node);
    if (DSA_SUCCESS != result) {
        return result;
    }

    result = attach_node_after(previous_node, new_node);
    if (DSA_SUCCESS != result) {
        free(new_node);
        return result;
    }

    this->size++;
    return DSA_SUCCESS;
}

/**
 * @brief 从指定节点后分离下一个节点
 * @param node 要从其后分离节点的节点
 * @return 被分离的节点指针，失败时返回NULL
 * @note 内部辅助函数：用于删除操作
 * @details 断开链接关系但不释放节点内存
 */
static node_t *detach_node_after(node_t *node) {
    if (!node || !node->next) {
        return NULL;
    }
    node_t *next = node->next;
    node->next = next->next;
    return next;
}

/**
 * @brief 删除指定索引位置的元素
 * @param list 链表容器指针
 * @param index 要删除元素的索引位置
 * @return 被删除的元素指针，失败时返回NULL
 * @note 查询类函数：返回数据，NULL表示错误或空
 * @details 删除节点并返回其中存储的数据
 */
static dsa_element_pt singly_linked_remove_at(dsa_container_pt list, size_t index) {
    singly_linked_t *this = list;
    if (!this || !this->head || index >= this->size) {
        return NULL;
    }

    // 使用统一的查找函数找到要删除节点的前一个节点
    node_t *prev = find_node_before(this, index);
    if (!prev || !prev->next) {
        return NULL; // 没有要删除的节点
    }

    node_t *to_remove = detach_node_after(prev);
    if (!to_remove) {
        return NULL;
    }

    dsa_element_pt data = to_remove->data;
    free(to_remove);
    this->size--;
    return data;
}

/**
 * @brief 随机访问特性接口实现
 * @details 定义了单链表的随机访问操作接口
 */
static trait_random_access_t const random_access_trait = {
    .get_at = singly_linked_get,
    .set_at = singly_linked_set,
    .insert_at = singly_linked_insert_at,
    .remove_at = singly_linked_remove_at,
};

/**
 * @brief 获取链表类型
 * @return 返回单链表类型标识
 * @note 查询类函数：返回数据
 */
static dsa_linked_list_type_t singly_linked_get_type(void) {
    return LINKED_LIST_TYPE_SINGLY;
}

/**
 * @brief 获取链表类型名称
 * @return 返回链表类型的中文名称
 * @note 查询类函数：返回数据
 */
static char const *singly_linked_get_type_name(void) {
    return "单链表(Singly Linked List)";
}

/**
 * @brief 链表特性接口实现
 * @details 组合了基本特性和随机访问特性，提供完整的链表操作接口
 */
static trait_linked_list_t const linked_list_trait = {
    .basic = &basic_trait,
    .random_access = &random_access_trait,
    .get_type = singly_linked_get_type,
    .get_type_name = singly_linked_get_type_name,
};

/**
 * @brief 创建新的单链表实例
 * @return 成功时返回新创建的链表指针，失败时返回NULL
 * @note 创建类函数：返回对象指针，NULL表示失败
 * @details 分配内存并初始化单链表结构，包括创建哨兵头节点
 */
dsa_linked_list_t *singly_linked_create(void) {
    singly_linked_t *list = malloc(sizeof(singly_linked_t));
    if (!list) { 
        return NULL; 
    }

    node_t *head;
    dsa_result_t result = create_node(NULL, &head);  // 传入NULL作为头结点数据
    if (result != DSA_SUCCESS) {
        free(list);
        return NULL;
    }

    list->head = head;
    list->size = 0;
    list->traits = &linked_list_trait;
    return (dsa_linked_list_t *) list;
}
