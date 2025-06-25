#include "ds/linked_list.h"
#include <stdlib.h>
#include <stdio.h>

#include <common.h>
#include <internal/linked_list_traits.h>

struct linked_list_t {
    trait_linked_list_t const *traits;
};

// 定义链表节点的结构（内部）
typedef struct Node {
    dsa_element_pt data;       // 指向节点存储数据的指针
    struct Node* next; // 指向列表中下一个节点的指针
} Node;

// 定义链表的结构（实现细节）
// struct linked_list_t {
//     Node* head;     // 指向列表中第一个节点的指针
//     Node* tail;     // 指向列表中最后一个节点的指针
//     size_t size;    // 当前元素数量
// };

// 内部辅助函数，用于创建新节点
static Node* create_node(dsa_element_pt data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        perror("为新列表节点分配内存失败");
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

dsa_linked_list_t* linked_list_create() {
    dsa_linked_list_t* list = (dsa_linked_list_t*)malloc(sizeof(dsa_linked_list_t));
    if (!list) {
        perror("为 linked_list_t 结构分配内存失败");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void linked_list_destroy(dsa_linked_list_t* list, FreeDataFunc free_data_func) {
    if (!list) {
        return;
    }
    Node* current = list->head;
    Node* next_node;
    while (current != NULL) {
        next_node = current->next;
        if (free_data_func) {
            free_data_func(current->data);
        }
        free(current);
        current = next_node;
    }
    free(list);
}

bool linked_list_add_first(dsa_linked_list_t* list, dsa_element_pt data) {
    if (!list) {
        return false;
    }
    Node* new_node = create_node(data);
    if (!new_node) {
        return false; // 节点创建失败
    }

    if (list->head == NULL) { // 列表为空
        list->head = new_node;
        list->tail = new_node;
    } else { // 列表不为空
        new_node->next = list->head;
        list->head = new_node;
    }
    list->size++;
    return true;
}

bool linked_list_add_last(dsa_linked_list_t* list, dsa_element_pt data) {
    if (!list) {
        return false;
    }
    Node* new_node = create_node(data);
    if (!new_node) {
        return false; // 节点创建失败
    }

    if (list->head == NULL) { // 列表为空
        list->head = new_node;
        list->tail = new_node;
    } else { // 列表不为空
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    return true;
}

bool linked_list_insert(dsa_linked_list_t* list, size_t index, dsa_element_pt data) {
    if (!list || index > list->size) {
        fprintf(stderr, "错误：索引 %zu 超出链表大小 %zu 的范围。\n", index, list->size);
        return false; // 索引越界（允许 index == size 用于追加）
    }

    if (index == 0) {
        return linked_list_add_first(list, data);
    }
    if (index == list->size) {
        return linked_list_add_last(list, data);
    }

    Node* new_node = create_node(data);
    if (!new_node) {
        return false; // 节点创建失败
    }

    Node* current = list->head;
    for (size_t i = 0; i < index - 1; ++i) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
    list->size++;
    return true;
}

dsa_element_pt linked_list_remove_first(dsa_linked_list_t* list) {
    if (!list || list->head == NULL) {
        return NULL; // 列表为空或为 null
    }

    Node* node_to_remove = list->head;
    dsa_element_pt data = node_to_remove->data;

    list->head = list->head->next;
    if (list->head == NULL) { // 列表变为空
        list->tail = NULL;
    }

    free(node_to_remove);
    list->size--;
    return data;
}

dsa_element_pt linked_list_remove_last(dsa_linked_list_t* list) {
     if (!list || list->head == NULL) {
        return NULL; // 列表为空或为 null
    }

    dsa_element_pt data;
    if (list->head == list->tail) { // 只有一个元素
        data = list->head->data;
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
    } else { // 不止一个元素
        Node* current = list->head;
        while (current->next != list->tail) {
            current = current->next;
        }
        data = list->tail->data;
        free(list->tail);
        list->tail = current;
        list->tail->next = NULL;
    }

    list->size--;
    return data;
}

dsa_element_pt linked_list_remove(dsa_linked_list_t* list, size_t index) {
    if (!list || index >= list->size) {
         if(list) {
            fprintf(stderr, "错误：索引 %zu 超出链表大小 %zu 的范围。\n", index, list->size);
         }
        return NULL; // 索引越界
    }

    if (index == 0) {
        return linked_list_remove_first(list);
    }
    if (index == list->size - 1) {
        return linked_list_remove_last(list);
    }

    Node* current = list->head;
    for (size_t i = 0; i < index - 1; ++i) {
        current = current->next;
    }

    Node* node_to_remove = current->next;
    dsa_element_pt data = node_to_remove->data;
    current->next = node_to_remove->next;

    free(node_to_remove);
    list->size--;
    return data;
}

dsa_element_pt linked_list_get(const dsa_linked_list_t* list, size_t index) {
    if (!list || index >= list->size) {
         if(list) {
             fprintf(stderr, "错误：索引 %zu 超出链表大小 %zu 的范围。\n", index, list->size);
         }
        return NULL; // 索引越界或列表为 null
    }

    Node* current = list->head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

bool linked_list_set(dsa_linked_list_t* list, size_t index, dsa_element_pt data) {
    if (!list || index >= list->size) {
        if(list) {
            fprintf(stderr, "错误：索引 %zu 超出链表大小 %zu 的范围。\n", index, list->size);
        }
        return false; // 索引越界或列表为 null
    }

    Node* current = list->head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    // 注意：此函数不会释放旧数据。
    current->data = data;
    return true;
}

size_t linked_list_size(const dsa_linked_list_t* list) {
    return list ? list->size : 0;
}

bool linked_list_is_empty(const dsa_linked_list_t* list) {
    return list ? (list->size == 0) : true;
}