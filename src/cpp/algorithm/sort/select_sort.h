// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-03

#ifndef _ALGORITHM_SORT_SELECT_SORT_H_
#define _ALGORITHM_SORT_SELECT_SORT_H_

#include "common/algo_util.h"
#include "include/sort_interface.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 选择排序, 原理为:
// 每次在未排序的数组里选择最小的元素放到已排序数组的结尾
// n-1轮后数组有序, 和冒泡很相似, 但是不需要那么多的交换操作
// 平均时间复杂度: Θ(n^2)
template<typename T>
class SelectSort : public SortInterface<T> {
  public:
    void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const override;
};

// pseudo code
// SELECTION-SORT(A)
//   for j = 1 to A.length - 1
//     min = A[j]
//     index = j
//     for i = j to A.length
//       if A[i] < min
//         min = A[i]
//         index = i
//     tmp = A[j]
//     A[j] = A[index]
//     A[index] = tmp
template<typename T>
void SelectSort<T>::Sort(int array_len, int (*comp)(const T&, const T&),
                        T* array) const {
    // 只需要length-1次，是因为前n-1个都比他小，最后一个自然是最大的
    for (int i = 0; i < array_len - 1; ++i) {
        T min = array[i];
        int index = i;
        // 寻找最小值
        for (int j = i; j < array_len; ++j) {
            if (comp(array[j], min) < 0) {
                min = array[j];
                index = j;
            }
        }

        // 将最小值放到已经排序完毕的数组的后面
        Swap(array[i], array[index]);
    }
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_SORT_SELECT_SORT_H_
