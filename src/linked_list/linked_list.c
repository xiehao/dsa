#include "ds/linked_list.h"
#include <stdlib.h>
#include <stdio.h>

#include <common.h>
#include <internal/linked_list_traits.h>
#include <internal/singly_linked.h>

struct linked_list_t {
    trait_linked_list_t const *traits;
};

dsa_linked_list_t *linked_list_create(dsa_linked_list_type_t type) {
    switch (type) {
        case LINKED_LIST_TYPE_SINGLY:
            return singly_linked_create();
        case LINKED_LIST_TYPE_DOUBLY:
        // return NULL;
        case LINKED_LIST_TYPE_CIRCULAR:
            // return NULL;
        default:
            return NULL;
    }
}

size_t linked_list_size(const dsa_linked_list_t *linked_list) {
    return linked_list ? linked_list->traits->basic->get_size(linked_list) : 0;
}

bool linked_list_is_empty(const dsa_linked_list_t *linked_list) {
    return linked_list ? linked_list->traits->basic->is_empty(linked_list) : true;
}

dsa_result_t linked_list_clear(dsa_linked_list_t *linked_list) {
    if (!linked_list) { return DSA_ERROR_NULL_POINTER; }
    return linked_list->traits->basic->clear(linked_list);
}

dsa_result_t linked_list_destroy(dsa_linked_list_t *linked_list) {
    if (!linked_list) { return DSA_ERROR_NULL_POINTER; }
    return linked_list->traits->basic->destroy(linked_list);
}

dsa_element_pt linked_list_get(const dsa_linked_list_t *linked_list, size_t index) {
    if (!linked_list) { return NULL; }
    return linked_list->traits->random_access->get_at(linked_list, index);
}

dsa_result_t linked_list_set(dsa_linked_list_t *linked_list, size_t index, dsa_element_pt data) {
    if (!linked_list) { return DSA_ERROR_NULL_POINTER; }
    return linked_list->traits->random_access->set_at(linked_list, index, data);
}

dsa_result_t linked_list_insert_at(dsa_linked_list_t *linked_list, size_t index, dsa_element_pt data) {
    if (!linked_list) { return DSA_ERROR_NULL_POINTER; }
    return linked_list->traits->random_access->insert_at(linked_list, index, data);
}

dsa_element_pt linked_list_remove_at(dsa_linked_list_t *linked_list, size_t index) {
    if (!linked_list) { return NULL; }
    return linked_list->traits->random_access->remove_at(linked_list, index);
}

// dsa_element_pt linked_list_remove_first(dsa_linked_list_t *list) {
//     if (!list || list->head == NULL) {
//         return NULL; // 列表为空或为 null
//     }
//
//     Node *node_to_remove = list->head;
//     dsa_element_pt data = node_to_remove->data;
//
//     list->head = list->head->next;
//     if (list->head == NULL) {
//         // 列表变为空
//         list->tail = NULL;
//     }
//
//     free(node_to_remove);
//     list->size--;
//     return data;
// }
//
// dsa_element_pt linked_list_remove_last(dsa_linked_list_t *list) {
//     if (!list || list->head == NULL) {
//         return NULL; // 列表为空或为 null
//     }
//
//     dsa_element_pt data;
//     if (list->head == list->tail) {
//         // 只有一个元素
//         data = list->head->data;
//         free(list->head);
//         list->head = NULL;
//         list->tail = NULL;
//     } else {
//         // 不止一个元素
//         Node *current = list->head;
//         while (current->next != list->tail) {
//             current = current->next;
//         }
//         data = list->tail->data;
//         free(list->tail);
//         list->tail = current;
//         list->tail->next = NULL;
//     }
//
//     list->size--;
//     return data;
// }
