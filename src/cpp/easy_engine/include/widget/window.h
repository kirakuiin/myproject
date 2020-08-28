// window.h - 窗口
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_WIDGET_WINDOW_H__
#define __EASY_ENGINE_INCLUDE_WIDGET_WINDOW_H__

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace easy_engine {
namespace widget {

using KeyboardCallback   = void (*)(GLFWwindow* w, int key, int scancode,
                                  int action, int mods);
using CursorPosCallback  = void (*)(GLFWwindow* w, double pos_x, double pos_y);
using MouseBtnCallback   = void (*)(GLFWwindow* w, int button, int action,
                                  int mods);
using ScrollCallback     = void (*)(GLFWwindow* w, double pos_x, double pos_y);
using WindowSizeCallback = void (*)(GLFWwindow* w, int width, int height);

// 窗口类, 每一个实例代表屏幕上的一个窗口
class Window final {
 public:
  // @param screen_width: 窗口宽度
  // @param screen_height: 窗口高度
  // @param window_name: 窗口名称
  Window(unsigned int screen_width, unsigned int screen_height,
         const std::string& window_name);
  ~Window();

  // 激活当前窗口, 成为上下文环境
  void Activate();

  // 查询窗口是否应被关闭
  // @return: bool
  bool ShouldClose();

  // 更新窗口状态, 包括缓冲, 事件, 回调等等
  void Update();

  // 获得窗口大小
  // @param width: 窗口宽度
  // @param height: 窗口高度
  void GetWindowSize(int* width, int* height);

  // 设置指针隐形
  // @param hide: 指针是否隐形
  void SetCursorInvisble(bool hide);

  // 设置光标位置回调函数
  // @param callback: 光标位置回调函数
  //   @w(GLFWwindow*): 无意义, @pos_x(int): 相对左上角鼠标的x坐标
  //   @pos_y(int): 相对左上角鼠标的y坐标
  void SetCursorPosCallback(CursorPosCallback callback);

  // 设置鼠标按键回调函数
  // @param callback: 鼠标按键回调函数
  //   @w(GLFWwindow*): 无意义, @button(int): 鼠标按键, @action(int): 点击或松开
  //   @mods(int): 是否按下辅助键(ctrl, etc)
  void SetMouseBtnCallback(MouseBtnCallback callback);

  // 设置滚轮回调函数
  // @param callback: 滚轮回调函数
  //   @w(GLFWwindow*): 无意义, @pos_x(double): 滚轮x轴偏移量
  //   @pos_y(double): 滚轮y轴偏移量
  void SetScrollCallback(ScrollCallback callback);

  // 设置键盘回调函数
  // @param callback: 键盘回调函数
  //   @w(GLFWwindow*): 无意义, @key(int): 键盘按键, @scancode(int): 系统机械码
  //   @action(int): 按住还是松开, @mods(int): 是否按下辅助键(ctrl, etc)
  void SetKeyboardCallback(KeyboardCallback callback);

  // 设置窗口大小变化回调函数
  // @param callback: 变化回调函数
  //   @w(GLFWwindow*): 无意义, @width(int): 窗口新宽度
  // @height(int): 窗口新高度
  void SetWindowSizeCallback(WindowSizeCallback callback);

  // 得到窗口原始指针
  // @return GLFWwindow*: 窗口原始指针
  GLFWwindow* GetPtr() { return _p_window; }

 private:
  // 初始化opengl函数
  static void InitOpenGl();

  // 窗口类初始化
  void InitWindow();

  GLFWwindow* _p_window;  // 实际的窗口指针
};

}  // namespace widget
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_WIDGET_WINDOW_H__
