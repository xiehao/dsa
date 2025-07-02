/**
 * @file doubly_linked_list.h
 * @brief 双向链表数据结构的头文件
 * @author oaheixiehao
 * @date 2025/7/1
 * @version 1.0
 *
 * 本文件定义了双向链表数据结构的接口函数。
 * 双向链表是一种线性数据结构，其中每个节点包含数据和两个指针，
 * 分别指向前一个节点和后一个节点。
 */

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <ds/linked_list.h>

/**
 * @brief 创建一个新的双向链表
 *
 * 该函数用于创建并初始化一个新的双向链表实例。
 * 新创建的链表为空，不包含任何节点。
 *
 * @return dsa_linked_list_t* 指向新创建的双向链表的指针
 * @retval NULL 如果内存分配失败
 * @retval 非NULL 指向有效双向链表结构的指针
 *
 * @note 调用者有责任在不再需要链表时调用相应的销毁函数来释放内存
 * @warning 返回的指针必须进行NULL检查
 *
 * @par 示例用法:
 * @code
 * dsa_linked_list_t *list = doubly_linked_create();
 * if (list != NULL) {
 *     // 使用链表
 *     // ...
 *     // 记得释放内存
 * }
 * @endcode
 *
 * @see doubly_linked_destroy() 用于销毁链表
 * @see doubly_linked_insert() 用于插入节点
 * @see doubly_linked_delete() 用于删除节点
 */
dsa_linked_list_t *doubly_linked_create();

#endif //DOUBLY_LINKED_LIST_H