# Define the test executable
add_executable(test_singly_linked_list test_singly_linked_list.c)

# Link the linked_list library and CMocka
target_link_libraries(test_singly_linked_list PRIVATE ds_linked_list CMocka::CMocka)

# Add the test to CTest
add_test(NAME SinglyLinkedListTest COMMAND test_singly_linked_list)

# Define the test executable
add_executable(test_doubly_linked_list test_doubly_linked_list.c)

# Link the linked_list library and CMocka
target_link_libraries(test_doubly_linked_list PRIVATE ds_linked_list CMocka::CMocka)

# Add the test to CTest
add_test(NAME DoublyLinkedListTest COMMAND test_doubly_linked_list)

# Define the test executable for circular linked list
add_executable(test_circular_linked_list test_circular_linked_list.c)

# Link the linked_list library and CMocka
target_link_libraries(test_circular_linked_list PRIVATE ds_linked_list CMocka::CMocka)

# Add the test to CTest
add_test(NAME CircularLinkedListTest COMMAND test_circular_linked_list)

# Define the test executable for linked list iterator
add_executable(test_linked_list_iterator test_linked_list_iterator.c)

# Link the linked_list library and common library
target_link_libraries(test_linked_list_iterator PRIVATE ds_linked_list dsa_common_impl)

# Add the test to CTest
add_test(NAME LinkedListIteratorTest COMMAND test_linked_list_iterator)