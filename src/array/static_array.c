#include "dsa/static_array.h"
#include <string.h> // For memcpy and memmove
#include <assert.h> // For assert

// Helper macro to get the address of an element at a given index
#define ELEMENT_ADDR(arr, index) ((char*)((arr)->data) + (index) * (arr)->element_size)

bool static_array_init(StaticArray *arr, void *data_buffer, size_t capacity, size_t element_size) {
    if (!arr || !data_buffer || capacity == 0 || element_size == 0) {
        return false;
    }
    arr->data = data_buffer;
    arr->size = 0;
    arr->capacity = capacity;
    arr->element_size = element_size;
    return true;
}

void static_array_destroy(StaticArray *arr) {
    // For static arrays where memory is managed externally (e.g., VLA on stack),
    // destroy doesn't need to free memory. It just resets the structure.
    if (arr) {
        arr->data = NULL;
        arr->size = 0;
        arr->capacity = 0;
        arr->element_size = 0;
    }
}

ElementPtr static_array_get(const StaticArray *arr, size_t index) {
    assert(arr != NULL);
    if (index >= arr->size) {
        return NULL; // Index out of bounds
    }
    return ELEMENT_ADDR(arr, index);
}

bool static_array_set(StaticArray *arr, size_t index, ElementPtr value) {
    assert(arr != NULL);
    assert(value != NULL);
    if (index >= arr->size) {
        return false; // Index out of bounds
    }
    memcpy(ELEMENT_ADDR(arr, index), value, arr->element_size);
    return true;
}

size_t static_array_size(const StaticArray *arr) {
    assert(arr != NULL);
    return arr->size;
}

size_t static_array_capacity(const StaticArray *arr) {
    assert(arr != NULL);
    return arr->capacity;
}

bool static_array_is_empty(const StaticArray *arr) {
    assert(arr != NULL);
    return arr->size == 0;
}

bool static_array_is_full(const StaticArray *arr) {
    assert(arr != NULL);
    return arr->size == arr->capacity;
}

bool static_array_push_back(StaticArray *arr, ElementPtr value) {
    assert(arr != NULL);
    assert(value != NULL);
    if (static_array_is_full(arr)) {
        return false; // Array is full
    }
    memcpy(ELEMENT_ADDR(arr, arr->size), value, arr->element_size);
    arr->size++;
    return true;
}

bool static_array_pop_back(StaticArray *arr) {
    assert(arr != NULL);
    if (static_array_is_empty(arr)) {
        return false; // Array is empty
    }
    arr->size--;
    // Optionally clear the popped element's memory (not strictly necessary)
    // memset(ELEMENT_ADDR(arr, arr->size), 0, arr->element_size);
    return true;
}

bool static_array_insert(StaticArray *arr, size_t index, ElementPtr value) {
    assert(arr != NULL);
    assert(value != NULL);
    if (static_array_is_full(arr)) {
        return false; // Array is full
    }
    if (index > arr->size) { // Allow insertion at the end (index == size)
        return false; // Index out of bounds
    }

    // Shift elements to the right to make space for the new element
    if (index < arr->size) {
        memmove(ELEMENT_ADDR(arr, index + 1),
                ELEMENT_ADDR(arr, index),
                (arr->size - index) * arr->element_size);
    }

    // Insert the new element
    memcpy(ELEMENT_ADDR(arr, index), value, arr->element_size);
    arr->size++;
    return true;
}

bool static_array_delete(StaticArray *arr, size_t index) {
    assert(arr != NULL);
    if (static_array_is_empty(arr) || index >= arr->size) {
        return false; // Array is empty or index out of bounds
    }

    // Shift elements to the left to fill the gap
    if (index < arr->size - 1) {
        memmove(ELEMENT_ADDR(arr, index),
                ELEMENT_ADDR(arr, index + 1),
                (arr->size - index - 1) * arr->element_size);
    }

    arr->size--;
    // Optionally clear the last element's old memory (not strictly necessary)
    // memset(ELEMENT_ADDR(arr, arr->size), 0, arr->element_size);
    return true;
}

void static_array_clear(StaticArray *arr) {
    assert(arr != NULL);
    arr->size = 0;
    // Optionally clear the memory (not strictly necessary)
    // memset(arr->data, 0, arr->capacity * arr->element_size);
}