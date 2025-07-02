# Add library target for linked_list
add_library(ds_linked_list STATIC
        linked_list.c
        singly_linked_list.c
        doubly_linked_list.c
        circular_linked_list.c
)

# 继承公共设置
target_link_libraries(ds_linked_list
        PUBLIC
        dsa_common
)

# 只需要设置私有的include目录
target_include_directories(ds_linked_list
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/internal>
)

# Add compile options if needed (can inherit from parent)
# target_compile_options(dsa_linked_list PRIVATE ...)

# Export the library for installation
install(TARGETS ds_linked_list EXPORT dsaTargets)