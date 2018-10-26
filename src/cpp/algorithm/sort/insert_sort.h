// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-02

#ifndef _ALGORITHM_SORT_INSERT_SORT_H_
#define _ALGORITHM_SORT_INSERT_SORT_H_

#include "common/algo_util.h"
#include "include/sort_interface.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 插入排序, 原理为:
// 假设最左边的子数组已经排序完毕, 要做的就是从还未排序的右边数组里依次
// 取出数据并放在左边已排序数组里合适的位置里, 很像抓牌时向里面插牌的操作
// 平均时间复杂度: Θ(n^2)
template<typename T>
class InsertSort : public SortInterface<T> {
  public:
    void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const override;
};

// pseudo code
// INSERTION-SORT(A)
//   for j = 2 to A.length
//     key = A[j]
//     i = j - 1
//     while i > 0 and A[i] > key
//       A[i+1] = A[i]
//       i = i - 1
//     A[i+1] = key
template<typename T>
void InsertSort<T>::Sort(int array_len, int (*comp)(const T&, const T&),
                        T* array) const {
    for (int i = 1; i < array_len; ++i) {
        T key = array[i];
        // 将key插入到已经排好序的array[0..i-1]中
        int j = i - 1;
        while ((comp(key, array[j]) < 0) && j >= 0) {
            array[j+1] = array[j];
            --j;
        }
        array[j+1] = key;
    }
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_SORT_INSERT_SORT_H_
