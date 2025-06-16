#ifndef DSA_STATIC_ARRAY_H
#define DSA_STATIC_ARRAY_H

#include <stddef.h>
#include <ds/array.h>

dsa_array_t *static_array_create(void *data_buffer, size_t capacity, size_t element_size);

#endif // DSA_STATIC_ARRAY_H