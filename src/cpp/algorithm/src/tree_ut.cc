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
#include "advanced_struct/binary_search_tree.h"
#include "advanced_struct/red_and_black_tree.h"

namespace structspace = algorithm::advanced_struct;

class TreeUT : public testing::Test {
  public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    void SetUp() override {
        algorithm::RandomizeInPlace(10, _array);
    }
    void TearDown() override {}

  protected:
    constexpr static int kArrayLen = 10;    // 数组的最大长度
    static int _array[kArrayLen];  // 数组本身
};

int TreeUT::_array[kArrayLen]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

TEST_F(TreeUT, BinarySearchTree) {
    PrintArray(_array, kArrayLen);
    structspace::BinarySearchTree<int> tree;

    for (int i = 0; i < kArrayLen; ++i) {
        tree.Insert(_array[i]);
    }

    ASSERT_EQ(tree.Maximum()->value, 9);
    ASSERT_EQ(tree.Minimum()->value, 0);
    auto node = tree.Search(5);
    auto succ = tree.Successor(node);
    auto pred = tree.Predecessor(node);
    printf("%d's succ=%d, pred=%d\n", node->value, succ->value, pred->value);
    tree.Delete(node);
    tree.Delete(succ);
    tree.Delete(pred);

    tree.Traverse();
    printf("\n");
}

TEST_F(TreeUT, RedAndBlackTree) {
    PrintArray(_array, kArrayLen);
    structspace::RedBlackTree<int> tree;

    for (int i = 0; i < kArrayLen; ++i) {
        tree.Insert(_array[i]);
    }

    tree.Traverse();
    printf("\n");
}
