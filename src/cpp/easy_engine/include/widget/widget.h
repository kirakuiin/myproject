// widget.h - 控件基类
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 28
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_WIDGET_WIDGET_H__
#define __EASY_ENGINE_INCLUDE_WIDGET_WIDGET_H__

#include "../graphics/sprite.h"

namespace easy_engine {
namespace widget {

// 组件基类
class Widget {
 public:
  virtual ~Widget() {}

  // 更新组件
  virtual void Update() = 0;

  // 画出组件
  //
  // @param render: 渲染器
  virtual void Draw(std::shared_ptr<graphics::SpriteRender> render) = 0;

  // 启用组件
  virtual void Enable() = 0;

  // 禁用组件
  virtual void Disable() = 0;
};

}  // namespace widget
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_WIDGET_WIDGET_H__
