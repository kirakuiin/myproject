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
#include "common/hash_func.h"
#include "advanced_struct/chaining_hash.h"
#include "advanced_struct/open_addressing_hash.h"
#include "advanced_struct/perfect_hash.h"

namespace structspace = algorithm::advanced_struct;

class HashUT : public testing::Test {
  public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(HashUT, ChainingHash) {
    const int kTableSize = 16;
    structspace::ChainingHash<int, kTableSize> hash_table;
    hash_table.SetHashMethod(algorithm::UniversalHash<16>);

    for (int i = 0; i < 20; ++i) {
        int hash_value = algorithm::UniversalHash<16>(i);
        printf("hash(%d)=%d\n", i, hash_value);
        hash_table.HashInsert(i, hash_value);
    }

    hash_table.Print();
}

TEST_F(HashUT, OpenAddressingHash) {
    const int kTableSize = 16;
    structspace::OpenAddressingHash<int, kTableSize> hash_table;
    hash_table.SetHashMethod(algorithm::MultiplicationHash<4>);

    for (int i = 0; i < 16; ++i) {
        int hash_value = algorithm::MultiplicationHash<4>(i);
        printf("hash(%d)=%d\n", i, hash_value);
        hash_table.HashInsert(i, hash_value);
    }

    int hash_value = algorithm::MultiplicationHash<4>(3);
    ASSERT_EQ(hash_table.HashSearch(3, hash_value), hash_value);
    hash_table.HashDelete(4, algorithm::MultiplicationHash<4>(4));

    hash_table.Print();
}

TEST_F(HashUT, PerfectHash) {
    const int kTableSize = 10;
    int keylist[kTableSize]{0,1,2,3,4,5,6,7,8,9};
    structspace::PerfectHash<int, kTableSize> hash_table(keylist);

    for (int i = 0; i < kTableSize; ++i) {
        hash_table.HashInsert(i, i);
    }

    hash_table.Print();
}
