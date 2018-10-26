// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-05

#ifndef _ALGORITHM_COMMON_ALGO_UTIL_H_
#define _ALGORITHM_COMMON_ALGO_UTIL_H_

#include <cmath>
#include <random>
#include <iostream>

namespace algorithm {

// 交换两个变量的值
template <typename T>
void Swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

// 交换两个数组的前n位
template <typename T>
void SwapArray(T* a, T* b, int array_len) {
    for (int i = 0; i < array_len; ++i) {
        Swap(a[i], b[i]);
    }
}

// base case
template <typename T>
T Max(const T& a) {
    return a;
}

// 得到1到n个同类型参数的最大值, 类型必须支持<操作符
template <typename T, typename... Args>
T Max(const T& a, const T& b, Args... args) {
    return Max((a < b ? b : a), args...);
}

// base case
template <typename T>
T Min(const T& a) {
    return a;
}

// 得到1到n个同类型参数的最小值, 类型必须支持>操作符
template <typename T, typename... Args>
T Min(const T& a, const T& b, Args... args) {
    return Min((a > b ? b : a), args...);
}

// 获得一个在[begin, end]内的随机整数
inline int RandomInt(int begin, int end) {
    static std::mt19937 engine(time(nullptr));    // 梅森旋转算法- MT19937
    std::uniform_int_distribution<int> distribution(begin, end);
    return distribution(engine);
}

// 将一个数组就地随机排列
template <typename T>
void RandomizeInPlace(int array_len, T* array) {
    std::mt19937 engine(time(nullptr));    // 梅森旋转算法- MT19937
    std::uniform_int_distribution<int> distribution(0, array_len-1);
    for (int i = 0; i < array_len; ++i) {
        Swap(array[i], array[distribution(engine)]);
    }
}

// 格式化输出打印支持<<操作符的类型
// 基础情况
inline void Printf(const char* fmt) {
    while (*fmt) {
        if (*fmt == '%') {
            if (*(fmt+1) == '%') {
                ++fmt;
            } else {
                throw std::runtime_error("Invalid format: missing arg.");
            }
        }
        std::cout<<*fmt++;
    }
}

template <typename T, typename... Args>
void Printf(const char* fmt, const T& value, Args... args) {
    while (*fmt) {
        if (*fmt == '%') {
            if (*(fmt+1) == '%') {
                ++fmt;
            } else {
                std::cout<<value;
                Printf(fmt+1, args...);
                return;
            }
        }
        std::cout<<*fmt++;
    }
    throw std::logic_error("Extra arguments provide to Print.");
}

} // namespace algorithm

#endif // _ALGORITHM_COMMON_ALGO_UTIL_H_
