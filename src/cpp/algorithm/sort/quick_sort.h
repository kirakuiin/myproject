// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-09

#ifndef _ALGORITHM_SORT_QUICK_SORT_H_
#define _ALGORITHM_SORT_QUICK_SORT_H_

#include "common/algo_util.h"
#include "include/sort_interface.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 快速排序, 原理为:
// 采用分治策略, 每次挑选一个随机主元进行划分, 小于主元的放到主元左边,
// 大于主元的放在主元右边, 依次对子序列进行此操作, 最后使全体有序
// 平均时间复杂度: Θ(nlgn)
template<typename T>
class QuickSort : public SortInterface<T> {
  public:
    void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const override;

  private:
    // 划分数组并分别进行快速排序, p <= r
    void InnerQuickSort(int p, int r, int (*comp)(const T&, const T&),
                        T* array) const;

    // 辅助函数
    // 将数组划分为两个子数组, 左边的全部小于array[r], 右边的全部大于array[r]
    // 并返回主元的位置 p <= r
    int Partition(int p, int r, int (*comp)(const T&, const T&),
                   T* array) const;

    // 随机化版本的划分, 每次随机选择一个数据作为主元, p <= r
    int RandomizedPartition(int p, int r, int (*comp)(const T&, const T&),
                            T* array) const;
};

template<typename T>
void QuickSort<T>::Sort(int array_len, int (*comp)(const T&, const T&),
                        T* array) const {
    InnerQuickSort(0, array_len-1, comp, array);
}

// pseudo code
// QUICK-SORT(A, p, r)
//   if p < r
//     q = PARTITION(A, p, r)
//     QUICK-SORT(A, p, q-1)
//     QUICK-SORT(A, q+1, r)
template<typename T>
void QuickSort<T>::InnerQuickSort(int p, int r,
                                  int (*comp)(const T&, const T&),
                                  T* array) const {
    if (p < r) {
        int q = Partition(p, r, comp, array);
        InnerQuickSort(p, q-1, comp, array);
        InnerQuickSort(q+1, r, comp, array);
    }
}

// pseudo code
// PARTITION(A, p, r)
//   key = A[r]
//   i = p - 1
//   for j = p to r - 1
//     if A[j] <= key
//       i = i + 1
//       exchange A[j] with A[i]
//   exchange A[r] with A[i+1]
//   return i+1
template<typename T>
int QuickSort<T>::Partition(int p, int r, int (*comp)(const T&, const T&),
                             T* array) const {
    T key = array[r];
    int i = p - 1;
    for (int j = p; j < r; ++j) {
        if (comp(array[j], key) <= 0) {
            ++i;
            Swap(array[i], array[j]);
        }
    }
    Swap(array[r], array[i+1]);
    return i+1;
}

// pseudo code
// Randomized-PARTITION(A, p, r)
//   index = RANDOM(p, r)
//   exchange A[r] with A[index]
//   return PARTITION(A, p, r)
template<typename T>
int QuickSort<T>::RandomizedPartition(int p, int r,
                                      int (*comp)(const T&, const T&),
                                      T* array) const {
    Swap(array[r], array[RandomInt(p, r)]);
    return Partition(p, r, comp, array);
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_SORT_QUICK_SORT_H_
