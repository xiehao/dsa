/**
 * @file singly_linked_list_example.c
 * @brief 单链表使用示例
 * @details 演示单链表的创建、插入、删除、查询等基本操作
 * @author oaheixiehao
 * @date 2025/7/2
 */

#include <stdio.h>
#include <stdlib.h>
#include <ds/linked_list.h>

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
 * @brief 辅助函数：打印链表内容
 */
static void print_list(dsa_linked_list_t *list, const char *description) {
    printf("\n=== %s ===\n", description);
    printf("链表大小: %zu\n", linked_list_size(list));
    printf("是否为空: %s\n", linked_list_is_empty(list) ? "是" : "否");
    
    if (!linked_list_is_empty(list)) {
        printf("链表内容: ");
        for (size_t i = 0; i < linked_list_size(list); i++) {
            int *value = (int*)linked_list_get(list, i);
            if (value) {
                printf("%d ", *value);
            }
        }
        printf("\n");
    }
}

/**
 * @brief 演示单链表的基本操作
 */
static void demonstrate_basic_operations(void) {
    printf("\n➡️ 单链表基本操作演示\n");
    printf("========================================\n");
    
    // 创建单链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    if (!list) {
        printf("❌ 创建单链表失败\n");
        return;
    }
    printf("✅ 成功创建单链表\n");
    
    print_list(list, "初始状态");
    
    // 插入元素
    printf("\n📥 插入操作演示\n");
    int *data1 = create_int(10);
    int *data2 = create_int(20);
    int *data3 = create_int(30);
    
    if (linked_list_insert_at(list, 0, data1) == DSA_SUCCESS) {
        printf("✅ 在位置0插入元素10\n");
    }
    print_list(list, "插入第一个元素后");
    
    if (linked_list_insert_at(list, 1, data2) == DSA_SUCCESS) {
        printf("✅ 在位置1插入元素20\n");
    }
    print_list(list, "插入第二个元素后");
    
    if (linked_list_insert_at(list, 2, data3) == DSA_SUCCESS) {
        printf("✅ 在位置2插入元素30\n");
    }
    print_list(list, "插入第三个元素后");
    
    // 在中间插入元素
    int *data_middle = create_int(15);
    if (linked_list_insert_at(list, 1, data_middle) == DSA_SUCCESS) {
        printf("✅ 在位置1插入元素15（中间插入）\n");
    }
    print_list(list, "中间插入后");
    
    // 获取元素
    printf("\n🔍 元素访问演示\n");
    for (size_t i = 0; i < linked_list_size(list); i++) {
        int *value = (int*)linked_list_get(list, i);
        if (value) {
            printf("索引%zu的元素: %d\n", i, *value);
        }
    }
    
    // 修改元素
    printf("\n✏️ 元素修改演示\n");
    int *new_value = create_int(99);
    if (linked_list_set(list, 2, new_value) == DSA_SUCCESS) {
        printf("✅ 将索引2的元素修改为99\n");
        // 释放被替换的元素
        free(data2);
    }
    print_list(list, "修改元素后");
    
    // 删除元素
    printf("\n🗑️ 删除操作演示\n");
    int *removed = (int*)linked_list_remove_at(list, 1);
    if (removed) {
        printf("✅ 删除索引1的元素: %d\n", *removed);
        free(removed);
    }
    print_list(list, "删除元素后");
    
    // 清理剩余元素
    free(data1);
    free(data3);
    free(new_value);
    
    // 销毁链表
    linked_list_destroy(list);
    printf("✅ 链表已销毁\n");
}

/**
 * @brief 演示单链表的性能特点
 */
static void demonstrate_performance_characteristics(void) {
    printf("\n⚡ 单链表性能特点演示\n");
    printf("========================================\n");
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    if (!list) {
        printf("❌ 创建单链表失败\n");
        return;
    }
    
    printf("📊 单链表的时间复杂度特点:\n");
    printf("  - 头部插入: O(1) - 非常快\n");
    printf("  - 尾部插入: O(n) - 需要遍历到末尾\n");
    printf("  - 中间插入: O(n) - 需要遍历到指定位置\n");
    printf("  - 随机访问: O(n) - 需要从头遍历\n");
    printf("  - 删除操作: O(n) - 需要找到前一个节点\n");
    
    // 演示头部插入的效率
    printf("\n🚀 演示头部插入（O(1)操作）:\n");
    for (int i = 1; i <= 5; i++) {
        int *data = create_int(i * 10);
        linked_list_insert_at(list, 0, data);
        printf("  插入%d到头部，当前大小: %zu\n", i * 10, linked_list_size(list));
    }
    print_list(list, "头部插入结果（注意顺序是反的）");
    
    // 演示随机访问
    printf("\n🎯 演示随机访问:\n");
    printf("  访问中间元素（索引2）: ");
    int *middle = (int*)linked_list_get(list, 2);
    if (middle) {
        printf("%d\n", *middle);
    }
    
    // 清理
    linked_list_destroy(list);
    printf("✅ 性能演示完成\n");
}

/**
 * @brief 演示单链表的linear_trait功能
 */
static void demonstrate_linear_trait_operations(void) {
    printf("\n🔄 Linear Trait 操作演示\n");
    printf("========================================\n");
    printf("演示新增的push_front, push_back, pop_front, pop_back操作\n");

    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    if (!list) {
        printf("❌ 创建单链表失败\n");
        return;
    }

    print_list(list, "初始状态");

    // 演示push_front操作
    printf("\n📥 Push Front 操作演示\n");
    for (int i = 1; i <= 3; i++) {
        int *data = create_int(i * 10);
        if (linked_list_push_front(list, data) == DSA_SUCCESS) {
            printf("✅ push_front(%d) 成功\n", i * 10);
            print_list(list, "当前状态");
        }
    }

    // 演示push_back操作
    printf("\n📥 Push Back 操作演示\n");
    for (int i = 4; i <= 6; i++) {
        int *data = create_int(i * 10);
        if (linked_list_push_back(list, data) == DSA_SUCCESS) {
            printf("✅ push_back(%d) 成功\n", i * 10);
            print_list(list, "当前状态");
        }
    }

    // 演示pop_front操作
    printf("\n📤 Pop Front 操作演示\n");
    for (int i = 0; i < 3; i++) {
        int *popped = (int*)linked_list_pop_front(list);
        if (popped) {
            printf("✅ pop_front() 返回: %d\n", *popped);
            free(popped);
            print_list(list, "当前状态");
        } else {
            printf("❌ pop_front() 返回 NULL（链表为空）\n");
            break;
        }
    }

    // 演示pop_back操作
    printf("\n📤 Pop Back 操作演示\n");
    while (!linked_list_is_empty(list)) {
        int *popped = (int*)linked_list_pop_back(list);
        if (popped) {
            printf("✅ pop_back() 返回: %d\n", *popped);
            free(popped);
            print_list(list, "当前状态");
        } else {
            printf("❌ pop_back() 返回 NULL（链表为空）\n");
            break;
        }
    }

    // 测试空链表上的操作
    printf("\n⚠️ 空链表操作测试\n");
    printf("pop_front() 在空链表上: %s\n",
           linked_list_pop_front(list) == NULL ? "返回NULL（正确）" : "异常");
    printf("pop_back() 在空链表上: %s\n",
           linked_list_pop_back(list) == NULL ? "返回NULL（正确）" : "异常");

    linked_list_destroy(list);
    printf("✅ Linear Trait 演示完成\n");
}

/**
 * @brief 演示单链表的实际应用场景
 */
static void demonstrate_practical_usage(void) {
    printf("\n🎯 实际应用场景演示\n");
    printf("========================================\n");
    printf("场景1: 栈操作（LIFO - 后进先出）\n");

    dsa_linked_list_t *stack = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    if (!stack) {
        printf("❌ 创建栈失败\n");
        return;
    }

    // 使用push_front模拟栈的push操作
    printf("📚 压栈操作（使用push_front）:\n");
    const char *books[] = {"《数据结构》", "《算法导论》", "《编程珠玑》", "《代码大全》"};

    for (int i = 0; i < 4; i++) {
        int *book_id = create_int(i + 1);
        if (linked_list_push_front(stack, book_id) == DSA_SUCCESS) {
            printf("  📖 压栈: %s (ID: %d)\n", books[i], i + 1);
        }
    }
    print_list(stack, "栈状态");

    // 使用pop_front模拟栈的pop操作
    printf("\n📚 出栈操作（使用pop_front）:\n");
    while (!linked_list_is_empty(stack)) {
        int *book_id = (int*)linked_list_pop_front(stack);
        if (book_id) {
            printf("  📖 出栈: %s (ID: %d)\n", books[*book_id - 1], *book_id);
            printf("     剩余书籍数: %zu\n", linked_list_size(stack));
            free(book_id);
        }
    }
    printf("✅ 栈演示完成\n");

    printf("\n场景2: 队列操作（FIFO - 先进先出）\n");

    // 使用push_back和pop_front模拟队列操作
    printf("🎫 排队买票（使用push_back入队，pop_front出队）:\n");
    const char *customers[] = {"张三", "李四", "王五", "赵六"};

    for (int i = 0; i < 4; i++) {
        int *customer_id = create_int(i + 1);
        if (linked_list_push_back(stack, customer_id) == DSA_SUCCESS) {
            printf("  👤 %s 排队 (ID: %d)\n", customers[i], i + 1);
        }
    }
    print_list(stack, "队列状态");

    printf("\n🎫 开始售票:\n");
    while (!linked_list_is_empty(stack)) {
        int *customer_id = (int*)linked_list_pop_front(stack);
        if (customer_id) {
            printf("  🎟️ %s 买票完成 (ID: %d)\n", customers[*customer_id - 1], *customer_id);
            printf("     队列剩余人数: %zu\n", linked_list_size(stack));
            free(customer_id);
        }
    }

    printf("✅ 队列演示完成\n");

    // 清理
    linked_list_destroy(stack);
    printf("✅ 实际应用场景演示完成\n");
}

/**
 * @brief 演示单链表的边界情况
 */
static void demonstrate_edge_cases(void) {
    printf("\n⚠️ 边界情况处理演示\n");
    printf("========================================\n");
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    if (!list) {
        printf("❌ 创建单链表失败\n");
        return;
    }
    
    // 空链表操作
    printf("📋 空链表操作测试:\n");
    printf("  - 获取元素: %s\n", linked_list_get(list, 0) == NULL ? "返回NULL（正确）" : "异常");
    printf("  - 删除元素: %s\n", linked_list_remove_at(list, 0) == NULL ? "返回NULL（正确）" : "异常");
    
    // 单元素操作
    int *single_data = create_int(42);
    linked_list_insert_at(list, 0, single_data);
    printf("📋 单元素链表测试:\n");
    printf("  - 大小: %zu\n", linked_list_size(list));
    printf("  - 获取元素: %d\n", *(int*)linked_list_get(list, 0));
    
    // 删除唯一元素
    int *removed = (int*)linked_list_remove_at(list, 0);
    printf("  - 删除后大小: %zu\n", linked_list_size(list));
    printf("  - 是否为空: %s\n", linked_list_is_empty(list) ? "是" : "否");
    free(removed);
    
    // 索引越界测试
    printf("📋 索引越界测试:\n");
    printf("  - 获取越界索引: %s\n", linked_list_get(list, 5) == NULL ? "返回NULL（正确）" : "异常");
    printf("  - 插入越界索引: %s\n", 
           linked_list_insert_at(list, 5, create_int(1)) == DSA_ERROR_INDEX_OUT_OF_BOUNDS ? 
           "返回错误（正确）" : "异常");
    
    linked_list_destroy(list);
    printf("✅ 边界情况测试完成\n");
}

int main(void) {
    printf("🎉 单链表示例程序\n");
    printf("========================================\n");
    printf("本程序演示单链表的各种操作和特性\n");

    // 基本操作演示
    demonstrate_basic_operations();

    // Linear Trait 操作演示
    demonstrate_linear_trait_operations();

    // 性能特点演示
    demonstrate_performance_characteristics();

    // 实际应用场景
    demonstrate_practical_usage();

    // 边界情况演示
    demonstrate_edge_cases();

    printf("\n🎊 所有演示完成！\n");
    printf("单链表特点总结:\n");
    printf("✅ 动态大小，内存使用灵活\n");
    printf("✅ 头部插入效率高 O(1)\n");
    printf("✅ 支持双端操作（push_front/back, pop_front/back）\n");
    printf("⚠️ 随机访问效率低 O(n)\n");
    printf("⚠️ 不支持反向遍历\n");
    printf("🎯 适用场景: 栈、队列、频繁头部插入的数据集合\n");

    return 0;
}
