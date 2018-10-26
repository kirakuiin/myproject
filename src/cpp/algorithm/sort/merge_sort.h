// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-03

#ifndef _ALGORITHM_SORT_MERGE_SORT_H_
#define _ALGORITHM_SORT_MERGE_SORT_H_

#include "common/algo_util.h"
#include "include/sort_interface.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 归并排序, 原理为:
// 将数组拆分为两个子数组, 分别对两个子数组进行归并排序
// 然后再将两个子数组合并到一起, 排序完毕
// 平均时间复杂度: Θ(nlgn)
// 当前实现的空间复杂度: Θ(n)
template<typename T>
class MergeSort : public SortInterface<T> {
  public:
    void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const override;

  private:
    // 辅助函数，将局部有序的数组array[p, q]和array[q+1, r]
    // 合并成一个array[p,r]有序的数组
    // 其中array是一个数组, p, q, r为下标, 并且p <= q < r
    void Merge(int p, int q, int r, int (*comp)(const T&, const T&),
               T* array) const;

    // 归并排序，将序列分解为两个n/2的子序列, 使用归并排序继续处理两个子序列
    // 最后将两个子序列的结果合并，最后整体有序
    // 其中array是一个数组, p, r为下标, 并且p <= r
    void InnerMergeSort(int p, int r, int (*comp)(const T&, const T&),
                        T* array) const;
};

template<typename T>
void MergeSort<T>::Sort(int array_len, int (*comp)(const T&, const T&),
                        T* array) const {
    InnerMergeSort(0, array_len-1, comp, array);
}

// pseudo code
// MERGE(A, p, q, r)
//   n1 = q - p + 1
//   n2 = r - q
//   let L[1..n1+1] and R[1..n2+1] be new arrays
//   for i = 1 to n1
//     L[i] = A[p+i-1]
//   for j = 1 to n2
//     R[j] = A[q+j]
//   L[n1+1] = INF // INF means infinity
//   L[n2+1] = INF // add two sentry at array end
//   i = j = 1
//   for k = p to r
//     if L[i] <= R[j]
//       A[k] = L[i]
//       i = i + 1
//     else
//       A[k] = R[j]
//       j = j + 1
template<typename T>
void MergeSort<T>::Merge(int p, int q, int r, int(*comp)(const T&, const T&),
                         T* array) const {
    int n1 = q - p + 1;
    int n2 = r - q;
    // 由于无法得知T的类型最大值, 因此无法采用哨兵, 所以不多申请一个空间
    T array_l[n1];
    T array_r[n2];
    for (int i = 0; i < n1; ++i) {
        array_l[i] = array[p+i];
    }
    for (int j = 0; j < n2; ++j) {
        array_r[j] = array[q+j+1];
    }

    int i = 0;
    int j = 0;
    int k = p;
    while ((i < n1) && (j < n2)) {
        if (comp(array_l[i], array_r[j]) <= 0) {
            array[k] = array_l[i];
            ++i;
        } else {
            array[k] = array_r[j];
            ++j;
        }
        ++k;
    }
    for (; i < n1; ++i,++k) {
        array[k] = array_l[i];
    }
    for (; j < n2; ++j,++k) {
        array[k] = array_r[j];
    }
}

// pseudo code
// MERGE-SORT(A, p, r)
//   if p < r
//     q = (p + r)/2 // floor
//     MERGE-SORT(A, p, q)
//     MERGE-SORT(A, q+1, r)
//     MERGE(A, p, q, r)
template<typename T>
void MergeSort<T>::InnerMergeSort(int p, int r, int(*comp)(const T&, const T&),
                                  T* array) const {
    if (p < r) {
        int q = (p + r)/2;
        InnerMergeSort(p, q, comp, array);
        InnerMergeSort(q+1, r, comp, array);
        Merge(p, q, r, comp, array);
    }
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_SORT_MERGE_SORT_H_
