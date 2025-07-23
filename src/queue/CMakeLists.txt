# Add library target for deque
add_library(adt_queue STATIC
        queue.c
        circular_array_queue.c
        doubly_linked_queue.c
)

# 继承公共设置
target_link_libraries(adt_queue
        PUBLIC
        dsa_common
        adt_deque
        ds_linked_list
)

# 只需要设置私有的include目录
target_include_directories(adt_queue
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/internal>
)

# Export the library for installation
install(TARGETS adt_queue EXPORT dsaTargets)