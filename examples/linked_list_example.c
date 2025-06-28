//
// Created by oaheixiehao on 2025/6/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <ds/linked_list.h>

int main() {
    // Create a singly linked list
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    if (!list) {
        printf("Failed to create linked list\n");
        return 1;
    }

    // Create some test data
    int *data1 = malloc(sizeof(int));
    int *data2 = malloc(sizeof(int));
    int *data3 = malloc(sizeof(int));
    *data1 = 10;
    *data2 = 20;
    *data3 = 30;

    // Insert elements
    linked_list_insert_at(list, 0, data1);
    linked_list_insert_at(list, 1, data2);
    linked_list_insert_at(list, 2, data3);

    // Print the list size
    printf("List size: %zu\n", linked_list_size(list));

    // Get and print elements
    for (size_t i = 0; i < linked_list_size(list); i++) {
        int *value = linked_list_get(list, i);
        if (value) {
            printf("Element at index %zu: %d\n", i, *value);
        }
    }

    // Remove middle element
    int *removed = linked_list_remove_at(list, 1);
    if (removed) {
        printf("Removed element: %d\n", *removed);
        free(removed);
    }

    // Print final size
    printf("Final list size: %zu\n", linked_list_size(list));

    // Clean up
    linked_list_destroy(list);

    return 0;
}
