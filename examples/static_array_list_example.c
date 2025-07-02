/**
 * @file static_array_list_example.c
 * @brief 静态数组使用示例
 * @details 演示静态数组的创建、插入、删除、查询等基本操作
 * @author oaheixiehao
 * @date 2025/7/2
 */

#include <stdio.h>
#include <stdlib.h>
#include <ds/array_list.h>

/**
 * @brief 辅助函数：打印数组内容
 */
static void print_array(dsa_array_list_t *arr, const char *description) {
    printf("\n=== %s ===\n", description);
    printf("数组大小: %zu\n", array_list_size(arr));
    printf("数组容量: %zu\n", array_list_capacity(arr));
    printf("是否为空: %s\n", array_list_is_empty(arr) ? "是" : "否");
    printf("是否已满: %s\n", array_list_is_full(arr) ? "是" : "否");
    
    if (!array_list_is_empty(arr)) {
        printf("数组内容: ");
        for (size_t i = 0; i < array_list_size(arr); i++) {
            dsa_element_pt element = array_list_get(arr, i);
            if (element) {
                int value = ELEMENT_VALUE(int, element);
                printf("%d ", value);
            }
        }
        printf("\n");
    }
}

/**
 * @brief 演示静态数组的基本操作
 */
static void demonstrate_basic_operations(void) {
    printf("\n📦 静态数组基本操作演示\n");
    printf("========================================\n");
    
    // 创建静态数组
    int buffer[10];  // 静态缓冲区
    dsa_array_list_t *arr = array_list_create_static(buffer, 10, sizeof(int));
    if (!arr) {
        printf("❌ 创建静态数组失败\n");
        return;
    }
    printf("✅ 成功创建容量为10的静态数组\n");
    
    print_array(arr, "初始状态");
    
    // 添加元素
    printf("\n📥 添加元素演示\n");
    for (int i = 1; i <= 5; i++) {
        int value = i * 10;
        dsa_result_t result = array_list_push_back(arr, &value);
        if (result == DSA_SUCCESS) {
            printf("✅ 添加元素%d\n", value);
        } else {
            printf("❌ 添加元素%d失败\n", value);
        }
    }
    print_array(arr, "添加5个元素后");
    
    // 插入元素
    printf("\n📍 插入元素演示\n");
    int insert_value = 25;
    if (array_list_insert(arr, 2, &insert_value) == DSA_SUCCESS) {
        printf("✅ 在索引2插入元素25\n");
    }
    print_array(arr, "插入元素后");
    
    // 获取元素
    printf("\n🔍 元素访问演示\n");
    for (size_t i = 0; i < array_list_size(arr); i++) {
        dsa_element_pt element = array_list_get(arr, i);
        if (element) {
            int value = ELEMENT_VALUE(int, element);
            printf("索引%zu的元素: %d\n", i, value);
        }
    }
    
    // 修改元素
    printf("\n✏️ 元素修改演示\n");
    int new_value = 99;
    if (array_list_set(arr, 3, &new_value) == DSA_SUCCESS) {
        printf("✅ 将索引3的元素修改为99\n");
    }
    print_array(arr, "修改元素后");
    
    // 删除元素
    printf("\n🗑️ 删除操作演示\n");
    dsa_element_pt removed = array_list_remove(arr, 2);
    if (removed) {
        int value = ELEMENT_VALUE(int, removed);
        printf("✅ 删除索引2的元素: %d\n", value);
    }
    print_array(arr, "删除元素后");
    
    // 弹出元素
    printf("\n📤 弹出操作演示\n");
    dsa_element_pt popped = array_list_pop_back(arr);
    if (popped) {
        int value = ELEMENT_VALUE(int, popped);
        printf("✅ 弹出尾部元素: %d\n", value);
    }
    print_array(arr, "弹出元素后");
    
    // 销毁数组
    array_list_destroy(arr);
    printf("✅ 数组已销毁\n");
}

/**
 * @brief 演示静态数组的容量限制
 */
static void demonstrate_capacity_limits(void) {
    printf("\n⚠️ 静态数组容量限制演示\n");
    printf("========================================\n");
    
    // 创建小容量静态数组
    int small_buffer[3];
    dsa_array_list_t *arr = array_list_create_static(small_buffer, 3, sizeof(int));
    if (!arr) {
        printf("❌ 创建静态数组失败\n");
        return;
    }
    printf("✅ 创建容量为3的静态数组\n");
    
    print_array(arr, "初始状态");
    
    // 填满数组
    printf("\n📦 填满数组:\n");
    for (int i = 1; i <= 3; i++) {
        int value = i * 10;
        dsa_result_t result = array_list_push_back(arr, &value);
        printf("  添加%d: %s\n", value, 
               result == DSA_SUCCESS ? "成功" : "失败");
    }
    print_array(arr, "数组已满");
    
    // 尝试继续添加
    printf("\n🚫 尝试超出容量:\n");
    int overflow_value = 40;
    dsa_result_t result = array_list_push_back(arr, &overflow_value);
    printf("  尝试添加%d: %s\n", overflow_value,
           result == DSA_SUCCESS ? "成功" : "失败（预期）");
    
    printf("📊 容量状态:\n");
    printf("  - 当前大小: %zu\n", array_list_size(arr));
    printf("  - 最大容量: %zu\n", array_list_capacity(arr));
    printf("  - 是否已满: %s\n", array_list_is_full(arr) ? "是" : "否");
    
    array_list_destroy(arr);
    printf("✅ 容量限制演示完成\n");
}

/**
 * @brief 演示静态数组的性能特点
 */
static void demonstrate_performance_characteristics(void) {
    printf("\n⚡ 静态数组性能特点演示\n");
    printf("========================================\n");
    
    int buffer[100];
    dsa_array_list_t *arr = array_list_create_static(buffer, 100, sizeof(int));
    if (!arr) {
        printf("❌ 创建静态数组失败\n");
        return;
    }
    
    printf("📊 静态数组的时间复杂度特点:\n");
    printf("  - 随机访问: O(1) - 非常快 ✅\n");
    printf("  - 尾部插入: O(1) - 非常快 ✅\n");
    printf("  - 中间插入: O(n) - 需要移动元素 ⚠️\n");
    printf("  - 删除操作: O(n) - 需要移动元素 ⚠️\n");
    printf("  - 查找元素: O(n) - 线性搜索 ⚠️\n");
    
    // 演示快速随机访问
    printf("\n🚀 演示快速随机访问:\n");
    // 先添加一些数据
    for (int i = 0; i < 10; i++) {
        int value = i * 5;
        array_list_push_back(arr, &value);
    }
    
    // 随机访问不同位置
    size_t indices[] = {0, 4, 7, 9};
    for (int i = 0; i < 4; i++) {
        dsa_element_pt element = array_list_get(arr, indices[i]);
        if (element) {
            int value = ELEMENT_VALUE(int, element);
            printf("  快速访问索引%zu: %d\n", indices[i], value);
        }
    }
    
    // 演示高效的尾部操作
    printf("\n📤 演示高效的尾部操作:\n");
    printf("  添加到尾部: ");
    int tail_value = 999;
    if (array_list_push_back(arr, &tail_value) == DSA_SUCCESS) {
        printf("成功 ✅\n");
    }
    
    printf("  从尾部删除: ");
    dsa_element_pt popped = array_list_pop_back(arr);
    if (popped) {
        printf("成功，删除了%d ✅\n", ELEMENT_VALUE(int, popped));
    }
    
    array_list_destroy(arr);
    printf("✅ 性能演示完成\n");
}

/**
 * @brief 演示静态数组的实际应用场景
 */
static void demonstrate_practical_usage(void) {
    printf("\n🎯 实际应用场景演示\n");
    printf("========================================\n");
    printf("场景: 传感器数据缓冲区（固定大小）\n");
    
    // 创建传感器数据缓冲区
    int sensor_buffer[5];  // 只保存最近5个读数
    dsa_array_list_t *sensor_data = array_list_create_static(sensor_buffer, 5, sizeof(int));
    if (!sensor_data) {
        printf("❌ 创建传感器缓冲区失败\n");
        return;
    }
    
    printf("🌡️ 模拟传感器数据采集:\n");
    
    // 模拟传感器读数
    int readings[] = {23, 25, 24, 26, 28, 30, 29, 27};
    
    for (int i = 0; i < 8; i++) {
        printf("\n  📊 第%d次读数: %d°C\n", i + 1, readings[i]);
        
        if (array_list_is_full(sensor_data)) {
            // 缓冲区满了，移除最旧的数据
            dsa_element_pt oldest = array_list_remove(sensor_data, 0);
            if (oldest) {
                printf("    🗑️ 移除最旧数据: %d°C\n", ELEMENT_VALUE(int, oldest));
            }
        }
        
        // 添加新数据
        if (array_list_push_back(sensor_data, &readings[i]) == DSA_SUCCESS) {
            printf("    ✅ 添加新数据: %d°C\n", readings[i]);
        }
        
        // 显示当前缓冲区状态
        printf("    📋 当前缓冲区: ");
        for (size_t j = 0; j < array_list_size(sensor_data); j++) {
            dsa_element_pt element = array_list_get(sensor_data, j);
            if (element) {
                printf("%d°C ", ELEMENT_VALUE(int, element));
            }
        }
        printf("(大小: %zu/%zu)\n", 
               array_list_size(sensor_data), 
               array_list_capacity(sensor_data));
    }
    
    // 计算平均温度
    printf("\n📈 计算平均温度:\n");
    if (!array_list_is_empty(sensor_data)) {
        int sum = 0;
        size_t count = array_list_size(sensor_data);
        
        for (size_t i = 0; i < count; i++) {
            dsa_element_pt element = array_list_get(sensor_data, i);
            if (element) {
                sum += ELEMENT_VALUE(int, element);
            }
        }
        
        double average = (double)sum / count;
        printf("  📊 最近%zu次读数的平均温度: %.1f°C\n", count, average);
    }
    
    array_list_destroy(sensor_data);
    printf("✅ 传感器数据演示完成\n");
}

/**
 * @brief 演示静态数组的内存特点
 */
static void demonstrate_memory_characteristics(void) {
    printf("\n💾 静态数组内存特点演示\n");
    printf("========================================\n");
    
    printf("📋 静态数组的内存特点:\n");
    printf("  ✅ 内存在栈上分配，速度快\n");
    printf("  ✅ 不需要动态内存管理\n");
    printf("  ✅ 内存布局连续，缓存友好\n");
    printf("  ⚠️ 容量固定，不能扩展\n");
    printf("  ⚠️ 生命周期受限于作用域\n");
    
    // 演示栈上分配
    printf("\n🏗️ 演示栈上内存分配:\n");
    {
        int local_buffer[5] = {1, 2, 3, 4, 5};
        dsa_array_list_t *local_arr = array_list_create_static(local_buffer, 5, sizeof(int));
        
        if (local_arr) {
            printf("  ✅ 在局部作用域创建静态数组\n");
            printf("  📍 缓冲区地址: %p\n", (void*)local_buffer);
            printf("  📦 数组容量: %zu\n", array_list_capacity(local_arr));
            
            // 预填充数据
            for (int i = 0; i < 5; i++) {
                // 数据已经在缓冲区中，只需要设置大小
                // 这里我们手动设置，实际使用中应该通过API
            }
            
            array_list_destroy(local_arr);
            printf("  ✅ 数组已销毁，但缓冲区仍在栈上\n");
        }
        printf("  📝 离开作用域时，栈内存自动回收\n");
    }
    
    printf("\n💡 使用建议:\n");
    printf("  🎯 适用于: 大小固定、性能敏感的场景\n");
    printf("  🎯 避免: 需要动态扩展的场景\n");
    printf("  🎯 注意: 避免返回指向局部缓冲区的数组\n");
    
    printf("✅ 内存特点演示完成\n");
}

int main(void) {
    printf("🎉 静态数组示例程序\n");
    printf("========================================\n");
    printf("本程序演示静态数组的各种操作和特性\n");
    
    // 基本操作演示
    demonstrate_basic_operations();
    
    // 容量限制演示
    demonstrate_capacity_limits();
    
    // 性能特点演示
    demonstrate_performance_characteristics();
    
    // 实际应用场景
    demonstrate_practical_usage();
    
    // 内存特点演示
    demonstrate_memory_characteristics();
    
    printf("\n🎊 所有演示完成！\n");
    printf("静态数组特点总结:\n");
    printf("✅ 快速随机访问 O(1)\n");
    printf("✅ 内存效率高，栈上分配\n");
    printf("✅ 缓存友好，性能优秀\n");
    printf("⚠️ 容量固定，不可扩展\n");
    printf("🎯 适用场景: 大小确定、性能要求高的数据集合\n");
    
    return 0;
}
