// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-05

#ifndef _ALGORITHM_MISC_MAX_SUBARRAY_H_
#define _ALGORITHM_MISC_MAX_SUBARRAY_H_

namespace algorithm {
namespace misc {

// 存放最大子数组的相关信息
struct MaxSubarrayInfo {
    int left;   // 左侧下标
    int right;  // 右侧下标
    int sum;    // 最大连续和
};

// 获得在[low, high]之间, array里最大子数组的信息
// 平均时间复杂度: Θ(nlgn)
MaxSubarrayInfo FindMaximumSubarray(int low, int high, const int* array);

} // namespace misc
} // namespace algorithm

#endif // _ALGORITHM_MISC_MAX_SUBARRAY_H_
