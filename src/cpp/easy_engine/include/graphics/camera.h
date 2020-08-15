// camera.h - 摄像机
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 15
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_GRAPHICS_CAMERA_H__
#define __EASY_ENGINE_INCLUDE_GRAPHICS_CAMERA_H__

#include "../common/const.h"
#include "../common/math.h"

namespace easy_engine {
namespace graphics {

// 2d俯视摄像机
// 本质上是根据制定参数生成一个正射投影矩阵
class Camera2D final {
 public:
  // @param pos: 摄像机左上角坐标
  // @param size: 摄像机高度和宽度
  Camera2D(const vec2& pos, const vec2& size) : _tl_pos(pos), _size(size) {
    CalcProjection();
  }

  // 移动摄像机到制定坐标
  // @param pos: 新的摄像机左上角坐标
  void Move(const vec2& pos) {
    _tl_pos = pos;
    CalcProjection();
  }

  // 设置摄像机的宽度和高度
  // @param size: 新的宽度和高度
  void SetSize(const vec2& size) {
    _size = size;
    CalcProjection();
  }

  // 得到投影矩阵
  // @return mat4: 投影矩阵
  mat4 GetProjectionMatrix() const { return _projection; }

 private:
  // 重新计算投影矩阵
  void CalcProjection() {
    // 由于屏幕坐标在y轴上是相反的, 因此顶底互换
    _projection = ortho(static_cast<float>(_tl_pos.x),
                        static_cast<float>(_tl_pos.x + _size.x),
                        static_cast<float>(_tl_pos.y),
                        static_cast<float>(_tl_pos.y - _size.y), -1.0f, 1.0f);
  }

  ivec2 _tl_pos;      // 摄像机左上角坐标
  ivec2 _size;        // 摄像机宽度和高度
  mat4  _projection;  // 投影矩阵
};

}  // namespace graphics
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_GRAPHICS_CAMERA_H__
