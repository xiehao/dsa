# C语言数据结构与算法实现

本项目包含了一些常见数据结构和算法的C语言实现。

## 功能特性

目前已实现的数据结构包括：

*   **统一数组接口 (Unified Array Interface):** 位于 `include/dsa/array.h`
    - 支持静态数组和动态数组的统一操作接口
    - 提供类型安全的便利函数（如 `array_push_back_int`）
    - 函数式接口，便于调试和教学
    - 内部实现封装在 `src/array/` 目录
*   **链表 (Linked List):** 位于 `src/linked_list` 和 `include/dsa/linked_list.h`
*   **栈 (Stack):** 位于 `src/stack` 和 `include/dsa/stack.h` (基于统一数组接口实现)
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
build/tests/array/test_unified_array    # 统一数组接口测试
build/tests/array/test_static_array     # 静态数组测试
build/tests/array/test_array            # 动态数组测试
build/tests/linked_list/test_linked_list
build/tests/stack/test_stack
build/tests/deque/test_deque
```

(注意：你需要先成功构建项目，这些可执行文件才会存在于 `build` 目录下)

## 示例

示例代码位于 `examples` 目录下，展示了如何使用这些数据结构。

```bash
# 确保从项目根目录运行，或者先 cd build
build/examples/dsa_example      # 综合示例
build/examples/array_example    # 统一数组接口使用示例
```

(注意：你需要先成功构建项目，这些可执行文件才会存在于 `build` 目录下)

## 统一数组接口详解

### 设计特点

统一数组接口是本项目的核心特性，它提供了一个统一的API来操作静态数组和动态数组：

- **函数式接口**: 使用函数而非宏，便于调试和设置断点
- **类型安全**: 提供类型安全的便利函数，如 `array_push_back_int()`
- **统一操作**: 同一套API可以操作静态数组和动态数组
- **性能优化**: 直接分发调用，无适配器层开销
- **教学友好**: 清晰的错误信息和调试体验

### 基本用法

```c
#include <dsa/array.h>

// 创建静态数组
int buffer[10];
Array* static_arr = array_create_static(buffer, 10, sizeof(int));

// 创建动态数组
Array* dynamic_arr = array_create_dynamic(5);

// 统一的操作接口
array_push_back_int(static_arr, 42);
array_push_back_int(dynamic_arr, 42);

// 获取元素
int value;
array_get_int(static_arr, 0, &value);   // value = 42
array_get_int(dynamic_arr, 0, &value);  // value = 42

// 查询数组信息
printf("静态数组类型: %s\n", array_get_type_name(static_arr));
printf("动态数组类型: %s\n", array_get_type_name(dynamic_arr));

// 清理
array_destroy(static_arr);
array_destroy(dynamic_arr);
```

### 架构优势

1. **封装性**: 内部实现细节隐藏在 `src/array/internal/` 目录
2. **可扩展性**: 易于添加新的数组类型
3. **向后兼容**: 保持原有静态数组和动态数组API的兼容性
4. **模块化**: 清晰的模块边界，便于维护和测试