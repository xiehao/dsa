#ifndef DSA_STACK_H
#define DSA_STACK_H

#include <stdbool.h>
#include <stddef.h>

// Opaque pointer type for the Stack structure
typedef struct Stack Stack;

/**
 * @brief Creates a new empty stack.
 *
 * @return A pointer to the newly created stack, or NULL if memory allocation fails.
 */
Stack* stack_create();

/**
 * @brief Destroys the stack and frees its associated memory.
 *        Note: This function does not free the elements stored in the stack.
 *        The caller is responsible for managing the memory of the elements themselves.
 *
 * @param stack A pointer to the stack to be destroyed.
 */
void stack_destroy(Stack* stack);

/**
 * @brief Pushes an element onto the top of the stack.
 *
 * @param stack A pointer to the stack.
 * @param element A pointer to the element to push.
 * @return true if the element was pushed successfully, false otherwise (e.g., memory allocation failure).
 */
bool stack_push(Stack* stack, void* element);

/**
 * @brief Removes and returns the element from the top of the stack.
 *
 * @param stack A pointer to the stack.
 * @return A pointer to the element removed from the top, or NULL if the stack is empty.
 */
void* stack_pop(Stack* stack);

/**
 * @brief Returns the element at the top of the stack without removing it.
 *
 * @param stack A pointer to the stack.
 * @return A pointer to the element at the top, or NULL if the stack is empty.
 */
void* stack_peek(const Stack* stack);

/**
 * @brief Returns the number of elements in the stack.
 *
 * @param stack A pointer to the stack.
 * @return The number of elements in the stack.
 */
size_t stack_size(const Stack* stack);

/**
 * @brief Checks if the stack is empty.
 *
 * @param stack A pointer to the stack.
 * @return true if the stack is empty, false otherwise.
 */
bool stack_is_empty(const Stack* stack);

#endif // DSA_STACK_H