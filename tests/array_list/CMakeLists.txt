# Define the static array test executable
add_executable(test_static_array test_static_array.c)

# Link the array library (which now includes static_array) and CMocka
target_link_libraries(test_static_array PRIVATE ds_array_list CMocka::CMocka)

# Add the static array test to CTest
add_test(NAME StaticArrayTest COMMAND test_static_array)

# Define the dynamic array test executable
add_executable(test_dynamic_array test_dynamic_array.c)

# Link the dsa_array library and CMocka
target_link_libraries(test_dynamic_array PRIVATE ds_array_list CMocka::CMocka)

# Add the dynamic array test to CTest
add_test(NAME DynamicArrayTest COMMAND test_dynamic_array)