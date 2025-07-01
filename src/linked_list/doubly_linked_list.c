//

#include <internal/linked_list_traits.h>
#include <internal/doubly_linked_list.h>

typedef struct node_t {
    struct node_t *previous;
    struct node_t *next;
    dsa_element_pt data;
} node_t;

typedef struct {
    trait_linked_list_t const *traits;
    node_t *head;
    node_t *tail;
    size_t size;
} doubly_linked_t;

static size_t doubly_linked_size(dsa_const_container_pt list) {
    doubly_linked_t const *this = list;
    return this ? this->size : SIZE_MAX;
}

static bool doubly_linked_is_empty(dsa_const_container_pt list) {
    doubly_linked_t const *this = list;
    if (!this) return true;
    return this->size == 0;
}

static dsa_result_t doubly_linked_clear(dsa_container_pt list) {
    doubly_linked_t *this = list;
    if (!this || !this->head) {
        return DSA_ERROR_NULL_POINTER;
    }
    node_t *current = this->head->next;
    while (current != this->tail) {
        node_t *next = current->next;
        free(current);
        current = next;
    }
    this->head->next = this->tail;
    this->tail->previous = this->head;
    this->size = 0;
    return DSA_SUCCESS;
}

static dsa_result_t doubly_linked_destroy(dsa_container_pt list) {
    doubly_linked_t *this = list;
    if (!this) {
        return DSA_ERROR_NULL_POINTER;
    }
    dsa_result_t result = doubly_linked_clear(list);
    if (this->head) {
        free(this->head);
    }
    if (this->tail) {
        free(this->tail);
    }
    free(this);
    return result;
}

static trait_basic_t const basic_trait = {
    .get_size = doubly_linked_size,
    .is_empty = doubly_linked_is_empty,
    .clear = doubly_linked_clear,
    .destroy = doubly_linked_destroy,
};

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

static dsa_element_pt doubly_linked_get(dsa_const_container_pt list, size_t index) {
    doubly_linked_t const *this = list;
    node_t const *node = find_node_before(this, index + 1);
    return node ? node->data : NULL;
}

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

static dsa_result_t destroy_node(node_t *node) {
    if (!node) {
        return DSA_ERROR_NULL_POINTER;
    }
    free(node);
    return DSA_SUCCESS;
}

static dsa_result_t attach_node_after(node_t *node, node_t *new_node) {
    if (!node || !node->next || !new_node) {
        return DSA_ERROR_NULL_POINTER;
    }
    new_node->previous = node;
    new_node->next = node->next;
    node->next->previous = new_node;
    node->next = new_node;
    return DSA_SUCCESS;
}

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

static node_t *detach_node_after(node_t *node) {
    if (!node) {
        return NULL;
    }
    node_t *next_node = node->next;
    if (!next_node || !next_node->next) {
        return NULL;
    }
    node->next = next_node->next;
    next_node->next->previous = node;
    return next_node;
}

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

static trait_random_access_t const random_access_trait = {
    .get_at = doubly_linked_get,
    .set_at = doubly_linked_set,
    .insert_at = doubly_linked_insert_at,
    .remove_at = doubly_linked_remove_at,
};

static dsa_linked_list_type_t doubly_linked_get_type() {
    return LINKED_LIST_TYPE_DOUBLY;
}

static char const *doubly_linked_get_type_name() {
    return "双链表(Doubly Linked List)";
}

static trait_linked_list_t const doubly_linked_trait = {
    .basic = &basic_trait,
    .random_access = &random_access_trait,
    .get_type = doubly_linked_get_type,
    .get_type_name = doubly_linked_get_type_name,
};

dsa_linked_list_t *doubly_linked_create() {
    doubly_linked_t *list = malloc(sizeof(doubly_linked_t));
    if (!list) {
        return NULL;
    }
    node_t *head, *tail;
    if (DSA_SUCCESS != create_node(NULL, &head)) {
        free(list);
        return NULL;
    }
    if (DSA_SUCCESS != create_node(NULL, &tail)) {
        destroy_node(head);
        free(list);
        return NULL;
    }
    head->next = tail;
    tail->previous = head;
    list->head = head;
    list->tail = tail;
    list->size = 0;
    list->traits = &doubly_linked_trait;
    return (dsa_linked_list_t *) list;
}
