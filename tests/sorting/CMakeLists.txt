# Test executable for sorting utilities
add_executable(test_sorting_utils
        test_sorting_utils.c
)

# Link with the main library and CMocka
target_link_libraries(test_sorting_utils
        PRIVATE
        dsa
        CMocka::CMocka
)

# Test executable for insertion sort
add_executable(test_insertion_sort
        test_insertion_sort.c
)

target_link_libraries(test_insertion_sort
        PRIVATE
        dsa
        CMocka::CMocka
)

# Test executable for selection sort
add_executable(test_selection_sort
        test_selection_sort.c
)

target_link_libraries(test_selection_sort
        PRIVATE
        dsa
        CMocka::CMocka
)

# Add tests to CTest
add_test(NAME SortingUtilsTests COMMAND test_sorting_utils)
add_test(NAME InsertionSortTests COMMAND test_insertion_sort)
add_test(NAME SelectionSortTests COMMAND test_selection_sort)
