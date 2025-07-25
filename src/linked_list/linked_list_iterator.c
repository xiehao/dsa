/**
 * @file linked_list_iterator.c
 * @brief 链表迭代器的实现
 * @details 提供了链表的迭代器功能，支持单链表、双链表和循环链表
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <a/iterator.h>
#include <ds/linked_list_iterator.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <../iterator/internal/iterator_internal.h>
#include <internal/linked_list_iterator_internal.h>
#include <internal/singly_linked_list.h>
#include <internal/doubly_linked_list.h>
#include <internal/circular_linked_list.h>

/**
 * @brief 将迭代器向前移动n步
 * @param iterator 当前迭代器
 * @param n 移动的步数
 * @return 指向移动后位置的迭代器，如果移动超出范围则返回等同于end()的迭代器
 * @note 对于链表，这是O(n)操作
 */
static dsa_iterator_pt linked_list_iterator_next_n(dsa_iterator_pt iterator, size_t n) {
    if (!iterator) {
        return NULL;
    }

    // 如果n为0，不移动
    if (n == 0) {
        return iterator;
    }

    linked_list_iterator_t *iter = (linked_list_iterator_t *)iterator;

    // 对于链表，我们需要逐步移动n次
    for (size_t i = 0; i < n; i++) {
        // 检查是否已经到达末尾
        if (iter->current_node == iter->end_marker) {
            break;
        }

        switch (iter->iterator_type) {
            case LINKED_LIST_ITERATOR_TYPE_SINGLY: {
                singly_node_t *node = (singly_node_t *)iter->current_node;
                if (node) {
                    singly_linked_t *list = (singly_linked_t *)iter->container;
                    // 检查下一个节点是否是哨兵节点（头节点）
                    if (node->next == list->head) {
                        // 到达末尾，设置为end标记
                        iter->current_node = iter->end_marker;
                    } else {
                        iter->current_node = node->next;
                    }
                }
                break;
            }
            case LINKED_LIST_ITERATOR_TYPE_DOUBLY: {
                doubly_node_t *node = (doubly_node_t *)iter->current_node;
                if (node) {
                    doubly_linked_t *list = (doubly_linked_t *)iter->container;
                    // 检查下一个节点是否是尾哨兵节点
                    if (node->next == list->tail) {
                        // 到达末尾，设置为end标记
                        iter->current_node = iter->end_marker;
                    } else {
                        iter->current_node = node->next;
                    }
                }
                break;
            }
            case LINKED_LIST_ITERATOR_TYPE_CIRCULAR: {
                circular_node_t *node = (circular_node_t *)iter->current_node;
                if (node) {
                    circular_linked_t *list = (circular_linked_t *)iter->container;
                    // 检查下一个节点是否回到头节点
                    if (node->next == list->head) {
                        // 到达末尾，设置为end标记
                        iter->current_node = iter->end_marker;
                    } else {
                        iter->current_node = node->next;
                    }
                }
                break;
            }
        }
    }

    return iterator;
}

/**
 * @brief 将迭代器向后移动n步
 * @param iterator 当前迭代器
 * @param n 移动的步数
 * @return 指向移动后位置的迭代器，如果移动超出范围则返回NULL
 * @note 单链表和循环链表不支持反向遍历，对于双链表这是O(n)操作
 */
static dsa_iterator_pt linked_list_iterator_prev_n(dsa_iterator_pt iterator, size_t n) {
    if (!iterator) {
        return NULL;
    }

    // 如果n为0，不移动
    if (n == 0) {
        return iterator;
    }

    linked_list_iterator_t *iter = (linked_list_iterator_t *)iterator;

    switch (iter->iterator_type) {
        case LINKED_LIST_ITERATOR_TYPE_SINGLY:
        case LINKED_LIST_ITERATOR_TYPE_CIRCULAR:
            // 单链表和循环链表不支持反向遍历
            return NULL;

        case LINKED_LIST_ITERATOR_TYPE_DOUBLY: {
            // 对于双链表，逐步向后移动n次
            for (size_t i = 0; i < n; i++) {
                doubly_node_t *node = (doubly_node_t *)iter->current_node;
                if (!node || !node->previous) {
                    // 已经到达开头或无效状态
                    iter->current_node = NULL;
                    break;
                }

                // 检查是否到达头哨兵节点
                doubly_linked_t *list = (doubly_linked_t *)iter->container;
                if (node->previous == list->head) {
                    // 到达开头，设置为无效状态
                    iter->current_node = NULL;
                    break;
                } else {
                    iter->current_node = node->previous;
                }
            }
            break;
        }
    }

    return iterator;
}

/**
 * @brief 获取迭代器当前指向的元素
 * @param iterator 当前迭代器
 * @return 当前元素的指针，如果迭代器无效则返回NULL
 */
static dsa_element_pt linked_list_iterator_get_value(dsa_iterator_pt iterator) {
    if (!iterator) {
        return NULL;
    }
    
    linked_list_iterator_t *iter = (linked_list_iterator_t *)iterator;
    
    // 检查迭代器是否指向有效位置
    if (!iter->current_node || iter->current_node == iter->end_marker) {
        return NULL;
    }
    
    switch (iter->iterator_type) {
        case LINKED_LIST_ITERATOR_TYPE_SINGLY: {
            singly_node_t *node = (singly_node_t *)iter->current_node;
            return node->data;
        }
        case LINKED_LIST_ITERATOR_TYPE_DOUBLY: {
            doubly_node_t *node = (doubly_node_t *)iter->current_node;
            return node->data;
        }
        case LINKED_LIST_ITERATOR_TYPE_CIRCULAR: {
            circular_node_t *node = (circular_node_t *)iter->current_node;
            return node->data;
        }
    }
    
    return NULL;
}

/**
 * @brief 设置迭代器当前指向的元素值
 * @param iterator 当前迭代器
 * @param value 要设置的新值指针
 * @return 操作结果，成功返回DSA_SUCCESS
 */
static dsa_result_t linked_list_iterator_set_value(dsa_iterator_pt iterator, dsa_element_pt value) {
    if (!iterator || !value) {
        return DSA_ERROR_INVALID_PARAMETER;
    }
    
    linked_list_iterator_t *iter = (linked_list_iterator_t *)iterator;
    
    // 检查迭代器是否指向有效位置
    if (!iter->current_node || iter->current_node == iter->end_marker) {
        return DSA_ERROR_INDEX_OUT_OF_BOUNDS;
    }
    
    switch (iter->iterator_type) {
        case LINKED_LIST_ITERATOR_TYPE_SINGLY: {
            singly_node_t *node = (singly_node_t *)iter->current_node;
            node->data = value;
            return DSA_SUCCESS;
        }
        case LINKED_LIST_ITERATOR_TYPE_DOUBLY: {
            doubly_node_t *node = (doubly_node_t *)iter->current_node;
            node->data = value;
            return DSA_SUCCESS;
        }
        case LINKED_LIST_ITERATOR_TYPE_CIRCULAR: {
            circular_node_t *node = (circular_node_t *)iter->current_node;
            node->data = value;
            return DSA_SUCCESS;
        }
    }
    
    return DSA_ERROR_INVALID_PARAMETER;
}

/**
 * @brief 检查迭代器是否有效
 * @param iterator 要检查的迭代器
 * @return 如果迭代器有效返回true，否则返回false
 */
static bool linked_list_iterator_is_valid(dsa_iterator_pt iterator) {
    if (!iterator) {
        return false;
    }
    
    linked_list_iterator_t *iter = (linked_list_iterator_t *)iterator;
    return iter->current_node != NULL && iter->current_node != iter->end_marker;
}

/**
 * @brief 链表迭代器接口实现
 */
static trait_iterator_t const linked_list_iterator_trait = {
    .begin = NULL,  // 不再需要，因为begin/end函数直接实现
    .end = NULL,    // 不再需要，因为begin/end函数直接实现
    .next_n = linked_list_iterator_next_n,
    .prev_n = linked_list_iterator_prev_n,
    .get_value = linked_list_iterator_get_value,
    .set_value = linked_list_iterator_set_value,
    .is_valid = linked_list_iterator_is_valid,
};

/**
 * @brief 创建链表迭代器的通用函数
 * @param linked_list 链表指针
 * @param current_node 当前节点指针
 * @param end_marker 结束标记
 * @param iterator_type 迭代器类型
 * @return 新创建的迭代器指针，失败时返回NULL
 */
static dsa_iterator_t *create_linked_list_iterator(const dsa_linked_list_t *linked_list,
                                                   void *current_node,
                                                   void *end_marker,
                                                   dsa_linked_list_iterator_type_t iterator_type) {
    if (!linked_list) {
        return NULL;
    }

    // 分配迭代器内存
    linked_list_iterator_t *iter = malloc(sizeof(linked_list_iterator_t));
    if (!iter) {
        return NULL;
    }

    // 初始化迭代器
    iter->trait = &linked_list_iterator_trait;
    iter->container = linked_list;
    iter->current_node = current_node;
    iter->end_marker = end_marker;
    iter->iterator_type = iterator_type;
    iter->version = 0;  // 版本控制暂时不实现

    return (dsa_iterator_t *)iter;
}

/**
 * @brief 创建指向链表开始位置的迭代器
 */
dsa_iterator_t *linked_list_begin(const dsa_linked_list_t *linked_list) {
    if (!linked_list) {
        return NULL;
    }

    // 获取链表类型
    dsa_linked_list_type_t type = linked_list_get_type(linked_list);

    switch (type) {
        case LINKED_LIST_TYPE_SINGLY: {
            singly_linked_t *list = (singly_linked_t *)linked_list;
            if (list->size == 0) {
                // 空链表，返回end迭代器
                return create_linked_list_iterator(linked_list, list->head, list->head,
                                                   LINKED_LIST_ITERATOR_TYPE_SINGLY);
            }
            // 第一个数据节点是head->next
            return create_linked_list_iterator(linked_list, list->head->next, list->head,
                                               LINKED_LIST_ITERATOR_TYPE_SINGLY);
        }
        case LINKED_LIST_TYPE_DOUBLY: {
            doubly_linked_t *list = (doubly_linked_t *)linked_list;
            if (list->size == 0) {
                // 空链表，返回end迭代器
                return create_linked_list_iterator(linked_list, list->tail, list->tail,
                                                   LINKED_LIST_ITERATOR_TYPE_DOUBLY);
            }
            // 第一个数据节点是head->next
            return create_linked_list_iterator(linked_list, list->head->next, list->tail,
                                               LINKED_LIST_ITERATOR_TYPE_DOUBLY);
        }
        case LINKED_LIST_TYPE_CIRCULAR: {
            circular_linked_t *list = (circular_linked_t *)linked_list;
            if (list->size == 0) {
                // 空链表，返回end迭代器
                return create_linked_list_iterator(linked_list, list->head, list->head,
                                                   LINKED_LIST_ITERATOR_TYPE_CIRCULAR);
            }
            // 第一个数据节点是head->next
            return create_linked_list_iterator(linked_list, list->head->next, list->head,
                                               LINKED_LIST_ITERATOR_TYPE_CIRCULAR);
        }
        default:
            return NULL;
    }
}

/**
 * @brief 创建指向链表结束位置的迭代器
 */
dsa_iterator_t *linked_list_end(const dsa_linked_list_t *linked_list) {
    if (!linked_list) {
        return NULL;
    }

    // 获取链表类型
    dsa_linked_list_type_t type = linked_list_get_type(linked_list);

    switch (type) {
        case LINKED_LIST_TYPE_SINGLY: {
            singly_linked_t *list = (singly_linked_t *)linked_list;
            return create_linked_list_iterator(linked_list, list->head, list->head,
                                               LINKED_LIST_ITERATOR_TYPE_SINGLY);
        }
        case LINKED_LIST_TYPE_DOUBLY: {
            doubly_linked_t *list = (doubly_linked_t *)linked_list;
            return create_linked_list_iterator(linked_list, list->tail, list->tail,
                                               LINKED_LIST_ITERATOR_TYPE_DOUBLY);
        }
        case LINKED_LIST_TYPE_CIRCULAR: {
            circular_linked_t *list = (circular_linked_t *)linked_list;
            return create_linked_list_iterator(linked_list, list->head, list->head,
                                               LINKED_LIST_ITERATOR_TYPE_CIRCULAR);
        }
        default:
            return NULL;
    }
}
