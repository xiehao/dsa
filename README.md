# C语言数据结构与算法实现

本项目包含了一些常见数据结构和算法的C语言实现。

## 功能特性

目前已实现的数据结构包括：

*   **动态数组 (Dynamic Array):** 位于 `src/dynamic_array` 和 `include/dsa/dynamic_array.h`
*   **链表 (Linked List):** 位于 `src/linked_list` 和 `include/dsa/linked_list.h`
*   **栈 (Stack):** 位于 `src/stack` 和 `include/dsa/stack.h` (可能基于其他数据结构实现)
*   **双端队列 (Deque):** 位于 `src/deque` 和 `include/dsa/deque.h`

## 构建项目

本项目使用 CMake 进行构建。

1.  **配置项目 (包含测试):**
    为了确保包含单元测试，推荐使用 `-DBUILD_TESTING=ON` 选项：
    ```bash
    cmake -S . -B build -DBUILD_TESTING=ON
    ```
    如果不需要构建测试，可以省略该选项：
    ```bash
    cmake -S . -B build
    ```

2.  **构建项目:**
    ```bash
    cmake --build build
    ```

编译后的库和可执行文件将位于 `build` 目录下。

## 运行测试

项目包含针对各个数据结构的单元测试，位于 `tests` 目录下。

在 `build` 目录下，运行以下命令来执行所有测试：

```bash
ctest --test-dir build --output-on-failure
```

或者，你可以运行特定的测试可执行文件，例如：

```bash
# 确保从项目根目录运行，或者先 cd build
build/tests/dynamic_array/test_dynamic_array
build/tests/linked_list/test_linked_list
build/tests/stack/test_stack
build/tests/deque/test_deque
```

(注意：你需要先成功构建项目，这些可执行文件才会存在于 `build` 目录下)

## 示例

示例代码位于 `examples` 目录下，展示了如何使用这些数据结构。

```bash
# 确保从项目根目录运行，或者先 cd build
build/examples/test_main
```

(注意：你需要先成功构建项目，这个可执行文件才会存在于 `build` 目录下)