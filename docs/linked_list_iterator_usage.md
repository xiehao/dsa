# 链表迭代器使用指南

## 概述

链表迭代器为单链表、双链表和循环链表提供了统一的遍历接口。迭代器支持正向遍历，双链表还支持反向遍历，并提供了类型安全的元素访问方式。

## 特性

- **统一接口**：单链表、双链表和循环链表使用相同的迭代器API
- **正向遍历**：所有链表类型都支持正向遍历
- **反向遍历**：仅双链表支持反向遍历
- **读写操作**：支持读取和修改元素值
- **类型安全**：通过trait模式确保类型安全
- **内存安全**：自动管理迭代器生命周期
- **错误处理**：完整的边界检查和错误码返回

## 支持的链表类型

### 单链表 (Singly Linked List)
- **正向遍历**：✅ 支持
- **反向遍历**：❌ 不支持
- **时间复杂度**：O(1) 移动到下一个元素

### 双链表 (Doubly Linked List)
- **正向遍历**：✅ 支持
- **反向遍历**：✅ 支持
- **时间复杂度**：O(1) 移动到下一个/前一个元素

### 循环链表 (Circular Linked List)
- **正向遍历**：✅ 支持
- **反向遍历**：❌ 不支持
- **时间复杂度**：O(1) 移动到下一个元素

## 基本用法

### 包含头文件

```c
#include <ds/linked_list.h>
#include <ds/linked_list_iterator.h>
#include <a/iterator.h>
```

### 正向遍历

```c
// 创建链表并添加元素
dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
int data[] = {10, 20, 30, 40, 50};
for (int i = 0; i < 5; i++) {
    linked_list_push_back(list, &data[i]);
}

// 使用迭代器遍历
dsa_iterator_t *iter = linked_list_begin(list);
while (iterator_is_valid(iter)) {
    int *value = (int *)iterator_get_value(iter);
    printf("%d ", *value);
    iterator_next(iter);
}

// 清理
iterator_destroy(iter);
linked_list_destroy(list);
```

### 反向遍历（仅双链表）

```c
// 从末尾开始反向遍历
dsa_iterator_t *iter = linked_list_end(list);
iterator_prev(iter);  // 移动到最后一个有效元素

while (iterator_is_valid(iter)) {
    int *value = (int *)iterator_get_value(iter);
    printf("%d ", *value);
    iterator_prev(iter);
}

iterator_destroy(iter);
```

### 使用end迭代器进行边界检查

```c
dsa_iterator_t *iter = linked_list_begin(list);
dsa_iterator_t *end_iter = linked_list_end(list);

while (iterator_is_valid(iter) && !iterator_equals(iter, end_iter)) {
    // 处理元素
    iterator_next(iter);
}

iterator_destroy(iter);
iterator_destroy(end_iter);
```

### 修改元素值

```c
dsa_iterator_t *iter = linked_list_begin(list);
int new_value = 100;

if (iterator_is_valid(iter)) {
    dsa_result_t result = iterator_set_value(iter, &new_value);
    if (result == DSA_SUCCESS) {
        printf("元素值修改成功\n");
    }
}

iterator_destroy(iter);
```

## API 参考

### 创建迭代器

- `dsa_iterator_t *linked_list_begin(const dsa_linked_list_t *linked_list)`
  - 创建指向链表第一个元素的迭代器
  - 返回：迭代器指针，失败时返回NULL

- `dsa_iterator_t *linked_list_end(const dsa_linked_list_t *linked_list)`
  - 创建指向链表末尾（最后一个元素之后）的迭代器
  - 返回：迭代器指针，失败时返回NULL

### 迭代器操作

- `dsa_iterator_t *iterator_next(dsa_iterator_t *iter)`
  - 将迭代器移动到下一个元素
  - 返回：迭代器本身

- `dsa_iterator_t *iterator_prev(dsa_iterator_t *iter)`
  - 将迭代器移动到前一个元素（仅双链表支持）
  - 返回：迭代器本身，不支持时返回NULL

- `dsa_element_pt iterator_get_value(dsa_iterator_t *iter)`
  - 获取迭代器当前指向的元素
  - 返回：元素指针，无效时返回NULL

- `dsa_result_t iterator_set_value(dsa_iterator_t *iter, dsa_element_pt value)`
  - 设置迭代器当前指向的元素值
  - 参数：iter - 迭代器指针，value - 新值指针
  - 返回：操作结果，成功返回DSA_SUCCESS

- `bool iterator_is_valid(dsa_iterator_t *iter)`
  - 检查迭代器是否有效
  - 返回：有效返回true，否则返回false

- `bool iterator_equals(dsa_iterator_t *iter1, dsa_iterator_t *iter2)`
  - 比较两个迭代器是否相等
  - 返回：相等返回true，否则返回false

- `void iterator_destroy(dsa_iterator_t *iter)`
  - 销毁迭代器并释放内存
  - 注意：销毁后的迭代器指针不能再使用

## 注意事项

1. **内存管理**：使用完迭代器后必须调用 `iterator_destroy()` 释放内存
2. **反向遍历限制**：只有双链表支持反向遍历，单链表和循环链表不支持
3. **边界检查**：在使用迭代器前应检查 `iterator_is_valid()`
4. **元素修改**：只有当迭代器指向有效元素时才能设置值
5. **迭代器失效**：修改链表结构（插入/删除节点）可能导致迭代器失效

## 错误处理

迭代器操作可能返回以下错误码：

- `DSA_SUCCESS`：操作成功
- `DSA_ERROR_INVALID_PARAMETER`：参数无效
- `DSA_ERROR_INDEX_OUT_OF_BOUNDS`：迭代器指向无效位置

## 性能特性

- **时间复杂度**：
  - 移动到下一个/前一个元素：O(1)
  - 获取/设置元素值：O(1)
  - 创建迭代器：O(1)

- **空间复杂度**：O(1) 每个迭代器

## 示例程序

完整的示例程序可以在 `examples/linked_list_iterator_example.c` 中找到，演示了：

- 单链表迭代器的使用
- 双链表迭代器的双向遍历
- 循环链表迭代器的使用
- 迭代器修改元素值的功能

运行示例：
```bash
./build/examples/example_linked_list_iterator
```
