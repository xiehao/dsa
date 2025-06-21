# Add unified array example executable
add_executable(example_array_list array_list_example.c)

# Link with the main library
target_link_libraries(example_array_list PRIVATE ds_array_list)

# Set include directories
target_include_directories(example_array_list
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)