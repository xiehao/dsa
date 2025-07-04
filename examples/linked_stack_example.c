/**
 * @file linked_stack_example.c
 * @brief 链表栈使用示例
 * @details 演示基于单链表实现的栈的创建、压入、弹出、查看等基本操作
 * @author oaheixiehao
 * @date 2025/7/4
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
    printf("\n🔄 链表栈基本操作演示\n");
    printf("========================================\n");
    
    // 创建链表栈
    dsa_stack_t *stack = stack_create(STACK_TYPE_LINKED_LIST);
    if (!stack) {
        printf("❌ 创建链表栈失败\n");
        return;
    }
    printf("✅ 成功创建链表栈\n");
    
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
    
    dsa_stack_t *stack = stack_create(STACK_TYPE_LINKED_LIST);
    if (!stack) {
        printf("❌ 创建栈失败\n");
        return;
    }
    
    printf("🔄 演示LIFO原理:\n");
    printf("想象一摞盘子，只能从顶部放入和取出\n\n");
    
    // 模拟放盘子
    const char *plates[] = {"白色盘子", "蓝色盘子", "红色盘子", "绿色盘子"};
    printf("🍽️ 依次放入盘子:\n");
    for (int i = 0; i < 4; i++) {
        int *plate_id = create_int(i + 1);
        if (stack_push(stack, plate_id) == DSA_SUCCESS) {
            printf("  %d. 放入: %s\n", i + 1, plates[i]);
        } else {
            free(plate_id);
        }
    }
    
    printf("\n🍽️ 当前盘子堆状态:\n");
    printf("  顶部 → 底部: ");
    // 这里我们只能看到栈顶
    if (!stack_is_empty(stack)) {
        dsa_element_pt top = stack_peek(stack);
        if (top) {
            int plate_id = ELEMENT_VALUE(int, top);
            printf("%s (栈顶)", plates[plate_id - 1]);
        }
    }
    printf("\n  栈中共有 %zu 个盘子\n", stack_size(stack));
    
    // 模拟取盘子
    printf("\n📤 依次取出盘子 (LIFO顺序):\n");
    int order = 1;
    while (!stack_is_empty(stack)) {
        dsa_element_pt plate = stack_pop(stack);
        if (plate) {
            int plate_id = ELEMENT_VALUE(int, plate);
            printf("  %d. 取出: %s\n", order++, plates[plate_id - 1]);
            free(plate);
        }
    }
    
    printf("\n💡 观察结果:\n");
    printf("  - 放入顺序: 白色 → 蓝色 → 红色 → 绿色\n");
    printf("  - 取出顺序: 绿色 → 红色 → 蓝色 → 白色\n");
    printf("  - 特点: 最后放入的最先取出 (Last In, First Out)\n");
    
    stack_destroy(stack);
    printf("✅ LIFO演示完成\n");
}

/**
 * @brief 演示链表栈的特点
 */
static void demonstrate_linked_stack_features(void) {
    printf("\n🔗 链表栈特点演示\n");
    printf("========================================\n");
    
    dsa_stack_t *stack = stack_create(STACK_TYPE_LINKED_LIST);
    if (!stack) {
        printf("❌ 创建栈失败\n");
        return;
    }
    
    printf("📊 链表栈的特点:\n");
    printf("  ✅ 动态内存分配，无需预设大小\n");
    printf("  ✅ 压入/弹出操作始终O(1)时间复杂度\n");
    printf("  ✅ 内存使用灵活，按需分配\n");
    printf("  ⚠️ 获取大小需要O(n)时间复杂度\n");
    printf("  ⚠️ 内存局部性相对较差\n");
    printf("  ⚠️ 每个节点有额外的指针开销\n");
    
    printf("\n🚀 演示动态特性:\n");
    
    // 演示无需预设大小
    printf("  📈 无需预设大小，可以任意添加元素:\n");
    for (int i = 1; i <= 10; i++) {
        int *data = create_int(i * 10);
        if (stack_push(stack, data) == DSA_SUCCESS) {
            printf("    添加第%d个元素: %d\n", i, i * 10);
        } else {
            free(data);
        }
    }
    
    printf("  📊 当前栈大小: %zu\n", stack_size(stack));
    
    // 演示O(1)压入弹出
    printf("\n  ⚡ 演示O(1)压入弹出操作:\n");
    int *extra_data = create_int(999);
    printf("    压入元素999: ");
    if (stack_push(stack, extra_data) == DSA_SUCCESS) {
        printf("成功 ✅\n");
    } else {
        printf("失败 ❌\n");
        free(extra_data);
    }
    
    printf("    弹出栈顶元素: ");
    dsa_element_pt popped = stack_pop(stack);
    if (popped) {
        printf("%d ✅\n", ELEMENT_VALUE(int, popped));
        free(popped);
    }
    
    // 清理剩余元素
    while (!stack_is_empty(stack)) {
        dsa_element_pt element = stack_pop(stack);
        if (element) {
            free(element);
        }
    }
    
    stack_destroy(stack);
    printf("✅ 链表栈特点演示完成\n");
}

/**
 * @brief 演示链表栈与数组栈的对比
 */
static void demonstrate_comparison_with_array_stack(void) {
    printf("\n🆚 链表栈 vs 数组栈对比\n");
    printf("========================================\n");
    
    printf("📊 性能对比:\n");
    printf("┌─────────────────┬─────────────┬─────────────┐\n");
    printf("│ 操作            │ 数组栈      │ 链表栈      │\n");
    printf("├─────────────────┼─────────────┼─────────────┤\n");
    printf("│ 压入(push)      │ O(1)* ✅    │ O(1) ✅     │\n");
    printf("│ 弹出(pop)       │ O(1) ✅     │ O(1) ✅     │\n");
    printf("│ 查看栈顶(peek)  │ O(1) ✅     │ O(1) ✅     │\n");
    printf("│ 获取大小(size)  │ O(1) ✅     │ O(n) ⚠️     │\n");
    printf("│ 内存局部性      │ 好 ✅       │ 一般 ⚠️     │\n");
    printf("│ 内存开销        │ 较小 ✅     │ 较大 ⚠️     │\n");
    printf("│ 动态性          │ 扩容 ⚠️     │ 完全动态 ✅ │\n");
    printf("└─────────────────┴─────────────┴─────────────┘\n");
    printf("  *数组栈的压入操作平摊O(1)，扩容时为O(n)\n");
    
    printf("\n💡 选择建议:\n");
    printf("  🎯 选择数组栈: 频繁查询大小、注重内存局部性\n");
    printf("  🎯 选择链表栈: 完全动态、不确定数据量大小\n");
    
    printf("✅ 对比演示完成\n");
}

int main(void) {
    printf("🎉 链表栈示例程序\n");
    printf("========================================\n");
    printf("本程序演示基于单链表实现的栈的各种操作和特性\n");
    
    // 基本操作演示
    demonstrate_basic_operations();
    
    // LIFO特性演示
    demonstrate_lifo_behavior();
    
    // 链表栈特点演示
    demonstrate_linked_stack_features();
    
    // 与数组栈对比
    demonstrate_comparison_with_array_stack();
    
    printf("\n🎊 所有演示完成！\n");
    printf("链表栈特点总结:\n");
    printf("✅ LIFO (后进先出) 访问模式\n");
    printf("✅ 真正的O(1)压入/弹出操作\n");
    printf("✅ 完全动态内存分配\n");
    printf("✅ 无需预设容量限制\n");
    printf("⚠️ 获取大小需要O(n)时间\n");
    printf("⚠️ 内存局部性相对较差\n");
    printf("🎯 适用场景: 数据量不确定、很少查询大小的应用\n");
    
    return 0;
}
