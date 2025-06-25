//
// Created by oaheixiehao on 2025/6/23.
//

#ifndef LINKED_LIST_TRAIT_H
#define LINKED_LIST_TRAIT_H

#include <traits.h>
#include <ds/linked_list.h>

typedef struct {
    trait_basic_t const *basic;
    trait_linear_t const *linear;
    trait_random_access_t const *random_access;
    dsa_linked_list_type_t (*get_type)();
    char const *(*get_type_name)();
} trait_linked_list_t;

#endif //LINKED_LIST_TRAIT_H
