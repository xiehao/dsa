//
// Created by oaheixiehao on 2025/7/6.
//

#ifndef DSA_DEQUE_TRAITS_H
#define DSA_DEQUE_TRAITS_H

#include <traits.h>
#include <adt/deque.h>

typedef struct {
    trait_basic_t const *basic;
    trait_linear_t const *linear;
    dsa_deque_type_t (*get_type)(void);
    char const *(*get_type_name)(void);
} trait_deque_t;

#endif // DSA_DEQUE_TRAITS_H
