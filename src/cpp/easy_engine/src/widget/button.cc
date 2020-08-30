// button.cc - 按钮实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 28
// License: GPL.v3

#include "include/widget/button.h"

#include "GLFW/glfw3.h"
#include "include/physics/collision.h"

namespace easy_engine {
namespace widget {

using physics::CheckCollision;
using physics::CollisionResult;
using physics::DotBox;

namespace {
const vec3 NormalColor(1);
const vec3 DimColor(0.6);
const vec3 BrightColor(1.4);
}  // namespace

void Button::Update() {
  if (!_enable) {
    return;
  }
  double x, y;
  _p_window->GetCursor(&x, &y);

  CollisionResult res = CheckCollision(DotBox(vec2(x, y)), *_p_hitbox.get());
  if (res.IsIntersect) {
    _status         = ButtonStatus::CURSOR_ON;
    int left_press  = _p_window->GetMouse(GLFW_MOUSE_BUTTON_LEFT);
    int right_press = _p_window->GetMouse(GLFW_MOUSE_BUTTON_RIGHT);
    if (left_press == GLFW_PRESS || right_press == GLFW_PRESS) {
      _status = ButtonStatus::CLICK;
      if (left_press == GLFW_PRESS && _callback) {
        _callback(GLFW_MOUSE_BUTTON_LEFT);
      }
      if (right_press == GLFW_PRESS && _callback) {
        _callback(GLFW_MOUSE_BUTTON_RIGHT);
      }
    }
  } else {
    _status = ButtonStatus::CURSOR_OFF;
  }
}

void Button::Draw(std::shared_ptr<graphics::SpriteRender> render) {
  if (!_enable) {
    return;
  }
  vec3 color;
  switch (_status) {
    case ButtonStatus::CURSOR_OFF:
      color = NormalColor;
      break;
    case ButtonStatus::CURSOR_ON:
      color = BrightColor;
      break;
    case ButtonStatus::CLICK:
      color = DimColor;
  }
  render->DrawSprite(_p_sprite, _btn_pos, _size, 0, color);
}

}  // namespace widget
}  // namespace easy_engine
