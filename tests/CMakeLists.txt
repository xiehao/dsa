# Enable testing
enable_testing()

# Find CMocka headers
find_path(CMOCKA_INCLUDE_DIR cmocka.h
    HINTS /opt/homebrew/include /usr/local/include /usr/include
)

# Find CMocka library
find_library(CMOCKA_LIBRARY NAMES cmocka
    HINTS /opt/homebrew/lib /usr/local/lib /usr/lib
)

if(CMOCKA_INCLUDE_DIR AND CMOCKA_LIBRARY)
    message(STATUS "Found CMocka headers: ${CMOCKA_INCLUDE_DIR}")
    message(STATUS "Found CMocka library: ${CMOCKA_LIBRARY}")
    # Define interface library for CMocka
    add_library(CMocka::CMocka INTERFACE IMPORTED)
    set_target_properties(CMocka::CMocka PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMOCKA_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${CMOCKA_LIBRARY}"
    )
else()
    message(FATAL_ERROR "CMocka not found. Please install CMocka and ensure it's findable by CMake (headers in include path, library in lib path).")
endif()

# Add subdirectories for each data structure's tests
add_subdirectory(array_list)
add_subdirectory(linked_list)
add_subdirectory(stack)
add_subdirectory(deque)