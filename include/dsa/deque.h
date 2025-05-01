#ifndef DSA_DEQUE_H
#define DSA_DEQUE_H

#include <stdbool.h>
#include <stddef.h>

// Opaque pointer type for the Deque structure
typedef struct Deque Deque;

#include "linked_list.h"  // For FreeDataFunc definition

/**
 * @brief Creates a new empty deque.
 *
 * @return A pointer to the newly created deque, or NULL if memory allocation fails.
 */
Deque* deque_create();

/**
 * @brief Destroys the deque and frees its associated memory.
 *
 * If a `free_data_func` is provided, it will be called for each element's data
 * before freeing the underlying storage.
 *
 * @param deque A pointer to the deque to be destroyed.
 * @param free_data_func A function pointer to free the data stored in each element. Can be NULL.
 */
void deque_destroy(Deque* deque, FreeDataFunc free_data_func);

/**
 * @brief Adds an element to the front of the deque.
 *
 * @param deque A pointer to the deque.
 * @param data A pointer to the data to add.
 * @return true if the element was added successfully, false otherwise (e.g., memory allocation failure).
 */
bool deque_add_first(Deque* deque, void* data);

/**
 * @brief Adds an element to the back of the deque.
 *
 * @param deque A pointer to the deque.
 * @param data A pointer to the data to add.
 * @return true if the element was added successfully, false otherwise (e.g., memory allocation failure).
 */
bool deque_add_last(Deque* deque, void* data);

/**
 * @brief Removes and returns the element from the front of the deque.
 *
 * @param deque A pointer to the deque.
 * @return A pointer to the data of the removed element, or NULL if the deque is empty.
 *         The caller is responsible for freeing the returned data if necessary.
 */
void* deque_remove_first(Deque* deque);

/**
 * @brief Removes and returns the element from the back of the deque.
 *
 * @param deque A pointer to the deque.
 * @return A pointer to the data of the removed element, or NULL if the deque is empty.
 *         The caller is responsible for freeing the returned data if necessary.
 */
void* deque_remove_last(Deque* deque);

/**
 * @brief Returns the element at the front of the deque without removing it.
 *
 * @param deque A pointer to the deque.
 * @return A pointer to the data at the front, or NULL if the deque is empty.
 */
void* deque_peek_first(const Deque* deque);

/**
 * @brief Returns the element at the back of the deque without removing it.
 *
 * @param deque A pointer to the deque.
 * @return A pointer to the data at the back, or NULL if the deque is empty.
 */
void* deque_peek_last(const Deque* deque);

/**
 * @brief Returns the number of elements in the deque.
 *
 * @param deque A pointer to the deque.
 * @return The number of elements in the deque.
 */
size_t deque_size(const Deque* deque);

/**
 * @brief Checks if the deque is empty.
 *
 * @param deque A pointer to the deque.
 * @return true if the deque is empty, false otherwise.
 */
bool deque_is_empty(const Deque* deque);

#endif // DSA_DEQUE_H