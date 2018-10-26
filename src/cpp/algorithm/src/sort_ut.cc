// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-02

#include <cstdio>
#include <gtest/gtest.h>
#include "common/ut_util.h"
#include "sort_factory.h"

namespace sortspace = algorithm::sort_and_orderer_statistics;

class SortUT : public testing::Test {
  public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    void SetUp() override {
        int array[10] = {712, 339, 182, 1, 402, 501, 921, 23, 921, 47};
        memcpy(_array, array, kArrayLen*sizeof(int));
    }
    void TearDown() override {}

  protected:
    constexpr static int kArrayLen = 10;    // 数组的最大长度
    int _array[kArrayLen];  // 数组本身
};

// 测试插入排序
TEST_F(SortUT, InsertSort) {
    PrintArray(_array, kArrayLen);

    auto sort_ins = SortFactory::GetInstance()->GetInsertSort<int>();
    sort_ins->Sort(kArrayLen, Comp<int>, _array);

    PrintArray(_array, kArrayLen);
}

// 测试选择排序
TEST_F(SortUT, SelectSort) {
    PrintArray(_array, kArrayLen);

    auto sort_ins = SortFactory::GetInstance()->GetSelectSort<int>();
    sort_ins->Sort(kArrayLen, Comp<int>, _array);

    PrintArray(_array, kArrayLen);
}

// 测试归并排序
TEST_F(SortUT, MergeSort) {
    PrintArray(_array, kArrayLen);

    auto sort_ins = SortFactory::GetInstance()->GetMergeSort<int>();
    sort_ins->Sort(kArrayLen, Comp<int>, _array);

    PrintArray(_array, kArrayLen);
}

// 测试冒泡排序
TEST_F(SortUT, BubbleSort) {
    PrintArray(_array, kArrayLen);

    auto sort_ins = SortFactory::GetInstance()->GetBubbleSort<int>();
    sort_ins->Sort(kArrayLen, Comp<int>, _array);

    PrintArray(_array, kArrayLen);
}

// 测试堆排序
TEST_F(SortUT, HeapSort) {
    PrintArray(_array, kArrayLen);

    auto sort_ins = SortFactory::GetInstance()->GetHeapSort<int>();
    sort_ins->Sort(kArrayLen, Comp<int>, _array);

    PrintArray(_array, kArrayLen);
}

// 测试快速排序
TEST_F(SortUT, QuickSort) {
    PrintArray(_array, kArrayLen);

    auto sort_ins = SortFactory::GetInstance()->GetQuickSort<int>();
    sort_ins->Sort(kArrayLen, Comp<int>, _array);

    PrintArray(_array, kArrayLen);
}

// 测试计数排序
TEST_F(SortUT, CountingSort) {
    PrintArray(_array, kArrayLen);

    auto sort_ins = SortFactory::GetInstance()->GetCountingSort<int>();
    sort_ins->Sort(kArrayLen, Comp<int>, _array);

    PrintArray(_array, kArrayLen);
}

// 测试基数排序
TEST_F(SortUT, RadixSort) {
    PrintArray(_array, kArrayLen);

    auto sort_ins = SortFactory::GetInstance()->GetRadixSort<int>();
    sort_ins->Sort(kArrayLen, Comp<int>, _array);

    PrintArray(_array, kArrayLen);
}

// 测试桶排序
TEST_F(SortUT, BucketSort) {
    PrintArray(_array, kArrayLen);

    auto sort_ins = SortFactory::GetInstance()->GetBucketSort<int>();
    sort_ins->Sort(kArrayLen, Comp<int>, _array);

    PrintArray(_array, kArrayLen);
}

// 测试寻找最大最小值
TEST_F(SortUT, MaxAndMin) {
    PrintArray(_array, kArrayLen);
    int max, min;
    sortspace::FindMaxAndMin(_array, 10, &max, &min);
    printf("max=%d, min=%d\n", max, min);
}

// 测试随机化选择算法
TEST_F(SortUT, RandomizedSelect) {
    PrintArray(_array, kArrayLen);
    for (int order = 1; order <= 10; ++order) {
        int value = sortspace::RandomizedSelect(_array, 0, 9, order);
        printf("the %dth value = %d\n", order, value);
    }
}

// 测试选择算法
TEST_F(SortUT, Select) {
    PrintArray(_array, kArrayLen);
    for (int order = 1; order <= 10; ++order) {
        int value = sortspace::Select(_array, 0, 9, order);
        printf("the %dth value = %d\n", order, value);
    }
}
