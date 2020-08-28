// container.cc - 容器实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 28
// License: GPL.v3

#include "include/widget/container.h"

namespace easy_engine {
namespace widget {

void WidgetContainer::Update() {
  if (!_enable) {
    return;
  }
  for (auto& widget : _m_widgets) {
    widget.second->Update();
  }
}

void WidgetContainer::Draw(std::shared_ptr<graphics::SpriteRender> render) {
  if (!_enable) {
    return;
  }
  for (auto& widget : _m_widgets) {
    widget.second->Draw(render);
  }
}

}  // namespace widget
}  // namespace easy_engine
