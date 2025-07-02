/**
 * @file dynamic_array_list_example.c
 * @brief 动态数组使用示例
 * @details 演示动态数组的创建、插入、删除、查询等基本操作
 * @author oaheixiehao
 * @date 2025/7/2
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ds/array_list.h>

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
 * @brief 辅助函数：打印数组内容
 */
static void print_array(dsa_array_list_t *arr, const char *description) {
    printf("\n=== %s ===\n", description);
    printf("数组大小: %zu\n", array_list_size(arr));
    printf("数组容量: %zu\n", array_list_capacity(arr));
    printf("是否为空: %s\n", array_list_is_empty(arr) ? "是" : "否");
    
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
 * @brief 演示动态数组的基本操作
 */
static void demonstrate_basic_operations(void) {
    printf("\n🔄 动态数组基本操作演示\n");
    printf("========================================\n");
    
    // 创建动态数组
    dsa_array_list_t *arr = array_list_create_dynamic(5);
    if (!arr) {
        printf("❌ 创建动态数组失败\n");
        return;
    }
    printf("✅ 成功创建初始容量为5的动态数组\n");
    
    print_array(arr, "初始状态");
    
    // 添加元素
    printf("\n📥 添加元素演示\n");
    for (int i = 1; i <= 5; i++) {
        int *data = create_int(i * 10);
        dsa_result_t result = array_list_push_back(arr, data);
        if (result == DSA_SUCCESS) {
            printf("✅ 添加元素%d\n", i * 10);
        } else {
            printf("❌ 添加元素%d失败\n", i * 10);
            free(data);
        }
    }
    print_array(arr, "添加5个元素后");
    
    // 插入元素
    printf("\n📍 插入元素演示\n");
    int *insert_data = create_int(25);
    if (array_list_insert(arr, 2, insert_data) == DSA_SUCCESS) {
        printf("✅ 在索引2插入元素25\n");
    } else {
        free(insert_data);
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
    int *new_data = create_int(99);
    if (array_list_set(arr, 3, new_data) == DSA_SUCCESS) {
        printf("✅ 将索引3的元素修改为99\n");
    } else {
        free(new_data);
    }
    print_array(arr, "修改元素后");
    
    // 删除元素
    printf("\n🗑️ 删除操作演示\n");
    dsa_element_pt removed = array_list_remove(arr, 2);
    if (removed) {
        int value = ELEMENT_VALUE(int, removed);
        printf("✅ 删除索引2的元素: %d\n", value);
        free(removed);
    }
    print_array(arr, "删除元素后");
    
    // 弹出元素
    printf("\n📤 弹出操作演示\n");
    dsa_element_pt popped = array_list_pop_back(arr);
    if (popped) {
        int value = ELEMENT_VALUE(int, popped);
        printf("✅ 弹出尾部元素: %d\n", value);
        free(popped);
    }
    print_array(arr, "弹出元素后");
    
    // 清理并销毁数组
    array_list_clear_with_free(arr);
    array_list_destroy(arr);
    printf("✅ 数组已清理并销毁\n");
}

/**
 * @brief 演示动态数组的自动扩容
 */
static void demonstrate_auto_expansion(void) {
    printf("\n📈 动态数组自动扩容演示\n");
    printf("========================================\n");
    
    // 创建小容量动态数组
    dsa_array_list_t *arr = array_list_create_dynamic(2);
    if (!arr) {
        printf("❌ 创建动态数组失败\n");
        return;
    }
    printf("✅ 创建初始容量为2的动态数组\n");
    
    print_array(arr, "初始状态");
    
    // 逐步添加元素，观察扩容
    printf("\n🔄 逐步添加元素，观察自动扩容:\n");
    for (int i = 1; i <= 8; i++) {
        int *data = create_int(i * 5);
        
        printf("  添加元素%d前 - 容量: %zu, 大小: %zu\n", 
               i * 5, array_list_capacity(arr), array_list_size(arr));
        
        dsa_result_t result = array_list_push_back(arr, data);
        if (result == DSA_SUCCESS) {
            printf("  ✅ 成功添加元素%d\n", i * 5);
            printf("  添加后 - 容量: %zu, 大小: %zu", 
                   array_list_capacity(arr), array_list_size(arr));
            
            // 检查是否发生了扩容
            static size_t last_capacity = 2;
            if (array_list_capacity(arr) > last_capacity) {
                printf(" 🚀 发生扩容！\n");
                last_capacity = array_list_capacity(arr);
            } else {
                printf("\n");
            }
        } else {
            printf("  ❌ 添加元素%d失败\n", i * 5);
            free(data);
        }
        printf("\n");
    }
    
    print_array(arr, "最终状态");
    
    printf("📊 扩容策略观察:\n");
    printf("  - 初始容量: 2\n");
    printf("  - 最终容量: %zu\n", array_list_capacity(arr));
    printf("  - 扩容次数: 约%zu次\n", 
           (size_t)(log2(array_list_capacity(arr) / 2.0)));
    
    // 清理
    array_list_clear_with_free(arr);
    array_list_destroy(arr);
    printf("✅ 扩容演示完成\n");
}

/**
 * @brief 演示动态数组的性能特点
 */
static void demonstrate_performance_characteristics(void) {
    printf("\n⚡ 动态数组性能特点演示\n");
    printf("========================================\n");
    
    dsa_array_list_t *arr = array_list_create_dynamic(10);
    if (!arr) {
        printf("❌ 创建动态数组失败\n");
        return;
    }
    
    printf("📊 动态数组的时间复杂度特点:\n");
    printf("  - 随机访问: O(1) - 非常快 ✅\n");
    printf("  - 尾部插入: O(1)* - 平摊常数时间 ✅\n");
    printf("  - 中间插入: O(n) - 需要移动元素 ⚠️\n");
    printf("  - 删除操作: O(n) - 需要移动元素 ⚠️\n");
    printf("  - 自动扩容: O(n) - 偶尔发生 ⚠️\n");
    printf("  (*平摊分析：大多数情况下是O(1))\n");
    
    // 演示快速随机访问
    printf("\n🚀 演示快速随机访问:\n");
    // 先添加一些数据
    for (int i = 0; i < 10; i++) {
        int *data = create_int(i * 3);
        array_list_push_back(arr, data);
    }
    
    // 随机访问不同位置
    size_t indices[] = {0, 3, 6, 9};
    for (int i = 0; i < 4; i++) {
        dsa_element_pt element = array_list_get(arr, indices[i]);
        if (element) {
            int value = ELEMENT_VALUE(int, element);
            printf("  快速访问索引%zu: %d\n", indices[i], value);
        }
    }
    
    // 演示高效的尾部操作
    printf("\n📤 演示高效的尾部操作:\n");
    int *tail_data = create_int(999);
    printf("  添加到尾部: ");
    if (array_list_push_back(arr, tail_data) == DSA_SUCCESS) {
        printf("成功 ✅\n");
    } else {
        printf("失败 ❌\n");
        free(tail_data);
    }
    
    printf("  从尾部删除: ");
    dsa_element_pt popped = array_list_pop_back(arr);
    if (popped) {
        printf("成功，删除了%d ✅\n", ELEMENT_VALUE(int, popped));
        free(popped);
    }
    
    // 清理
    array_list_clear_with_free(arr);
    array_list_destroy(arr);
    printf("✅ 性能演示完成\n");
}

/**
 * @brief 演示动态数组的实际应用场景
 */
static void demonstrate_practical_usage(void) {
    printf("\n🎯 实际应用场景演示\n");
    printf("========================================\n");
    printf("场景: 动态购物车（可变商品数量）\n");
    
    // 创建购物车
    dsa_array_list_t *shopping_cart = array_list_create_dynamic(3);
    if (!shopping_cart) {
        printf("❌ 创建购物车失败\n");
        return;
    }
    
    printf("🛒 模拟购物车操作:\n");
    
    // 商品信息（用ID表示）
    const char *products[] = {
        "苹果", "香蕉", "橙子", "牛奶", "面包", 
        "鸡蛋", "大米", "蔬菜", "肉类", "饮料"
    };
    
    // 添加商品到购物车
    printf("\n📦 添加商品到购物车:\n");
    int product_ids[] = {1, 3, 5, 7};
    for (int i = 0; i < 4; i++) {
        int *product_id = create_int(product_ids[i]);
        if (array_list_push_back(shopping_cart, product_id) == DSA_SUCCESS) {
            printf("  ✅ 添加: %s (ID: %d)\n", 
                   products[product_ids[i] - 1], product_ids[i]);
        } else {
            free(product_id);
        }
    }
    print_array(shopping_cart, "当前购物车");
    
    // 在中间插入商品
    printf("\n📍 在购物车中间插入商品:\n");
    int *insert_product = create_int(2);
    if (array_list_insert(shopping_cart, 1, insert_product) == DSA_SUCCESS) {
        printf("  ✅ 在位置1插入: %s (ID: 2)\n", products[1]);
    } else {
        free(insert_product);
    }
    print_array(shopping_cart, "插入商品后");
    
    // 移除商品
    printf("\n🗑️ 移除不需要的商品:\n");
    dsa_element_pt removed = array_list_remove(shopping_cart, 2);
    if (removed) {
        int product_id = ELEMENT_VALUE(int, removed);
        printf("  ✅ 移除: %s (ID: %d)\n", 
               products[product_id - 1], product_id);
        free(removed);
    }
    print_array(shopping_cart, "移除商品后");
    
    // 继续添加更多商品（测试扩容）
    printf("\n🛍️ 继续购物（测试自动扩容）:\n");
    int more_products[] = {6, 8, 9, 10};
    for (int i = 0; i < 4; i++) {
        int *product_id = create_int(more_products[i]);
        printf("  容量: %zu, 大小: %zu - ", 
               array_list_capacity(shopping_cart), 
               array_list_size(shopping_cart));
        
        if (array_list_push_back(shopping_cart, product_id) == DSA_SUCCESS) {
            printf("添加 %s ✅\n", products[more_products[i] - 1]);
        } else {
            printf("添加失败 ❌\n");
            free(product_id);
        }
    }
    print_array(shopping_cart, "最终购物车");
    
    // 结算
    printf("\n💰 购物车结算:\n");
    printf("  📋 购物清单:\n");
    for (size_t i = 0; i < array_list_size(shopping_cart); i++) {
        dsa_element_pt element = array_list_get(shopping_cart, i);
        if (element) {
            int product_id = ELEMENT_VALUE(int, element);
            printf("    %zu. %s (ID: %d)\n", 
                   i + 1, products[product_id - 1], product_id);
        }
    }
    printf("  📊 总商品数: %zu\n", array_list_size(shopping_cart));
    
    // 清理购物车
    array_list_clear_with_free(shopping_cart);
    array_list_destroy(shopping_cart);
    printf("✅ 购物车已清空\n");
}

/**
 * @brief 演示动态数组与静态数组的对比
 */
static void demonstrate_comparison_with_static(void) {
    printf("\n🆚 动态数组 vs 静态数组对比\n");
    printf("========================================\n");
    
    printf("📊 功能对比:\n");
    printf("┌─────────────────┬─────────────┬─────────────┐\n");
    printf("│ 特性            │ 静态数组    │ 动态数组    │\n");
    printf("├─────────────────┼─────────────┼─────────────┤\n");
    printf("│ 容量            │ 固定 ⚠️     │ 可扩展 ✅   │\n");
    printf("│ 内存分配        │ 栈/静态 ✅  │ 堆 ⚠️       │\n");
    printf("│ 随机访问        │ O(1) ✅     │ O(1) ✅     │\n");
    printf("│ 尾部插入        │ O(1) ✅     │ O(1)* ✅    │\n");
    printf("│ 内存开销        │ 较小 ✅     │ 较大 ⚠️     │\n");
    printf("│ 扩容能力        │ 无 ❌       │ 自动 ✅     │\n");
    printf("│ 内存管理        │ 自动 ✅     │ 手动 ⚠️     │\n");
    printf("└─────────────────┴─────────────┴─────────────┘\n");
    
    printf("\n💡 选择建议:\n");
    printf("  🎯 选择静态数组: 大小确定、性能要求极高\n");
    printf("  🎯 选择动态数组: 大小不确定、需要灵活性\n");
    
    printf("✅ 对比演示完成\n");
}

int main(void) {
    printf("🎉 动态数组示例程序\n");
    printf("========================================\n");
    printf("本程序演示动态数组的各种操作和特性\n");
    
    // 基本操作演示
    demonstrate_basic_operations();
    
    // 自动扩容演示
    demonstrate_auto_expansion();
    
    // 性能特点演示
    demonstrate_performance_characteristics();
    
    // 实际应用场景
    demonstrate_practical_usage();
    
    // 与静态数组对比
    demonstrate_comparison_with_static();
    
    printf("\n🎊 所有演示完成！\n");
    printf("动态数组特点总结:\n");
    printf("✅ 自动扩容，大小灵活\n");
    printf("✅ 快速随机访问 O(1)\n");
    printf("✅ 高效尾部操作 O(1)*\n");
    printf("⚠️ 需要手动内存管理\n");
    printf("⚠️ 扩容时有性能开销\n");
    printf("🎯 适用场景: 大小变化频繁、需要随机访问的数据集合\n");
    
    return 0;
}
