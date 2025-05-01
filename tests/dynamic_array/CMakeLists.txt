# Define the test executable
add_executable(test_dynamic_array test_dynamic_array.c)

# Add include directories (project root include)
target_include_directories(test_dynamic_array PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link the dynamic_array library and CMocka
target_link_libraries(test_dynamic_array PRIVATE dsa_dynamic_array CMocka::CMocka)

# Add the test to CTest
add_test(NAME DynamicArrayTest COMMAND test_dynamic_array)