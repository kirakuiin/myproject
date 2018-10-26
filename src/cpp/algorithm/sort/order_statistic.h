// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-12

#ifndef _ALGORITHM_MISC_ORDER_STATISTIC_H_
#define _ALGORITHM_MISC_ORDER_STATISTIC_H_

#include "common/algo_util.h"
#include "sort/insert_sort.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 同时寻找一个数组里的最大值和最小值
// 只需要比较3n/2次即可得到结果
// 平均时间复杂度: Θ(n)
//
// pseudo code
// FIND-MAX-MIN(A)
//   if A.length is and odd number
//     min = A[1]
//     max = A[1]
//   else
//     // only compare once
//     index = GET-MIN-INDEX(A[1], A[2])
//     min = A[index]
//     // calcuate another index, start add offset
//     max = A[1+(1+1-index)]
//     // skip the first or two element
//   i = 3 - A.length%2
//   while i <= A.length
//     index = GET-MIN-INDEX(A[i], A[i+1])
//     min = MIN(A[index], min)
//     max = MAX(A[i+(i+1-index)], max)
//     i = i + 2
//   return (min, max)
template <typename T>
void FindMaxAndMin(T* array, int array_len, T* max, T* min) {
    auto GetMaxIndex = [&array](int i, int j) {
        return array[i] > array[j] ? i : j;
    };
    if (array_len%2) {
        *max = *min = array[0];
    } else {
        int max_index = GetMaxIndex(0, 1);
        *max = array[max_index];
        *min = array[1-max_index];
    }
    for (int i = 2-array_len%2; i < array_len; i+=2) {
        int max_index = GetMaxIndex(i, i+1);
        *max = Max(*max, array[max_index]);
        *min = Min(*min, array[i+(i+1-max_index)]);
    }
}

// pseudo code
// RANDOMIZED-PARTITION(A, p, r)
//   index = RANDOM(p, r)
//   exchange A[r] with A[index]
//   key = A[r]
//   i = p - 1
//   for j = p to r - 1
//     if A[j] <= key
//       i = i + 1
//       exchange A[j] with A[i]
//   exchange A[r] with A[i+1]
//   return i+1
template<typename T>
int RandomizedPartition(T* array, int p, int r) {
    Swap(array[r], array[RandomInt(p, r)]);
    T key = array[r];
    int i = p - 1;
    for (int j = p; j < r; ++j) {
        if (array[j] <= key) {
            ++i;
            Swap(array[i], array[j]);
        }
    }
    Swap(array[r], array[i+1]);
    return i+1;
}

// 寻找数组里的第index小的元素
// 结合了快排的划分和二分搜索
// 平均时间复杂度: Θ(n)
//
// pseudo code
// RANDOMIZED-SELECT(A, p, r, i)
//   if p == r
//     return A[p]
//   q = RANDOMIZED-PARTITION(A, p, r)
//   k = q - p + 1
//   if i == k
//     return A[q]
//   else if i < k
//     return RANDOMIZED-SELECT(A, p, q-1, i)
//   else
//     return RANDOMIZED-SELECT(A, q+1, r, i-k)
template <typename T>
T RandomizedSelect(T* array, int start, int end, int index)
{
    if (start == end) {
        return array[start];
    } else {
        int mid = RandomizedPartition(array, start, end);
        int range = mid - start + 1;
        if (index == range) {
            return array[mid];
        } else if (index < range) {
            return RandomizedSelect(array, start, mid-1, index);
        } else {
            return RandomizedSelect(array, mid+1, end, index-range);
        }
    }
}

// pseudo code
// KEY-PARTITION(A, p, r, key)
//   for i = p to r - 1
//     if A[i] == key
//       exchange A[i] with A[r]
//       break
//   i = p - 1
//   for j = p to r - 1
//     if A[j] <= key
//       i = i + 1
//       exchange A[j] with A[i]
//   exchange A[r] with A[i+1]
//   return i+1
template<typename T>
int KeyPartition(T* array, int p, int r, T key) {
    for (int i = p; i < r; ++i) {
        if (key == array[i]) {
            Swap(array[r], array[i]);
            break;
        }
    }
    int i = p - 1;
    for (int j = p; j < r; ++j) {
        if (array[j] <= key) {
            ++i;
            Swap(array[i], array[j]);
        }
    }
    Swap(array[r], array[i+1]);
    return i+1;
}

// 最坏情况为Θ(n)的选择算法
// 将数组划分为每个大小为5的子数组, 然后用插入排序寻找所有子数组的中位数
// 然后对这些中位数继续调用选择算法选出中位数里的中位数
// 在用此中位数作为主元继续划分, 直到找到要找的元素为止
//
// pseudo code
// SELECT(A, p, r, i)
//   if p == r
//     return A[p]
//   divide A into ceiling(A.length/5) arrays
//   let M[1..ceiling(A.length/5)] be a new array
//   for j = 1 to floor(A.length/5)
//     INSERTION-SORT(A[p+(j-1)*5+1..p+(j-1)*5+5])
//     // put median into M
//     M[j] = A[p+j+2]
//   if A.lenght%5 != 0
//     // dealing with remaining elements
//     j = floor(A.length/5)
//     INSERTION-SORT(A[p+5*j+1..p+5*j+A.length%5])
//     M[j+1] = A[p+j*5+floor((A.length%5)/2)+1]
//   // find median of M
//   median = SELECT(M, 1, M.length, ceiling(M.length/2))
//   q = MEDIAN-PARTITION(A, p, r, median)
//   k = p - q + 1
//   if i == k
//     return A[q]
//   else if i < k
//     return SELECT(A, p, q-1, i)
//   else
//     return SELECT(A, q+1, r, i-k)
template <typename T>
T Select(T* array, int start, int end, int index) {
    if (start == end) {
        return array[start];
    }
    int length = end - start + 1;
    int mid_array_len = length/5+(length%5 ? 1 : 0);
    T midarray[mid_array_len];

    // 取得中位数
    InsertSort<T> sortins;
    for (int i = 0; i < length/5; ++i) {
        sortins.Sort(5, Comp<T>, &array[start+i*5]);
        midarray[i] = array[start+i+2];
    }
    if (mid_array_len > length/5) {
        sortins.Sort(length%5, Comp<T>, &array[length/5*5]);
        midarray[mid_array_len-1] = array[start+length/5*5+(length%5-1)/2];
    }

    int median = Select(midarray, 0, mid_array_len-1,
                        std::ceil(mid_array_len/2.0));
    int mid = KeyPartition(array, start, end, median);
    int range = mid - start + 1;
    if (index == range) {
        return array[mid];
    } else if (index < range) {
        return Select(array, start, mid-1, index);
    } else {
        return Select(array, mid+1, end, index-range);
    }
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_MISC_ORDER_STATISTIC_H_
