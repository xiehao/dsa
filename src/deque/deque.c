#include "dsa/deque.h"
#include <stdlib.h>

// 定义双端队列的底层结构（例如，使用链表或动态数组）
// 对于此实现，我们在内部使用双向链表。

typedef struct DequeNode {
    void* data;
    struct DequeNode* prev;
    struct DequeNode* next;
} DequeNode;

struct Deque {
    DequeNode* head;
    DequeNode* tail;
    size_t size;
};

Deque* deque_create() {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    if (!deque) {
        return NULL;
    }
    deque->head = NULL;
    deque->tail = NULL;
    deque->size = 0;
    return deque;
}

void deque_destroy(Deque* deque, FreeDataFunc free_data_func) {
    if (!deque) {
        return;
    }
    DequeNode* current = deque->head;
    while (current != NULL) {
        DequeNode* next = current->next;
        if (free_data_func && current->data) {
            free_data_func(current->data);
        }
        free(current);
        current = next;
    }
    free(deque);
}

bool deque_add_first(Deque* deque, void* data) {
    if (!deque) {
        return false;
    }
    DequeNode* newNode = (DequeNode*)malloc(sizeof(DequeNode));
    if (!newNode) {
        return false;
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = deque->head;

    if (deque->head) {
        deque->head->prev = newNode;
    }
    deque->head = newNode;

    if (deque->tail == NULL) { // 如果队列为空
        deque->tail = newNode;
    }

    deque->size++;
    return true;
}

bool deque_add_last(Deque* deque, void* data) {
    if (!deque) {
        return false;
    }
    DequeNode* newNode = (DequeNode*)malloc(sizeof(DequeNode));
    if (!newNode) {
        return false;
    }
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = deque->tail;

    if (deque->tail) {
        deque->tail->next = newNode;
    }
    deque->tail = newNode;

    if (deque->head == NULL) { // 如果队列为空
        deque->head = newNode;
    }

    deque->size++;
    return true;
}

void* deque_remove_first(Deque* deque) {
    if (!deque || deque->size == 0 || !deque->head) {
        return NULL;
    }

    DequeNode* nodeToRemove = deque->head;
    void* data = nodeToRemove->data;

    deque->head = nodeToRemove->next;
    if (deque->head) {
        deque->head->prev = NULL;
    } else {
        // 队列变为空
        deque->tail = NULL;
    }

    free(nodeToRemove);
    deque->size--;
    return data;
}

void* deque_remove_last(Deque* deque) {
    if (!deque || deque->size == 0 || !deque->tail) {
        return NULL;
    }

    DequeNode* nodeToRemove = deque->tail;
    void* data = nodeToRemove->data;

    deque->tail = nodeToRemove->prev;
    if (deque->tail) {
        deque->tail->next = NULL;
    } else {
        // 队列变为空
        deque->head = NULL;
    }

    free(nodeToRemove);
    deque->size--;
    return data;
}

void* deque_peek_first(const Deque* deque) {
    if (!deque || deque->size == 0 || !deque->head) {
        return NULL;
    }
    return deque->head->data;
}

void* deque_peek_last(const Deque* deque) {
    if (!deque || deque->size == 0 || !deque->tail) {
        return NULL;
    }
    return deque->tail->data;
}

size_t deque_size(const Deque* deque) {
    return deque ? deque->size : 0;
}

bool deque_is_empty(const Deque* deque) {
    return deque ? deque->size == 0 : true;
}