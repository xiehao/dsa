#ifndef DSA_LINKED_LIST_H
#define DSA_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include <dsa/common.h> // 包含通用定义

// Opaque pointer type for the linked list
typedef struct LinkedList LinkedList;

// Type definition for a function pointer to free user-provided data
typedef void (*FreeDataFunc)(ElementPtr data);

/**
 * @brief Creates a new linked list.
 *
 * @return A pointer to the newly created LinkedList, or NULL if allocation fails.
 */
LinkedList* linked_list_create();

/**
 * @brief Destroys the linked list and frees its associated memory.
 *
 * If a `free_data_func` is provided, it will be called for each element's data
 * before freeing the node itself.
 *
 * @param list A pointer to the LinkedList to destroy.
 * @param free_data_func A function pointer to free the data stored in each node. Can be NULL if data freeing is handled elsewhere or not needed.
 */
void linked_list_destroy(LinkedList* list, FreeDataFunc free_data_func);

/**
 * @brief Adds an element to the beginning of the linked list.
 *
 * @param list A pointer to the LinkedList.
 * @param data A pointer to the data to add.
 * @return True if the element was added successfully, false otherwise (e.g., allocation failure).
 */
bool linked_list_add_first(LinkedList* list, ElementPtr data);

/**
 * @brief Adds an element to the end of the linked list.
 *
 * @param list A pointer to the LinkedList.
 * @param data A pointer to the data to add.
 * @return True if the element was added successfully, false otherwise (e.g., allocation failure).
 */
bool linked_list_add_last(LinkedList* list, ElementPtr data);

/**
 * @brief Inserts an element at the specified index.
 *
 * Elements at and after the index are shifted one position to the right.
 * Index 0 inserts at the beginning, index `size` inserts at the end.
 *
 * @param list A pointer to the LinkedList.
 * @param index The index at which to insert the element.
 * @param data A pointer to the data to insert.
 * @return True if the element was inserted successfully, false if the index is out of bounds or allocation fails.
 */
bool linked_list_insert(LinkedList* list, size_t index, ElementPtr data);

/**
 * @brief Removes and returns the element from the beginning of the linked list.
 *
 * @param list A pointer to the LinkedList.
 * @return A pointer to the data of the removed element, or NULL if the list is empty.
 *         The caller is responsible for freeing the returned data if necessary.
 */
ElementPtr linked_list_remove_first(LinkedList* list);

/**
 * @brief Removes and returns the element from the end of the linked list.
 *
 * @param list A pointer to the LinkedList.
 * @return A pointer to the data of the removed element, or NULL if the list is empty.
 *         The caller is responsible for freeing the returned data if necessary.
 */
ElementPtr linked_list_remove_last(LinkedList* list);

/**
 * @brief Removes and returns the element at the specified index.
 *
 * Elements after the index are shifted one position to the left.
 *
 * @param list A pointer to the LinkedList.
 * @param index The index of the element to remove.
 * @return A pointer to the data of the removed element, or NULL if the index is out of bounds or the list is empty.
 *         The caller is responsible for freeing the returned data if necessary.
 */
ElementPtr linked_list_remove(LinkedList* list, size_t index);

/**
 * @brief Gets the element at the specified index without removing it.
 *
 * @param list A pointer to the LinkedList.
 * @param index The index of the element to retrieve.
 * @return A pointer to the data at the specified index, or NULL if the index is out of bounds.
 */
ElementPtr linked_list_get(const LinkedList* list, size_t index);

/**
 * @brief Sets the data at the specified index.
 *
 * @param list A pointer to the LinkedList.
 * @param index The index to set the data at.
 * @param data A pointer to the new data. The old data at this index is overwritten but not freed by this function.
 * @return True if the data was set successfully, false if the index is out of bounds.
 */
bool linked_list_set(LinkedList* list, size_t index, ElementPtr data);

/**
 * @brief Gets the current number of elements in the linked list.
 *
 * @param list A pointer to the LinkedList.
 * @return The number of elements in the list.
 */
size_t linked_list_size(const LinkedList* list);

/**
 * @brief Checks if the linked list is empty.
 *
 * @param list A pointer to the LinkedList.
 * @return True if the list is empty, false otherwise.
 */
bool linked_list_is_empty(const LinkedList* list);

#endif // DSA_LINKED_LIST_H