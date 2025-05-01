# Add library target for deque
add_library(dsa_deque STATIC
    deque.c
)

# Set target properties
target_include_directories(dsa_deque
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:include>
)

# Add compile options if needed (can inherit from parent)
# target_compile_options(dsa_deque PRIVATE ...)

# Export the library for installation
install(TARGETS dsa_deque EXPORT dsaTargets)