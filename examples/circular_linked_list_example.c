/**
 * @file circular_linked_list_example.c
 * @brief 循环链表使用示例
 * @details 演示循环链表的创建、插入、删除、查询等基本操作
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
 * @brief 演示循环链表的基本操作
 */
static void demonstrate_basic_operations(void) {
    printf("\n🔄 循环链表基本操作演示\n");
    printf("========================================\n");
    
    // 创建循环链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    if (!list) {
        printf("❌ 创建循环链表失败\n");
        return;
    }
    printf("✅ 成功创建循环链表\n");
    
    print_list(list, "初始状态");
    
    // 插入元素到头部
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
 * @brief 演示循环链表的边界情况处理
 */
static void demonstrate_edge_cases(void) {
    printf("\n⚠️ 边界情况处理演示\n");
    printf("========================================\n");
    
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    if (!list) {
        printf("❌ 创建循环链表失败\n");
        return;
    }
    
    // 空链表操作
    printf("📋 空链表操作测试:\n");
    printf("  - 获取元素: %s\n", linked_list_get(list, 0) == NULL ? "返回NULL（正确）" : "异常");
    printf("  - 删除元素: %s\n", linked_list_remove_at(list, 0) == NULL ? "返回NULL（正确）" : "异常");
    
    // 索引越界测试
    int *data = create_int(42);
    linked_list_insert_at(list, 0, data);
    
    printf("📋 索引越界测试:\n");
    printf("  - 获取越界索引: %s\n", linked_list_get(list, 5) == NULL ? "返回NULL（正确）" : "异常");
    printf("  - 插入越界索引: %s\n", 
           linked_list_insert_at(list, 5, create_int(1)) == DSA_ERROR_INDEX_OUT_OF_BOUNDS ? 
           "返回错误（正确）" : "异常");
    
    // NULL指针测试
    printf("📋 NULL指针测试:\n");
    printf("  - 插入NULL数据: %s\n", 
           linked_list_insert_at(list, 0, NULL) == DSA_ERROR_NULL_POINTER ? 
           "返回错误（正确）" : "异常");
    printf("  - 设置NULL数据: %s\n", 
           linked_list_set(list, 0, NULL) == DSA_ERROR_NULL_POINTER ? 
           "返回错误（正确）" : "异常");
    
    free(data);
    linked_list_destroy(list);
    printf("✅ 边界情况测试完成\n");
}

/**
 * @brief 演示循环链表与其他链表类型的对比
 */
static void demonstrate_comparison(void) {
    printf("\n🔄 循环链表 vs 其他链表类型\n");
    printf("========================================\n");
    
    // 创建三种类型的链表
    dsa_linked_list_t *singly = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    dsa_linked_list_t *doubly = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    dsa_linked_list_t *circular = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    
    if (!singly || !doubly || !circular) {
        printf("❌ 创建链表失败\n");
        return;
    }
    
    // 在每种链表中插入相同的数据
    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        int *data1 = create_int(values[i]);
        int *data2 = create_int(values[i]);
        int *data3 = create_int(values[i]);
        
        linked_list_insert_at(singly, i, data1);
        linked_list_insert_at(doubly, i, data2);
        linked_list_insert_at(circular, i, data3);
    }
    
    printf("📊 三种链表的基本信息:\n");
    printf("  单链表   - 大小: %zu, 类型: 单向\n", linked_list_size(singly));
    printf("  双链表   - 大小: %zu, 类型: 双向\n", linked_list_size(doubly));
    printf("  循环链表 - 大小: %zu, 类型: 循环\n", linked_list_size(circular));
    
    printf("\n📋 所有链表都支持相同的操作接口:\n");
    printf("  ✅ 插入 (insert_at)\n");
    printf("  ✅ 删除 (remove_at)\n");
    printf("  ✅ 获取 (get)\n");
    printf("  ✅ 设置 (set)\n");
    printf("  ✅ 大小查询 (size)\n");
    printf("  ✅ 空检查 (is_empty)\n");
    
    // 清理
    linked_list_destroy(singly);
    linked_list_destroy(doubly);
    linked_list_destroy(circular);
    printf("✅ 对比演示完成\n");
}

/**
 * @brief 演示循环链表的实际应用场景
 */
static void demonstrate_practical_usage(void) {
    printf("\n🎯 实际应用场景演示\n");
    printf("========================================\n");
    printf("场景: 音乐播放列表（循环播放）\n");
    
    dsa_linked_list_t *playlist = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    if (!playlist) {
        printf("❌ 创建播放列表失败\n");
        return;
    }
    
    // 添加歌曲（用数字代表歌曲ID）
    const char *songs[] = {"歌曲1", "歌曲2", "歌曲3", "歌曲4"};
    printf("🎵 添加歌曲到播放列表:\n");
    
    for (int i = 0; i < 4; i++) {
        int *song_id = create_int(i + 1);
        if (linked_list_insert_at(playlist, i, song_id) == DSA_SUCCESS) {
            printf("  ✅ 添加 %s (ID: %d)\n", songs[i], i + 1);
        }
    }
    
    print_list(playlist, "完整播放列表");
    
    // 模拟播放（循环访问）
    printf("\n🔄 模拟循环播放（播放10首，观察循环效果）:\n");
    size_t playlist_size = linked_list_size(playlist);
    for (int play_count = 0; play_count < 10; play_count++) {
        size_t current_index = play_count % playlist_size;
        int *current_song = (int*)linked_list_get(playlist, current_index);
        if (current_song) {
            printf("  🎵 正在播放: %s (ID: %d)\n", 
                   songs[*current_song - 1], *current_song);
        }
    }
    
    // 插入新歌曲
    printf("\n➕ 在播放列表中间插入新歌曲:\n");
    int *new_song = create_int(5);
    if (linked_list_insert_at(playlist, 2, new_song) == DSA_SUCCESS) {
        printf("  ✅ 在位置2插入新歌曲 (ID: 5)\n");
    }
    print_list(playlist, "插入新歌曲后的播放列表");
    
    // 删除歌曲
    printf("\n➖ 从播放列表删除歌曲:\n");
    int *removed_song = (int*)linked_list_remove_at(playlist, 1);
    if (removed_song) {
        printf("  ✅ 删除歌曲 (ID: %d)\n", *removed_song);
        free(removed_song);
    }
    print_list(playlist, "删除歌曲后的播放列表");
    
    // 清理
    linked_list_destroy(playlist);
    printf("✅ 播放列表演示完成\n");
}

int main(void) {
    printf("🎉 循环链表示例程序\n");
    printf("========================================\n");
    printf("本程序演示循环链表的各种操作和应用场景\n");
    
    // 基本操作演示
    demonstrate_basic_operations();
    
    // 边界情况演示
    demonstrate_edge_cases();
    
    // 与其他链表类型对比
    demonstrate_comparison();
    
    // 实际应用场景
    demonstrate_practical_usage();
    
    printf("\n🎊 所有演示完成！\n");
    printf("循环链表提供了与单链表和双链表相同的接口，\n");
    printf("但具有循环结构的特殊性质，适用于需要循环访问的场景。\n");
    
    return 0;
}
