# Define the test executable
add_executable(test_array test_dynamic_array.c)

# Add include directories (project root include and src for internal headers)
target_include_directories(test_array PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# Link the dynamic_array library and CMocka
target_link_libraries(test_array PRIVATE dsa_array CMocka::CMocka)

# Add the test to CTest
add_test(NAME DynamicArrayTest COMMAND test_array)

# Define the static array test executable
add_executable(test_static_array test_static_array.c)

# Add include directories (project root include and src for internal headers)
target_include_directories(test_static_array PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# Link the array library (which now includes static_array) and CMocka
target_link_libraries(test_static_array PRIVATE dsa_array CMocka::CMocka)

# Add the static array test to CTest
add_test(NAME StaticArrayTest COMMAND test_static_array)

# Generic array test has been replaced by unified array test

# Define the unified array test executable
add_executable(test_unified_array test_array.c)

# Add include directories (project root include)
target_include_directories(test_unified_array PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# Link the array library and CMocka
target_link_libraries(test_unified_array PRIVATE dsa_array CMocka::CMocka)

# Add the unified array test to CTest
add_test(NAME UnifiedArrayTest COMMAND test_unified_array)