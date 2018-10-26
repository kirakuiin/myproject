// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-02

#include <cstdio>
#include <iostream>
#include <gtest/gtest.h>
#include "common/ut_util.h"
#include "common/algo_util.h"
#include "basic_struct/priority_queue.h"
#include "basic_struct/stack.h"
#include "basic_struct/queue.h"
#include "basic_struct/list.h"

namespace structspace = algorithm::basic_struct;

TEST(StructUT, PriorityQueue) {
    int array[10] = {7, 3, 1, 1, 4, 5, 2, 0, 9, 6};
    PrintArray(array, 10);

    structspace::PriorityQueue<int> minQueue;
    minQueue.BuildMinPriorityQueue(10, Comp<int>, array);
    printf("size = %d\n", minQueue.size());

    for (int j = 0; j < 9; ++j) {
        printf("%d ", minQueue.Pop());
    }
    printf("%d", minQueue.Top());
    printf("\n");
}

TEST(StructUT, Stack) {
    structspace::Stack<int> stack(5);
    ASSERT_TRUE(stack.Empty());
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);
    EXPECT_FALSE(stack.Empty());
    ASSERT_EQ(stack.Pop(), 3);
    ASSERT_EQ(stack.Pop(), 2);
    ASSERT_EQ(stack.Pop(), 1);
    ASSERT_TRUE(stack.Empty());
}

TEST(StructUT, Queue) {
    structspace::Queue<int> queue(3);
    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);
    ASSERT_EQ(queue.Dequeue(), 1);
    ASSERT_EQ(queue.Dequeue(), 2);
    ASSERT_EQ(queue.Dequeue(), 3);
}

TEST(StructUT, List) {
    auto elem1 = structspace::CreateListNode<int>(1);
    auto elem2 = structspace::CreateListNode<int>(2);
    auto elem3 = structspace::CreateListNode<int>(3);

    structspace::List<int> mylist;
    mylist.Insert(elem1);
    mylist.Insert(elem2);
    mylist.Insert(elem3);

    auto expect2 = mylist.Search(2);
    EXPECT_EQ(expect2, elem2);

    mylist.Print();
    mylist.Delete(expect2);
    mylist.Print();

    EXPECT_EQ(mylist.Search(2), nullptr);
}
