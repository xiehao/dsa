# Define test executables for different deque implementations

# Circular Array Deque Test
add_executable(test_circular_array_deque test_circular_array_deque.c)
target_link_libraries(test_circular_array_deque PRIVATE adt_deque CMocka::CMocka)
add_test(NAME CircularArrayDequeTest COMMAND test_circular_array_deque)

# Doubly Linked Deque Test
add_executable(test_doubly_linked_deque test_doubly_linked_deque.c)
target_link_libraries(test_doubly_linked_deque PRIVATE adt_deque ds_linked_list CMocka::CMocka)
add_test(NAME DoublyLinkedDequeTest COMMAND test_doubly_linked_deque)