# ============================================================================
# DSA - C语言数据结构与算法实现库
# ============================================================================
# 项目描述: 现代化的C语言数据结构库，提供完整的数据结构实现、
#          全面的单元测试和丰富的示例代码
# 作者:     DSA项目团队
# 版本:     1.0.0
# 许可证:   MIT License
# ============================================================================

# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.16)

# 定义项目信息
project(dsa
        VERSION 1.0.0
        DESCRIPTION "Data Structures and Algorithms Library"
        LANGUAGES C
)

# ============================================================================
# 构建配置选项
# ============================================================================

# 设置默认构建类型为Release（如果未指定）
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# 启用编译命令导出（用于IDE和代码分析工具）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 项目构建选项
option(DSA_BUILD_TESTS "构建单元测试" ON)
option(DSA_BUILD_EXAMPLES "构建示例程序" ON)
option(DSA_BUILD_DOCS "构建文档" OFF)
option(DSA_ENABLE_SANITIZERS "在Debug模式下启用内存检查工具" OFF)

# ============================================================================
# 公共接口库配置
# ============================================================================

# 创建公共接口库（用于传递配置信息给各个内部子目标）
add_library(dsa_common INTERFACE)

# 设置头文件包含目录
target_include_directories(dsa_common
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>  # 构建时使用的头文件路径
        $<INSTALL_INTERFACE:include>                    # 安装后使用的头文件路径
)

# 设置C语言标准为C11
target_compile_features(dsa_common INTERFACE c_std_11)

# 添加编译器特定的编译选项
target_compile_options(dsa_common
        INTERFACE
        $<$<C_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>      # GCC编译器选项
        $<$<C_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>    # Clang编译器选项
        $<$<C_COMPILER_ID:MSVC>:/W4>                          # MSVC编译器选项
)

# ============================================================================
# 内部开发接口库配置
# ============================================================================

# 创建内部开发用的接口库（包含内部头文件访问权限）
add_library(dsa_internal_dev INTERFACE)
target_link_libraries(dsa_internal_dev INTERFACE dsa_common)

# 添加内部头文件目录（如果存在）
target_include_directories(dsa_internal_dev
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/internal>
)

# 调试模式下的内存检查工具配置
if (DSA_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 添加AddressSanitizer和UndefinedBehaviorSanitizer编译选项
    target_compile_options(dsa_internal_dev
            INTERFACE
            $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-fsanitize=address,undefined>
    )
    # 添加对应的链接选项
    target_link_options(dsa_internal_dev
            INTERFACE
            $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-fsanitize=address,undefined>
    )
endif ()

# ============================================================================
# 源代码目录配置
# ============================================================================

# 添加源码子目录
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    add_subdirectory(src)
else ()
    message(WARNING "源代码目录 src 未找到")
endif ()

# ============================================================================
# 主库目标配置
# ============================================================================

# 创建主库目标（接口库）
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE dsa_common)

# 检查并链接内部目标
if (TARGET dsa_internal)
    target_link_libraries(${PROJECT_NAME} INTERFACE dsa_internal)
else ()
    message(WARNING "内部目标 dsa_internal 未找到")
endif ()

# 创建别名目标（便于在子项目中使用）
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ============================================================================
# 构建信息输出
# ============================================================================

# 打印配置信息
message(STATUS "正在配置 ${PROJECT_NAME} 项目...")
message(STATUS "项目版本: ${PROJECT_VERSION}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "C语言标准: C11")
message(STATUS "构建测试: ${DSA_BUILD_TESTS}")
message(STATUS "构建示例: ${DSA_BUILD_EXAMPLES}")
message(STATUS "构建文档: ${DSA_BUILD_DOCS}")

# ============================================================================
# 子目录配置
# ============================================================================

# 示例程序目录
if (DSA_BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    message(STATUS "添加示例程序目录")
    add_subdirectory(examples)
endif ()

# 测试配置
if (DSA_BUILD_TESTS)
    enable_testing()
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        message(STATUS "添加测试目录")
        add_subdirectory(tests)
    endif ()
endif ()

# 文档配置（可选）
if (DSA_BUILD_DOCS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/docs")
    message(STATUS "添加文档目录")
    add_subdirectory(docs)
endif ()

# ============================================================================
# 安装配置
# ============================================================================

# 包含GNU标准安装目录定义
include(GNUInstallDirs)

# 安装库目标和公共接口
install(TARGETS ${PROJECT_NAME} dsa_common
        EXPORT ${PROJECT_NAME}Targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 安装头文件
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h"
    )
endif ()

# ============================================================================
# CMake包配置
# ============================================================================

# 导出目标配置文件
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# 生成包配置文件
include(CMakePackageConfigHelpers)

# 配置包配置文件（如果模板文件存在）
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in")
    configure_package_config_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
else ()
    message(STATUS "包配置模板文件未找到，跳过配置文件生成")
endif ()

# 生成版本兼容性文件
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

# 安装包配置文件
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# ============================================================================
# 构建完成信息
# ============================================================================

message(STATUS "${PROJECT_NAME} 项目配置完成")
message(STATUS "使用 'cmake --build .' 来构建项目")
message(STATUS "使用 'ctest' 来运行测试")
message(STATUS "使用 'cmake --install .' 来安装项目")