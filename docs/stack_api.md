# 栈 API 文档

栈提供了LIFO（后进先出）的数据访问模式，广泛应用于函数调用、表达式求值和回溯算法。

## 📚 概述

栈支持两种实现：
- **数组栈**: 基于动态数组实现，LIFO访问模式，内存局部性好
- **链表栈**: 基于单链表实现，完全动态内存分配，无容量限制

## 🔧 头文件

```c
#include <adt/stack.h>
```

## 🏗️ 创建和销毁

### 创建栈

```c
// 创建数组栈
dsa_stack_t *array_stack = stack_create(STACK_TYPE_ARRAY_LIST);

// 创建链表栈
dsa_stack_t *linked_stack = stack_create(STACK_TYPE_LINKED_LIST);
```

### 销毁栈

```c
// 销毁栈
stack_destroy(stack);
```

## 📝 基本操作

### 压栈操作（LIFO - 后进先出）

```c
// 元素压栈（压入栈顶）
int *data1 = malloc(sizeof(int));
*data1 = 42;
dsa_result_t result = stack_push(stack, data1);

int *data2 = malloc(sizeof(int));
*data2 = 99;
result = stack_push(stack, data2);
```

### 弹栈操作（LIFO - 后进先出）

```c
// 元素弹栈（从栈顶移除）
dsa_element_pt top = stack_pop(stack);
if (top) {
    int value = *(int*)top;
    printf("弹栈: %d\n", value);  // 输出: 99 (后压入的元素)
    // 注意：需要手动释放元素内存
}

dsa_element_pt second = stack_pop(stack);
if (second) {
    int value = *(int*)second;
    printf("弹栈: %d\n", value);  // 输出: 42 (先压入的元素)
    // 注意：需要手动释放元素内存
}
```

### 查看栈顶（不移除）

```c
// 查看栈顶元素但不移除
dsa_element_pt peek = stack_peek(stack);
if (peek) {
    int value = *(int*)peek;
    printf("栈顶元素: %d\n", value);
}
```

## 📊 查询操作

### 状态查询

```c
// 获取栈中元素数量
size_t size = stack_size(stack);

// 检查栈是否为空
bool is_empty = stack_is_empty(stack);
```

### 类型信息

```c
// 获取栈类型
dsa_stack_type_t type = stack_get_type(stack);

// 获取类型名称
const char *type_name = stack_get_type_name(stack);
```

## 🧹 清理操作

```c
// 清空栈（不释放元素内存）
stack_clear(stack);
```

## ⚠️ 错误处理

```c
dsa_result_t result = stack_push(stack, data);
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

| 操作     | 数组栈    | 链表栈    | 说明                     |
| -------- | --------- | --------- | ------------------------ |
| 压栈     | O(1)* ✅  | O(1) ✅   | 数组栈偶尔需要扩容       |
| 弹栈     | O(1) ✅   | O(1) ✅   | 都支持高效的LIFO弹栈     |
| 查看栈顶 | O(1) ✅   | O(1) ✅   | 都支持常数时间查看       |
| 随机访问 | 不支持 ❌ | 不支持 ❌ | 栈只支持LIFO访问模式     |
| 内存局部性 | 好 ✅   | 差 ⚠️   | 数组连续存储，缓存友好   |
| 内存开销 | 低 ✅     | 高 ⚠️   | 链表需要额外指针开销     |
| 扩容能力 | 自动 ✅   | 无限 ✅   | 数组自动扩容，链表理论无限 |
| 扩容成本 | O(n) ⚠️ | 无 ✅     | 数组扩容需要复制元素     |

## 💡 使用建议

1. **选择合适的实现**：
   - 数组栈：高性能应用，元素数量相对稳定
   - 链表栈：元素数量变化很大，需要动态调整

2. **内存管理**：
   - 栈不负责元素内存管理
   - 弹栈后记得释放元素内存
   - 销毁栈前确保所有元素已正确处理

3. **使用场景**：
   - 函数调用栈
   - 表达式求值
   - 括号匹配检查
   - 回溯算法
   - 撤销操作

## 🎯 LIFO特性演示

```c
// 演示栈的后进先出特性
printf("=== 栈LIFO特性演示 ===\n");

dsa_stack_t *stack = stack_create(STACK_TYPE_ARRAY_LIST);

// 依次压栈
for (int i = 1; i <= 5; i++) {
    int *data = malloc(sizeof(int));
    *data = i * 10;
    stack_push(stack, data);
    printf("压栈: %d\n", i * 10);
}

// 依次弹栈（与压栈顺序相反）
printf("弹栈顺序:\n");
while (!stack_is_empty(stack)) {
    dsa_element_pt element = stack_pop(stack);
    printf("弹栈: %d\n", *(int*)element);
    free(element);
}
// 输出: 50, 40, 30, 20, 10 (与压栈顺序相反)

stack_destroy(stack);
```

## 🧮 表达式求值示例

```c
// 使用栈计算后缀表达式
dsa_stack_t *calc_stack = stack_create(STACK_TYPE_LINKED_LIST);

// 后缀表达式: "3 4 + 2 * 7 /"  等价于 ((3 + 4) * 2) / 7
char *tokens[] = {"3", "4", "+", "2", "*", "7", "/", NULL};

for (int i = 0; tokens[i] != NULL; i++) {
    if (isdigit(tokens[i][0])) {
        // 数字：压栈
        int *num = malloc(sizeof(int));
        *num = atoi(tokens[i]);
        stack_push(calc_stack, num);
        printf("压栈数字: %d\n", *num);
    } else {
        // 操作符：弹出两个操作数计算
        int *b = (int*)stack_pop(calc_stack);
        int *a = (int*)stack_pop(calc_stack);
        
        int *result = malloc(sizeof(int));
        switch (tokens[i][0]) {
            case '+': *result = *a + *b; break;
            case '-': *result = *a - *b; break;
            case '*': *result = *a * *b; break;
            case '/': *result = *a / *b; break;
        }
        
        printf("计算: %d %c %d = %d\n", *a, tokens[i][0], *b, *result);
        stack_push(calc_stack, result);
        
        free(a);
        free(b);
    }
}

// 最终结果
int *final_result = (int*)stack_pop(calc_stack);
printf("最终结果: %d\n", *final_result);  // 输出: 2
free(final_result);

stack_destroy(calc_stack);
```

## 🔄 与其他数据结构的对比

- **vs 队列**: 栈是LIFO，队列是FIFO
- **vs 双端队列**: 栈只能一端操作，双端队列两端都可以
- **vs 数组**: 栈提供受限的访问模式，数组支持随机访问

---

返回 [文档目录](README.md) | 查看 [双端队列 API](deque_api.md)
