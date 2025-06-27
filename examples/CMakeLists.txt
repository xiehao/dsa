# Add array list example executable
add_executable(example_array_list array_list_example.c)

# Link with the main library
target_link_libraries(example_array_list PRIVATE ds_array_list)

# Set include directories
target_include_directories(example_array_list
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Add linked list example executable
add_executable(example_linked_list linked_list_example.c)

# Link with the main library
target_link_libraries(example_linked_list PRIVATE ds_linked_list)

# Set include directories
target_include_directories(example_linked_list
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)