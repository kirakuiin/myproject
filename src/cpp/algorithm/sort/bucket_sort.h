// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-03

#ifndef _ALGORITHM_SORT_BUCKET_SORT_H_
#define _ALGORITHM_SORT_BUCKET_SORT_H_

#include <type_traits>
#include "common/algo_util.h"
#include "include/sort_interface.h"
#include "insert_sort.h"

namespace algorithm {
namespace sort_and_orderer_statistics {

// 桶排序, 原理为:
// 假设数据平均分布在某一个区间内, 那我们可以将这个区间划分为若干个桶
// 根据一定规则将元素分布到各个桶中, 然后对每个桶都进行插入排序,
// 最后在将结果合并到一起即完成排序
// 平均时间复杂度: Θ(n)
template<typename T>
class BucketSort : public SortInterface<T> {
  public:
    void Sort(int array_len, int (*comp)(const T&, const T&),
              T* array) const override;

  private:
    // 存储桶的容器
    struct Bucket
    {
        int len = 0;
        T* array = nullptr;
    };
    // 桶的数量
    const int _bucket_num = 10;
    // 获得桶的位置, 分为10个桶[0,99],[100,199],...
    int GetBucketIndex(const T& number) const { return number/100; }
};

// pseudo code
// BUCKET-SORT(A)
//   n = A.length
//   let B[0..n-1] be a new array
//   for i = 0 to n-1
//     make B[i] an empty list
//   for i = 1 to n
//     insert A[i] to list B[floor(nA[i])]
//   for i = 0 to n-1
//     sort list B[i] with insertion sort
//   concatenate the lists B[0],B[1],...,B[n-1] together in order
template<typename T>
void BucketSort<T>::Sort(int array_len, int (*comp)(const T&, const T&),
                        T* array) const {
    static_assert(std::is_integral<T>::value, "Integral required.");
    Bucket bucket_array[_bucket_num];
    for (int i = 0; i < _bucket_num; ++i) {
        bucket_array[i].len = 0;
        bucket_array[i].array = new T[array_len];
    }

    // dispatch
    // 由于仅是为了示范所以将输入范围限定[0,1000]
    for (int i = 0; i < array_len; ++i) {
        assert(array[i] < 1000);
        int index = GetBucketIndex(array[i]);
        int& curr_len = bucket_array[index].len;
        bucket_array[index].array[curr_len++] = array[i];
    }
    // sort
    InsertSort<T> sortins;
    for (int i = 0; i < _bucket_num; ++i) {
        sortins.Sort(bucket_array[i].len, comp, bucket_array[i].array);
    }
    // merge
    int array_index = 0;
    for (int i = 0; i < _bucket_num; ++i) {
        for (int j = 0; j < bucket_array[i].len; ++j) {
            array[array_index++] = bucket_array[i].array[j];
        }
    }

    for (int i = 0; i < _bucket_num; ++i) {
        delete[] bucket_array[i].array;
    }
}

} // namespace sort_and_orderer_statistics
} // namespace algorithm

#endif // _ALGORITHM_SORT_BUCKET_SORT_H_
