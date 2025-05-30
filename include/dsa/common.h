#ifndef DSA_COMMON_H
#define DSA_COMMON_H

#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

// 定义通用的元素指针类型别名
typedef void* dsa_element_pt;

// 宏定义，用于简化获取元素值的操作
// 在调试模式下提供额外的安全检查
#ifdef DEBUG
#define ELEMENT_VALUE(type, ptr) \
    (assert((ptr) != NULL), \
     assert(sizeof(type) > 0), \
     *(type*)(ptr))
#else
#define ELEMENT_VALUE(type, ptr) (*(type*)(ptr))
#endif

#endif // DSA_COMMON_H