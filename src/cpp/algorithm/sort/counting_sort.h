// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-03

#ifndef _ALGORITHM_SORT_COUNTING_SORT_H_
#define _ALGORITHM_SORT_COUNTING_SORT_H_

#include <type_traits>
#include "common/algo_util.h"
#include "include/sort_interface.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 计数排序, 原理为:
// 假设数组里的每一个元素都是[0, k]区间的整数, 对于每一个输入x
// 求出小于x的元素个数, 利用这个信息可以直接把元素放到合适的位置上
// 这是一种非比较算法, 当k=Ο(n)时
// 平均时间复杂度: Θ(n+k)
template<typename T>
class CountingSort : public SortInterface<T> {
  public:
    void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const override;
};

// pseudo code
// COUNTING-SORT(A, B, k)
//   let C[0..k] be a new array
//   for i = 0 to k
//     C[i] = 0
//   for j = 1 to A.length
//     C[A[j]] = C[A[j]]+1
//   // C[j] now contains the number of elements equal to i.
//   for i = 1 to k
//     C[i] = C[i]+C[i-1]
//   // C[i] now contains the number of elements less than or equal to i.
//   for j = A.length downto 1
//     B[C[A[j]]] = A[j]
//     C[A[j]] = C[A[j]]-1
//   //downto keep stable
template<typename T>
void CountingSort<T>::Sort(int array_len, int (*comp)(const T&, const T&),
                        T* array) const {
    // 计数排序要求元素必须为正整数
    static_assert(std::is_integral<T>::value, "Integral required.");
    T k = array[0]; // k代表最大的数
    for (int i = 0; i < array_len; ++i) {
        assert(array[i] >= 0);
        if (k < array[i]) {
            k = array[i];
        }
    }

    T count_array[k+1];    // 计数数组
    for (int i = 0; i <= k; ++i) {
        count_array[i] = 0;
    }
    for (int i = 0; i < array_len; ++i) {
        ++count_array[array[i]];
    }
    for (int i = 1; i <= k; ++i) {
        count_array[i] += count_array[i-1];
    }

    T result_array[array_len];
    for (int j = array_len-1; j >= 0; --j) {
        result_array[count_array[array[j]]-1] = array[j];
        --count_array[array[j]];
    }
    SwapArray(result_array, array, array_len);
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_SORT_COUNTING_SORT_H_
