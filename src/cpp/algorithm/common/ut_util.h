// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-05

#ifndef _ALGORITHM_COMMON_UT_DISPLAY_H_
#define _ALGORITHM_COMMON_UT_DISPLAY_H_

#include <random>
#include <iostream>

// 打印数组array的前array_len个元素
template<typename T>
void PrintArray(const T& array, int array_len) {
    for (int i = 0; i < array_len; ++i) {
        std::cout<<array[i]<<' ';
    }
    std::cout<<std::endl;
}

// 比较函数
template<typename T>
int Comp(const T& f, const T& s) {
    if (f > s) {
        return 1;
    } else if (f == s) {
        return 0;
    } else {
        return -1;
    }
}

#endif // _ALGORITHM_COMMON_UT_DISPLAY_H_
