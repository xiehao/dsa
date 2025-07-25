/**
 * @file iterator_utils.c
 * @brief 迭代器工具函数实现
 * @details 提供迭代器相关的工具函数，如元素交换等
 * @author DSA项目组
 * @date 2025-07-25
 * @version 1.0
 */

#include <a/iterator.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 交换两个迭代器指向的元素
 */
dsa_result_t dsa_iterator_swap(dsa_iterator_t *iter1, dsa_iterator_t *iter2) {
    if (!iter1 || !iter2) {
        return DSA_ERROR_NULL_POINTER;
    }

    if (!iterator_is_valid(iter1) || !iterator_is_valid(iter2)) {
        return DSA_ERROR_INVALID_PARAMETER;
    }

    dsa_element_pt value1 = iterator_get_value(iter1);
    dsa_element_pt value2 = iterator_get_value(iter2);

    if (!value1 || !value2) {
        return DSA_ERROR_NULL_POINTER;
    }

    // 为了处理静态数组，我们需要创建临时缓冲区来交换值
    // 假设元素大小为sizeof(int)，这是一个简化的实现
    char temp_buffer[sizeof(int)];

    // 保存第一个值
    memcpy(temp_buffer, value1, sizeof(int));

    // 将第二个值复制到第一个位置
    memcpy(value1, value2, sizeof(int));

    // 将保存的第一个值复制到第二个位置
    memcpy(value2, temp_buffer, sizeof(int));

    return DSA_SUCCESS;
}
