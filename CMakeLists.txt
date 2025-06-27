cmake_minimum_required(VERSION 3.16)
project(dsa
        VERSION 1.0.0
        DESCRIPTION "Data Structures and Algorithms Library"
        LANGUAGES C
)

# 设置默认构建类型（推荐添加）
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加项目选项（推荐添加）
option(DSA_BUILD_TESTS "Build tests" ON)
option(DSA_BUILD_EXAMPLES "Build examples" ON)
option(DSA_BUILD_DOCS "Build documentation" OFF)
option(DSA_ENABLE_SANITIZERS "Enable sanitizers in debug builds" OFF)

# 创建公共接口库（用于传递配置信息给各个内部子目标）
add_library(dsa_common INTERFACE)
target_include_directories(dsa_common
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(dsa_common INTERFACE c_std_11)

# 添加编译选项到公共接口
target_compile_options(dsa_common
        INTERFACE
        $<$<C_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<C_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
        $<$<C_COMPILER_ID:MSVC>:/W4>
)

# 创建内部开发用的接口库
add_library(dsa_internal_dev INTERFACE)
target_link_libraries(dsa_internal_dev INTERFACE dsa_common)
target_include_directories(dsa_internal_dev
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/internal>
)

# 可选：添加调试选项
if (DSA_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(dsa_internal_dev
            INTERFACE
            $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-fsanitize=address,undefined>
    )
    target_link_options(dsa_internal_dev
            INTERFACE
            $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-fsanitize=address,undefined>
    )
endif ()

# 添加源码子目录
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    add_subdirectory(src)
else ()
    message(WARNING "src directory not found")
endif ()

# 创建主库目标
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE dsa_common)

# 检查并链接内部目标
if (TARGET dsa_internal)
    target_link_libraries(${PROJECT_NAME} INTERFACE dsa_internal)
else ()
    message(WARNING "dsa_internal target not found")
endif ()

# 创建别名目标（推荐添加）
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# 打印配置信息
message(STATUS "Configuring ${PROJECT_NAME} project...")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Standard: C11")

# 示例目录
if (DSA_BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif ()

# 测试配置
if (DSA_BUILD_TESTS)
    enable_testing()
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        add_subdirectory(tests)
    endif ()
endif ()

# 文档配置（可选）
if (DSA_BUILD_DOCS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/docs")
    add_subdirectory(docs)
endif ()

# 安装配置
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} dsa_common
        EXPORT ${PROJECT_NAME}Targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif ()

# 导出目标
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# 包配置文件（建议取消注释）
include(CMakePackageConfigHelpers)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)