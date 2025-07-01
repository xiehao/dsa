# Add library target for array modules
add_library(ds_array_list STATIC
        dynamic_array_list.c
        static_array_list.c
        array_list.c
)

# 继承公共设置
target_link_libraries(ds_array_list
        PUBLIC
        dsa_common
)

# 只需要设置私有的include目录
target_include_directories(ds_array_list
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/internal>
)

# Add compile options if needed (can inherit from parent)
# target_compile_options(ds_array PRIVATE ...)

# Export the library for installation
install(TARGETS ds_array_list EXPORT dsaTargets)