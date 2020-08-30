// button.h - 按钮
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 28
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_WIDGET_BUTTON_H__
#define __EASY_ENGINE_INCLUDE_WIDGET_BUTTON_H__

#include <functional>
#include <memory>

#include "../opengl/texture.h"
#include "../physics/collision.h"
#include "widget.h"
#include "window.h"

namespace easy_engine {
namespace widget {

// 按钮回调函数原型
using ButtonCallBack = std::function<void(int button)>;

// 按钮基类
class Button : public Widget {
 public:
  Button(std::shared_ptr<Window> window, const vec2& pos, const vec2& size,
         std::shared_ptr<opengl::Texture2D> sprite)
      : _btn_pos(pos),
        _size(size),
        _enable(true),
        _p_window(window),
        _p_sprite(sprite),
        _callback(nullptr) {
    _p_hitbox.reset(new physics::PolygonBox{pos, pos + vec2(size.x, 0),
                                            pos + vec2(0, size.y), pos + size});
  }
  ~Button() {}

  void Update() override;
  void Draw(std::shared_ptr<graphics::SpriteRender> render) override;
  void Enable() override { _enable = true; }
  void Disable() override { _enable = false; }

  // 设置鼠标点击的回调函数
  //
  // @param callback: 当点击按钮时触发的函数
  void SetCallback(ButtonCallBack callback) { _callback = callback; }

  // 设置碰撞盒
  //
  // @param hitbox: 按钮的碰撞盒
  void SetHitBox(std::shared_ptr<physics::HitBox> hitbox) {
    _p_hitbox = hitbox;
  }

 private:
  // 按钮状态
  enum class ButtonStatus {
    CURSOR_OFF = 0,  // 光标不在按钮上
    CURSOR_ON  = 1,  // 光标在按钮上
    CLICK      = 2,  // 光标按下
  };

  vec2         _btn_pos;  // 按钮位置
  vec2         _size;     // 按钮大小
  ButtonStatus _status;   // 按钮状态
  bool         _enable;   // 按钮是否启用

  std::shared_ptr<physics::HitBox>   _p_hitbox;  // 碰撞盒
  std::shared_ptr<Window>            _p_window;  // 窗口
  std::shared_ptr<opengl::Texture2D> _p_sprite;  // 按钮图标
  ButtonCallBack                     _callback;  // 回调函数
};

}  // namespace widget
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_WIDGET_BUTTON_H__
