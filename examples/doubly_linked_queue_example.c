/**
 * @file doubly_linked_queue_example.c
 * @brief 双向链表队列使用示例
 * @details 演示双向链表队列的FIFO特性和实际应用场景
 * @author DSA Team
 * @date 2025-07-22
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <adt/queue.h>

// 辅助函数：创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    if (data) {
        *data = value;
    }
    return data;
}

// 辅助函数：创建字符串数据
static char* create_string(const char* str) {
    char* data = malloc(strlen(str) + 1);
    if (data) {
        strcpy(data, str);
    }
    return data;
}

// 辅助函数：打印队列状态
static void print_queue_status(dsa_queue_t *queue, const char* operation) {
    printf("操作: %s\n", operation);
    printf("  大小: %zu\n", queue_size(queue));
    printf("  是否为空: %s\n", queue_is_empty(queue) ? "是" : "否");
    printf("\n");
}

/**
 * @brief 演示队列的基本操作
 */
static void demonstrate_basic_operations(void) {
    printf("\n🔗 双向链表队列基本操作演示\n");
    printf("========================================\n");
    
    // 创建双向链表队列
    dsa_queue_t *queue = queue_create(QUEUE_TYPE_DOUBLY_LINKED_LIST);
    if (!queue) {
        printf("❌ 创建双向链表队列失败\n");
        return;
    }
    printf("✅ 成功创建双向链表队列\n");
    
    print_queue_status(queue, "初始状态");
    
    // 入队元素
    printf("\n📥 入队元素演示\n");
    int values[] = {100, 200, 300, 400, 500};
    for (int i = 0; i < 5; i++) {
        int *data = create_int(values[i]);
        dsa_result_t result = queue_enqueue(queue, data);
        if (result == DSA_SUCCESS) {
            printf("✅ 入队元素: %d\n", values[i]);
        } else {
            printf("❌ 入队元素%d失败\n", values[i]);
            free(data);
        }
    }
    
    print_queue_status(queue, "入队5个元素后");
    
    // 出队元素
    printf("\n📤 出队元素演示\n");
    for (int i = 0; i < 3; i++) {
        dsa_element_pt element = queue_dequeue(queue);
        if (element) {
            printf("✅ 出队元素: %d\n", *(int*)element);
            free(element);
        } else {
            printf("❌ 出队失败\n");
        }
    }
    
    print_queue_status(queue, "出队3个元素后");
    
    // 清空队列
    printf("\n🧹 清空队列\n");
    dsa_result_t result = queue_clear(queue);
    if (result == DSA_SUCCESS) {
        printf("✅ 队列已清空\n");
    }
    print_queue_status(queue, "清空后");
    
    // 销毁队列
    queue_destroy(queue);
    printf("✅ 队列已销毁\n");
}

/**
 * @brief 演示队列的动态特性
 */
static void demonstrate_dynamic_nature(void) {
    printf("\n🌊 双向链表队列动态特性演示\n");
    printf("========================================\n");
    
    dsa_queue_t *queue = queue_create(QUEUE_TYPE_DOUBLY_LINKED_LIST);
    if (!queue) {
        printf("❌ 创建队列失败\n");
        return;
    }
    
    printf("🔄 演示动态内存分配特性:\n");
    printf("双向链表队列无需预设容量，可以动态增长\n\n");
    
    // 演示动态增长
    printf("📈 动态增长演示:\n");
    for (int i = 1; i <= 10; i++) {
        int *data = create_int(i * 10);
        if (queue_enqueue(queue, data) == DSA_SUCCESS) {
            printf("  第%d次入队: %d (当前大小: %zu)\n", 
                   i, i * 10, queue_size(queue));
        } else {
            free(data);
        }
    }
    
    printf("\n📉 动态缩减演示:\n");
    for (int i = 1; i <= 5; i++) {
        dsa_element_pt element = queue_dequeue(queue);
        if (element) {
            printf("  第%d次出队: %d (剩余大小: %zu)\n", 
                   i, *(int*)element, queue_size(queue));
            free(element);
        }
    }
    
    printf("\n💡 动态特性优势:\n");
    printf("  ✅ 无需预设容量限制\n");
    printf("  ✅ 内存按需分配\n");
    printf("  ✅ 适合数据量不确定的场景\n");
    printf("  ✅ 避免内存浪费\n");
    
    // 清理剩余元素
    while (!queue_is_empty(queue)) {
        dsa_element_pt element = queue_dequeue(queue);
        if (element) {
            free(element);
        }
    }
    
    queue_destroy(queue);
    printf("✅ 动态特性演示完成\n");
}

/**
 * @brief 演示队列处理不同数据类型
 */
static void demonstrate_data_type_flexibility(void) {
    printf("\n🎭 数据类型灵活性演示\n");
    printf("========================================\n");
    printf("场景: 消息队列处理不同类型的消息\n");
    
    dsa_queue_t *message_queue = queue_create(QUEUE_TYPE_DOUBLY_LINKED_LIST);
    if (!message_queue) {
        printf("❌ 创建消息队列失败\n");
        return;
    }
    
    printf("📨 模拟处理不同类型的消息:\n");
    
    // 添加字符串消息
    const char *messages[] = {
        "系统启动完成", "用户登录成功", "数据备份开始", 
        "网络连接异常", "任务执行完成"
    };
    
    printf("\n📤 发送消息:\n");
    for (int i = 0; i < 5; i++) {
        char *message = create_string(messages[i]);
        if (queue_enqueue(message_queue, message) == DSA_SUCCESS) {
            printf("  → 消息 %d: \"%s\" 已入队\n", i + 1, messages[i]);
            printf("    队列中消息数: %zu\n", queue_size(message_queue));
        } else {
            free(message);
        }
    }
    
    printf("\n📥 处理消息 (FIFO顺序):\n");
    int processed = 0;
    while (!queue_is_empty(message_queue)) {
        dsa_element_pt message = queue_dequeue(message_queue);
        if (message) {
            processed++;
            printf("  ✅ 处理消息 %d: \"%s\"\n", processed, (char*)message);
            printf("    剩余消息数: %zu\n", queue_size(message_queue));
            free(message);
        }
    }
    
    printf("\n📊 消息处理总结:\n");
    printf("  ✅ 共处理 %d 条消息\n", processed);
    printf("  ✅ 按接收顺序处理\n");
    printf("  ✅ 支持任意数据类型\n");
    
    queue_destroy(message_queue);
    printf("✅ 数据类型演示完成\n");
}

/**
 * @brief 演示队列的内存管理特点
 */
static void demonstrate_memory_management(void) {
    printf("\n🧠 内存管理特点演示\n");
    printf("========================================\n");
    
    dsa_queue_t *queue = queue_create(QUEUE_TYPE_DOUBLY_LINKED_LIST);
    if (!queue) {
        printf("❌ 创建队列失败\n");
        return;
    }
    
    printf("💾 双向链表队列的内存管理特点:\n");
    printf("  - 节点内存: 按需分配 ✅\n");
    printf("  - 内存释放: 及时回收 ✅\n");
    printf("  - 内存碎片: 可能存在 ⚠️\n");
    printf("  - 指针开销: 每节点2个指针 📊\n");
    
    // 演示内存使用模式
    printf("\n🔄 演示内存使用模式:\n");
    
    // 大量入队
    printf("  📈 大量入队 (模拟内存分配):\n");
    for (int i = 1; i <= 1000; i++) {
        int *data = create_int(i);
        if (queue_enqueue(queue, data) == DSA_SUCCESS) {
            if (i % 200 == 0) {
                printf("    已入队 %d 个元素 (队列大小: %zu)\n", 
                       i, queue_size(queue));
            }
        } else {
            free(data);
        }
    }
    
    // 部分出队
    printf("  📉 部分出队 (模拟内存释放):\n");
    for (int i = 1; i <= 500; i++) {
        dsa_element_pt element = queue_dequeue(queue);
        if (element) {
            free(element);
            if (i % 100 == 0) {
                printf("    已出队 %d 个元素 (剩余: %zu)\n", 
                       i, queue_size(queue));
            }
        }
    }
    
    // 清空队列
    printf("  🧹 清空队列 (释放所有内存):\n");
    size_t remaining = queue_size(queue);
    dsa_result_t result = queue_clear(queue);
    if (result == DSA_SUCCESS) {
        printf("    ✅ 已释放 %zu 个节点的内存\n", remaining);
        printf("    ✅ 队列大小: %zu\n", queue_size(queue));
    }
    
    printf("\n💡 内存管理总结:\n");
    printf("  🎯 优点: 按需分配，无容量限制\n");
    printf("  🎯 缺点: 可能产生内存碎片\n");
    printf("  🎯 适用: 数据量变化大的场景\n");
    
    queue_destroy(queue);
    printf("✅ 内存管理演示完成\n");
}

int main(void) {
    printf("🎉 双向链表队列示例程序\n");
    printf("========================================\n");
    printf("本程序演示基于双向链表实现的队列的各种操作和特性\n");
    
    // 基本操作演示
    demonstrate_basic_operations();
    
    // 动态特性演示
    demonstrate_dynamic_nature();
    
    // 数据类型灵活性演示
    demonstrate_data_type_flexibility();
    
    // 内存管理特点演示
    demonstrate_memory_management();
    
    printf("\n🎊 所有演示完成！\n");
    printf("双向链表队列特点总结:\n");
    printf("✅ FIFO (先进先出) 访问模式\n");
    printf("✅ 动态内存分配，无容量限制\n");
    printf("✅ O(1)时间复杂度的入队/出队操作\n");
    printf("✅ 支持任意数据类型\n");
    printf("✅ 内存按需分配和释放\n");
    printf("⚠️ 可能产生内存碎片\n");
    printf("⚠️ 每个节点有指针开销\n");
    printf("🎯 适用场景: 数据量不确定、需要灵活内存管理的应用\n");
    
    return 0;
}
