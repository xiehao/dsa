cmake_minimum_required(VERSION 3.31.6)

project(dsa
    VERSION 1.0.0
    DESCRIPTION "Data Structures and Algorithms Library"
    LANGUAGES C
)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

# Add the source directory which contains the sub-libraries
add_subdirectory(src)

# Create the main library target and link the internal library
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE dsa_internal)

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
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Add examples directory
add_subdirectory(examples)

# Enable testing
enable_testing()

# Add tests directory if BUILD_TESTING is ON (or always add if preferred)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Export targets
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)