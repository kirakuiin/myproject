// window.cc - 窗口实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#include "include/widget/window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <boost/circular_buffer.hpp>
#include <functional>
#include <iostream>
#include <map>

#include "include/opengl/glad/glad.h"
#include "include/widget/exception.h"

namespace easy_engine {
namespace widget {

// 窗口事件类型
enum class WinEventType {
  KEY     = 0,  // 键盘点击
  MOUSE   = 1,  // 鼠标点击
  CURSOR  = 2,  // 光标移动
  SCROLL  = 3,  // 滚轮移动
  WINSIZE = 4,  // 窗口大小调整
};

// 键盘事件
struct KeyEvent {
  KeyEvent(int k, int s, int a, int m)
      : key(k), scancode(s), action(a), mods(m) {}
  int key;       // 键位码
  int scancode;  // 扫描码
  int action;    // 动作(按下还是释放)
  int mods;      // 辅助键(shift, ctrl等等)
};

// 鼠标事件
struct MouseEvent {
  MouseEvent(int b, int a, int m) : button(b), action(a), mods(m) {}
  int button;  // 按钮
  int action;  // 动作
  int mods;    // 辅助键
};

// 光标事件
struct CursorEvent {
  CursorEvent(double x, double y) : pos_x(x), pos_y(y) {}
  double pos_x;  // 光标x轴坐标
  double pos_y;  // 光标y轴坐标
};

// 滚轮事件
struct ScrollEvent {
  ScrollEvent(double x, double y) : pos_x(x), pos_y(y) {}
  double pos_x;  // 滚轮x轴偏移量
  double pos_y;  // 滚轮y轴偏移量
};

// 窗口调整事件
struct WinSizeEvent {
  WinSizeEvent(int w, int h) : width(w), height(h) {}
  int width;   // 窗口宽度
  int height;  // 窗口高度
};

// 通用事件
union WinEvent {
  WinEvent() {}
  KeyEvent     key;
  MouseEvent   mouse;
  CursorEvent  cursor;
  ScrollEvent  scroll;
  WinSizeEvent winsize;
};

const int MaxQueueSize = 10;  // 队列最大容量
static std::map<GLFWwindow*,
                std::map<WinEventType, boost::circular_buffer<WinEvent>>>
    gm_winevent_queue;

void InnerKeyboardCallback(GLFWwindow* w, int key, int scancode, int action,
                           int mods) {
  WinEvent event;
  event.key = KeyEvent(key, scancode, action, mods);

  gm_winevent_queue[w][WinEventType::KEY].push_back(event);
}

void InnerCursorPosCallback(GLFWwindow* w, double pos_x, double pos_y) {
  WinEvent event;
  event.cursor = CursorEvent(pos_x, pos_y);

  gm_winevent_queue[w][WinEventType::CURSOR].push_back(event);
}

void InnerMouseBtnCallback(GLFWwindow* w, int button, int action, int mods) {
  WinEvent event;
  event.mouse = MouseEvent(button, action, mods);

  gm_winevent_queue[w][WinEventType::MOUSE].push_back(event);
}

void InnerScrollCallback(GLFWwindow* w, double pos_x, double pos_y) {
  WinEvent event;
  event.scroll = ScrollEvent(pos_x, pos_y);

  gm_winevent_queue[w][WinEventType::SCROLL].push_back(event);
}

void InnerWindowSizeCallback(GLFWwindow* w, int width, int height) {
  WinEvent event;
  event.winsize = WinSizeEvent(width, height);

  gm_winevent_queue[w][WinEventType::WINSIZE].push_back(event);
}

void Window::InitOpenGl() {
  static bool is_first_time = true;
  if (is_first_time) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      throw WidgetException("Init OpenGl library failed");
    }
    is_first_time = false;
  }
}

Window::Window(unsigned int window_width, unsigned int window_height,
               const std::string& window_name)
    : _key_callback(nullptr),
      _mouse_callback(nullptr),
      _cursor_callback(nullptr),
      _scroll_callback(nullptr),
      _winsize_callback(nullptr) {
  InitWindow();
  _p_window = glfwCreateWindow(window_width, window_height, window_name.c_str(),
                               NULL, NULL);
  // 创建失败
  if (!_p_window) {
    glfwTerminate();
    throw WidgetException("Create window failed");
  }
  // 激活窗口
  Activate();
  // 设定视口
  glViewport(0, 0, window_width, window_height);
  // 开启多重采样
  glEnable(GL_MULTISAMPLE);
  // 开启混合
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window() {
  gm_winevent_queue.erase(_p_window);
  glfwDestroyWindow(_p_window);
}

void Window::Activate() {
  glfwMakeContextCurrent(_p_window);
  // 激活之后才能初始化opengl环境
  InitOpenGl();
}

bool Window::ShouldClose() {
  return static_cast<bool>(glfwWindowShouldClose(_p_window));
}

void Window::Update() {
  // 交换缓冲
  glfwSwapBuffers(_p_window);
  // 检查触发事件, 更新窗口状态, 调用回调函数
  glfwPollEvents();
  // 更新事件队列
  PushEvents();
}

void Window::GetWindowSize(int* width, int* height) {
  glfwGetWindowSize(_p_window, width, height);
}

int Window::GetWindowMode(int mode) {
  return glfwGetInputMode(_p_window, mode);
}

void Window::SetWindowMode(int mode, int value) {
  glfwSetInputMode(_p_window, mode, value);
}

void Window::SetCursorInvisble(bool hide) {
  if (hide) {
    SetWindowMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  } else {
    SetWindowMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void Window::SetKeyboardCallback(KeyboardCallback callback) {
  _key_callback = callback;
  gm_winevent_queue[_p_window][WinEventType::KEY] =
      boost::circular_buffer<WinEvent>(MaxQueueSize);
  glfwSetKeyCallback(_p_window, InnerKeyboardCallback);
}

void Window::SetScrollCallback(ScrollCallback callback) {
  _scroll_callback = callback;
  gm_winevent_queue[_p_window][WinEventType::SCROLL] =
      boost::circular_buffer<WinEvent>(MaxQueueSize);
  glfwSetScrollCallback(_p_window, InnerScrollCallback);
}

void Window::SetMouseBtnCallback(MouseBtnCallback callback) {
  _mouse_callback = callback;
  gm_winevent_queue[_p_window][WinEventType::MOUSE] =
      boost::circular_buffer<WinEvent>(MaxQueueSize);
  glfwSetMouseButtonCallback(_p_window, InnerMouseBtnCallback);
}

void Window::SetCursorPosCallback(CursorPosCallback callback) {
  _cursor_callback = callback;
  gm_winevent_queue[_p_window][WinEventType::CURSOR] =
      boost::circular_buffer<WinEvent>(MaxQueueSize);
  glfwSetCursorPosCallback(_p_window, InnerCursorPosCallback);
}

void Window::SetWindowSizeCallback(WindowSizeCallback callback) {
  _winsize_callback = callback;
  gm_winevent_queue[_p_window][WinEventType::WINSIZE] =
      boost::circular_buffer<WinEvent>(MaxQueueSize);
  glfwSetWindowSizeCallback(_p_window, InnerWindowSizeCallback);
}

int Window::GetKey(int keycode) { return glfwGetKey(_p_window, keycode); }

int Window::GetMouse(int button) {
  return glfwGetMouseButton(_p_window, button);
}

void Window::GetCursor(double* x, double* y) {
  glfwGetCursorPos(_p_window, x, y);
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

void Window::PushEvents() {
  PushKeyEvents();
  PushCursorEvents();
  PushMouseEvents();
  PushScrollEvents();
  PushWinSizeEvents();
}

void Window::PushKeyEvents() {
  if (_key_callback) {
    auto& queue = gm_winevent_queue[_p_window][WinEventType::KEY];
    while (!queue.empty()) {
      auto& event = queue.front().key;
      _key_callback(event.key, event.scancode, event.action, event.mods);
      queue.pop_front();
    }
  }
}

void Window::PushMouseEvents() {
  if (_mouse_callback) {
    auto& queue = gm_winevent_queue[_p_window][WinEventType::MOUSE];
    while (!queue.empty()) {
      auto& event = queue.front().mouse;
      _mouse_callback(event.button, event.action, event.mods);
      queue.pop_front();
    }
  }
}

void Window::PushCursorEvents() {
  if (_cursor_callback) {
    auto& queue = gm_winevent_queue[_p_window][WinEventType::CURSOR];
    while (!queue.empty()) {
      auto& event = queue.front().cursor;
      _cursor_callback(event.pos_x, event.pos_y);
      queue.pop_front();
    }
  }
}

void Window::PushScrollEvents() {
  if (_scroll_callback) {
    auto& queue = gm_winevent_queue[_p_window][WinEventType::SCROLL];
    while (!queue.empty()) {
      auto& event = queue.front().scroll;
      _scroll_callback(event.pos_x, event.pos_y);
      queue.pop_front();
    }
  }
}

void Window::PushWinSizeEvents() {
  if (_winsize_callback) {
    auto& queue = gm_winevent_queue[_p_window][WinEventType::WINSIZE];
    while (!queue.empty()) {
      auto& event = queue.front().winsize;
      _winsize_callback(event.width, event.height);
      queue.pop_front();
    }
  }
}

}  // namespace widget
}  // namespace easy_engine
