// log_ut.cc - 日志ut
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 11
// License: GPL.v3

#include <gtest/gtest.h>

#include <iostream>

#include "tools/log.h"

using easy_engine::tools::Logging;
using easy_engine::tools::LoggingLevel;
using std::cout;
using std::endl;

class LOGUT : public testing::Test {
 public:
  static void SetUpTestCase();
  static void TearDownTestCase() {}
  void        SetUp() override {}
  void        TearDown() override {}
};

void LOGUT::SetUpTestCase() {}

TEST_F(LOGUT, BasicTest) {
  auto logger = Logging::GetLogger();
  logger->Config(LoggingLevel::WARN, true, "", "[%s]");
  logger->Warn("It's a warn message");
  logger->Critical("It's a critical message");
  auto ano = Logging::GetLogger();
  ano->Info("It's a info message");
  logger->Config(LoggingLevel::INFO);
  ano->Info("It's a info message");
  logger = Logging::GetLogger("new");
  logger->Config(LoggingLevel::ERROR, true);
  logger->Error("It's a error message");
  logger->Debug("It's a debug message");
}
