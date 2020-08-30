// cursor.cc - 光标实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 30
// License: GPL.v3

#include "include/widget/cursor.h"

#include "GLFW/glfw3.h"

namespace easy_engine {
namespace widget {

void Cursor::Update() {
  if (!_enable) {
    return;
  }
  double x, y;
  _p_window->GetCursor(&x, &y);
  _cursor_pos.x = x;
  _cursor_pos.y = y;
}

void Cursor::Draw(std::shared_ptr<graphics::SpriteRender> render) {
  if (!_enable) {
    return;
  }
  render->DrawSprite(_p_sprite, _cursor_pos, _size);
}

void Cursor::Enable() {
  _cursor_mode = _p_window->GetWindowMode(GLFW_CURSOR);
  _p_window->SetCursorInvisble(true);
  _enable = true;
}

void Cursor::Disable() {
  _enable = false;
  _p_window->SetWindowMode(GLFW_CURSOR, _cursor_mode);
}

}  // namespace widget
}  // namespace easy_engine
