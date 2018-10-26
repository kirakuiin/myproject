// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-03

#ifndef _ALGORITHM_SORT_BUBBLE_SORT_H_
#define _ALGORITHM_SORT_BUBBLE_SORT_H_

#include "common/algo_util.h"
#include "include/sort_interface.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 冒泡排序, 原理为:
// 每一轮将未排序数组的最小的元素交换到数组的开始位置,
// 经过A.lenght-1轮后, 数组有序
// 平均时间复杂度: Θ(n^2)
template<typename T>
class BubbleSort : public SortInterface<T> {
  public:
    void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const override;
};

// pseudo code
// BUBBLE-SORT(A)
//   for i = 1 to A.length - 1
//     for j = A.length downto i + 1
//       if A[j] < A[j-1]
//         exchange A[j] with A[j-1]
template<typename T>
void BubbleSort<T>::Sort(int array_len, int (*comp)(const T&, const T&),
                        T* array) const {
    // 核心思想就是每一轮将最小的元素放到未排序数组的开始
    for (int i = 0; i < array_len - 1; ++i) {
        for (int j = array_len - 1; j > i; --j) {
            if (comp(array[j], array[j-1]) < 0) {
                Swap(array[j], array[j-1]);
            }
        }
    }
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_SORT_BUBBLE_SORT_H_
