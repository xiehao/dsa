# Add library target for stack
add_library(adt_stack STATIC
        stack.c
        array_stack.c
        linked_stack.c
)

# 继承公共设置
target_link_libraries(adt_stack
        PUBLIC
        dsa_common
        ds_array_list
        ds_linked_list
)

# 只需要设置私有的include目录
target_include_directories(adt_stack
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/internal>
)

# Add compile options if needed (can inherit from parent)
# target_compile_options(dsa_stack PRIVATE ...)

# Export the library for installation
install(TARGETS adt_stack EXPORT dsaTargets)
