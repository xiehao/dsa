#include <adt/deque.h>
#include <traits.h>
#include <stdlib.h>
#include <internal/deque_traits.h>
#include <internal/circular_array_deque.h>
#include <internal/doubly_linked_deque.h>

struct deque_t {
    trait_deque_t const *traits;
};

dsa_deque_t *deque_create(dsa_deque_type_t type) {
    switch (type) {
        case DEQUE_TYPE_CIRCULAR_ARRAY_LIST:
            return circular_array_deque_create();
        case DEQUE_TYPE_DOUBLY_LINKED_LIST:
            return doubly_linked_deque_create();
        default:
            return NULL;
    }
}

size_t deque_size(dsa_deque_t const *deque) {
    return deque ? deque->traits->basic->get_size(deque) : 0;
}

bool deque_is_empty(dsa_deque_t const *deque) {
    return deque ? deque->traits->basic->is_empty(deque) : true;
}

dsa_result_t deque_clear(dsa_deque_t *deque) {
    return deque ? deque->traits->basic->clear(deque) : DSA_ERROR_NULL_POINTER;
}

dsa_result_t deque_destroy(dsa_deque_t *deque) {
    return deque ? deque->traits->basic->destroy(deque) : DSA_ERROR_NULL_POINTER;
}

dsa_result_t deque_add_first(dsa_deque_t *deque, dsa_element_pt element) {
    return deque ? deque->traits->linear->push_front(deque, element) : DSA_ERROR_NULL_POINTER;
}

dsa_result_t deque_add_last(dsa_deque_t *deque, dsa_element_pt element) {
    return deque ? deque->traits->linear->push_back(deque, element) : DSA_ERROR_NULL_POINTER;
}

dsa_element_pt deque_remove_first(dsa_deque_t *deque) {
    return deque ? deque->traits->linear->pop_front(deque) : NULL;
}

dsa_element_pt deque_remove_last(dsa_deque_t *deque) {
    return deque ? deque->traits->linear->pop_back(deque) : NULL;
}

dsa_element_pt deque_peek_first(dsa_deque_t const *deque) {
    return deque ? deque->traits->linear->peek_front(deque) : NULL;
}

dsa_element_pt deque_peek_last(dsa_deque_t const *deque) {
    return deque ? deque->traits->linear->peek_back(deque) : NULL;
}

dsa_deque_type_t deque_get_type(dsa_deque_t const *deque) {
    return deque ? deque->traits->get_type() : DEQUE_TYPE_CIRCULAR_ARRAY_LIST;
}

char const *deque_get_type_name(dsa_deque_t const *deque) {
    return deque ? deque->traits->get_type_name() : "循环双端队列(Circular Array List)";
}