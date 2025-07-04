/**
 * @file array_stack_example.c
 * @brief 数组栈使用示例
 * @details 演示基于动态数组实现的栈的创建、压入、弹出、查看等基本操作
 * @author oaheixiehao
 * @date 2025/7/3
 */

#include <stdio.h>
#include <stdlib.h>
#include <adt/stack.h>

/**
 * @brief 辅助函数：创建整数数据
 */
static int *create_int(int value) {
    int *data = malloc(sizeof(int));
    if (data) {
        *data = value;
    }
    return data;
}

/**
 * @brief 辅助函数：打印栈的状态
 */
static void print_stack_status(dsa_stack_t *stack, const char *description) {
    printf("\n=== %s ===\n", description);
    printf("栈类型: %s\n", stack_get_type_name(stack));
    printf("栈大小: %zu\n", stack_size(stack));
    printf("是否为空: %s\n", stack_is_empty(stack) ? "是" : "否");

    if (!stack_is_empty(stack)) {
        dsa_element_pt top_element = stack_peek(stack);
        if (top_element) {
            int top_value = ELEMENT_VALUE(int, top_element);
            printf("栈顶元素: %d\n", top_value);
        }
    }
}

/**
 * @brief 演示栈的基本操作
 */
static void demonstrate_basic_operations(void) {
    printf("\n🔄 数组栈基本操作演示\n");
    printf("========================================\n");
    
    // 创建数组栈
    dsa_stack_t *stack = stack_create(STACK_TYPE_ARRAY_LIST);
    if (!stack) {
        printf("❌ 创建数组栈失败\n");
        return;
    }
    printf("✅ 成功创建数组栈\n");
    
    print_stack_status(stack, "初始状态");
    
    // 压入元素
    printf("\n📥 压入元素演示\n");
    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        int *data = create_int(values[i]);
        dsa_result_t result = stack_push(stack, data);
        if (result == DSA_SUCCESS) {
            printf("✅ 压入元素: %d\n", values[i]);
        } else {
            printf("❌ 压入元素%d失败\n", values[i]);
            free(data);
        }
    }
    print_stack_status(stack, "压入5个元素后");
    
    // 查看栈顶元素
    printf("\n👀 查看栈顶元素演示\n");
    dsa_element_pt top = stack_peek(stack);
    if (top) {
        int value = ELEMENT_VALUE(int, top);
        printf("✅ 栈顶元素: %d (不移除)\n", value);
    } else {
        printf("❌ 栈为空，无法查看栈顶\n");
    }
    print_stack_status(stack, "查看栈顶后");
    
    // 弹出元素
    printf("\n📤 弹出元素演示\n");
    for (int i = 0; i < 3; i++) {
        dsa_element_pt popped = stack_pop(stack);
        if (popped) {
            int value = ELEMENT_VALUE(int, popped);
            printf("✅ 弹出元素: %d\n", value);
            free(popped);
        } else {
            printf("❌ 栈为空，无法弹出\n");
        }
    }
    print_stack_status(stack, "弹出3个元素后");
    
    // 清空栈
    printf("\n🗑️ 清空栈演示\n");
    // 先弹出剩余元素并释放内存
    while (!stack_is_empty(stack)) {
        dsa_element_pt element = stack_pop(stack);
        if (element) {
            free(element);
        }
    }
    printf("✅ 栈已清空\n");
    print_stack_status(stack, "清空后");
    
    // 销毁栈
    stack_destroy(stack);
    printf("✅ 栈已销毁\n");
}

/**
 * @brief 演示栈的LIFO特性
 */
static void demonstrate_lifo_behavior(void) {
    printf("\n📚 栈的LIFO(后进先出)特性演示\n");
    printf("========================================\n");
    
    dsa_stack_t *stack = stack_create(STACK_TYPE_ARRAY_LIST);
    if (!stack) {
        printf("❌ 创建栈失败\n");
        return;
    }
    
    printf("🔄 演示LIFO原理:\n");
    printf("想象一摞书，只能从顶部放入和取出\n\n");
    
    // 模拟放书
    const char *books[] = {"《数据结构》", "《算法导论》", "《C程序设计》", "《操作系统》"};
    printf("📖 依次放入书籍:\n");
    for (int i = 0; i < 4; i++) {
        int *book_id = create_int(i + 1);
        if (stack_push(stack, book_id) == DSA_SUCCESS) {
            printf("  %d. 放入: %s\n", i + 1, books[i]);
        } else {
            free(book_id);
        }
    }
    
    printf("\n📚 当前书堆状态:\n");
    printf("  顶部 → 底部: ");
    // 这里我们只能看到栈顶
    if (!stack_is_empty(stack)) {
        dsa_element_pt top = stack_peek(stack);
        if (top) {
            int book_id = ELEMENT_VALUE(int, top);
            printf("%s (栈顶)", books[book_id - 1]);
        }
    }
    printf("\n  栈中共有 %zu 本书\n", stack_size(stack));
    
    // 模拟取书
    printf("\n📤 依次取出书籍 (LIFO顺序):\n");
    int order = 1;
    while (!stack_is_empty(stack)) {
        dsa_element_pt book = stack_pop(stack);
        if (book) {
            int book_id = ELEMENT_VALUE(int, book);
            printf("  %d. 取出: %s\n", order++, books[book_id - 1]);
            free(book);
        }
    }
    
    printf("\n💡 观察结果:\n");
    printf("  - 放入顺序: 数据结构 → 算法导论 → C程序设计 → 操作系统\n");
    printf("  - 取出顺序: 操作系统 → C程序设计 → 算法导论 → 数据结构\n");
    printf("  - 特点: 最后放入的最先取出 (Last In, First Out)\n");
    
    stack_destroy(stack);
    printf("✅ LIFO演示完成\n");
}

/**
 * @brief 演示栈的实际应用场景
 */
static void demonstrate_practical_usage(void) {
    printf("\n🎯 实际应用场景演示\n");
    printf("========================================\n");
    printf("场景: 函数调用栈模拟\n");
    
    dsa_stack_t *call_stack = stack_create(STACK_TYPE_ARRAY_LIST);
    if (!call_stack) {
        printf("❌ 创建调用栈失败\n");
        return;
    }
    
    printf("🔄 模拟函数调用过程:\n");
    
    // 函数名称
    const char *functions[] = {
        "main()", "calculate()", "factorial()", "multiply()"
    };
    
    // 模拟函数调用
    printf("\n📞 函数调用序列:\n");
    for (int i = 0; i < 4; i++) {
        int *func_id = create_int(i);
        if (stack_push(call_stack, func_id) == DSA_SUCCESS) {
            printf("  → 调用 %s (压入调用栈)\n", functions[i]);
            printf("    当前栈深度: %zu\n", stack_size(call_stack));
        } else {
            free(func_id);
        }
    }
    
    print_stack_status(call_stack, "调用栈状态");
    
    // 模拟函数返回
    printf("\n🔙 函数返回序列:\n");
    while (!stack_is_empty(call_stack)) {
        dsa_element_pt func = stack_pop(call_stack);
        if (func) {
            int func_id = ELEMENT_VALUE(int, func);
            printf("  ← %s 执行完毕 (从调用栈弹出)\n", functions[func_id]);
            printf("    当前栈深度: %zu\n", stack_size(call_stack));
            free(func);
        }
    }
    
    printf("\n💡 调用栈特点:\n");
    printf("  - 函数调用时压入栈\n");
    printf("  - 函数返回时从栈弹出\n");
    printf("  - 保证了正确的返回顺序\n");
    printf("  - 支持递归调用\n");
    
    stack_destroy(call_stack);
    printf("✅ 函数调用栈演示完成\n");
}

/**
 * @brief 演示栈的性能特点
 */
static void demonstrate_performance_characteristics(void) {
    printf("\n⚡ 数组栈性能特点演示\n");
    printf("========================================\n");
    
    dsa_stack_t *stack = stack_create(STACK_TYPE_ARRAY_LIST);
    if (!stack) {
        printf("❌ 创建栈失败\n");
        return;
    }
    
    printf("📊 数组栈的时间复杂度特点:\n");
    printf("  - 压入操作: O(1)* - 平摊常数时间 ✅\n");
    printf("  - 弹出操作: O(1) - 常数时间 ✅\n");
    printf("  - 查看栈顶: O(1) - 常数时间 ✅\n");
    printf("  - 获取大小: O(1) - 常数时间 ✅\n");
    printf("  - 空间复杂度: O(n) - 线性空间 ✅\n");
    printf("  (*平摊分析：底层数组扩容时偶尔为O(n))\n");
    
    // 演示高效操作
    printf("\n🚀 演示高效的栈操作:\n");
    
    // 快速压入
    printf("  📥 快速压入10个元素: ");
    for (int i = 1; i <= 10; i++) {
        int *data = create_int(i * 5);
        if (stack_push(stack, data) == DSA_SUCCESS) {
            // 成功
        } else {
            free(data);
        }
    }
    printf("完成 ✅\n");
    printf("    栈大小: %zu\n", stack_size(stack));
    
    // 快速查看栈顶
    printf("  👀 快速查看栈顶: ");
    dsa_element_pt top = stack_peek(stack);
    if (top) {
        printf("%d ✅\n", ELEMENT_VALUE(int, top));
    }
    
    // 快速弹出
    printf("  📤 快速弹出5个元素: ");
    for (int i = 0; i < 5; i++) {
        dsa_element_pt popped = stack_pop(stack);
        if (popped) {
            free(popped);
        }
    }
    printf("完成 ✅\n");
    printf("    剩余大小: %zu\n", stack_size(stack));
    
    // 清理剩余元素
    while (!stack_is_empty(stack)) {
        dsa_element_pt element = stack_pop(stack);
        if (element) {
            free(element);
        }
    }
    
    stack_destroy(stack);
    printf("✅ 性能演示完成\n");
}

int main(void) {
    printf("🎉 数组栈示例程序\n");
    printf("========================================\n");
    printf("本程序演示基于动态数组实现的栈的各种操作和特性\n");
    
    // 基本操作演示
    demonstrate_basic_operations();
    
    // LIFO特性演示
    demonstrate_lifo_behavior();
    
    // 实际应用场景
    demonstrate_practical_usage();
    
    // 性能特点演示
    demonstrate_performance_characteristics();
    
    printf("\n🎊 所有演示完成！\n");
    printf("数组栈特点总结:\n");
    printf("✅ LIFO (后进先出) 访问模式\n");
    printf("✅ 高效的压入/弹出操作 O(1)\n");
    printf("✅ 基于动态数组，自动扩容\n");
    printf("✅ 内存局部性好，缓存友好\n");
    printf("⚠️ 需要手动管理元素内存\n");
    printf("🎯 适用场景: 函数调用、表达式求值、撤销操作等\n");
    
    return 0;
}
