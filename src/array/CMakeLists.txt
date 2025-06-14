# Add library target for array modules
add_library(ds_array STATIC
    dynamic_array.c
    static_array.c
    array.c
)

# Set target properties
target_include_directories(ds_array
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Add compile options if needed (can inherit from parent)
# target_compile_options(ds_array PRIVATE ...)

# Export the library for installation
install(TARGETS ds_array EXPORT dsaTargets)