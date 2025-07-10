# Add library target for deque
add_library(adt_deque STATIC
        deque.c
        circular_array_deque.c
        doubly_linked_deque.c
)

# 继承公共设置
target_link_libraries(adt_deque
        PUBLIC
        dsa_common
#        ds_array_list
        ds_linked_list
)

# 只需要设置私有的include目录
target_include_directories(adt_deque
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/internal>
)

# Export the library for installation
install(TARGETS adt_deque EXPORT dsaTargets)