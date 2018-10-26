// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-13

#ifndef _ALGORITHM_STRUCT_STACK_H_
#define _ALGORITHM_STRUCT_STACK_H_

namespace algorithm {
namespace basic_struct {

// 栈, 一种实现了后进先出(LIFO)策略的容器
// 这里实现一个只能容纳n个元素的定长栈
// example:
//   Stack<int> stack(3);
//   stack.Push(1);
//   stack.Push(2);
//   stack.Pop();  // return 2
//   stack.Pop();  // return 1
template<typename T>
class Stack {
  public:
    explicit Stack(const int& size)
        : _container(new T[size]), _top(-1), _size(size) {}
    ~Stack() {
        if (_container) {
            delete[] _container;
            _container = nullptr;
        }
    };

    // 判断栈是否为空
    //
    // pseudo code
    // STACK-EMPTY(S)
    //   if S.top == 0
    //     return TRUE
    //   else
    //     return FALSE
    bool Empty() const { return _top == -1; }

    // 向栈顶加入元素
    //
    // pseudo code
    // PUSH(S, x)
    //   if STACK-FULL(S)
    //     error "overflow"
    //   else
    //     S.top = S.top + 1
    //     S.[S.top] = x
    void Push(const T& elem) {
        if (!Full()) {
            ++_top;
            _container[_top] = elem;
        } else {
            throw std::runtime_error("overflow");
        }
    }

    // 弹出栈顶的元素
    //
    // pseudo code
    // POP(S)
    //   if STACK-EMPTY(S)
    //     error "underflow"
    //   else
    //     S.top = S.top - 1
    //     return S[S.top+1]
    T Pop() {
        if (!Empty()) {
            --_top;
            return _container[_top+1];
        } else {
            throw std::runtime_error("underflow");
        }
    }

  private:
    // non copyable
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    // 判断栈是否满了
    bool Full() const { return _top == _size-1; }

    T*  _container;     // 栈容器
    int _top;           // 栈顶指针
    int _size;          // 栈的大小
};

} // namespace basic_struct
} // namespace algorithm

#endif // _ALGORITHM_STRUCT_STACK_H_
