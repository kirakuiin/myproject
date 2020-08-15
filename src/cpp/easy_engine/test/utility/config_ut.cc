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

TEST_F(CONFIGUT, Struct) {
  ptree root;
  ptree obj;
  Configurator::Set("v", 1.5f, &obj);
  Configurator::Set("n", 2.5f, &obj);
  Configurator::Set("n", 3.5f, &obj);
  std::vector<float> v{1.5, 2.2, 3.3};
  FlatConfigurator::SetVector("vec", v, &obj);
  Configurator::SetObject("obj1", obj, &root);
  Configurator::SetObject("obj2", obj, &root);
  Configurator::SetVector("vec", v, &root);
  ptree res = Configurator::GetObject(root, "obj1");
  v.clear();
  Configurator::GetVector(res, "vec", &v);
  cout << Configurator::Get<float>(res, "v") << endl;
  cout << v[0] << v[1] << v[2] << endl;
}
