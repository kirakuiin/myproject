// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-02

#ifndef _ALGORITHM_SRC_SORT_FACTORY_H_
#define _ALGORITHM_SRC_SORT_FACTORY_H_

#include <memory>
#include "sort/insert_sort.h"
#include "sort/select_sort.h"
#include "sort/merge_sort.h"
#include "sort/bubble_sort.h"
#include "sort/heap_sort.h"
#include "sort/quick_sort.h"
#include "sort/counting_sort.h"
#include "sort/radix_sort.h"
#include "sort/bucket_sort.h"
#include "sort/order_statistic.h"

namespace sortspace = ::algorithm::sort_and_orderer_statistics;

// 单例工厂类, 用来创建不同的排序对象
class SortFactory {
  public:
    // 获得工厂类的实例, 懒汉式实现
    static SortFactory* GetInstance() {
        static SortFactory instance;
        return &instance;
    }

    // 获得插入排序实例
    template<typename T>
    std::unique_ptr<sortspace::SortInterface<T>> GetInsertSort() {
        auto sortIns = new sortspace::InsertSort<T>();
        return std::unique_ptr<sortspace::SortInterface<T>>(sortIns);
    }

    // 获得选择排序实例
    template<typename T>
    std::unique_ptr<sortspace::SortInterface<T>> GetSelectSort() {
        auto sortIns = new sortspace::SelectSort<T>();
        return std::unique_ptr<sortspace::SortInterface<T>>(sortIns);
    }

    // 获得归并排序实例
    template<typename T>
    std::unique_ptr<sortspace::SortInterface<T>> GetMergeSort() {
        auto sortIns = new sortspace::MergeSort<T>();
        return std::unique_ptr<sortspace::SortInterface<T>>(sortIns);
    }

    // 获得冒泡排序实例
    template<typename T>
    std::unique_ptr<sortspace::SortInterface<T>> GetBubbleSort() {
        auto sortIns = new sortspace::BubbleSort<T>();
        return std::unique_ptr<sortspace::SortInterface<T>>(sortIns);
    }

    // 获得堆排序实例
    template<typename T>
    std::unique_ptr<sortspace::SortInterface<T>> GetHeapSort() {
        auto sortIns = new sortspace::HeapSort<T>();
        return std::unique_ptr<sortspace::SortInterface<T>>(sortIns);
    }

    // 获得快速排序实例
    template<typename T>
    std::unique_ptr<sortspace::SortInterface<T>> GetQuickSort() {
        auto sortIns = new sortspace::QuickSort<T>();
        return std::unique_ptr<sortspace::SortInterface<T>>(sortIns);
    }

    // 获得计数排序实例
    template<typename T>
    std::unique_ptr<sortspace::SortInterface<T>> GetCountingSort() {
        auto sortIns = new sortspace::CountingSort<T>();
        return std::unique_ptr<sortspace::SortInterface<T>>(sortIns);
    }

    // 获得基数排序实例
    template<typename T>
    std::unique_ptr<sortspace::SortInterface<T>> GetRadixSort() {
        auto sortIns = new sortspace::RadixSort<T>();
        return std::unique_ptr<sortspace::SortInterface<T>>(sortIns);
    }

    // 获得桶排序实例
    template<typename T>
    std::unique_ptr<sortspace::SortInterface<T>> GetBucketSort() {
        auto sortIns = new sortspace::BucketSort<T>();
        return std::unique_ptr<sortspace::SortInterface<T>>(sortIns);
    }
};

#endif // _ALGORITHM_SRC_SORT_FACTORY_H_
