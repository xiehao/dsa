# 数组列表 API 文档

数组列表提供了统一的接口来操作静态数组和动态数组，支持高效的随机访问和尾部操作。

## 📊 概述

数组列表支持两种实现：
- **静态数组**: 固定容量，栈上分配，高性能
- **动态数组**: 自动扩容，堆上分配，灵活性强

## 🔧 头文件

```c
#include <ds/array_list.h>
```

## 🏗️ 创建和销毁

### 创建数组

```c
// 创建静态数组（栈上分配）
int buffer[10];
dsa_array_list_t *static_arr = array_list_create_static(buffer, 10, sizeof(int));

// 创建动态数组（堆上分配）
dsa_array_list_t *dynamic_arr = array_list_create_dynamic(5);
```

### 销毁数组

```c
// 销毁数组（释放数组结构，但不释放元素）
array_list_destroy(arr);
```

## 📝 基本操作

### 添加元素

```c
// 在尾部添加元素
int value = 42;
dsa_result_t result = array_list_push_back(arr, &value);

// 在指定位置插入元素
result = array_list_insert(arr, 1, &value);
```

### 获取和设置元素

```c
// 获取元素
dsa_element_pt element = array_list_get(arr, 0);
int retrieved_value = ELEMENT_VALUE(int, element);

// 设置元素
int new_value = 99;
result = array_list_set(arr, 0, &new_value);
```

### 删除元素

```c
// 删除指定位置的元素
dsa_element_pt removed = array_list_remove(arr, 0);

// 删除尾部元素
dsa_element_pt popped = array_list_pop_back(arr);
```

## 📊 查询操作

### 状态查询

```c
// 获取元素数量
size_t size = array_list_size(arr);

// 获取容量
size_t capacity = array_list_capacity(arr);

// 检查是否为空
bool is_empty = array_list_is_empty(arr);

// 检查是否已满（仅静态数组）
bool is_full = array_list_is_full(arr);
```

### 类型信息

```c
// 获取数组类型
dsa_array_list_type_t type = array_list_get_type(arr);

// 获取类型名称
const char *type_name = array_list_get_type_name(arr);
```

## 🧹 清理操作

```c
// 清空元素（不释放元素内存）
array_list_clear(arr);

// 清空并释放元素内存
array_list_clear_with_free(arr);
```

## ⚠️ 错误处理

所有修改操作都返回 `dsa_result_t` 类型的错误码：

```c
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

| 操作     | 静态数组  | 动态数组  | 说明                 |
| -------- | --------- | --------- | -------------------- |
| 随机访问 | O(1) ✅   | O(1) ✅   | 都支持常数时间访问   |
| 尾部插入 | O(1) ✅   | O(1)* ✅  | 动态数组偶尔需要扩容 |
| 中间插入 | O(n) ⚠️ | O(n) ⚠️ | 需要移动后续元素     |
| 删除操作 | O(n) ⚠️ | O(n) ⚠️ | 需要移动后续元素     |
| 内存开销 | 低 ✅     | 中等 ⚠️ | 静态数组无额外开销   |
| 扩容能力 | 无 ❌     | 自动 ✅   | 动态数组可自动扩容   |

## 💡 使用建议

1. **选择合适的类型**：
   - 如果元素数量固定且已知，使用静态数组
   - 如果需要动态调整大小，使用动态数组

2. **内存管理**：
   - 静态数组的buffer由调用者管理
   - 动态数组会自动管理内部存储
   - 元素的内存始终由调用者负责

3. **性能优化**：
   - 尽量使用尾部操作（push_back/pop_back）
   - 避免频繁的中间插入和删除
   - 对于动态数组，可以预先分配足够的容量

---

返回 [文档目录](README.md) | 查看 [链表 API](linked_list_api.md)
