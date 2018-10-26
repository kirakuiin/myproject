// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-05

#include "max_subarray.h"
#include "common/algo_util.h"

namespace algorithm {
namespace misc {
namespace {
// 获得在[low, high]之间, 通过mid的array的最大连续子数组的信息
//
// pseudo code
// FIND-MAX-CROSSING-SUBARRAY(A, low, mid, high)
//   left-sum = -INF
//   sum = 0
//   for i = mid down to low
//     sum = sum + A[i]
//     if sum > left-sum
//       left-sum = sum
//       max-left = i
//   right-sum = -INF
//   sum = 0
//   for j = mid+1 to high
//     sum = sum + A[j]
//     if sum > right-sum
//       right-sum = sum
//       max-right = j
//   return (max-left, max-right, left-sum+right-sum)
MaxSubarrayInfo FindMaxCrossingSubarray(int low, int mid, int high,
                                        const int* array) {
    int left_sum = array[mid];
    int max_left = mid;
    int sum = 0;
    for (int i = mid; i >= low; --i) {
        sum += array[i];
        if (sum > left_sum) {
            left_sum = sum;
            max_left = i;
        }
    }
    int right_sum = array[mid+1];
    int max_right = mid+1;
    sum = 0;
    for (int j = mid+1; j <= high; ++j) {
        sum += array[j];
        if (sum > right_sum) {
            right_sum = sum;
            max_right = j;
        }
    }
    return MaxSubarrayInfo{max_left, max_right, left_sum+right_sum};
}

} // namespace

// pseudo code
// FIND-MAXIMUM-SUBARRAY(A, low, high)
//   if high == low     // base case: only one element
//     return (low, high, A[low])
//   else
//      mid = floor((low+high)/2)
//      (left-low, left-high, left-sum) =
//          FIND-MAXIMUM-SUBARRAY(A, low, mid)
//      (right-low, right-high, right-sum) =
//          FIND-MAXIMUM-SUBARRAY(A, mid+1, high)
//      (cross-low, cross-high, cross-sum) =
//          FIND-MAX-CROSSING-SUBARRAY(A, low, mid, high)
//   if left-sum >= right-sum and left-sum >= cross-sum
//      return (left-low, left-high, left-sum)
//   else if right-sum >= left-sum and right-sum >= cross-sum
//      return (right-low, right-high, right-sum)
//   else
//      return (cross-low, cross-high, cross-sum)
MaxSubarrayInfo FindMaximumSubarray(int low, int high, const int* array) {
    if (low == high) {
        return MaxSubarrayInfo{low, high, array[low]};
    } else {
        int mid = (low+high)/2;
        auto left_info = FindMaximumSubarray(low, mid, array);
        auto right_info = FindMaximumSubarray(mid+1, high, array);
        auto cross_info = FindMaxCrossingSubarray(low, mid, high, array);

        if (left_info.sum == Max(left_info.sum,
                                 right_info.sum, cross_info.sum)) {
            return left_info;
        } else if (right_info.sum == Max(left_info.sum,
                                         right_info.sum, cross_info.sum)) {
            return right_info;
        } else {
            return cross_info;
        }
    }
}

} // namespace misc
} // namespace algorithm
