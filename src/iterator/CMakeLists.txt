# Define the iterator utilities library
add_library(dsa_iterator_utils STATIC iterator_utils.c)

# Set target properties
target_include_directories(dsa_iterator_utils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/internal
)

# Link with common interface
target_link_libraries(dsa_iterator_utils PUBLIC dsa_common)

# Set compiler flags
target_compile_features(dsa_iterator_utils PUBLIC c_std_99)

# Add alias for consistent naming
add_library(dsa::iterator_utils ALIAS dsa_iterator_utils)

# Export the library for installation
install(TARGETS dsa_iterator_utils EXPORT dsaTargets)
