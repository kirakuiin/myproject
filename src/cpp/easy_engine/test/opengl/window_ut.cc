// opengl_ut.cc - 所有的opengl相关ut
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#include <gtest/gtest.h>

#include <boost/format.hpp>
#include <iostream>
#include <memory>

#include "opengl/framebuffer.h"
#include "opengl/shader.h"
#include "opengl/window.h"

using boost::format;
using namespace easy_engine::opengl;
using std::cout;
using std::endl;

namespace {
const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;
void      Pos(GLFWwindow* w, double x, double y) {
  cout << format("x=%f, y=%f") % x % y << endl;
}
void Btn(GLFWwindow* w, int k, int a, int m) {
  cout << format("key=%d, action=%d, mods=%d") % k % a % m << endl;
}
void Key(GLFWwindow* w, int k, int s, int a, int m) {
  cout << format("key=%d, scancode=%d, action=%d, mods=%d") % k % s % a % m
       << endl;
}
}  // namespace

class WINUT : public testing::Test {
 public:
  static void SetUpTestCase() {}
  static void TearDownTestCase() {}
  void        SetUp() override {}
  void        TearDown() override {}
};

TEST_F(WINUT, BasicTest) {
  Window w(SCREEN_WIDTH, SCREEN_HEIGHT, "basictest");
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

TEST_F(WINUT, FrameBufferTest) {
  Window w(800, 600, "framebuffertest");

  // 创建一个新的帧缓冲
  auto color_attachment = std::shared_ptr<ColorAttachment>(
      new TextureColorAttachment(SCREEN_WIDTH, SCREEN_HEIGHT));
  Framebuffer fb;
  fb.Attach(color_attachment);
  fb.Bind();

  DefaultFramebufferColor(0, 0, 0, 0);
  while (!w.ShouldClose()) {
    ClearFramebuffer();
    w.Update();
  }
  UnbindFramebuffer();
}

TEST_F(WINUT, ShaderTest) {
  Window w(1024, 768, "shadertest");

  std::shared_ptr<VertexShader>   vs(new VertexShader("shaders/test.vert"));
  std::shared_ptr<FragmentShader> fs(new FragmentShader("shaders/test.frag"));
  ShaderProgram                   pro;
  pro.Attach(vs);
  pro.Attach(fs);
  pro.Bind();

  DefaultFramebufferColor(0, 0, 0, 0);
  while (!w.ShouldClose()) {
    ClearFramebuffer();
    w.Update();
  }
}
