// Copyright (c) Eisoo Software, Inc.(2004 - 2016), All rights reserved.
// Author:
//     wang.zhuowei@eisoo.com
// Creating Time:
//     2018- 5-02

#include <cstdio>
#include <gtest/gtest.h>
#include "common/ut_util.h"
#include "common/algo_util.h"
#include "misc/max_subarray.h"

namespace miscspace = algorithm::misc;

TEST(MiscUT, MaxSubarray) {
    int array[10] = {1, 3, -1, 5, -8, 9, -3, 5, 2, 1};
    PrintArray(array, 10);
    auto info = miscspace::FindMaximumSubarray(0, 9, array);
    printf("left=%d, right=%d, sum=%d\n", info.left, info.right, info.sum);
}
