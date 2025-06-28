# Define the test executable
add_executable(test_linked_list test_linked_list.c)

# Link the linked_list library and CMocka
target_link_libraries(test_linked_list PRIVATE ds_linked_list CMocka::CMocka)

# Add the test to CTest
add_test(NAME LinkedListTest COMMAND test_linked_list)