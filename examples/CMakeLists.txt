# Add example executable
add_executable(dsa_example test_main.c)

# Link with the main library
target_link_libraries(dsa_example PRIVATE ${PROJECT_NAME})

# Set include directories
target_include_directories(dsa_example
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)