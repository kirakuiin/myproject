// config_ut.cc - 配置器ut
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 11
// License: GPL.v3

#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>

#include "boost/serialization/level_enum.hpp"
#include "utility/config.h"

namespace {
const std::string ConfPath("/Users/lambda/conf.json");
}

using boost::property_tree::ptree;
using easy_engine::utility::Configurator;
using easy_engine::utility::FlatConfigurator;
using std::cout;
using std::endl;

class CONFIGUT : public testing::Test {
 public:
  static void SetUpTestCase();
  static void TearDownTestCase() { std::remove(ConfPath.c_str()); }
  void        SetUp() override {}
  void        TearDown() override {}
};

void CONFIGUT::SetUpTestCase() {}

TEST_F(CONFIGUT, Write) {
  FlatConfigurator conf;
  conf.SetValue("a", 10);
  conf.SetValue("bb", "string");
  conf.SetValue("c", 1e-10);
  conf.SaveConfig(ConfPath);
}

TEST_F(CONFIGUT, Read) {
  FlatConfigurator conf;
  conf.LoadConfig(ConfPath);
  cout << conf.GetValue<float>("c") << endl;
}
