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
  for (auto& prior : _m_prior) {
    for (auto& index : _m_prior[prior.first]) {
      if (_m_widgets.count(index)) {
        _m_widgets[index]->Draw(render);
      }
    }
  }
}

void WidgetContainer::Add(const std::string&               widget_name,
                          std::shared_ptr<WidgetContainer> widget, int prior) {
  widget->SetParent(this);
  Add(widget_name, std::shared_ptr<Widget>(widget), prior);
}

}  // namespace widget
}  // namespace easy_engine
