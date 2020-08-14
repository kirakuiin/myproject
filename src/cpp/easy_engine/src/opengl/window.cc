// window.cc - 窗口实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#include "opengl/window.h"

#include <functional>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "opengl/exception.h"
#include "opengl/glad/glad.h"

namespace easy_engine {
namespace opengl {

void Window::InitOpenGl() {
  static bool is_first_time = true;
  if (is_first_time) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      throw GlException("Init OpenGl library failed");
    }
    is_first_time = false;
  }
}

Window::Window(unsigned int window_width, unsigned int window_height,
               const std::string& window_name) {
  InitWindow();
  _window = glfwCreateWindow(window_width, window_height, window_name.c_str(),
                             NULL, NULL);
  // 创建失败
  if (!_window) {
    glfwTerminate();
    throw GlException("Create window failed");
  }
  Activate();
  // 设定视口
  glViewport(0, 0, window_width, window_height);
  // 开启多重采样
  glEnable(GL_MULTISAMPLE);
  // 开启混合
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  SetChangeSizeCallback();
}

Window::~Window() { glfwDestroyWindow(_window); }

void Window::Activate() {
  glfwMakeContextCurrent(_window);
  // 激活之后才能初始化opengl环境
  InitOpenGl();
}

bool Window::ShouldClose() {
  return static_cast<bool>(glfwWindowShouldClose(_window));
}

void Window::Update() {
  // 交换缓冲
  glfwSwapBuffers(_window);
  // 检查触发事件, 更新窗口状态, 调用回调函数
  glfwPollEvents();
}

void Window::GetWindowSize(int* width, int* height) {
  glfwGetWindowSize(_window, width, height);
}

void Window::SetCursorInvisble(bool hide) {
  if (hide) {
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  } else {
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void Window::SetKeyboardCallback(KeyboardCallback callback) {
  glfwSetKeyCallback(_window, callback);
}

void Window::SetScrollCallback(ScrollCallback callback) {
  glfwSetScrollCallback(_window, callback);
}

void Window::SetMouseBtnCallback(MouseBtnCallback callback) {
  glfwSetMouseButtonCallback(_window, callback);
}

void Window::SetCursorPosCallback(CursorPosCallback callback) {
  glfwSetCursorPosCallback(_window, callback);
}

void Window::InitWindow() {
  static bool is_first_time = true;
  if (is_first_time) {
    glfwInit();
    // 重置设置
    glfwDefaultWindowHints();
    // 核心版本3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 启用双缓冲
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    // 窗口大小可调节
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    // MSAA采样数
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    // 前向兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    is_first_time = false;
  }
}

void Window::SetChangeSizeCallback() {
  GLFWframebuffersizefun func = [](GLFWwindow* win, int w, int h) {
    glViewport(0, 0, w, h);
  };
  glfwSetFramebufferSizeCallback(_window, func);
}

}  // namespace opengl
}  // namespace easy_engine
