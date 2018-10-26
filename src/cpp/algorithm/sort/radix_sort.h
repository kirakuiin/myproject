// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-03

#ifndef _ALGORITHM_SORT_RADIX_SORT_H_
#define _ALGORITHM_SORT_RADIX_SORT_H_

#include <type_traits>
#include "common/algo_util.h"
#include "include/sort_interface.h"
#include "counting_sort.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 基数排序, 原理为:
// 对数组里的所有的数字按位排序, 先按最低有效位进行稳定排序, 然后排序高一位
// 即从个位到十位, 百位，...依次排序, 当最高位排好序时, 整体有序
// 平均时间复杂度: Θ(n)
template<typename T>
class RadixSort : public SortInterface<T> {
  public:
    void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const override;

  private:
    // 基于位数的计数排序, 每次只对某一位排序, 如radix=1则对个位排序
    void RadixCountingSort(int array_len, int radix, T* array) const;

    // 得到某个数指定位数的值
    int GetDigit(int number, int radix) const { return (number/radix)%10; }
};

// pseudo code
// RADIX-SORT(A, d)
// for i = 1 to d
//   use a stable sort to sort array A on digit i
template<typename T>
void RadixSort<T>::Sort(int array_len, int (*comp)(const T&, const T&),
                        T* array) const {
    static_assert(std::is_integral<T>::value, "Integral required.");
    T max = array[0];
    for (int i = 0; i < array_len; ++i) {
        assert(array[i] >= 0);
        if (max < array[i]) {
            max = array[i];
        }
    }

    int radix = 1;
    // 共循环位数趟
    while (max/radix > 0) {
        RadixCountingSort(array_len, radix, array);
        radix *= 10;
    }
}

template<typename T>
void RadixSort<T>::RadixCountingSort(int array_len, int radix,
                                     T* array) const {
    constexpr int k = 9;    // 每一位最大为9
    T count_array[k+1];     // 计数数组

    for (int i = 0; i <= k; ++i) {
        count_array[i] = 0;
    }
    for (int i = 0; i < array_len; ++i) {
        ++count_array[GetDigit(array[i], radix)];
    }
    for (int i = 1; i <= k; ++i) {
        count_array[i] += count_array[i-1];
    }

    T result_array[array_len];  // 存放临时结果的数组
    for (int j = array_len-1; j >= 0; --j) {
        result_array[count_array[GetDigit(array[j], radix)]-1] = array[j];
        --count_array[GetDigit(array[j], radix)];
    }
    SwapArray(result_array, array, array_len);
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_SORT_RADIX_SORT_H_
