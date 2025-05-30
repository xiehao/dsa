#include <stdio.h>
#include <stdlib.h>
#include "dsa/array.h"
#include "dsa/linked_list.h"
#include "dsa/stack.h"
#include "dsa/deque.h"

int main() {
    printf("Data Structures and Algorithms Demo\n\n");

    // Unified Array Demo
    printf("=== Unified Array Demo ===\n");
    Array* arr = array_create_dynamic(10);
    for (int i = 0; i < 5; i++) {
        array_push_back_int(arr, i);
    }
    printf("Array size: %zu\n", array_size(arr));
    printf("Array type: %s\n", array_get_type_name(arr));
    printf("Array elements: ");
    for (size_t i = 0; i < array_size(arr); i++) {
        int value;
        if (array_get_int(arr, i, &value) == ARRAY_SUCCESS) {
            printf("%d ", value);
        }
    }
    printf("\n\n");
    // 清理动态数组中的元素
    while (!array_is_empty(arr)) {
        void* element = array_pop_back(arr);
        if (element) free(element);
    }
    array_destroy(arr);

    // Linked List Demo
    printf("=== Linked List Demo ===\n");
    LinkedList* list = linked_list_create();
    for (int i = 0; i < 5; i++) {
        int* value = malloc(sizeof(int));
        *value = i;
        linked_list_add_last(list, value);
    }
    printf("List size: %zu\n", linked_list_size(list));
    printf("List elements: ");
    for (size_t i = 0; i < linked_list_size(list); i++) {
        int* value = linked_list_get(list, i);
        printf("%d ", *value);
    }
    printf("\n\n");
    linked_list_destroy(list, free);

    // Stack Demo
    printf("=== Stack Demo ===\n");
    Stack* stack = stack_create();
    for (int i = 0; i < 5; i++) {
        int* value = malloc(sizeof(int));
        *value = i;
        stack_push(stack, value);
    }
    printf("Stack size: %zu\n", stack_size(stack));
    printf("Stack elements (top to bottom): ");
    while (!stack_is_empty(stack)) {
        int* value = stack_pop(stack);
        if (value != NULL) {
            printf("%d ", *value);
            free(value);
        }
    }
    printf("\n\n");
    stack_destroy(stack);

    // Deque Demo
    printf("=== Deque Demo ===\n");
    Deque* deque = deque_create();
    for (int i = 0; i < 3; i++) {
        int* value = malloc(sizeof(int));
        *value = i;
        deque_add_first(deque, value);
        value = malloc(sizeof(int));
        *value = i + 3;
        deque_add_last(deque, value);
    }
    printf("Deque size: %zu\n", deque_size(deque));
    printf("Deque elements (front to back): ");
    while (!deque_is_empty(deque)) {
        int* value = deque_remove_first(deque);
        printf("%d ", *value);
        free(value);
    }
    printf("\n");
    deque_destroy(deque, free);

    return 0;
}