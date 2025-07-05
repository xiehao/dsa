/**
 * @file doubly_linked_list_example.c
 * @brief 双链表使用示例
 * @details 演示双链表的创建、插入、删除、查询等基本操作
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
 * @brief 演示双链表的基本操作
 */
static void demonstrate_basic_operations(void) {
    printf("\n⬅️➡️ 双链表基本操作演示\n");
    printf("========================================\n");
    
    // 创建双链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    if (!list) {
        printf("❌ 创建双链表失败\n");
        return;
    }
    printf("✅ 成功创建双链表\n");
    
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
 * @brief 演示双链表的linear_trait功能
 */
static void demonstrate_linear_trait_operations(void) {
    printf("\n🔄 Linear Trait 操作演示\n");
    printf("========================================\n");
    printf("演示双链表的push_front, push_back, pop_front, pop_back操作\n");
    printf("双链表的优势：所有操作都是O(1)时间复杂度！\n");

    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    if (!list) {
        printf("❌ 创建双链表失败\n");
        return;
    }

    print_list(list, "初始状态");

    // 演示push_front操作
    printf("\n📥 Push Front 操作演示（O(1)）\n");
    for (int i = 1; i <= 3; i++) {
        int *data = create_int(i * 10);
        if (linked_list_push_front(list, data) == DSA_SUCCESS) {
            printf("✅ push_front(%d) 成功\n", i * 10);
            print_list(list, "当前状态");
        }
    }

    // 演示push_back操作
    printf("\n📥 Push Back 操作演示（O(1)）\n");
    for (int i = 4; i <= 6; i++) {
        int *data = create_int(i * 10);
        if (linked_list_push_back(list, data) == DSA_SUCCESS) {
            printf("✅ push_back(%d) 成功\n", i * 10);
            print_list(list, "当前状态");
        }
    }

    // 演示混合操作
    printf("\n🔀 混合操作演示\n");
    printf("交替使用前端和后端操作:\n");

    // 从前端弹出一个
    int *front_popped = (int*)linked_list_pop_front(list);
    if (front_popped) {
        printf("✅ pop_front() 返回: %d\n", *front_popped);
        free(front_popped);
        print_list(list, "pop_front后");
    }

    // 从后端弹出一个
    int *back_popped = (int*)linked_list_pop_back(list);
    if (back_popped) {
        printf("✅ pop_back() 返回: %d\n", *back_popped);
        free(back_popped);
        print_list(list, "pop_back后");
    }

    // 在前端添加一个
    int *new_front = create_int(99);
    if (linked_list_push_front(list, new_front) == DSA_SUCCESS) {
        printf("✅ push_front(99) 成功\n");
        print_list(list, "push_front后");
    }

    // 在后端添加一个
    int *new_back = create_int(88);
    if (linked_list_push_back(list, new_back) == DSA_SUCCESS) {
        printf("✅ push_back(88) 成功\n");
        print_list(list, "push_back后");
    }

    // 清空链表
    printf("\n🗑️ 清空链表演示\n");
    printf("使用pop操作清空链表:\n");
    int count = 0;
    while (!linked_list_is_empty(list)) {
        if (count % 2 == 0) {
            // 偶数次从前端弹出
            int *popped = (int*)linked_list_pop_front(list);
            if (popped) {
                printf("  📤 pop_front() 返回: %d\n", *popped);
                free(popped);
            }
        } else {
            // 奇数次从后端弹出
            int *popped = (int*)linked_list_pop_back(list);
            if (popped) {
                printf("  📤 pop_back() 返回: %d\n", *popped);
                free(popped);
            }
        }
        count++;
        printf("     剩余元素数: %zu\n", linked_list_size(list));
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
 * @brief 演示双链表的性能优势
 */
static void demonstrate_performance_advantages(void) {
    printf("\n⚡ 双链表性能优势演示\n");
    printf("========================================\n");
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    if (!list) {
        printf("❌ 创建双链表失败\n");
        return;
    }
    
    printf("📊 双链表的时间复杂度特点:\n");
    printf("  - 头部插入: O(1) - 非常快\n");
    printf("  - 尾部插入: O(1) - 比单链表快！\n");
    printf("  - 中间插入: O(n) - 但可以从两端优化\n");
    printf("  - 随机访问: O(n) - 可以选择更近的一端\n");
    printf("  - 删除操作: O(1) - 如果已知节点位置\n");
    
    // 演示高效的尾部插入
    printf("\n🚀 演示高效的尾部插入（O(1)操作）:\n");
    for (int i = 1; i <= 5; i++) {
        int *data = create_int(i * 10);
        linked_list_insert_at(list, linked_list_size(list), data);
        printf("  插入%d到尾部，当前大小: %zu\n", i * 10, linked_list_size(list));
    }
    print_list(list, "尾部插入结果（顺序保持）");
    
    // 演示从两端访问的优化
    printf("\n🎯 演示双向访问优势:\n");
    size_t size = linked_list_size(list);
    printf("  访问前端元素（索引0）: ");
    int *front = (int*)linked_list_get(list, 0);
    if (front) printf("%d\n", *front);
    
    printf("  访问后端元素（索引%zu）: ", size - 1);
    int *back = (int*)linked_list_get(list, size - 1);
    if (back) printf("%d\n", *back);
    
    // 清理
    linked_list_destroy(list);
    printf("✅ 性能演示完成\n");
}

/**
 * @brief 演示双链表的实际应用场景
 */
static void demonstrate_practical_usage(void) {
    printf("\n🎯 实际应用场景演示\n");
    printf("========================================\n");
    printf("场景: 浏览器历史记录（支持前进后退）\n");
    
    dsa_linked_list_t *history = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    if (!history) {
        printf("❌ 创建历史记录失败\n");
        return;
    }
    
    // 添加浏览记录
    printf("🌐 浏览网页，添加到历史记录:\n");
    const char *pages[] = {"首页", "搜索页", "产品页", "购物车", "结算页"};
    
    for (int i = 0; i < 5; i++) {
        int *page_id = create_int(i + 1);
        if (linked_list_insert_at(history, linked_list_size(history), page_id) == DSA_SUCCESS) {
            printf("  🔗 访问: %s (ID: %d)\n", pages[i], i + 1);
        }
    }
    
    print_list(history, "完整浏览历史");
    
    // 模拟后退操作
    printf("\n⬅️ 模拟后退操作:\n");
    size_t current_pos = linked_list_size(history) - 1;
    for (int i = 0; i < 3; i++) {
        if (current_pos > 0) {
            current_pos--;
            int *page_id = (int*)linked_list_get(history, current_pos);
            if (page_id) {
                printf("  ⬅️ 后退到: %s (ID: %d)\n", pages[*page_id - 1], *page_id);
            }
        }
    }
    
    // 模拟前进操作
    printf("\n➡️ 模拟前进操作:\n");
    for (int i = 0; i < 2; i++) {
        if (current_pos < linked_list_size(history) - 1) {
            current_pos++;
            int *page_id = (int*)linked_list_get(history, current_pos);
            if (page_id) {
                printf("  ➡️ 前进到: %s (ID: %d)\n", pages[*page_id - 1], *page_id);
            }
        }
    }
    
    // 从中间位置访问新页面（清除后续历史）
    printf("\n🆕 从中间位置访问新页面:\n");
    // 删除当前位置之后的所有记录
    while (linked_list_size(history) > current_pos + 1) {
        int *removed = (int*)linked_list_remove_at(history, linked_list_size(history) - 1);
        if (removed) {
            printf("  🗑️ 清除后续历史: %s (ID: %d)\n", pages[*removed - 1], *removed);
            free(removed);
        }
    }
    
    // 添加新页面
    int *new_page = create_int(6);
    linked_list_insert_at(history, linked_list_size(history), new_page);
    printf("  🔗 访问新页面: 帮助页 (ID: 6)\n");
    
    print_list(history, "更新后的浏览历史");
    
    // 清理
    linked_list_destroy(history);
    printf("✅ 浏览历史演示完成\n");
}

/**
 * @brief 演示双链表与单链表的对比
 */
static void demonstrate_comparison_with_singly(void) {
    printf("\n🆚 双链表 vs 单链表对比\n");
    printf("========================================\n");
    
    // 创建两种链表
    dsa_linked_list_t *singly = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    dsa_linked_list_t *doubly = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    
    if (!singly || !doubly) {
        printf("❌ 创建链表失败\n");
        return;
    }
    
    // 在两种链表中插入相同数据
    for (int i = 1; i <= 3; i++) {
        int *data1 = create_int(i * 10);
        int *data2 = create_int(i * 10);
        linked_list_insert_at(singly, i - 1, data1);
        linked_list_insert_at(doubly, i - 1, data2);
    }
    
    printf("📊 功能对比:\n");
    printf("┌─────────────────┬─────────────┬─────────────┐\n");
    printf("│ 操作            │ 单链表      │ 双链表      │\n");
    printf("├─────────────────┼─────────────┼─────────────┤\n");
    printf("│ 头部插入        │ O(1) ✅     │ O(1) ✅     │\n");
    printf("│ 尾部插入        │ O(n) ⚠️     │ O(1) ✅     │\n");
    printf("│ 中间插入        │ O(n) ⚠️     │ O(n) ⚠️     │\n");
    printf("│ 随机访问        │ O(n) ⚠️     │ O(n) ⚠️     │\n");
    printf("│ 删除已知节点    │ O(n) ⚠️     │ O(1) ✅     │\n");
    printf("│ 反向遍历        │ 不支持 ❌   │ 支持 ✅     │\n");
    printf("│ 内存开销        │ 较小 ✅     │ 较大 ⚠️     │\n");
    printf("└─────────────────┴─────────────┴─────────────┘\n");
    
    printf("\n💡 选择建议:\n");
    printf("  🎯 选择单链表: 内存敏感、主要头部操作、简单场景\n");
    printf("  🎯 选择双链表: 需要高效尾部操作、双向遍历、复杂操作\n");
    
    // 清理
    linked_list_destroy(singly);
    linked_list_destroy(doubly);
    printf("✅ 对比演示完成\n");
}

int main(void) {
    printf("🎉 双链表示例程序\n");
    printf("========================================\n");
    printf("本程序演示双链表的各种操作和优势\n");
    
    // 基本操作演示
    demonstrate_basic_operations();

    // Linear Trait 操作演示
    demonstrate_linear_trait_operations();

    // 性能优势演示
    demonstrate_performance_advantages();
    
    // 实际应用场景
    demonstrate_practical_usage();
    
    // 与单链表对比
    demonstrate_comparison_with_singly();
    
    printf("\n🎊 所有演示完成！\n");
    printf("双链表特点总结:\n");
    printf("✅ 支持双向遍历\n");
    printf("✅ 高效的尾部操作 O(1)\n");
    printf("✅ 删除已知节点效率高 O(1)\n");
    printf("⚠️ 内存开销比单链表大\n");
    printf("🎯 适用场景: 需要双向访问、频繁尾部操作的数据集合\n");
    
    return 0;
}
