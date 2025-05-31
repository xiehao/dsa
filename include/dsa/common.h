#ifndef DSA_COMMON_H
#define DSA_COMMON_H

#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>  // 用于 uintptr_t
#include <stdio.h>   // 用于 fprintf
#include <stdlib.h>  // 用于 abort

// 定义通用的元素指针类型别名
typedef void* dsa_element_pt;

// 宏定义，用于简化获取元素值的操作
// 提供全面的安全检查，优先考虑教育代码库的安全性
#ifdef DEBUG
#define ELEMENT_VALUE(type, ptr) \
    (assert((ptr) != NULL), \
     assert(sizeof(type) > 0), \
     assert(((uintptr_t)(ptr) % _Alignof(type)) == 0), \
     *(type*)(ptr))
#else
// 即使在非DEBUG模式下也进行基本的安全检查
#define ELEMENT_VALUE(type, ptr) \
    ((ptr) != NULL ? *(type*)(ptr) : \
     (fprintf(stderr, "错误：ELEMENT_VALUE宏接收到NULL指针\n"), \
      abort(), *(type*)(ptr)))
#endif

// 提供更安全的替代接口，用于需要额外验证的场景
#define SAFE_ELEMENT_VALUE(type, ptr, default_value) \
    ((ptr) != NULL && ((uintptr_t)(ptr) % _Alignof(type)) == 0 ? \
     *(type*)(ptr) : (default_value))

#endif // DSA_COMMON_H