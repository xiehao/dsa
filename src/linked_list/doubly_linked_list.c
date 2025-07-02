/**
 * @file doubly_linked_list.c
 * @brief 双向链表的实现
 * @details 这个文件实现了双向链表的所有基本操作，包括创建、插入、删除、访问等功能。
 *          双向链表使用哨兵节点（头尾节点）来简化边界条件处理。
 * @author AI Assistant
 * @date 2025-07-02
 * @version 1.0
 */

#include <internal/linked_list_traits.h>
#include <internal/doubly_linked_list.h>

/**
 * @brief 双向链表节点结构
 * @details 双向链表的基本节点，包含指向前一个和后一个节点的指针，以及数据指针
 */
typedef struct node_t {
    struct node_t *previous;  /**< 指向前一个节点的指针 */
    struct node_t *next;      /**< 指向后一个节点的指针 */
    dsa_element_pt data;      /**< 节点存储的数据指针 */
} node_t;

/**
 * @brief 双向链表结构
 * @details 双向链表的主要结构，包含特征表、哨兵节点和大小信息
 */
typedef struct {
    trait_linked_list_t const *traits;  /**< 链表特征函数表指针 */
    node_t *head;                        /**< 头哨兵节点指针 */
    node_t *tail;                        /**< 尾哨兵节点指针 */
    size_t size;                         /**< 链表中元素的数量 */
} doubly_linked_t;

/**
 * @brief 获取双向链表的大小
 * @param list 链表容器指针
 * @return 链表中元素的数量，如果链表为NULL则返回SIZE_MAX
 * @retval SIZE_MAX 链表指针为NULL
 * @retval >=0 链表中实际元素数量
 */
static size_t doubly_linked_size(dsa_const_container_pt list) {
    doubly_linked_t const *this = list;
    return this ? this->size : SIZE_MAX;
}

/**
 * @brief 检查双向链表是否为空
 * @param list 链表容器指针
 * @return 链表是否为空
 * @retval true 链表为空或链表指针为NULL
 * @retval false 链表不为空
 */
static bool doubly_linked_is_empty(dsa_const_container_pt list) {
    doubly_linked_t const *this = list;
    if (!this) return true;
    return this->size == 0;
}

/**
 * @brief 清空双向链表中的所有元素
 * @param list 链表容器指针
 * @return 操作结果
 * @retval DSA_SUCCESS 清空操作成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针或头节点为NULL
 * @details 清空操作会释放所有数据节点的内存，但保留哨兵节点
 */
static dsa_result_t doubly_linked_clear(dsa_container_pt list) {
    doubly_linked_t *this = list;
    if (!this || !this->head) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 从第一个数据节点开始遍历并释放
    node_t *current = this->head->next;
    while (current != this->tail) {
        node_t *next = current->next;
        free(current);
        current = next;
    }

    // 重新连接哨兵节点
    this->head->next = this->tail;
    this->tail->previous = this->head;
    this->size = 0;
    return DSA_SUCCESS;
}

/**
 * @brief 销毁双向链表并释放所有内存
 * @param list 链表容器指针
 * @return 操作结果
 * @retval DSA_SUCCESS 销毁操作成功
 * @retval DSA_ERROR_NULL_POINTER 链表指针为NULL
 * @details 销毁操作会清空所有元素，释放哨兵节点，最后释放链表结构体本身
 */
static dsa_result_t doubly_linked_destroy(dsa_container_pt list) {
    doubly_linked_t *this = list;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 先清空所有数据节点
    dsa_result_t result = doubly_linked_clear(list);

    // 释放哨兵节点
    if (this->head) {
        free(this->head);
    }
    if (this->tail) {
        free(this->tail);
    }

    // 释放链表结构体
    free(this);
    return result;
}

/**
 * @brief 基本特征函数表
 * @details 包含链表的基本操作函数指针
 */
static trait_basic_t const basic_trait = {
    .get_size = doubly_linked_size,
    .is_empty = doubly_linked_is_empty,
    .clear = doubly_linked_clear,
    .destroy = doubly_linked_destroy,
};

/**
 * @brief 查找指定索引位置前的节点
 * @param this 双向链表指针
 * @param index 目标索引位置
 * @return 指定索引位置前的节点指针
 * @retval NULL 链表为NULL、头节点为NULL或索引超出范围
 * @retval node_t* 有效的节点指针
 * @details 从头哨兵节点开始，向前遍历index步，返回到达的节点
 */
static node_t *find_node_before(doubly_linked_t const *this, size_t index) {
    if (!this || !this->head || index > this->size) {
        return NULL;
    }

    node_t *node = this->head;
    for (size_t i = 0; i < index; ++i) {
        node = node->next;
    }
    return node;
}

/**
 * @brief 获取指定索引位置的元素
 * @param list 链表容器指针
 * @param index 要获取元素的索引位置
 * @return 指定位置的元素指针
 * @retval NULL 链表为NULL或索引超出范围
 * @retval dsa_element_pt 有效的元素指针
 */
static dsa_element_pt doubly_linked_get(dsa_const_container_pt list, size_t index) {
    doubly_linked_t const *this = list;
    node_t const *node = find_node_before(this, index + 1);
    return node ? node->data : NULL;
}

/**
 * @brief 设置指定索引位置的元素
 * @param list 链表容器指针
 * @param index 要设置元素的索引位置
 * @param element 要设置的元素指针
 * @return 操作结果
 * @retval DSA_SUCCESS 设置操作成功
 * @retval DSA_ERROR_NULL_POINTER 元素指针为NULL
 * @retval DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引超出范围
 */
static dsa_result_t doubly_linked_set(dsa_container_pt list, size_t index, dsa_element_pt element) {
    doubly_linked_t *this = list;
    if (!element) {
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
 * @brief 创建新的链表节点
 * @param element 要存储在节点中的元素指针
 * @param out_node 输出参数，用于返回创建的节点指针
 * @return 操作结果
 * @retval DSA_SUCCESS 节点创建成功
 * @retval DSA_ERROR_NULL_POINTER 输出参数为NULL
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 * @details 创建的节点的前后指针都初始化为NULL
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
    new_node->previous = new_node->next = NULL;
    *out_node = new_node;
    return DSA_SUCCESS;
}

/**
 * @brief 销毁链表节点
 * @param node 要销毁的节点指针
 * @return 操作结果
 * @retval DSA_SUCCESS 节点销毁成功
 * @retval DSA_ERROR_NULL_POINTER 节点指针为NULL
 * @details 只释放节点结构体的内存，不处理节点中存储的数据
 */
static dsa_result_t destroy_node(node_t *node) {
    if (!node) {
        return DSA_ERROR_NULL_POINTER;
    }
    free(node);
    return DSA_SUCCESS;
}

/**
 * @brief 将新节点插入到指定节点之后
 * @param node 要插入位置的前一个节点
 * @param new_node 要插入的新节点
 * @return 操作结果
 * @retval DSA_SUCCESS 插入操作成功
 * @retval DSA_ERROR_NULL_POINTER 节点指针为NULL或节点的next指针为NULL
 * @details 更新相关节点的前后指针以维护双向链表的完整性
 */
static dsa_result_t attach_node_after(node_t *node, node_t *new_node) {
    if (!node || !node->next || !new_node) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 设置新节点的指针
    new_node->previous = node;
    new_node->next = node->next;

    // 更新相邻节点的指针
    node->next->previous = new_node;
    node->next = new_node;

    return DSA_SUCCESS;
}

/**
 * @brief 在指定索引位置插入元素
 * @param list 链表容器指针
 * @param index 要插入的索引位置
 * @param element 要插入的元素指针
 * @return 操作结果
 * @retval DSA_SUCCESS 插入操作成功
 * @retval DSA_ERROR_NULL_POINTER 链表、头节点或元素指针为NULL
 * @retval DSA_ERROR_INDEX_OUT_OF_BOUNDS 索引超出范围
 * @retval DSA_ERROR_MEMORY_ALLOCATION 内存分配失败
 * @details 在指定位置创建新节点并插入，链表大小增加1
 */
static dsa_result_t doubly_linked_insert_at(dsa_container_pt list, size_t index, dsa_element_pt element) {
    doubly_linked_t *this = list;
    if (!this || !this->head || !element) {
        return DSA_ERROR_NULL_POINTER;
    }

    node_t *previous_node = find_node_before(this, index);
    if (!previous_node) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    node_t *new_node;
    dsa_result_t result = create_node(element, &new_node);
    if (DSA_SUCCESS != result) {
        return result;
    }

    if (DSA_SUCCESS != attach_node_after(previous_node, new_node)) {
        return destroy_node(new_node);
    }

    this->size++;
    return DSA_SUCCESS;
}

/**
 * @brief 分离指定节点之后的节点
 * @param node 要分离位置的前一个节点
 * @return 被分离的节点指针
 * @retval NULL 节点指针为NULL或没有可分离的节点
 * @retval node_t* 被分离的节点指针
 * @details 更新相关节点的前后指针，但不释放被分离节点的内存
 */
static node_t *detach_node_after(node_t *node) {
    if (!node) {
        return NULL;
    }

    node_t *next_node = node->next;
    if (!next_node || !next_node->next) {
        return NULL;
    }

    // 跳过要分离的节点，直接连接前后节点
    node->next = next_node->next;
    next_node->next->previous = node;

    return next_node;
}

/**
 * @brief 移除指定索引位置的元素
 * @param list 链表容器指针
 * @param index 要移除元素的索引位置
 * @return 被移除的元素指针
 * @retval NULL 链表为NULL或索引超出范围
 * @retval dsa_element_pt 被移除的元素指针
 * @details 移除指定位置的节点，释放节点内存，链表大小减少1
 */
static dsa_element_pt doubly_linked_remove_at(dsa_container_pt list, size_t index) {
    doubly_linked_t *this = list;

    node_t *previous_node = find_node_before(this, index);
    if (!previous_node) {
        return NULL;
    }

    node_t *node_to_remove = detach_node_after(previous_node);
    if (!node_to_remove) {
        return NULL;
    }

    dsa_element_pt element = node_to_remove->data;
    free(node_to_remove);
    this->size--;

    return element;
}

/**
 * @brief 随机访问特征函数表
 * @details 包含链表随机访问操作的函数指针
 */
static trait_random_access_t const random_access_trait = {
    .get_at = doubly_linked_get,
    .set_at = doubly_linked_set,
    .insert_at = doubly_linked_insert_at,
    .remove_at = doubly_linked_remove_at,
};

/**
 * @brief 获取链表类型
 * @return 链表类型枚举值
 * @retval LINKED_LIST_TYPE_DOUBLY 双向链表类型
 */
static dsa_linked_list_type_t doubly_linked_get_type() {
    return LINKED_LIST_TYPE_DOUBLY;
}

/**
 * @brief 获取链表类型名称
 * @return 链表类型的中文名称字符串
 * @retval "双链表(Doubly Linked List)" 双向链表的中文名称
 */
static char const *doubly_linked_get_type_name() {
    return "双链表(Doubly Linked List)";
}

/**
 * @brief 双向链表特征函数表
 * @details 包含双向链表所有操作的函数指针集合
 */
static trait_linked_list_t const doubly_linked_trait = {
    .basic = &basic_trait,
    .random_access = &random_access_trait,
    .get_type = doubly_linked_get_type,
    .get_type_name = doubly_linked_get_type_name,
};

/**
 * @brief 创建新的双向链表
 * @return 新创建的双向链表指针
 * @retval NULL 内存分配失败
 * @retval dsa_linked_list_t* 成功创建的链表指针
 * @details 创建空的双向链表，包含头尾哨兵节点，初始大小为0
 * @note 使用完毕后需要调用相应的销毁函数释放内存
 */
dsa_linked_list_t *doubly_linked_create() {
    // 分配链表结构体内存
    doubly_linked_t *list = malloc(sizeof(doubly_linked_t));
    if (!list) {
        return NULL;
    }

    // 创建头哨兵节点
    node_t *head, *tail;
    if (DSA_SUCCESS != create_node(NULL, &head)) {
        free(list);
        return NULL;
    }

    // 创建尾哨兵节点
    if (DSA_SUCCESS != create_node(NULL, &tail)) {
        destroy_node(head);
        free(list);
        return NULL;
    }

    // 连接头尾哨兵节点
    head->next = tail;
    tail->previous = head;

    // 初始化链表结构
    list->head = head;
    list->tail = tail;
    list->size = 0;
    list->traits = &doubly_linked_trait;

    return (dsa_linked_list_t *) list;
}