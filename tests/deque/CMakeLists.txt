# Define the test executable
add_executable(test_deque test_deque.c)

# Link the deque library and CMocka
target_link_libraries(test_deque PRIVATE dsa_deque CMocka::CMocka)

# Add the test to CTest
add_test(NAME DequeTest COMMAND test_deque)