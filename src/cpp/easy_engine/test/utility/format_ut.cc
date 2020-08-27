// format_ut.cc - 格式化ut
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 27
// License: GPL.v3

#include <gtest/gtest.h>

#include <iostream>

#include "utility/format.h"

using easy_engine::vec2;
using easy_engine::utility::Format;
using std::cout;
using std::endl;

class FMTUT : public testing::Test {
 public:
  static void SetUpTestCase();
  static void TearDownTestCase() {}
  void        SetUp() override {}
  void        TearDown() override {}
};

void FMTUT::SetUpTestCase() {}

TEST_F(FMTUT, BasicTest) {
  vec2 v1(1, 2);
  vec2 v2(3, 4);
  cout << Format("v1=%, v1.x=%, v2=%, v2.y=% %", v1, v1.x, v2, v2.y,
                 "test done")
       << endl;
}
