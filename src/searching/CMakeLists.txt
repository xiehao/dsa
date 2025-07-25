# Define the searching library
add_library(dsa_searching STATIC searching.c)

# Set target properties
target_include_directories(dsa_searching
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/internal
)

# Link with common interface
target_link_libraries(dsa_searching PUBLIC dsa_common)

# Set compiler flags
target_compile_features(dsa_searching PUBLIC c_std_99)

# Add alias for consistent naming
add_library(dsa::searching ALIAS dsa_searching)

# Export the library for installation
install(TARGETS dsa_searching EXPORT dsaTargets)
