// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-17

#ifndef _ALGORITHM_COMMON_HASH_FUNC_H_
#define _ALGORITHM_COMMON_HASH_FUNC_H_

#include "common/algo_util.h"

namespace algorithm {

// 除法散列法
// h(k) = k mod m, m一般选为一个不太接近2的幂的素数
// SIZE代表散列表的大小
template <int _Size>
int DivisionHash(const int& key) {
    return key % _Size;
}

// 乘法散列法
// h(k) = floor(m*(kA mod 1)), A为(0,1)内的常数, 推荐选择黄金分割0.618
// m选为2的幂, 可以很方便的转化这个函数为另一种形式, 因此POWER代表2的幂数
template <int _Pow>
int MultiplicationHash(const int& key) {
    constexpr int kbitOfFourBytes = 32;
    constexpr int64_t base = static_cast<int64_t>(2)<<(kbitOfFourBytes-1);
    constexpr int64_t kA = 0.618 * base;
    return ((key*kA) % base) >> (kbitOfFourBytes - _Pow);
}

// 全域散列法
// 假设一组函数h里有n个函数, 都将关键字域U映射到m个槽中
// 对于任意不同的两个关键字k, l属于U, 如果满足h(k)=h(l)的函数个数至多为n/m个
// 也就是说对一个函数组里的任意一个函数, 不同关键字得到相同散列值的概率为1/m
// 那么称这组函数为全域函数族
//
//  h(k) = ((ak+b) mod p) mod m
// 其中a属于[1,p-1], b属于[0,p-1]
// 每次程序运行在函数族里选出的函数是固定的
template <int _Size>
int UniversalHash(const int& key) {
    // 这里实现默认key小于10m
    constexpr int64_t kPrime = 10000019;
    static const int64_t kA = RandomInt(1, kPrime-1);
    static const int64_t kB = RandomInt(0, kPrime-1);
    return ((kA*key+kB)%kPrime)%_Size;
}

} // namespace algorithm

#endif // _ALGORITHM_COMMON_HASH_FUNC_H_
