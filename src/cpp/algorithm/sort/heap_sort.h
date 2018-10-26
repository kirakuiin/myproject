// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-03

#ifndef _ALGORITHM_SORT_HEAP_SORT_H_
#define _ALGORITHM_SORT_HEAP_SORT_H_

#include "common/algo_util.h"
#include "common/binary_heap.h"
#include "include/sort_interface.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 堆排序, 原理为:
// 将带排序数组变成一个最大堆, 根据堆的性质, 每次取出第一个元素放到数组的最后,
// 然后对剩下的元素重新应用维护堆性质的操作. 依次进行此操作可得到排序的数组
// 平均时间复杂度: Θ(nlgn)
template<typename T>
class HeapSort : public SortInterface<T> {
  public:
    void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const override;
};

// pseudo code
// HEAPSORT(A)
//   BUILD-MAX-HEAP(A)
//   for i = A.heap-size downto 2
//     exchange A[i] with A[1]
//     A.heap-size = A.heap-size - 1
//     MAX-HEAPIFY(A, 1)
template<typename T>

void HeapSort<T>::Sort(int array_len, int (*comp)(const T&, const T&),
                        T* array) const {
    BinaryHeap<T>::BuildMaxHeap(array_len, comp, array);
    for (int i = array_len-1; i > 0; --i) {
        Swap(array[i], array[0]);
        --array_len;
        BinaryHeap<T>::MaxHeapify(array_len, comp, 0, array);
    }
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_SORT_HEAP_SORT_H_
