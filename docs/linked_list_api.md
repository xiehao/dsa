# 链表 API 文档

链表提供了灵活的动态数据结构，支持高效的插入和删除操作。

## 🔗 概述

链表支持三种实现：
- **单链表**: 单向链接，内存效率高
- **双链表**: 双向链接，支持高效的双向遍历
- **循环链表**: 尾节点连接头节点，适合循环操作

## 🔧 头文件

```c
#include <ds/linked_list.h>
```

## 🏗️ 创建和销毁

### 创建链表

```c
// 创建不同类型的链表
dsa_linked_list_t *singly = linked_list_create(LINKED_LIST_TYPE_SINGLY);
dsa_linked_list_t *doubly = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
dsa_linked_list_t *circular = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
```

### 销毁链表

```c
// 销毁链表（释放所有节点，但不释放元素数据）
linked_list_destroy(list);
```

## 📝 基本操作

### 插入元素

```c
// 在指定位置插入元素
int *data = malloc(sizeof(int));
*data = 42;
dsa_result_t result = linked_list_insert_at(list, 0, data);

// 在头部插入
result = linked_list_push_front(list, data);

// 在尾部插入
result = linked_list_push_back(list, data);
```

### 获取和设置元素

```c
// 获取指定位置的元素
dsa_element_pt element = linked_list_get(list, 0);
int value = *(int*)element;

// 设置指定位置的元素
int *new_data = malloc(sizeof(int));
*new_data = 99;
result = linked_list_set(list, 0, new_data);
```

### 删除元素

```c
// 删除指定位置的元素
dsa_element_pt removed = linked_list_remove_at(list, 0);
free(removed);  // 记得释放元素内存

// 删除头部元素
dsa_element_pt head = linked_list_pop_front(list);
free(head);

// 删除尾部元素
dsa_element_pt tail = linked_list_pop_back(list);
free(tail);
```

## 📊 查询操作

### 状态查询

```c
// 获取元素数量
size_t size = linked_list_size(list);

// 检查是否为空
bool is_empty = linked_list_is_empty(list);
```

### 类型信息

```c
// 获取链表类型
dsa_linked_list_type_t type = linked_list_get_type(list);

// 获取类型名称
const char *type_name = linked_list_get_type_name(list);
```

## 🧹 清理操作

```c
// 清空链表（不释放元素内存）
linked_list_clear(list);

// 清空并释放元素内存
linked_list_clear_with_free(list);
```

## ⚠️ 错误处理

```c
dsa_result_t result = linked_list_insert_at(list, 0, data);
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
        default:
            printf("未知错误\n");
    }
}
```

## 📈 性能特点

| 操作         | 单链表    | 双链表    | 循环链表  | 说明                 |
| ------------ | --------- | --------- | --------- | -------------------- |
| 头部插入     | O(1) ✅   | O(1) ✅   | O(1) ✅   | 都支持高效头部操作   |
| 尾部插入     | O(n) ⚠️ | O(1) ✅   | O(1) ✅   | 双链表和循环链表更优 |
| 随机访问     | O(n) ⚠️ | O(n) ⚠️ | O(n) ⚠️ | 都需要遍历查找       |
| 删除已知节点 | O(n) ⚠️ | O(1) ✅   | O(1) ✅   | 双链表优势明显       |
| 反向遍历     | 不支持 ❌ | 支持 ✅   | 支持 ✅   | 双向链接的优势       |
| 内存开销     | 低 ✅     | 高 ⚠️   | 中等 ⚠️ | 指针数量影响开销     |

## 💡 使用建议

1. **选择合适的类型**：
   - 单链表：内存敏感，主要进行头部操作
   - 双链表：需要双向遍历或频繁删除
   - 循环链表：需要循环访问数据

2. **内存管理**：
   - 链表只管理节点内存，元素内存由调用者负责
   - 删除元素后记得释放元素内存
   - 使用 `clear_with_free` 可以自动释放元素内存

3. **性能优化**：
   - 尽量使用头部操作（对所有类型都是O(1)）
   - 避免频繁的随机访问
   - 对于大量删除操作，优先选择双链表

## 🔄 迭代器支持

链表支持迭代器遍历：

```c
#include <ds/linked_list_iterator.h>

// 创建迭代器
dsa_iterator_t *begin = linked_list_begin(list);
dsa_iterator_t *end = linked_list_end(list);

// 遍历链表
while (!iterator_equals(begin, end)) {
    dsa_element_pt element = iterator_get_value(begin);
    int value = *(int*)element;
    printf("%d ", value);
    iterator_next(begin);
}

// 释放迭代器
iterator_destroy(begin);
iterator_destroy(end);
```

---

返回 [文档目录](README.md) | 查看 [栈 API](stack_api.md)
