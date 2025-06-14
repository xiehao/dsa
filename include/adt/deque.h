#ifndef DSA_DEQUE_H
#define DSA_DEQUE_H

#include <stdbool.h>
#include <stddef.h>

// 双端队列结构的不透明指针类型
typedef struct Deque Deque;

#include "../ds/linked_list.h"

/**
 * @brief 创建一个新的空双端队列。
 *
 * @return 指向新创建的双端队列的指针，如果内存分配失败则返回 NULL。
 */
Deque* deque_create();

/**
 * @brief 销毁双端队列并释放其关联的内存。
 *
 * 如果提供了 `free_data_func`，它将在释放底层存储之前
 * 对每个元素的数据调用。
 *
 * @param deque 指向要销毁的双端队列的指针。
 * @param free_data_func 用于释放每个元素中存储的数据的函数指针。可以为 NULL。
 */
void deque_destroy(Deque* deque, FreeDataFunc free_data_func);

/**
 * @brief 在双端队列的前端添加一个元素。
 *
 * @param deque 指向双端队列的指针。
 * @param data 指向要添加的数据的指针。
 * @return 如果元素添加成功则返回 true，否则返回 false（例如，内存分配失败）。
 */
bool deque_add_first(Deque* deque, void* data);

/**
 * @brief 在双端队列的后端添加一个元素。
 *
 * @param deque 指向双端队列的指针。
 * @param data 指向要添加的数据的指针。
 * @return 如果元素添加成功则返回 true，否则返回 false（例如，内存分配失败）。
 */
bool deque_add_last(Deque* deque, void* data);

/**
 * @brief 从双端队列的前端移除并返回元素。
 *
 * @param deque 指向双端队列的指针。
 * @return 指向被移除元素数据的指针，如果双端队列为空则返回 NULL。
 *         如果需要，调用者负责释放返回的数据。
 */
void* deque_remove_first(Deque* deque);

/**
 * @brief 从双端队列的后端移除并返回元素。
 *
 * @param deque 指向双端队列的指针。
 * @return 指向被移除元素数据的指针，如果双端队列为空则返回 NULL。
 *         如果需要，调用者负责释放返回的数据。
 */
void* deque_remove_last(Deque* deque);

/**
 * @brief 返回双端队列前端的元素但不移除它。
 *
 * @param deque 指向双端队列的指针。
 * @return 指向前端数据的指针，如果双端队列为空则返回 NULL。
 */
void* deque_peek_first(const Deque* deque);

/**
 * @brief 返回双端队列后端的元素但不移除它。
 *
 * @param deque 指向双端队列的指针。
 * @return 指向后端数据的指针，如果双端队列为空则返回 NULL。
 */
void* deque_peek_last(const Deque* deque);

/**
 * @brief 返回双端队列中的元素数量。
 *
 * @param deque 指向双端队列的指针。
 * @return 双端队列中的元素数量。
 */
size_t deque_size(const Deque* deque);

/**
 * @brief 检查双端队列是否为空。
 *
 * @param deque 指向双端队列的指针。
 * @return 如果双端队列为空则返回 true，否则返回 false。
 */
bool deque_is_empty(const Deque* deque);

#endif // DSA_DEQUE_H