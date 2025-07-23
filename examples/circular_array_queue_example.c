/**
 * @file circular_array_queue_example.c
 * @brief 循环数组队列使用示例
 * @details 演示循环数组队列的FIFO特性和实际应用场景
 * @author DSA Team
 * @date 2025-07-22
 */

#include <stdio.h>
#include <stdlib.h>
#include <adt/queue.h>

// 辅助函数：创建整数数据
static int* create_int(int value) {
    int* data = malloc(sizeof(int));
    if (data) {
        *data = value;
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
    printf("\n🔄 循环数组队列基本操作演示\n");
    printf("========================================\n");
    
    // 创建循环数组队列
    dsa_queue_t *queue = queue_create(QUEUE_TYPE_CIRCULAR_ARRAY_LIST);
    if (!queue) {
        printf("❌ 创建循环数组队列失败\n");
        return;
    }
    printf("✅ 成功创建循环数组队列\n");
    
    print_queue_status(queue, "初始状态");
    
    // 入队元素
    printf("\n📥 入队元素演示\n");
    int values[] = {10, 20, 30, 40, 50};
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
 * @brief 演示队列的FIFO特性
 */
static void demonstrate_fifo_behavior(void) {
    printf("\n🎫 队列的FIFO(先进先出)特性演示\n");
    printf("========================================\n");
    
    dsa_queue_t *queue = queue_create(QUEUE_TYPE_CIRCULAR_ARRAY_LIST);
    if (!queue) {
        printf("❌ 创建队列失败\n");
        return;
    }
    
    printf("🔄 演示FIFO原理:\n");
    printf("想象银行排队，先到的客户先办理业务\n\n");
    
    // 模拟客户排队
    const char *customers[] = {"张三", "李四", "王五", "赵六", "钱七"};
    printf("👥 客户依次排队:\n");
    for (int i = 0; i < 5; i++) {
        int *customer_id = create_int(i + 1);
        if (queue_enqueue(queue, customer_id) == DSA_SUCCESS) {
            printf("  %d. %s 排队 (编号: %d)\n", i + 1, customers[i], i + 1);
        } else {
            free(customer_id);
        }
    }
    
    printf("\n🏦 当前排队状态:\n");
    printf("  队列中有 %zu 位客户等待\n", queue_size(queue));
    printf("  队首: %s (最先到达)\n", customers[0]);
    printf("  队尾: %s (最后到达)\n", customers[4]);
    
    // 模拟办理业务
    printf("\n💼 依次办理业务 (FIFO顺序):\n");
    int order = 1;
    while (!queue_is_empty(queue)) {
        dsa_element_pt customer = queue_dequeue(queue);
        if (customer) {
            int customer_id = *(int*)customer;
            printf("  %d. %s 办理业务完成\n", order++, customers[customer_id - 1]);
            printf("     剩余等待客户: %zu 位\n", queue_size(queue));
            free(customer);
        }
    }
    
    printf("\n💡 观察结果:\n");
    printf("  - 排队顺序: 张三 → 李四 → 王五 → 赵六 → 钱七\n");
    printf("  - 办理顺序: 张三 → 李四 → 王五 → 赵六 → 钱七\n");
    printf("  - 特点: 先到先服务 (First In, First Out)\n");
    
    queue_destroy(queue);
    printf("✅ FIFO演示完成\n");
}

/**
 * @brief 演示队列的实际应用场景
 */
static void demonstrate_practical_usage(void) {
    printf("\n🎯 实际应用场景演示\n");
    printf("========================================\n");
    printf("场景: 打印任务队列管理\n");
    
    dsa_queue_t *print_queue = queue_create(QUEUE_TYPE_CIRCULAR_ARRAY_LIST);
    if (!print_queue) {
        printf("❌ 创建打印队列失败\n");
        return;
    }
    
    printf("🖨️ 模拟打印任务管理:\n");
    
    // 打印任务信息
    const char *documents[] = {
        "年度报告.pdf", "会议纪要.docx", "项目计划.xlsx", 
        "用户手册.pdf", "财务报表.xlsx"
    };
    
    // 模拟提交打印任务
    printf("\n📄 提交打印任务:\n");
    for (int i = 0; i < 5; i++) {
        int *task_id = create_int(i + 1);
        if (queue_enqueue(print_queue, task_id) == DSA_SUCCESS) {
            printf("  → 任务 %d: %s 已加入打印队列\n", i + 1, documents[i]);
            printf("    当前队列长度: %zu\n", queue_size(print_queue));
        } else {
            free(task_id);
        }
    }
    
    printf("\n🖨️ 按顺序处理打印任务:\n");
    int completed = 0;
    while (!queue_is_empty(print_queue)) {
        dsa_element_pt task = queue_dequeue(print_queue);
        if (task) {
            int task_id = *(int*)task;
            completed++;
            printf("  ✅ 正在打印: %s\n", documents[task_id - 1]);
            printf("    已完成: %d/%d, 剩余: %zu\n", 
                   completed, 5, queue_size(print_queue));
            free(task);
        }
    }
    
    printf("\n📊 打印队列管理总结:\n");
    printf("  ✅ 所有打印任务已完成\n");
    printf("  ✅ 任务按提交顺序执行\n");
    printf("  ✅ 确保公平性和有序性\n");
    
    queue_destroy(print_queue);
    printf("✅ 应用场景演示完成\n");
}

/**
 * @brief 演示循环数组队列的性能特点
 */
static void demonstrate_performance_characteristics(void) {
    printf("\n⚡ 循环数组队列性能特点演示\n");
    printf("========================================\n");
    
    dsa_queue_t *queue = queue_create(QUEUE_TYPE_CIRCULAR_ARRAY_LIST);
    if (!queue) {
        printf("❌ 创建队列失败\n");
        return;
    }
    
    printf("📊 循环数组队列的性能特点:\n");
    printf("  - 入队操作: O(1)平摊 - 平摊常数时间 ✅\n");
    printf("  - 出队操作: O(1) - 常数时间 ✅\n");
    printf("  - 获取大小: O(1) - 常数时间 ✅\n");
    printf("  - 空间复杂度: O(n) - 线性空间 ✅\n");
    printf("  - 内存局部性: 优秀 - 缓存友好 ✅\n");
    
    // 演示高效操作
    printf("\n🚀 演示高效的队列操作:\n");
    
    // 快速入队
    printf("  📥 快速入队100个元素: ");
    for (int i = 1; i <= 100; i++) {
        int *data = create_int(i);
        if (queue_enqueue(queue, data) == DSA_SUCCESS) {
            // 成功
        } else {
            free(data);
        }
    }
    printf("完成 (队列大小: %zu)\n", queue_size(queue));
    
    // 快速出队
    printf("  📤 快速出队50个元素: ");
    for (int i = 0; i < 50; i++) {
        dsa_element_pt element = queue_dequeue(queue);
        if (element) {
            free(element);
        }
    }
    printf("完成 (剩余大小: %zu)\n", queue_size(queue));
    
    // 混合操作
    printf("  🔄 混合操作 (入队+出队): ");
    for (int i = 0; i < 25; i++) {
        // 入队一个
        int *data = create_int(i + 200);
        queue_enqueue(queue, data);
        
        // 出队一个
        dsa_element_pt element = queue_dequeue(queue);
        if (element) {
            free(element);
        }
    }
    printf("完成 (最终大小: %zu)\n", queue_size(queue));
    
    // 清理剩余元素
    while (!queue_is_empty(queue)) {
        dsa_element_pt element = queue_dequeue(queue);
        if (element) {
            free(element);
        }
    }
    
    printf("\n💡 性能优势总结:\n");
    printf("  🎯 适用场景: 需要高效入队出队的应用\n");
    printf("  🎯 内存效率: 连续内存分配，缓存友好\n");
    printf("  🎯 时间效率: 大部分操作都是O(1)时间复杂度\n");
    
    queue_destroy(queue);
    printf("✅ 性能演示完成\n");
}

int main(void) {
    printf("🎉 循环数组队列示例程序\n");
    printf("========================================\n");
    printf("本程序演示基于循环数组实现的队列的各种操作和特性\n");
    
    // 基本操作演示
    demonstrate_basic_operations();
    
    // FIFO特性演示
    demonstrate_fifo_behavior();
    
    // 实际应用场景演示
    demonstrate_practical_usage();
    
    // 性能特点演示
    demonstrate_performance_characteristics();
    
    printf("\n🎊 所有演示完成！\n");
    printf("循环数组队列特点总结:\n");
    printf("✅ FIFO (先进先出) 访问模式\n");
    printf("✅ 高效的O(1)入队/出队操作\n");
    printf("✅ 优秀的内存局部性\n");
    printf("✅ 自动扩容能力\n");
    printf("✅ 适合高性能应用场景\n");
    printf("🎯 适用场景: 任务调度、缓冲区管理、BFS算法等\n");
    
    return 0;
}
