// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-07

#ifndef _ALGORITHM_STRUCT_PRIORITY_QUEUE_H_
#define _ALGORITHM_STRUCT_PRIORITY_QUEUE_H_

#include "common/algo_util.h"
#include "common/binary_heap.h"

namespace algorithm {
namespace basic_struct {

// 优先队列, 根据优先级将数据组织为二叉堆
// 如果不算拷贝数组的开销，所有操作的时间复杂度为Ο(lgn)
// example:
//     int comp(const int&, const int&);
//     PriorityQueue<int> minQueue;
//     minQueue.BuildMinPriorityQueue(comp);
//     minQueue.Push(3);
//     minQueue.Size(); // return 1
template<typename T>
class PriorityQueue {
  public:
    PriorityQueue()
        : _container(nullptr), _size(0), _capacity(0), _comp(nullptr),
          _isMinQueue(true) {}

    ~PriorityQueue() {
        if (_container) {
            delete[] _container;
        }
    }

    // 构造最小优先队列, 用传入的参数作为初始化队列
    void BuildMinPriorityQueue(int array_len, int (*comp)(const T&, const T&),
                               const T* array);

    // 构造空的最小优先队列
    void BuildMinPriorityQueue(int (*comp)(const T&, const T&));

    // 构造最大优先队列, 用传入的参数作为初始化队列
    void BuildMaxPriorityQueue(int array_len, int (*comp)(const T&, const T&),
                               const T* array);

    // 构造空的最大优先队列
    void BuildMaxPriorityQueue(int (*comp)(const T&, const T&));

    // 向优先队列中插入一个元素
    void Push(const T& elem);

    // 返回优先级最高的元素, 如果队列为空则返回T类型默认构造的元素
    T Top() const;

    // 返回优先级最高的元素并将其在队列里删除
    T Pop();

    // 返回优先队列的大小
    int size() const { return _size; }

  private:
    // non-copyable
    PriorityQueue(const PriorityQueue&) = delete;
    void operator=(const PriorityQueue&) = delete;

    T*      _container;             // 存放数据的容器
    int     _size;                  // 队列元素数量
    int     _capacity;              // 队列元素容量, 为了避免频繁分配空间使用
    int  (* _comp)(const T&, const T&);     // 比较函数
    bool    _isMinQueue;            // 是否是最小堆
};

template<typename T>
void PriorityQueue<T>::BuildMinPriorityQueue(int array_len,
                                             int (*comp)(const T&, const T&),
                                             const T* array) {
    _size = array_len;
    _capacity = 2*_size+1;
    _container = new T[_capacity];
    for (int i = 0; i < _size; ++i) {
        _container[i] = array[i];
    }
    _comp = comp;
    _isMinQueue = true;
    BinaryHeap<T>::BuildMinHeap(_size, _comp, _container);
}

template<typename T>
void PriorityQueue<T>::BuildMinPriorityQueue(int (*comp)(const T&, const T&)) {
    _size = 0;
    _capacity = 1;
    _comp = comp;
    _isMinQueue = true;
    if (_container) {
        delete[] _container;
    }
    _container = new T[_capacity];
}

template<typename T>
void PriorityQueue<T>::BuildMaxPriorityQueue(int array_len,
                                             int (*comp)(const T&, const T&),
                                             const T* array) {
    _size = array_len;
    _capacity = 2*_size+1;
    _container = new T[_capacity];
    for (int i = 0; i < _size; ++i) {
        _container[i] = array[i];
    }
    _comp = comp;
    BinaryHeap<T>::BuildMinHeap(_size, _comp, _container);
    _isMinQueue = false;
}

template<typename T>
void PriorityQueue<T>::BuildMaxPriorityQueue(int (*comp)(const T&, const T&)) {
    _size = 0;
    _capacity = 1;
    _comp = comp;
    _isMinQueue = false;
    if (_container) {
        delete[] _container;
    }
    _container = new T[_capacity];
}

// pseudo code
// PUSH(A, key)
//   A.heap-size = A.heap-size + 1
//   A[A.heap-size] = SUITABLE-VALUE
//   HEAP-KEY(A, A.heap-size, key)
template<typename T>
void PriorityQueue<T>::Push(const T& elem) {
    // 重新分配空间
    if (_size == _capacity) {
        _capacity = _capacity*2 + 1;
        T* new_space = new T[_capacity];
        for (int i = 0; i < _size; ++i) {
            new_space[i] = _container[i];
        }
        delete[] _container;
        _container = new_space;
    }
    ++_size;
    if (_isMinQueue) {
        BinaryHeap<T>::HeapDecreaseKey(_comp, _size-1, elem, _container);
    } else {
        BinaryHeap<T>::HeapIncreaseKey(_comp, _size-1, elem, _container);
    }
}

// pseudo code
// TOP(A)
//   return A[1]
template<typename T>
T PriorityQueue<T>::Top() const {
    if (_size > 0) {
        return _container[0];
    } else {
        return T();
    }
}

// pseudo code
// HEAP-EXTRACT-TOP(A)
//   if A.heap-size < 1
//     error "heap underflow"
//   else
//     max = A[1]
//     exchange A[1] with A[A.heap-size]
//     A.heap-size = A.heap-size - 1
//     HEAPIFY(A, 1)
//     return max
template<typename T>
T PriorityQueue<T>::Pop() {
    if (_size > 0) {
        Swap(_container[0], _container[_size-1]);
        --_size;
        if (_isMinQueue) {
            BinaryHeap<T>::MinHeapify(_size, _comp, 0, _container);
        } else {
            BinaryHeap<T>::MaxHeapify(_size, _comp, 0, _container);
        }
        return _container[_size];
    } else {
        return T();
    }
}

} // namespace basic_struct
} // namespace algorithm

#endif // _ALGORITHM_STRUCT_PRIORITY_QUEUE_H_
