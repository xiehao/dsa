# Add library target for stack
add_library(dsa_stack STATIC
    stack.c
)

# Set target properties
target_include_directories(dsa_stack
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:include>
)

# Add compile options if needed (can inherit from parent)
# target_compile_options(dsa_stack PRIVATE ...)

# Export the library for installation
install(TARGETS dsa_stack EXPORT dsaTargets)