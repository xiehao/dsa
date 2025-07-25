# 数组列表迭代器使用指南

## 概述

数组列表迭代器为静态数组和动态数组提供了统一的遍历接口。迭代器支持正向和反向遍历，并提供了类型安全的元素访问方式。

## 特性

- **统一接口**：静态数组和动态数组使用相同的迭代器API
- **双向遍历**：支持正向和反向遍历
- **读写操作**：支持读取和修改元素值
- **类型安全**：通过trait模式确保类型安全
- **内存安全**：自动管理迭代器生命周期
- **错误处理**：完整的边界检查和错误码返回
- **高性能**：基于索引的O(1)访问和移动

## 基本用法

### 包含头文件

```c
#include <ds/array_list.h>
#include <a/iterator.h>
```

### 正向遍历

```c
// 创建数组并添加元素
int buffer[5];
dsa_array_list_t *arr = array_list_create_static(buffer, 5, sizeof(int));

for (int i = 0; i < 3; i++) {
    int value = (i + 1) * 10;
    array_list_push_back(arr, &value);
}

// 使用迭代器遍历
dsa_iterator_t *iter = array_list_begin(arr);
while (iterator_is_valid(iter)) {
    int *value = (int *)iterator_get_value(iter);
    printf("%d ", *value);
    iterator_next(iter);
}

// 清理
iterator_destroy(iter);
array_list_destroy(arr);
```

### 反向遍历

```c
// 从末尾开始反向遍历
dsa_iterator_t *iter = array_list_end(arr);
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
dsa_iterator_t *iter = array_list_begin(arr);
dsa_iterator_t *end_iter = array_list_end(arr);

while (iterator_is_valid(iter) && !iterator_equals(iter, end_iter)) {
    // 处理元素
    iterator_next(iter);
}

iterator_destroy(iter);
iterator_destroy(end_iter);
```

### 读写操作

```c
// 修改元素值
dsa_iterator_t *iter = array_list_begin(arr);
while (iterator_is_valid(iter)) {
    int *value = (int *)iterator_get_value(iter);
    if (*value < 0) {
        int new_value = -*value;  // 将负数变为正数
        dsa_result_t result = iterator_set_value(iter, &new_value);
        if (result != DSA_SUCCESS) {
            printf("设置值失败: %d\n", result);
        }
    }
    iterator_next(iter);
}
iterator_destroy(iter);
```

### 条件修改

```c
// 对满足条件的元素进行修改
dsa_iterator_t *iter = array_list_begin(arr);
while (iterator_is_valid(iter)) {
    int *value = (int *)iterator_get_value(iter);
    if (*value % 2 == 0) {  // 偶数
        int new_value = *value * 2;
        iterator_set_value(iter, &new_value);
    }
    iterator_next(iter);
}
iterator_destroy(iter);
```

## API 参考

### 创建迭代器

- `dsa_iterator_t *array_list_begin(const dsa_array_list_t *array)`
  - 创建指向数组第一个元素的迭代器
  - 返回：迭代器指针，失败时返回NULL

- `dsa_iterator_t *array_list_end(const dsa_array_list_t *array)`
  - 创建指向数组末尾（最后一个元素之后）的迭代器
  - 返回：迭代器指针，失败时返回NULL

### 迭代器操作

- `dsa_iterator_t *iterator_next(dsa_iterator_t *iter)`
  - 将迭代器移动到下一个元素
  - 返回：迭代器本身

- `dsa_iterator_t *iterator_prev(dsa_iterator_t *iter)`
  - 将迭代器移动到前一个元素
  - 返回：迭代器本身

- `dsa_element_pt iterator_get_value(dsa_iterator_t *iter)`
  - 获取迭代器当前指向的元素
  - 返回：元素指针，无效时返回NULL

- `dsa_result_t iterator_set_value(dsa_iterator_t *iter, dsa_element_pt value)`
  - 设置迭代器当前指向的元素值
  - 参数：iter - 迭代器指针，value - 新值指针
  - 返回：操作结果，成功返回DSA_SUCCESS
  - 注意：只有当迭代器指向有效元素时才能设置值

- `bool iterator_is_valid(dsa_iterator_t *iter)`
  - 检查迭代器是否指向有效元素
  - 返回：有效时返回true，否则返回false

- `bool iterator_equals(dsa_iterator_t *iter1, dsa_iterator_t *iter2)`
  - 比较两个迭代器是否相等
  - 返回：相等时返回true，否则返回false

- `void iterator_destroy(dsa_iterator_t *iter)`
  - 销毁迭代器并释放内存
  - 参数：要销毁的迭代器指针

## 注意事项

1. **内存管理**：使用完迭代器后必须调用`iterator_destroy()`释放内存
2. **容器修改**：在迭代过程中修改容器结构（添加/删除元素）可能导致迭代器失效
3. **元素修改**：可以安全地修改元素值，不会影响迭代器有效性
4. **边界检查**：使用`iterator_is_valid()`检查迭代器有效性
5. **错误处理**：`iterator_set_value()`返回错误码，应检查操作是否成功
6. **NULL安全**：所有API函数都能安全处理NULL指针
7. **类型转换**：`iterator_get_value()`返回`void*`，需要转换为正确类型
8. **动态数组特殊性**：对动态数组使用`iterator_set_value()`时，旧元素会被自动释放

## 性能特性

- **时间复杂度**：
  - 创建迭代器：O(1)
  - 移动迭代器：O(1)
  - 获取元素：O(1)
  - 比较迭代器：O(1)

- **空间复杂度**：O(1)

## 示例程序

完整的示例程序请参考：
- `examples/array_list_iterator_demo.c` - 基本使用演示
- `tests/array_list/test_array_iterator.c` - 单元测试示例

## 设计原理

迭代器采用基于索引的实现方式，具有以下优势：

1. **统一性**：静态数组和动态数组使用相同的迭代器结构
2. **效率**：基于索引的访问提供O(1)性能
3. **安全性**：通过trait模式确保类型安全
4. **扩展性**：易于扩展支持其他数组类型

迭代器内部维护容器指针、当前索引和容器大小，通过索引边界检查确保访问安全。
