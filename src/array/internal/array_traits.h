#ifndef DSA_ARRAY_TRAITS_H
#define DSA_ARRAY_TRAITS_H

#include <traits.h>
#include <ds/array.h>

/**
 * @brief 数组基类操作函数表
 */
typedef struct {
    dsa_array_type_t (*get_type)();
    char const *(*get_type_name)();
} array_interface_t;

#endif // DSA_ARRAY_TRAITS_H
