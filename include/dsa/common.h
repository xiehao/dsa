#ifndef DSA_COMMON_H
#define DSA_COMMON_H

#include <stddef.h>
#include <stdbool.h>

// 定义通用的元素指针类型别名
typedef void* ElementPtr;

// 宏定义，用于简化获取元素值的操作
#define ELEMENT_VALUE(type, ptr) (*(type *)(ptr))

#endif // DSA_COMMON_H