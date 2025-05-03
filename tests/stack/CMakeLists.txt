# Define the test executable
add_executable(test_stack test_stack.c)

# Link the stack library and CMocka
target_link_libraries(test_stack PRIVATE dsa_stack dsa_array CMocka::CMocka)

# Add the test to CTest
add_test(NAME StackTest COMMAND test_stack)