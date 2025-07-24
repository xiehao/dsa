# DSA - C语言数据结构与算法实现

一个现代化的C语言数据结构库，提供完整的数据结构实现、全面的单元测试和丰富的示例代码。

## 🌟 项目特色

- **🏗️ 现代化设计**: 使用CMake构建系统，支持模块化开发
- **🔒 类型安全**: 提供类型安全的API接口和错误处理
- **📚 教育友好**: 详细的文档、注释和示例代码
- **🧪 测试完备**: 使用CMocka框架进行全面的单元测试
- **⚡ 性能优化**: 高效的内存管理和算法实现
- **🎯 实用性强**: 包含实际应用场景的示例

## 📁 项目结构

```
dsa/
├── CMakeLists.txt              # CMake主配置文件
├── README.md                   # 项目文档
├── include/                    # 公共头文件目录
│   ├── common.h                # 通用定义和宏
│   ├── adt/                    # 抽象数据类型公共接口
│   │   ├── deque.h             # 双端队列接口
│   │   ├── queue.h             # 普通队列接口
│   │   └── stack.h             # 下压栈接口
│   └── ds/                     # 数据结构公共接口
│       ├── array_list.h        # 数组列表接口
│       └── linked_list.h       # 链表接口
├── src/                        # 源代码实现目录
│   ├── array_list/             # 数组列表实现
│   │   ├── internal/           # 内部头文件
│   │   ├── array_list.c        # 工厂函数实现
│   │   ├── static_array_list.c # 静态数组实现
│   │   └── dynamic_array_list.c# 动态数组实现
│   ├── linked_list/            # 链表实现
│   │   ├── internal/           # 内部头文件
│   │   ├── linked_list.c       # 工厂函数实现
│   │   ├── singly_linked_list.c# 单链表实现
│   │   ├── doubly_linked_list.c# 双链表实现
│   │   └── circular_linked_list.c # 循环链表实现
│   ├── stack/                  # 栈实现
│   │   ├── internal/           # 内部头文件
│   │   ├── stack.c             # 工厂函数实现
│   │   ├── array_stack.c       # 数组栈实现
│   │   └── linked_stack.c      # 链表栈实现
│   ├── deque/                  # 双端队列实现
│   │   ├── internal/           # 内部头文件
│   │   ├── deque.c             # 工厂函数实现
│   │   ├── circular_array_deque.c # 循环数组双端队列实现
│   │   └── doubly_linked_deque.c  # 双向链表双端队列实现
│   └── queue/                  # 队列实现
│       ├── internal/           # 内部头文件
│       ├── queue.c             # 工厂函数实现
│       ├── circular_array_queue.c # 循环数组队列实现
│       └── doubly_linked_queue.c  # 双向链表队列实现
├── tests/                      # 单元测试代码
│   ├── array_list/             # 数组测试
│   ├── linked_list/            # 链表测试
│   ├── stack/                  # 栈测试
│   ├── deque/                  # 双端队列测试
│   └── queue/                  # 队列测试
└── examples/                   # 使用示例和演示
    ├── *_example.c             # 各种数据结构的专用示例
    └── CMakeLists.txt          # 示例构建配置
```

## 🚀 已实现的数据结构

### 📊 数组列表 (Array List)

- **静态数组**: 固定容量，栈上分配，高性能
- **动态数组**: 自动扩容，堆上分配，灵活性强
- **统一接口**: 相同的API操作不同类型的数组
- **类型安全**: 使用宏提供类型安全的元素访问

### 🔗 链表 (Linked List)

- **单链表**: 单向链接，内存效率高
- **双链表**: 双向链接，支持高效的双向遍历
- **循环链表**: 尾节点连接头节点，适合循环操作
- **统一接口**: 相同的API操作不同类型的链表

### 📚 栈 (Stack)

- **数组栈**: 基于动态数组实现，LIFO访问模式，内存局部性好
- **链表栈**: 基于单链表实现，完全动态内存分配，无容量限制
- **高效操作**: O(1)时间复杂度的压入、弹出、查看栈顶
- **统一接口**: 相同的API操作不同类型的栈实现
- **类型安全**: 支持运行时类型识别和多态操作

### 🔄 双端队列 (Deque)

- **循环数组双端队列**: 基于循环数组实现，内存局部性好，支持自动扩容
- **双向链表双端队列**: 基于双向链表实现，完全动态内存分配，无容量限制
- **高效操作**: O(1)时间复杂度的两端插入和删除
- **统一接口**: 相同的API操作不同类型的双端队列实现
- **类型安全**: 支持运行时类型识别和多态操作

### 🎫 队列 (Queue)

- **循环数组队列**: 基于循环数组实现，FIFO访问模式，内存局部性优秀
- **双向链表队列**: 基于双向链表实现，动态内存分配，无容量限制
- **高效操作**: O(1)时间复杂度的入队和出队操作
- **统一接口**: 相同的API操作不同类型的队列实现
- **类型安全**: 支持运行时类型识别和多态操作
- **FIFO特性**: 严格的先进先出访问模式，适合任务调度和缓冲区管理

### 🎯 核心特性

- **工厂模式**: 通过类型参数创建不同的数据结构实例
- **内存管理**: 自动内存管理和清理功能
- **错误处理**: 完善的错误码和边界检查
- **性能优化**: 针对不同场景优化的算法实现

## 🛠️ 构建项目

本项目使用现代CMake进行构建，支持多种编译器和平台。

### 📋 前提条件

- **CMake**: 3.10 或更高版本
- **C编译器**: GCC, Clang, 或 MSVC
- **CMocka**: 单元测试框架（可选，用于运行测试）

### 🔧 构建步骤

1. **克隆项目**

   ```bash
   git clone <repository-url>
   cd dsa
   ```
2. **配置项目**

   ```bash
   # 推荐：包含编译命令导出和测试
   cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_TESTING=ON

   # 或者简单配置
   cmake -B build -S .
   ```
3. **构建项目**

   ```bash
   cmake --build build
   ```
4. **安装（可选）**

   ```bash
   cmake --install build --prefix /usr/local
   ```

编译后的库和可执行文件将位于 `build` 目录下。

## 🧪 运行测试

项目使用CMocka框架进行全面的单元测试，覆盖所有数据结构的核心功能。

### 运行所有测试

```bash
cd build
ctest --output-on-failure
```

### 运行特定测试

```bash
# 数组列表测试
./build/tests/array_list/test_static_array_list
./build/tests/array_list/test_dynamic_array_list

# 链表测试
./build/tests/linked_list/test_singly_linked_list
./build/tests/linked_list/test_doubly_linked_list
./build/tests/linked_list/test_circular_linked_list

# 栈测试
./build/tests/stack/test_stack              # 通用栈测试
./build/tests/stack/test_array_stack        # 数组栈专用测试
./build/tests/stack/test_linked_stack       # 链表栈专用测试

# 双端队列测试
./build/tests/deque/test_circular_array_deque   # 循环数组双端队列测试
./build/tests/deque/test_doubly_linked_deque    # 双向链表双端队列测试

# 队列测试
./build/tests/queue/test_queue                  # 通用队列测试
./build/tests/queue/test_circular_array_queue   # 循环数组队列专用测试
./build/tests/queue/test_doubly_linked_queue    # 双向链表队列专用测试
```

### 测试覆盖范围

- ✅ 基本操作（创建、插入、删除、查询）
- ✅ 边界条件（空容器、索引越界、内存不足）
- ✅ 错误处理（NULL指针、无效参数）
- ✅ 内存管理（内存泄漏检测）
- ✅ 性能测试（大数据量操作）

## 📚 示例代码

项目提供了丰富的示例代码，每种数据结构都有专门的演示程序。

### 🎯 专用示例

```bash
# 数组列表示例
./build/examples/example_static_array_list      # 静态数组演示
./build/examples/example_dynamic_array_list     # 动态数组演示

# 链表示例
./build/examples/example_singly_linked_list     # 单链表演示
./build/examples/example_doubly_linked_list     # 双链表演示
./build/examples/example_circular_linked_list   # 循环链表演示

# 栈示例
./build/examples/example_array_stack            # 数组栈演示
./build/examples/example_linked_stack           # 链表栈演示

# 双端队列示例
./build/examples/example_circular_array_deque   # 循环数组双端队列演示
./build/examples/example_doubly_linked_deque    # 双向链表双端队列演示

# 队列示例
./build/examples/example_circular_array_queue   # 循环数组队列演示
./build/examples/example_doubly_linked_queue    # 双向链表队列演示
```

### 🔄 综合示例

```bash
./build/examples/example_array_list             # 数组综合演示
./build/examples/example_linked_list            # 链表综合演示
```

### 🌟 示例特色

- **📖 教育性强**: 详细的操作说明和结果展示
- **🎨 美观输出**: 使用emoji和格式化输出
- **🎯 实用场景**: 包含实际应用场景的模拟
- **⚡ 性能分析**: 展示不同数据结构的性能特点
- **🆚 对比分析**: 不同数据结构之间的对比

## 💻 API 使用指南

### 📊 数组列表 API

#### 创建数组

```c
#include <ds/array_list.h>

// 创建静态数组（栈上分配）
int buffer[10];
dsa_array_list_t *static_arr = array_list_create_static(buffer, 10, sizeof(int));

// 创建动态数组（堆上分配）
dsa_array_list_t *dynamic_arr = array_list_create_dynamic(5);
```

#### 基本操作

```c
// 添加元素
int value = 42;
array_list_push_back(arr, &value);

// 插入元素
array_list_insert(arr, 1, &value);

// 获取元素
dsa_element_pt element = array_list_get(arr, 0);
int retrieved_value = ELEMENT_VALUE(int, element);

// 设置元素
array_list_set(arr, 0, &new_value);

// 删除元素
dsa_element_pt removed = array_list_remove(arr, 0);
dsa_element_pt popped = array_list_pop_back(arr);

// 查询状态
size_t size = array_list_size(arr);
size_t capacity = array_list_capacity(arr);
bool is_empty = array_list_is_empty(arr);
bool is_full = array_list_is_full(arr);  // 仅静态数组

// 清理
array_list_clear(arr);              // 清空元素
array_list_clear_with_free(arr);    // 清空并释放元素内存
array_list_destroy(arr);            // 销毁数组
```

### 🔗 链表 API

#### 创建链表

```c
#include <ds/linked_list.h>

// 创建不同类型的链表
dsa_linked_list_t *singly = linked_list_create(LINKED_LIST_TYPE_SINGLY);
dsa_linked_list_t *doubly = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
dsa_linked_list_t *circular = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
```

#### 基本操作

```c
// 插入元素
int *data = malloc(sizeof(int));
*data = 42;
linked_list_insert_at(list, 0, data);

// 获取元素
dsa_element_pt element = linked_list_get(list, 0);
int value = *(int*)element;

// 设置元素
int *new_data = malloc(sizeof(int));
*new_data = 99;
linked_list_set(list, 0, new_data);

// 删除元素
dsa_element_pt removed = linked_list_remove_at(list, 0);
free(removed);

// 查询状态
size_t size = linked_list_size(list);
bool is_empty = linked_list_is_empty(list);

// 清理
linked_list_clear(list);
linked_list_destroy(list);
```

### 🔄 双端队列 API

#### 创建双端队列

```c
#include <adt/deque.h>

// 创建循环数组双端队列
dsa_deque_t *circular_deque = deque_create(DEQUE_TYPE_CIRCULAR_ARRAY_LIST);

// 创建双向链表双端队列
dsa_deque_t *linked_deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
```

#### 基本操作

```c
// 双端插入
int *data1 = malloc(sizeof(int));
*data1 = 42;
deque_add_first(deque, data1);  // 前端插入

int *data2 = malloc(sizeof(int));
*data2 = 99;
deque_add_last(deque, data2);   // 后端插入

// 双端删除
dsa_element_pt first = deque_remove_first(deque);   // 前端删除
dsa_element_pt last = deque_remove_last(deque);     // 后端删除

// 查看元素（不删除）
dsa_element_pt peek_first = deque_peek_first(deque);
dsa_element_pt peek_last = deque_peek_last(deque);

// 查询状态
size_t size = deque_size(deque);
bool is_empty = deque_is_empty(deque);
bool is_full = deque_is_full(deque);        // 仅循环数组实现
size_t capacity = deque_capacity(deque);    // 仅循环数组实现

// 获取类型信息
dsa_deque_type_t type = deque_get_type(deque);
const char *type_name = deque_get_type_name(deque);

// 清理
deque_clear(deque);                 // 清空元素
deque_clear_with_free(deque);       // 清空并释放元素内存
deque_destroy(deque);               // 销毁双端队列
```

### 🎫 队列 API

#### 创建队列

```c
#include <adt/queue.h>

// 创建循环数组队列
dsa_queue_t *circular_queue = queue_create(QUEUE_TYPE_CIRCULAR_ARRAY_LIST);

// 创建双向链表队列
dsa_queue_t *linked_queue = queue_create(QUEUE_TYPE_DOUBLY_LINKED_LIST);
```

#### 基本操作

```c
// 入队操作（FIFO - 先进先出）
int *data1 = malloc(sizeof(int));
*data1 = 42;
dsa_result_t result = queue_enqueue(queue, data1);  // 元素加入队尾

int *data2 = malloc(sizeof(int));
*data2 = 99;
queue_enqueue(queue, data2);

// 出队操作（FIFO - 先进先出）
dsa_element_pt first_out = queue_dequeue(queue);   // 队首元素出队
// first_out 指向 data1 (值为42)，因为它先入队

dsa_element_pt second_out = queue_dequeue(queue);  // 下一个元素出队
// second_out 指向 data2 (值为99)

// 查询状态
size_t size = queue_size(queue);
bool is_empty = queue_is_empty(queue);

// 获取类型信息
dsa_queue_type_t type = queue_get_type(queue);
const char *type_name = queue_get_type_name(queue);

// 清理
queue_clear(queue);                 // 清空队列
queue_destroy(queue);               // 销毁队列
```

#### FIFO特性演示

```c
// 演示队列的先进先出特性
printf("=== 队列FIFO特性演示 ===\n");

dsa_queue_t *queue = queue_create(QUEUE_TYPE_CIRCULAR_ARRAY_LIST);

// 依次入队
for (int i = 1; i <= 5; i++) {
    int *data = malloc(sizeof(int));
    *data = i * 10;
    queue_enqueue(queue, data);
    printf("入队: %d\n", i * 10);
}

// 依次出队（按入队顺序）
printf("出队顺序:\n");
while (!queue_is_empty(queue)) {
    dsa_element_pt element = queue_dequeue(queue);
    printf("出队: %d\n", *(int*)element);
    free(element);
}
// 输出: 10, 20, 30, 40, 50 (与入队顺序相同)

queue_destroy(queue);
```

### 🎯 设计原则

1. **统一接口**: 相同的API操作不同类型的数据结构
2. **类型安全**: 使用宏和类型检查确保安全性
3. **内存管理**: 明确的内存所有权和清理责任
4. **错误处理**: 完善的错误码和边界检查
5. **性能优化**: 针对不同场景的优化实现

### 🔧 错误处理

```c
// 所有修改操作都返回错误码
dsa_result_t result = array_list_push_back(arr, &value);
if (result != DSA_SUCCESS) {
    switch (result) {
        case DSA_ERROR_NULL_POINTER:
            printf("空指针错误\n");
            break;
        case DSA_ERROR_MEMORY_ALLOCATION:
            printf("内存分配失败\n");
            break;
        case DSA_ERROR_INDEX_OUT_OF_BOUNDS:
            printf("索引越界\n");
            break;
        case DSA_ERROR_CONTAINER_FULL:
            printf("容器已满\n");
            break;
        default:
            printf("未知错误\n");
    }
}
```

## 📈 性能特点

### 数组列表性能对比

| 操作     | 静态数组  | 动态数组  | 说明                 |
| -------- | --------- | --------- | -------------------- |
| 随机访问 | O(1) ✅   | O(1) ✅   | 都支持常数时间访问   |
| 尾部插入 | O(1) ✅   | O(1)* ✅  | 动态数组偶尔需要扩容 |
| 中间插入 | O(n) ⚠️ | O(n) ⚠️ | 需要移动后续元素     |
| 删除操作 | O(n) ⚠️ | O(n) ⚠️ | 需要移动后续元素     |
| 内存开销 | 低 ✅     | 中等 ⚠️ | 静态数组无额外开销   |
| 扩容能力 | 无 ❌     | 自动 ✅   | 动态数组可自动扩容   |

### 链表性能对比

| 操作         | 单链表    | 双链表    | 循环链表  | 说明                 |
| ------------ | --------- | --------- | --------- | -------------------- |
| 头部插入     | O(1) ✅   | O(1) ✅   | O(1) ✅   | 都支持高效头部操作   |
| 尾部插入     | O(n) ⚠️ | O(1) ✅   | O(1) ✅   | 双链表和循环链表更优 |
| 随机访问     | O(n) ⚠️ | O(n) ⚠️ | O(n) ⚠️ | 都需要遍历查找       |
| 删除已知节点 | O(n) ⚠️ | O(1) ✅   | O(1) ✅   | 双链表优势明显       |
| 反向遍历     | 不支持 ❌ | 支持 ✅   | 支持 ✅   | 双向链接的优势       |
| 内存开销     | 低 ✅     | 高 ⚠️   | 中等 ⚠️ | 指针数量影响开销     |

### 双端队列性能对比

| 操作       | 循环数组双端队列 | 双向链表双端队列 | 说明                       |
| ---------- | ---------------- | ---------------- | -------------------------- |
| 前端插入   | O(1) ✅          | O(1) ✅          | 都支持高效前端操作         |
| 后端插入   | O(1) ✅          | O(1) ✅          | 都支持高效后端操作         |
| 前端删除   | O(1) ✅          | O(1) ✅          | 都支持高效前端删除         |
| 后端删除   | O(1) ✅          | O(1) ✅          | 都支持高效后端删除         |
| 随机访问   | 不支持 ❌        | 不支持 ❌        | 双端队列不提供随机访问     |
| 内存局部性 | 好 ✅            | 差 ⚠️          | 数组连续存储优势明显       |
| 内存开销   | 低 ✅            | 高 ⚠️          | 链表需要额外指针开销       |
| 扩容能力   | 自动 ✅          | 无限 ✅          | 数组自动扩容，链表理论无限 |
| 扩容成本   | O(n) ⚠️        | 无 ✅            | 数组扩容需要复制元素       |

### 队列性能对比

| 操作       | 循环数组队列  | 双向链表队列 | 说明                                   |
| ---------- | ------------- | ------------ | -------------------------------------- |
| 入队操作   | O(1)* ✅      | O(1) ✅      | 循环数组平摊常数时间，链表严格常数时间 |
| 出队操作   | O(1) ✅       | O(1) ✅      | 都支持高效的FIFO出队                   |
| 查看队首   | 不支持 ❌     | 不支持 ❌    | 队列ADT不提供peek操作                  |
| 随机访问   | 不支持 ❌     | 不支持 ❌    | 队列只支持FIFO访问模式                 |
| 内存局部性 | 好 ✅         | 差 ⚠️      | 数组连续存储，缓存友好                 |
| 内存开销   | 低 ✅         | 高 ⚠️      | 链表需要额外指针开销                   |
| 扩容能力   | 自动 ✅       | 无限 ✅      | 数组自动扩容，链表理论无限             |
| 扩容成本   | O(n) ⚠️     | 无 ✅        | 数组扩容需要复制元素                   |
| 适用场景   | 高性能应用 ✅ | 动态场景 ✅  | 数组适合性能敏感，链表适合大小变化     |

## 🎓 学习资源

### 推荐学习路径

1. **基础概念**: 先了解数据结构的基本概念
2. **静态数组**: 从最简单的静态数组开始
3. **动态数组**: 学习内存管理和扩容机制
4. **单链表**: 理解指针和链式结构
5. **双链表**: 掌握双向链接的优势
6. **循环链表**: 了解特殊的循环结构
7. **栈结构**: 学习LIFO访问模式和应用场景
8. **双端队列**: 掌握双端操作和不同实现方式的权衡
9. **队列结构**: 理解FIFO访问模式和任务调度应用

### 实践建议

- 🔍 **阅读源码**: 查看 `src/` 目录下的实现代码
- 🧪 **运行测试**: 执行单元测试了解边界情况
- 🎯 **运行示例**: 体验不同数据结构的特点
- 📝 **修改代码**: 尝试添加新功能或优化
- 🐛 **调试练习**: 使用调试器跟踪程序执行

## 🤝 贡献指南

欢迎贡献代码！请遵循以下步骤：

1. **Fork** 项目到你的GitHub账户
2. **创建分支** 用于你的功能开发
3. **编写代码** 并确保通过所有测试
4. **添加测试** 覆盖新功能
5. **更新文档** 包括README和代码注释
6. **提交PR** 并描述你的更改

### 代码规范

- 使用一致的命名约定（snake_case）
- 添加详细的函数注释
- 确保所有测试通过
- 遵循现有的代码风格

## 📄 许可证

本项目采用 [MIT许可证](LICENSE)，你可以自由使用、修改和分发。

## 🙏 致谢

感谢所有贡献者和使用者，让这个项目变得更好！

---

**Happy Coding! 🚀**
