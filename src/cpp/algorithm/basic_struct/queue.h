// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-13

#ifndef _ALGORITHM_STRUCT_QUEUE_H_
#define _ALGORITHM_STRUCT_QUEUE_H_

namespace algorithm {
namespace basic_struct {

// 队列, 一种实现了先进先出(FIFO)策略的容器
// 这里实现一个只能容纳n个元素的环形队列
// example:
//   Queue<int> que(3);
//   que.Enqueue(1);
//   que.Enqueue(2);
//   que.Dequeue(); // return 1
//   que.Dequeue(); // return 2
template<typename T>
class Queue {
  public:
    explicit Queue(const int& size) :
        _container(new T[size]), _size(size), _currelem(0),
        _head(0), _tail(0) {}
    ~Queue() {
        if (_container) {
            delete[] _container;
            _container = nullptr;
        }
    }

    // 入队列操作
    //
    // pseudo code
    // ENQUEUE(Q, x)
    //   Q[Q.tail] = x
    //   if Q.tail == Q.length
    //     Q.tail = 1
    //   else Q.tail = Q.tail + 1
    void Enqueue(const T& elem) {
        if (!Full()) {
            _container[_tail] = elem;
            _tail = (_tail+1)%_size;
            ++_currelem;
        } else {
            throw std::runtime_error("queue full");
        }
    }

    // 出队列操作
    //
    // pseudo code
    // DEQUEUE(Q)
    //   x = Q[Q.head]
    //   if Q.head == Q.length
    //     Q.head = 1
    //   else Q.head = Q.head + 1
    //   return x
    T Dequeue() {
        if (!Empty()) {
            T ret = _container[_head];
            _head = (_head+1)%_size;
            --_currelem;
            return ret;
        } else {
            throw std::runtime_error("queue empty");
        }
    }

  private:
    // non copyable
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    // 判断队列是否为空
    bool Empty() const { return _currelem == 0; }

    // 判断队列是否已满
    bool Full() const { return _currelem == _size; }

    T*  _container;     // 队列容器
    int _size;          // 队列大小
    int _currelem;      // 当前元素个数
    int _head;          // 队首指针
    int _tail;          // 队尾指针
};

} // namespace basic_struct
} // namespace algorithm

#endif // _ALGORITHM_STRUCT_QUEUE_H_
