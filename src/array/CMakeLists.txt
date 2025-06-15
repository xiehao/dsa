# Add library target for array modules
add_library(ds_array STATIC
    dynamic_array.c
    static_array.c
    array.c
)

# 继承公共设置
target_link_libraries(ds_array PUBLIC dsa_common)

# 只需要设置私有的include目录
target_include_directories(ds_array
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Add compile options if needed (can inherit from parent)
# target_compile_options(ds_array PRIVATE ...)

# Export the library for installation
install(TARGETS ds_array EXPORT dsaTargets)