// main.cc - minigames主函数
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 07
// License: GPL.v3

#include <functional>
#include <vector>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define __EASY_ENGINE_ALL__
#include <easy_engine.h>

#include "include/mainpage.h"
#include "include/snake/egg.h"
#include "include/snake/snake.h"

namespace {
const int Width  = 1024;
const int Height = 768;
}  // namespace

using namespace easy_engine;
using namespace minigames;

int main(void) {
  try {
    // initial window
    std::shared_ptr<widget::Window> p_window(
        new widget::Window(Width, Height, "minigames"));
    // initial camera
    std::shared_ptr<graphics::Camera2D> p_camera(
        new graphics::Camera2D(vec2(0, Height), vec2(Width, Height)));
    // initial render
    std::shared_ptr<graphics::SpriteRender> p_render(
        new graphics::SpriteRender(p_camera));
    MainPage mainpage(p_window);
    while (!p_window->ShouldClose()) {
      opengl::ClearFramebuffer();
      mainpage.Update();
      mainpage.Draw();
      p_window->Update();
    }
  } catch (easy_engine::Exception& e) {
    easy_engine::utility::Logging::GetLogger()->Error(e.what());
  }
  return 0;
}
