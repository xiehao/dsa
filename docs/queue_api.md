# 队列 API 文档

队列提供了FIFO（先进先出）的数据访问模式，广泛应用于任务调度和缓冲区管理。

## 🎫 概述

队列支持两种实现：
- **循环数组队列**: 基于循环数组实现，内存局部性好，性能优秀
- **双向链表队列**: 基于双向链表实现，动态内存分配，容量无限

## 🔧 头文件

```c
#include <adt/queue.h>
```

## 🏗️ 创建和销毁

### 创建队列

```c
// 创建循环数组队列
dsa_queue_t *circular_queue = queue_create(QUEUE_TYPE_CIRCULAR_ARRAY_LIST);

// 创建双向链表队列
dsa_queue_t *linked_queue = queue_create(QUEUE_TYPE_DOUBLY_LINKED_LIST);
```

### 销毁队列

```c
// 销毁队列
queue_destroy(queue);
```

## 📝 基本操作

### 入队操作（FIFO - 先进先出）

```c
// 元素入队（加入队尾）
int *data1 = malloc(sizeof(int));
*data1 = 42;
dsa_result_t result = queue_enqueue(queue, data1);

int *data2 = malloc(sizeof(int));
*data2 = 99;
result = queue_enqueue(queue, data2);
```

### 出队操作（FIFO - 先进先出）

```c
// 元素出队（从队首移除）
dsa_element_pt first_out = queue_dequeue(queue);
if (first_out) {
    int value = *(int*)first_out;
    printf("出队: %d\n", value);  // 输出: 42 (先入队的元素)
    // 注意：需要手动释放元素内存
}

dsa_element_pt second_out = queue_dequeue(queue);
if (second_out) {
    int value = *(int*)second_out;
    printf("出队: %d\n", value);  // 输出: 99 (后入队的元素)
    // 注意：需要手动释放元素内存
}
```

## 📊 查询操作

### 状态查询

```c
// 获取队列中元素数量
size_t size = queue_size(queue);

// 检查队列是否为空
bool is_empty = queue_is_empty(queue);
```

### 类型信息

```c
// 获取队列类型
dsa_queue_type_t type = queue_get_type(queue);

// 获取类型名称
const char *type_name = queue_get_type_name(queue);
```

## 🧹 清理操作

```c
// 清空队列（不释放元素内存）
queue_clear(queue);
```

## ⚠️ 错误处理

```c
dsa_result_t result = queue_enqueue(queue, data);
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

## 💡 使用建议

1. **选择合适的实现**：
   - 循环数组：高性能应用，元素数量相对稳定
   - 双向链表：元素数量变化很大，需要动态调整

2. **内存管理**：
   - 队列不负责元素内存管理
   - 出队后记得释放元素内存
   - 销毁队列前确保所有元素已正确处理

3. **使用场景**：
   - 任务调度系统
   - 缓冲区管理
   - 广度优先搜索（BFS）
   - 生产者-消费者模式

## 🎯 FIFO特性演示

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

## 🏭 生产者-消费者示例

```c
// 模拟生产者-消费者模式
dsa_queue_t *task_queue = queue_create(QUEUE_TYPE_DOUBLY_LINKED_LIST);

// 生产者：添加任务
printf("=== 生产者添加任务 ===\n");
for (int i = 1; i <= 3; i++) {
    int *task = malloc(sizeof(int));
    *task = i * 100;
    queue_enqueue(task_queue, task);
    printf("生产任务: Task-%d\n", *task);
}

// 消费者：处理任务
printf("\n=== 消费者处理任务 ===\n");
while (!queue_is_empty(task_queue)) {
    dsa_element_pt task = queue_dequeue(task_queue);
    printf("处理任务: Task-%d\n", *(int*)task);
    
    // 模拟任务处理时间
    // sleep(1);
    
    printf("任务 Task-%d 完成\n", *(int*)task);
    free(task);
}

queue_destroy(task_queue);
```

## 🔄 与其他数据结构的对比

- **vs 栈**: 队列是FIFO，栈是LIFO
- **vs 双端队列**: 队列只能一端进一端出，双端队列两端都可以
- **vs 数组**: 队列提供受限的访问模式，数组支持随机访问

---

返回 [文档目录](README.md) | 查看 [栈 API](stack_api.md)
