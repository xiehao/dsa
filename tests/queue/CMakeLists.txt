# Define test executables for different queue implementations

# General Queue Test
add_executable(test_queue test_queue.c)
target_link_libraries(test_queue PRIVATE adt_queue CMocka::CMocka)
add_test(NAME QueueTest COMMAND test_queue)

# Circular Array Queue Test
add_executable(test_circular_array_queue test_circular_array_queue.c)
target_link_libraries(test_circular_array_queue PRIVATE adt_queue adt_deque CMocka::CMocka)
add_test(NAME CircularArrayQueueTest COMMAND test_circular_array_queue)

# Doubly Linked Queue Test
add_executable(test_doubly_linked_queue test_doubly_linked_queue.c)
target_link_libraries(test_doubly_linked_queue PRIVATE adt_queue ds_linked_list CMocka::CMocka)
add_test(NAME DoublyLinkedQueueTest COMMAND test_doubly_linked_queue)
