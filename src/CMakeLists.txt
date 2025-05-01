# Add subdirectories for each data structure
add_subdirectory(dynamic_array)
add_subdirectory(linked_list)
add_subdirectory(stack)
add_subdirectory(deque)

# Create an interface library to aggregate sub-libraries
add_library(dsa_internal INTERFACE)

# Link sub-libraries to the interface library
target_link_libraries(dsa_internal INTERFACE
    dsa_dynamic_array
    dsa_linked_list
    dsa_stack
    dsa_deque
)

# Propagate include directories from the interface library
target_include_directories(dsa_internal INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

# Propagate compile options if needed (or set globally in root CMakeLists.txt)
# target_compile_options(dsa_internal INTERFACE ...)

# Export the internal interface library for installation
install(TARGETS dsa_internal EXPORT dsaTargets)