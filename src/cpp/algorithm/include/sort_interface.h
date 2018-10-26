// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-02

#ifndef _ALGORITHM_INCLUDE_SORT_INTERFACE_H_
#define _ALGORITHM_INCLUDE_SORT_INTERFACE_H_

namespace algorithm {
namespace sort_and_orderer_statistics {

// 排序类的接口, 所有具有排序功能的子类均要实现此接口
// example:
//     SortInterface* sort_ins = new ...Sort();
//     int array[10] = { 1, 3, -1, ... }
//     int comp(const int& a, const int& b) {
//         ...
//     }
//     sort_ins->Sort(10, comp, array); // {-1, 1, 3, ... }
template<typename T>
class SortInterface {
  public:
    virtual ~SortInterface(){};

    // 对array的前array_len个元素根据comp规则进行排序
    //
    // 对于两个元素a和b, 如果comp < 0, a小于b(a在b前)
    // 如果 comp = 0, 说明a == b
    // 如果 comp > 0, a大于b(a在b后)
    virtual void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const = 0;
};

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_INCLUDE_SORT_INTERFACE_H_
