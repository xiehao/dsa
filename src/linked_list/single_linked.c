//
// Created by oaheixiehao on 2025/6/23.
//

#include <internal/single_linked.h>
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

static size_t singly_linked_size(dsa_const_container_pt list) {
    singly_linked_t const *this = list;
    return this ? this->size : 0;
}

static bool singly_linked_is_empty(dsa_const_container_pt list) {
    singly_linked_t const *this = list;
    return this ? this->size == 0 : true;
}

static void singly_linked_clear(dsa_container_pt list) {
    singly_linked_t *this = list;
    if (this) {
        node_t *current = this->head;
        while (current) {
            node_t *next = current->next;
            free(current);
            current = next;
        }
        this->head = NULL;
        this->size = 0; // 重置大小计数器
    }
}

static void singly_linked_destroy(dsa_container_pt list) {
    singly_linked_clear(list);
    free(list);
}

static trait_basic_t const basic_trait = {
    .get_size = singly_linked_size,
    .is_empty = singly_linked_is_empty,
    .clear = singly_linked_clear,
    .destroy = singly_linked_destroy,
};

// 内部辅助函数：根据索引查找节点
static node_t *find_node_at(singly_linked_t *this, size_t index) {
    if (!this || index >= this->size) {
        return NULL;
    }

    node_t *current = this->head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current;
}

static dsa_element_pt singly_linked_get(dsa_const_container_pt list, size_t index) {
    singly_linked_t const *this = list;
    node_t *node = find_node_at(this, index);
    return node ? node->data : NULL;
}

static dsa_result_t singly_linked_set(dsa_container_pt list, size_t index, dsa_element_pt element) {
    singly_linked_t *this = list;
    if (!this || !element) {
        return DSA_ERROR_NULL_POINTER;
    }

    node_t *node = find_node_at(this, index);
    if (!node) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS;
    }

    node->data = element;
    return DSA_SUCCESS;
}

// 内部辅助函数：创建新节点
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

static dsa_result_t attach_node_after(node_t *node, node_t *new_node) {
    if (!node || !new_node) { return DSA_ERROR_NULL_POINTER; }
    new_node->next = node->next;
    node->next = new_node;
    return DSA_SUCCESS;
}

static dsa_result_t singly_linked_insert_after(dsa_container_pt list, size_t index, dsa_element_pt element) {
    singly_linked_t *this = list;
    if (!this || !element) {
        return DSA_ERROR_NULL_POINTER;
    }
    node_t *node = find_node_at(this, index);
    if (!node) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS;
    }
    node_t *new_node;
    dsa_result_t result = create_node(element, &new_node);
    if (DSA_SUCCESS != result) { return result; }
    result = attach_node_after(node, new_node);
    if (DSA_SUCCESS != result) { return result; }
    this->size++;
    return DSA_SUCCESS;
}

static node_t *detach_node_after(node_t *node) {
    if (!node || !node->next) { return NULL; }
    node_t *next = node->next;
    node->next = next->next;
    return next;
}

static dsa_element_pt singly_linked_remove_at(dsa_container_pt list, size_t index) {
    singly_linked_t *this = list;
    if (!this) {
        return NULL;
    }
    node_t *node = find_node_at(this, index - 1);
    if (!node || !node->next) {
        return NULL;
    }
    node_t *next = detach_node_after(node);
    dsa_element_pt data = next->data;
    free(next);
    this->size--;
    return data;
}

static trait_random_access_t const random_access_trait = {
    .get_at = singly_linked_get,
    .set_at = singly_linked_set,
    .insert_at = singly_linked_insert_after,
    .remove_at = singly_linked_remove_at,
};

static dsa_linked_list_type_t singly_linked_get_type(void) {
    return LINKED_LIST_TYPE_SINGLY;
}

static char const *singly_linked_get_type_name(void) {
    return "单链表(Singly Linked List)";
}

static trait_linked_list_t const linked_list_trait = {
    .basic = &basic_trait,
    .random_access = &random_access_trait,
    .get_type = singly_linked_get_type,
    .get_type_name = singly_linked_get_type_name,
};

dsa_linked_list_t *singly_linked_create() {
    singly_linked_t *list = malloc(sizeof(singly_linked_t));
    if (!list) { return NULL; }
    node_t *head = malloc(sizeof(node_t));
    if (!head || DSA_SUCCESS != create_node(NULL, &head)) {
        free(list);
        return NULL;
    }
    list->head = head;
    list->size = 0;
    list->traits = &linked_list_trait;
    return (dsa_linked_list_t *) list;
}
