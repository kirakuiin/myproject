// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-06

#ifndef _ALGORITHM_STRUCT_BINARY_HEAP_H_
#define _ALGORITHM_STRUCT_BINARY_HEAP_H_

#include "common/algo_util.h"

namespace algorithm {

// 二叉堆, 根据T的比较函数进行建堆
// example:
//     int array[LEN] = { 1, 3, -1, 4, 2, ...}
//     int comp(const int&, const int&);
//     // 生成最大堆, 方法内部只会改动array元素次序, 如果array是指针的话
//     // 释放它不是BinaryHeap的责任
//     BinaryHeap::BuildMaxHeap(LEN, comp, array);
template <typename T>
class BinaryHeap {
  public:
    // 生成最大堆, comp的比较规则参考qsort比较函数的规则
    // 性质为A[PARENT(i) >= A[i]]
    static void BuildMaxHeap(int array_len, int (*comp)(const T&, const T&),
                             T* array);

    // 生成最小堆
    // 性质为A[PARENT(i) <= A[i]]
    static void BuildMinHeap(int array_len, int (*comp)(const T&, const T&),
                             T* array);

    // 将index开始的堆进行处理, 使其符合最大堆的性质
    // 这里假设所有的子堆已经符合最大堆的性质
    static void MaxHeapify(int array_len, int (*comp)(const T&, const T&),
                           int index, T* array);

    // 将index开始的堆进行处理, 使其符合最小堆的性质
    // 这里假设所有的子堆已经符合最小堆的性质
    static void MinHeapify(int array_len, int (*comp)(const T&, const T&),
                           int index, T* array);

    // 将index处的值替换为新的key, 新的key必须大于index处的值
    static void HeapIncreaseKey(int (*comp)(const T&, const T&),
                                int index, const T& key, T* array);

    // 将index处的值替换为新的key, 新的key必须小于index处的值
    static void HeapDecreaseKey(int (*comp)(const T&, const T&),
                                int index, const T& key, T* array);

  private:
    // 根据自己的下标计算父子的位置
    // 如果下标从1开始, 则Parent(i) = floor(i/2), 所以下标2,3的父亲是1
    // Left(i) = i*2, Right(i) i*2+1, 所以left = 1*2 = 2, right = 1*2+1 = 3
    // 而实现数组的下标由0开始, 因此做一次转换
    static int Parent(int child_idx) { return ((child_idx+1) >> 1) - 1; }
    static int Left(int parent_idx) { return ((parent_idx+1) << 1) - 1; }
    static int Right(int parent_idx) { return (parent_idx+1) << 1; }
};

// pseudo code
// BUILD-MAX-HEAP(A)
//   for i = floor(A.heap-size/2) downto 1
//     MAX-HEAPIFY(A, i)
template<typename T>
void BinaryHeap<T>::BuildMaxHeap(int array_len, int (*comp)(const T&, const T&),
                              T* array) {
    for (int i = array_len/2-1; i >= 0; --i) {
        MaxHeapify(array_len, comp, i, array);
    }
}

// pseudo code
// BUILD-MIN-HEAP(A)
//   for i = floor(A.heap-size/2) downto 1
//     MIN-HEAPIFY(A, i)
template<typename T>
void BinaryHeap<T>::BuildMinHeap(int array_len, int (*comp)(const T&, const T&),
                              T* array) {
    for (int i = array_len/2-1; i >= 0; --i) {
        MinHeapify(array_len, comp, i, array);
    }
}

// pseudo code
// MAX-HEAPIFY(A, i)
//   l = LEFT(i)
//   r = RIGHT(i)
//   if l <= A.heapsize and A[l] > A[i]
//     largest = l
//   else
//     largest = i
//   if r <= A.heapsize and A[r] > A[largest]
//     largest = r
//   else
//     largest = i
//   if largest != i
//     exchange A[i] with A[largest]
//     MAX-HEAPIFY(A, largest)
template<typename T>
void BinaryHeap<T>::MaxHeapify(int array_len, int (*comp)(const T&, const T&),
                            int index, T* array) {
    int l = Left(index);
    int r = Right(index);
    int largest = index;
    if ((l < array_len) && comp(array[l], array[largest]) > 0) {
        largest = l;
    }
    if ((r < array_len) && comp(array[r], array[largest]) > 0) {
        largest = r;
    }
    if (largest != index) {
        Swap(array[index], array[largest]);
        MaxHeapify(array_len, comp, largest, array);
    }
}

// pseudo code
// MIN-HEAPIFY(A, i)
//   l = LEFT(i)
//   r = RIGHT(i)
//   if l <= A.heapsize and A[l] < A[i]
//     smallest = l
//   else
//     smallest = i
//   if r <= A.heapsize and A[r] < A[smallest]
//     smallest = r
//   else
//     smallest = i
//   if smallest != i
//     exchange A[i] with A[smallest]
//     MIN-HEAPIFY(A, smallest)
template<typename T>
void BinaryHeap<T>::MinHeapify(int array_len, int (*comp)(const T&, const T&),
                            int index, T* array) {
    int l = Left(index);
    int r = Right(index);
    int smallest = index;
    if ((l < array_len) && comp(array[l], array[smallest]) < 0) {
        smallest = l;
    }
    if ((r < array_len) && comp(array[r], array[smallest]) < 0) {
        smallest = r;
    }
    if (smallest != index) {
        Swap(array[index], array[smallest]);
        MinHeapify(array_len, comp, smallest, array);
    }
}

// pseudo code
// HEAP-INCREASE-KEY(A, i, key)
// if key < A[i]
//   error "new key is smaller than current key"
// else
//   A[i] = key
//   while i > 1 and key > A[i]
//     exchange A[i] with A[Parent(i)]
//     i = Parent(i)
template<typename T>
void BinaryHeap<T>::HeapIncreaseKey(int (*comp)(const T&, const T&),
                                    int index, const T& key, T* array) {
    array[index] = key;
    while ((index > 0) && comp(key, array[Parent(index)]) > 0) {
        Swap(array[index], array[Parent(index)]);
        index = Parent(index);
    }
}

// pseudo code
// HEAP-DECREASE-KEY(A, i, key)
// if key > A[i]
//   error "new key is bigger than current key"
// else
//   A[i] = key
//   while i > 1 and key < A[i]
//     exchange A[i] with A[Parent(i)]
//     i = Parent(i)
template<typename T>
void BinaryHeap<T>::HeapDecreaseKey(int (*comp)(const T&, const T&),
                                    int index, const T& key, T* array) {
    array[index] = key;
    while ((index > 0) && comp(key, array[Parent(index)]) < 0) {
        Swap(array[index], array[Parent(index)]);
        index = Parent(index);
    }
}

} // namespace algorithm

#endif // _ALGORITHM_STRUCT_BINARY_HEAP_H_
