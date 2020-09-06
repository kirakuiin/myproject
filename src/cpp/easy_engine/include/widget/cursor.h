// cursor.h - 光标
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 30
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_WIDGET_CURSOR_H__
#define __EASY_ENGINE_INCLUDE_WIDGET_CURSOR_H__

#include <memory>

#include "../physics/collision.h"
#include "widget.h"
#include "window.h"

namespace easy_engine {
namespace widget {

// 光标类
class Cursor : public Widget {
 public:
  Cursor(std::shared_ptr<Window> window, const vec2& pos, const vec2& size,
         std::shared_ptr<opengl::Texture2D> icon);
  ~Cursor() {}

  void Update() override;
  void Draw(std::shared_ptr<graphics::SpriteRender> render) override;
  void Enable() override;
  void Disable() override;

  // 修改光标图标
  //
  // @param icon: 新的光标图标
  void ChangeCursorIcon(std::shared_ptr<opengl::Texture2D> icon) {
    _p_sprite = icon;
  }

 private:
  vec2 _cursor_pos;   // 按钮位置
  vec2 _size;         // 光标大小
  bool _enable;       // 光标是否启用
  int  _cursor_mode;  // 光标状态

  std::shared_ptr<Window>            _p_window;  // 窗口
  std::shared_ptr<opengl::Texture2D> _p_sprite;  // 光标图标
};

}  // namespace widget
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_WIDGET_CURSOR_H__
