# Define the static array test executable
add_executable(test_static_array_list test_static_array_list.c)

# Link the array library (which now includes static_array) and CMocka
target_link_libraries(test_static_array_list PRIVATE ds_array_list CMocka::CMocka)

# Add the static array test to CTest
add_test(NAME StaticArrayListTest COMMAND test_static_array_list)

# Define the dynamic array test executable
add_executable(test_dynamic_array_list test_dynamic_array_list.c)

# Link the dsa_array library and CMocka
target_link_libraries(test_dynamic_array_list PRIVATE ds_array_list CMocka::CMocka)

# Add the dynamic array test to CTest
add_test(NAME DynamicArrayListTest COMMAND test_dynamic_array_list)