// container.h - 组件容器
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 28
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_WIDGET_CONTAINER_H__
#define __EASY_ENGINE_INCLUDE_WIDGET_CONTAINER_H__

#include <map>
#include <memory>

#include "widget.h"

namespace easy_engine {
namespace widget {

// 组件容器
class WidgetContainer : public Widget {
 public:
  WidgetContainer() : _enable(true) {}
  ~WidgetContainer() {}

  void Update() override;
  void Draw(std::shared_ptr<graphics::SpriteRender> render) override;
  void Enable() override { _enable = true; }
  void Disable() override { _enable = false; }

  // 向容器中添加组件
  //
  // @param widget_name: 组件名称
  // @param widget: 组件指针
  void Add(const std::string& widget_name, std::shared_ptr<Widget> widget) {
    _m_widgets[widget_name] = widget;
  }

  // 移除组件中容器, 如无此组件无事发生
  //
  // @param widget_name: 组件名称
  void Remove(const std::string& widget_name) {
    if (_m_widgets.count(widget_name)) {
      _m_widgets.erase(widget_name);
    }
  }

 private:
  bool                                           _enable;  // 是否启用组件
  std::map<std::string, std::shared_ptr<Widget>> _m_widgets;  // 组件容器
};

}  // namespace widget
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_WIDGET_CONTAINER_H__
