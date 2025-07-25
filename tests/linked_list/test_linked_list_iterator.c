/**
 * @file test_linked_list_iterator.c
 * @brief 链表迭代器功能测试
 * @details 测试单链表、双链表和循环链表的迭代器功能
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ds/linked_list.h>
#include <ds/linked_list_iterator.h>
#include <ds/iterator.h>

/**
 * @brief 测试单链表迭代器
 */
void test_singly_linked_list_iterator() {
    printf("测试单链表迭代器...\n");
    
    // 创建单链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_SINGLY);
    assert(list != NULL);
    
    // 添加测试数据
    int data[] = {10, 20, 30, 40, 50};
    int count = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < count; i++) {
        assert(linked_list_push_back(list, &data[i]) == DSA_SUCCESS);
    }
    
    // 测试正向遍历
    printf("正向遍历: ");
    dsa_iterator_t *iter = linked_list_begin(list);
    assert(iter != NULL);
    
    int index = 0;
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        assert(value != NULL);
        printf("期望: %d, 实际: %d ", data[index], *value);
        if (*value != data[index]) {
            printf("\n错误：索引 %d 处的值不匹配！\n", index);
            break;
        }
        printf("✓ ");
        iterator_next(iter);
        index++;
    }
    printf("\n");
    assert(index == count);
    
    // 测试反向遍历（单链表不支持）
    printf("测试反向遍历（应该不支持）: ");
    dsa_iterator_t *prev_iter = iterator_prev(iter);
    assert(prev_iter == NULL);
    printf("正确，单链表不支持反向遍历\n");
    
    // 清理
    iterator_destroy(iter);
    linked_list_destroy(list);
    printf("单链表迭代器测试通过！\n\n");
}

/**
 * @brief 测试双链表迭代器
 */
void test_doubly_linked_list_iterator() {
    printf("测试双链表迭代器...\n");
    
    // 创建双链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_DOUBLY);
    assert(list != NULL);
    
    // 添加测试数据
    int data[] = {100, 200, 300, 400, 500};
    int count = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < count; i++) {
        assert(linked_list_push_back(list, &data[i]) == DSA_SUCCESS);
    }
    
    // 测试正向遍历
    printf("正向遍历: ");
    dsa_iterator_t *iter = linked_list_begin(list);
    assert(iter != NULL);
    
    int index = 0;
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        assert(value != NULL);
        assert(*value == data[index]);
        printf("%d ", *value);
        iterator_next(iter);
        index++;
    }
    printf("\n");
    assert(index == count);
    
    // 测试反向遍历
    printf("反向遍历: ");
    dsa_iterator_t *end_iter = linked_list_end(list);
    assert(end_iter != NULL);
    
    // 从end向前移动到最后一个有效元素
    iterator_prev(end_iter);
    
    index = count - 1;
    while (iterator_is_valid(end_iter)) {
        int *value = (int *)iterator_get_value(end_iter);
        assert(value != NULL);
        assert(*value == data[index]);
        printf("%d ", *value);
        iterator_prev(end_iter);
        index--;
    }
    printf("\n");
    assert(index == -1);
    
    // 清理
    iterator_destroy(iter);
    iterator_destroy(end_iter);
    linked_list_destroy(list);
    printf("双链表迭代器测试通过！\n\n");
}

/**
 * @brief 测试循环链表迭代器
 */
void test_circular_linked_list_iterator() {
    printf("测试循环链表迭代器...\n");
    
    // 创建循环链表
    dsa_linked_list_t *list = linked_list_create(LINKED_LIST_TYPE_CIRCULAR);
    assert(list != NULL);
    
    // 添加测试数据
    int data[] = {1000, 2000, 3000};
    int count = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < count; i++) {
        assert(linked_list_push_back(list, &data[i]) == DSA_SUCCESS);
    }
    
    // 测试正向遍历
    printf("正向遍历: ");
    dsa_iterator_t *iter = linked_list_begin(list);
    assert(iter != NULL);
    
    int index = 0;
    while (iterator_is_valid(iter)) {
        int *value = (int *)iterator_get_value(iter);
        assert(value != NULL);
        assert(*value == data[index]);
        printf("%d ", *value);
        iterator_next(iter);
        index++;
    }
    printf("\n");
    assert(index == count);
    
    // 测试反向遍历（循环链表不支持）
    printf("测试反向遍历（应该不支持）: ");
    dsa_iterator_t *prev_iter = iterator_prev(iter);
    assert(prev_iter == NULL);
    printf("正确，循环链表不支持反向遍历\n");
    
    // 清理
    iterator_destroy(iter);
    linked_list_destroy(list);
    printf("循环链表迭代器测试通过！\n\n");
}

/**
 * @brief 测试空链表迭代器
 */
void test_empty_list_iterator() {
    printf("测试空链表迭代器...\n");
    
    // 测试各种类型的空链表
    dsa_linked_list_type_t types[] = {
        LINKED_LIST_TYPE_SINGLY,
        LINKED_LIST_TYPE_DOUBLY,
        LINKED_LIST_TYPE_CIRCULAR
    };
    
    const char *type_names[] = {
        "单链表",
        "双链表", 
        "循环链表"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("测试空%s: ", type_names[i]);
        
        dsa_linked_list_t *list = linked_list_create(types[i]);
        assert(list != NULL);
        
        dsa_iterator_t *begin_iter = linked_list_begin(list);
        dsa_iterator_t *end_iter = linked_list_end(list);
        
        assert(begin_iter != NULL);
        assert(end_iter != NULL);
        
        // 空链表的begin应该等于end
        assert(iterator_equals(begin_iter, end_iter));
        assert(!iterator_is_valid(begin_iter));
        assert(!iterator_is_valid(end_iter));
        
        printf("通过\n");
        
        iterator_destroy(begin_iter);
        iterator_destroy(end_iter);
        linked_list_destroy(list);
    }
    
    printf("空链表迭代器测试通过！\n\n");
}

/**
 * @brief 主测试函数
 */
int main() {
    printf("=== 链表迭代器功能测试 ===\n\n");
    
    test_singly_linked_list_iterator();
    test_doubly_linked_list_iterator();
    test_circular_linked_list_iterator();
    test_empty_list_iterator();
    
    printf("=== 所有链表迭代器测试通过！ ===\n");
    return 0;
}
