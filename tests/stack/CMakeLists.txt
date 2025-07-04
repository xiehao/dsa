# Define the general stack test executable
add_executable(test_stack test_stack.c)

# Link the stack library and CMocka
target_link_libraries(test_stack PRIVATE adt_stack CMocka::CMocka)

# Add the test to CTest
add_test(NAME StackTest COMMAND test_stack)

# Define the array stack specific test executable
add_executable(test_array_stack test_array_stack.c)

# Link the stack library and CMocka
target_link_libraries(test_array_stack PRIVATE adt_stack CMocka::CMocka)

# Add the array stack test to CTest
add_test(NAME ArrayStackTest COMMAND test_array_stack)

# Define the linked stack specific test executable
add_executable(test_linked_stack test_linked_stack.c)

# Link the stack library and CMocka
target_link_libraries(test_linked_stack PRIVATE adt_stack CMocka::CMocka)

# Add the linked stack test to CTest
add_test(NAME LinkedStackTest COMMAND test_linked_stack)