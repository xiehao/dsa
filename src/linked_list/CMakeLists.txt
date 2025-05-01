# Add library target for linked_list
add_library(dsa_linked_list STATIC
    linked_list.c
)

# Set target properties
target_include_directories(dsa_linked_list
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:include>
)

# Add compile options if needed (can inherit from parent)
# target_compile_options(dsa_linked_list PRIVATE ...)

# Export the library for installation
install(TARGETS dsa_linked_list EXPORT dsaTargets)