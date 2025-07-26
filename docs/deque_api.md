# 双端队列 API 文档

双端队列（Deque）支持在两端进行高效的插入和删除操作，结合了栈和队列的特性。

## 🔄 概述

双端队列支持两种实现：
- **循环数组双端队列**: 基于循环数组实现，内存局部性好，支持自动扩容
- **双向链表双端队列**: 基于双向链表实现，完全动态内存分配，无容量限制

## 🔧 头文件

```c
#include <adt/deque.h>
```

## 🏗️ 创建和销毁

### 创建双端队列

```c
// 创建循环数组双端队列
dsa_deque_t *circular_deque = deque_create(DEQUE_TYPE_CIRCULAR_ARRAY_LIST);

// 创建双向链表双端队列
dsa_deque_t *linked_deque = deque_create(DEQUE_TYPE_DOUBLY_LINKED_LIST);
```

### 销毁双端队列

```c
// 销毁双端队列
deque_destroy(deque);
```

## 📝 基本操作

### 双端插入

```c
// 前端插入
int *data1 = malloc(sizeof(int));
*data1 = 42;
dsa_result_t result = deque_add_first(deque, data1);

// 后端插入
int *data2 = malloc(sizeof(int));
*data2 = 99;
result = deque_add_last(deque, data2);
```

### 双端删除

```c
// 前端删除
dsa_element_pt first = deque_remove_first(deque);
if (first) {
    int value = *(int*)first;
    printf("前端删除: %d\n", value);
    // 注意：需要手动释放元素内存
}

// 后端删除
dsa_element_pt last = deque_remove_last(deque);
if (last) {
    int value = *(int*)last;
    printf("后端删除: %d\n", value);
    // 注意：需要手动释放元素内存
}
```

### 查看元素（不删除）

```c
// 查看前端元素
dsa_element_pt peek_first = deque_peek_first(deque);
if (peek_first) {
    int value = *(int*)peek_first;
    printf("前端元素: %d\n", value);
}

// 查看后端元素
dsa_element_pt peek_last = deque_peek_last(deque);
if (peek_last) {
    int value = *(int*)peek_last;
    printf("后端元素: %d\n", value);
}
```

## 📊 查询操作

### 状态查询

```c
// 获取元素数量
size_t size = deque_size(deque);

// 检查是否为空
bool is_empty = deque_is_empty(deque);
```

### 类型信息

```c
// 获取双端队列类型
dsa_deque_type_t type = deque_get_type(deque);

// 获取类型名称
const char *type_name = deque_get_type_name(deque);
```

## 🧹 清理操作

```c
// 清空双端队列（不释放元素内存）
deque_clear(deque);
```

## ⚠️ 错误处理

```c
dsa_result_t result = deque_add_first(deque, data);
if (result != DSA_SUCCESS) {
    switch (result) {
        case DSA_ERROR_NULL_POINTER:
            printf("空指针错误\n");
            break;
        case DSA_ERROR_MEMORY_ALLOCATION:
            printf("内存分配失败\n");
            break;
        default:
            printf("未知错误\n");
    }
}
```

## 📈 性能特点

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

## 💡 使用建议

1. **选择合适的实现**：
   - 循环数组：性能敏感的应用，元素数量相对稳定
   - 双向链表：元素数量变化很大，内存使用灵活

2. **内存管理**：
   - 双端队列不负责元素内存管理
   - 删除元素后记得释放元素内存
   - 销毁双端队列前确保所有元素已正确处理

3. **使用场景**：
   - 需要在两端都进行操作的场景
   - 实现滑动窗口算法
   - 双向BFS搜索
   - 撤销/重做功能

## 🎯 使用示例

### 滑动窗口最大值

```c
// 使用双端队列实现滑动窗口最大值
dsa_deque_t *deque = deque_create(DEQUE_TYPE_CIRCULAR_ARRAY_LIST);

int arr[] = {1, 3, -1, -3, 5, 3, 6, 7};
int k = 3;  // 窗口大小

for (int i = 0; i < 8; i++) {
    // 移除超出窗口的元素
    while (!deque_is_empty(deque)) {
        int *front = (int*)deque_peek_first(deque);
        if (*front <= i - k) {
            deque_remove_first(deque);
        } else {
            break;
        }
    }
    
    // 移除比当前元素小的元素
    while (!deque_is_empty(deque)) {
        int *back_idx = (int*)deque_peek_last(deque);
        if (arr[*back_idx] < arr[i]) {
            deque_remove_last(deque);
        } else {
            break;
        }
    }
    
    // 添加当前元素索引
    int *idx = malloc(sizeof(int));
    *idx = i;
    deque_add_last(deque, idx);
    
    // 输出窗口最大值
    if (i >= k - 1) {
        int *max_idx = (int*)deque_peek_first(deque);
        printf("窗口 [%d, %d] 最大值: %d\n", i - k + 1, i, arr[*max_idx]);
    }
}

deque_destroy(deque);
```

---

返回 [文档目录](README.md) | 查看 [队列 API](queue_api.md)
