#include <stdio.h>
#include <stdlib.h>
#include "dsa/dynamic_array.h"
#include "dsa/linked_list.h"
#include "dsa/stack.h"
#include "dsa/deque.h"

int main() {
    printf("Data Structures and Algorithms Demo\n\n");

    // Dynamic Array Demo
    printf("=== Dynamic Array Demo ===\n");
    DynamicArray* arr = dynamic_array_create(10);
    for (int i = 0; i < 5; i++) {
        int* value = malloc(sizeof(int));
        *value = i;
        dynamic_array_add(arr, value);
    }
    printf("Array size: %zu\n", dynamic_array_size(arr));
    printf("Array elements: ");
    for (size_t i = 0; i < dynamic_array_size(arr); i++) {
        int* value = dynamic_array_get(arr, i);
        printf("%d ", *value);
        free(value);
    }
    printf("\n\n");
    dynamic_array_destroy(arr);

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