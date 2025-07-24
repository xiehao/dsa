# Add library target for common modules
add_library(dsa_common_impl STATIC
        iterator_operations.c
)

# 继承公共设置
target_link_libraries(dsa_common_impl
        PUBLIC
        dsa_common
)

# 只需要设置私有的include目录
target_include_directories(dsa_common_impl
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/internal>
)

# Export the library for installation
install(TARGETS dsa_common_impl EXPORT dsaTargets)
