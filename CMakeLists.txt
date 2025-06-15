
cmake_minimum_required(VERSION 3.16)
project(dsa
        VERSION 1.0.0
        DESCRIPTION "Data Structures and Algorithms Library"
        LANGUAGES C
)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 创建公共接口库
add_library(dsa_common INTERFACE)

# 包含目录
target_include_directories(dsa_common
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 如果有公共的编译选项也可以添加
target_compile_features(dsa_common INTERFACE c_std_11)

# 将接口库也导出
install(TARGETS dsa_common EXPORT dsaTargets)


# Check if src directory exists before adding it
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    add_subdirectory(src)
else ()
    message(WARNING "src directory not found")
endif ()

# Create the main library target
add_library(${PROJECT_NAME} INTERFACE)

# Check if dsa_internal target exists before linking
if (TARGET dsa_internal)
    target_link_libraries(${PROJECT_NAME} INTERFACE dsa_internal)
else ()
    message(WARNING "dsa_internal target not found")
endif ()

# Set library properties
target_include_directories(${PROJECT_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Add compile options
target_compile_options(${PROJECT_NAME}
        INTERFACE
        $<$<C_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<C_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
        $<$<C_COMPILER_ID:MSVC>:/W4>
)

# Print configuration messages
message(STATUS "Configuring ${PROJECT_NAME} project...")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")

# Install rules
include(GNUInstallDirs)

# Correct install for INTERFACE library
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Check if include directory exists before installing
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif ()

# Add examples directory if it exists
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif ()

# Enable testing
enable_testing()

# Add tests directory if BUILD_TESTING is ON and directory exists
if (BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif ()

# Export targets
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Generate and install package config files
#include(CMakePackageConfigHelpers)
#
#configure_package_config_file(
#        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
#        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
#)
#
#write_basic_package_version_file(
#        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#        VERSION ${PROJECT_VERSION}
#        COMPATIBILITY SameMajorVersion
#)
#
#install(FILES
#        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
#)