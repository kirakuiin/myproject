// camera.cc - 摄像机实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 15
// License: GPL.v3

#include "include/graphics/camera.h"

namespace easy_engine {
namespace graphics {

void Camera2D::CalcProjection() {
  // 由于屏幕坐标在y轴上是相反的, 因此顶底互换
  _projection = ortho(static_cast<float>(_tl_pos.x),
                      static_cast<float>(_tl_pos.x + _size.x),
                      static_cast<float>(_tl_pos.y),
                      static_cast<float>(_tl_pos.y - _size.y), -1.0f, 1.0f);
}

}  // namespace graphics
}  // namespace easy_engine
