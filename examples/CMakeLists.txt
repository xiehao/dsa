# Array List Examples
# Add static array list example executable
add_executable(example_static_array_list static_array_list_example.c)
target_link_libraries(example_static_array_list PRIVATE ds_array_list)
target_include_directories(example_static_array_list PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add dynamic array list example executable
add_executable(example_dynamic_array_list dynamic_array_list_example.c)
target_link_libraries(example_dynamic_array_list PRIVATE ds_array_list)
target_include_directories(example_dynamic_array_list PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add unified array list example executable (original)
add_executable(example_array_list array_list_example.c)
target_link_libraries(example_array_list PRIVATE ds_array_list)
target_include_directories(example_array_list PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Linked List Examples
# Add singly linked list example executable
add_executable(example_singly_linked_list singly_linked_list_example.c)
target_link_libraries(example_singly_linked_list PRIVATE ds_linked_list)
target_include_directories(example_singly_linked_list PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add doubly linked list example executable
add_executable(example_doubly_linked_list doubly_linked_list_example.c)
target_link_libraries(example_doubly_linked_list PRIVATE ds_linked_list)
target_include_directories(example_doubly_linked_list PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add circular linked list example executable
add_executable(example_circular_linked_list circular_linked_list_example.c)
target_link_libraries(example_circular_linked_list PRIVATE ds_linked_list)
target_include_directories(example_circular_linked_list PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add original linked list example executable (simple singly linked list)
add_executable(example_linked_list linked_list_example.c)
target_link_libraries(example_linked_list PRIVATE ds_linked_list)
target_include_directories(example_linked_list PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add linked list iterator example executable
add_executable(example_linked_list_iterator linked_list_iterator_example.c)
target_link_libraries(example_linked_list_iterator PRIVATE ds_linked_list dsa_common_impl)
target_include_directories(example_linked_list_iterator PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Stack Examples
# Add array stack example executable
add_executable(example_array_stack array_stack_example.c)
target_link_libraries(example_array_stack PRIVATE adt_stack)
target_include_directories(example_array_stack PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add linked stack example executable
add_executable(example_linked_stack linked_stack_example.c)
target_link_libraries(example_linked_stack PRIVATE adt_stack)
target_include_directories(example_linked_stack PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Deque Examples
# Add circular array deque example executable
add_executable(example_circular_array_deque circular_array_deque_example.c)
target_link_libraries(example_circular_array_deque PRIVATE adt_deque)
target_include_directories(example_circular_array_deque PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add doubly linked deque example executable
add_executable(example_doubly_linked_deque doubly_linked_deque_example.c)
target_link_libraries(example_doubly_linked_deque PRIVATE adt_deque)
target_include_directories(example_doubly_linked_deque PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Queue Examples
# Add circular array queue example executable
add_executable(example_circular_array_queue circular_array_queue_example.c)
target_link_libraries(example_circular_array_queue PRIVATE adt_queue)
target_include_directories(example_circular_array_queue PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add doubly linked queue example executable
add_executable(example_doubly_linked_queue doubly_linked_queue_example.c)
target_link_libraries(example_doubly_linked_queue PRIVATE adt_queue)
target_include_directories(example_doubly_linked_queue PRIVATE ${CMAKE_SOURCE_DIR}/include)