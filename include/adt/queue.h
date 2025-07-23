//
// Created by oaheixiehao on 2025/7/22.
//

#ifndef DSA_QUEUE_H
#define DSA_QUEUE_H

#include <common.h>

typedef struct queue_t dsa_queue_t;

typedef enum {
    QUEUE_TYPE_CIRCULAR_ARRAY_LIST,
    QUEUE_TYPE_DOUBLY_LINKED_LIST,
} dsa_queue_type_t;

dsa_queue_t *queue_create(dsa_queue_type_t queue_type);

size_t queue_size(dsa_queue_t const *queue);

bool queue_is_empty(dsa_queue_t const *queue);

dsa_result_t queue_clear(dsa_queue_t *queue);

dsa_result_t queue_destroy(dsa_queue_t *queue);

dsa_result_t queue_enqueue(dsa_queue_t *queue, dsa_element_pt element);

dsa_element_pt queue_dequeue(dsa_queue_t *queue);

dsa_queue_type_t queue_get_type(dsa_queue_t const *queue);

char const *queue_get_type_name(dsa_queue_t const *queue);

#endif // DSA_QUEUE_H
