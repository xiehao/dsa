//
// Created by oaheixiehao on 2025/6/23.
//
#include <internal/singly_linked.h>
#include <internal/linked_list_traits.h>

typedef struct node_t {
    struct node_t *next;
    dsa_element_pt data;
} node_t;

typedef struct {
    trait_linked_list_t const *traits;
    node_t *head;
    size_t size;
} singly_linked_t;

// 查询类函数：返回数据，特殊值表示错误
static size_t singly_linked_size(dsa_const_container_pt list) {
    singly_linked_t const *this = list;
    return this ? this->size : SIZE_MAX; // SIZE_MAX表示错误
}

static bool singly_linked_is_empty(dsa_const_container_pt list) {
    singly_linked_t const *this = list;
    if (!this) return true; // NULL指针视为空
    return this->size == 0;
}

// 修改类函数：返回错误码
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

// 析构类函数：返回错误码
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

static trait_basic_t const basic_trait = {
    .get_size = singly_linked_size,
    .is_empty = singly_linked_is_empty,
    .clear = singly_linked_clear,
    .destroy = singly_linked_destroy,
};

// 内部辅助函数：根据索引查找前一个节点（用于插入和删除操作）
// 内部函数保持返回指针的方式
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

// 查询类函数：返回数据，NULL表示错误
static dsa_element_pt singly_linked_get(dsa_const_container_pt list, size_t index) {
    singly_linked_t const *this = list;
    if (!this || index >= this->size) {
        return NULL;
    }
    
    node_t const *node = find_node_before(this, index + 1);
    return node ? node->data : NULL;
}

// 修改类函数：返回错误码
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

// 内部辅助函数：创建新节点 - 保持当前风格
static dsa_result_t create_node(dsa_element_pt element, node_t **out_node) {
    if (!out_node) {
        return DSA_ERROR_NULL_POINTER;
    }

    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    if (!new_node) {
        return DSA_ERROR_MEMORY_ALLOCATION;
    }

    new_node->data = element;
    new_node->next = NULL;
    *out_node = new_node;

    return DSA_SUCCESS;
}

// 内部辅助函数 - 保持返回错误码风格
static dsa_result_t attach_node_after(node_t *node, node_t *new_node) {
    if (!node || !new_node) { 
        return DSA_ERROR_NULL_POINTER; 
    }
    new_node->next = node->next;
    node->next = new_node;
    return DSA_SUCCESS;
}

// 修改类函数：返回错误码
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

// 内部辅助函数 - 返回指针
static node_t *detach_node_after(node_t *node) {
    if (!node || !node->next) { 
        return NULL; 
    }
    node_t *next = node->next;
    node->next = next->next;
    return next;
}

// 查询类函数：返回数据，NULL表示错误或空
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

static trait_random_access_t const random_access_trait = {
    .get_at = singly_linked_get,
    .set_at = singly_linked_set,
    .insert_at = singly_linked_insert_at,
    .remove_at = singly_linked_remove_at,
};

// 查询类函数：返回数据
static dsa_linked_list_type_t singly_linked_get_type(void) {
    return LINKED_LIST_TYPE_SINGLY;
}

// 查询类函数：返回数据
static char const *singly_linked_get_type_name(void) {
    return "单链表(Singly Linked List)";
}

static trait_linked_list_t const linked_list_trait = {
    .basic = &basic_trait,
    .random_access = &random_access_trait,
    .get_type = singly_linked_get_type,
    .get_type_name = singly_linked_get_type_name,
};

// 创建类函数：返回对象指针，NULL表示失败
dsa_linked_list_t *singly_linked_create() {
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