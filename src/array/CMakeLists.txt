# Add library target for array modules
add_library(dsa_array STATIC
    dynamic_array.c
    static_array.c
    array.c
)

# Set target properties
target_include_directories(dsa_array
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Add compile options if needed (can inherit from parent)
# target_compile_options(dsa_dynamic_array PRIVATE ...)

# Export the library for installation
install(TARGETS dsa_array EXPORT dsaTargets)