// window_ut.cc - 窗口ut
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#include <gtest/gtest.h>

#include <boost/format.hpp>
#include <iostream>

#include "opengl/window.h"

using boost::format;
using easy_engine::opengl::Window;
using std::cout;
using std::endl;

void Pos(GLFWwindow* w, double x, double y) {
  cout << format("x=%f, y=%f") % x % y << endl;
}
void Btn(GLFWwindow* w, int k, int a, int m) {
  cout << format("key=%d, action=%d, mods=%d") % k % a % m << endl;
}
void Key(GLFWwindow* w, int k, int s, int a, int m) {
  cout << format("key=%d, scancode=%d, action=%d, mods=%d") % k % s % a % m
       << endl;
}

class WINUT : public testing::Test {
 public:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
  void        SetUp() override {}
  void        TearDown() override {}
};

TEST_F(WINUT, BasicTest) {
  Window w(800, 600, "hello opengl");
  w.SetCursorInvisble(true);
  w.SetCursorPosCallback(Pos);
  w.SetMouseBtnCallback(Btn);
  w.SetScrollCallback(Pos);
  w.SetKeyboardCallback(Key);
  while (!w.ShouldClose()) {
    w.Update();
  }
  int wd, hi;
  w.GetWindowSize(&wd, &hi);
  cout << format("width=%d, height=%d") % wd % hi << endl;
}
