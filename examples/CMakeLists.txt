# Add example executable
add_executable(dsa_example test_main.c)

# Link with the main library
target_link_libraries(dsa_example PRIVATE ${PROJECT_NAME})

# Set include directories
target_include_directories(dsa_example
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Add unified array example executable
add_executable(array_example array_example.c)

# Link with the main library
target_link_libraries(array_example PRIVATE ${PROJECT_NAME})

# Set include directories
target_include_directories(array_example
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)