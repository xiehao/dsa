# Add library target for sorting algorithms
add_library(dsa_sorting STATIC
        insertion_sort.c
        selection_sort.c
        sorting_utils.c
)

# 继承公共设置
target_link_libraries(dsa_sorting
        PUBLIC
        dsa_common
        dsa_common_impl
        dsa_iterator_utils
)

# 只需要设置私有的include目录
target_include_directories(dsa_sorting
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/internal>
)

# Export the library for installation
install(TARGETS dsa_sorting EXPORT dsaTargets)
