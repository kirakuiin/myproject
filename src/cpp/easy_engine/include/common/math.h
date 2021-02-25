// math.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 15
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_COMMON_MATH_H__
#define __EASY_ENGINE_INCLUDE_COMMON_MATH_H__

#include <random>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace easy_engine {
// 常用数据类型
using glm::fvec2;
using glm::ivec2;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

// 常用的矩阵运算
using glm::abs;
using glm::ceil;
using glm::clamp;
using glm::cos;
using glm::cross;
using glm::dot;
using glm::floor;
using glm::inverse;
using glm::length;
using glm::max;
using glm::min;
using glm::normalize;
using glm::ortho;
using glm::pow;
using glm::radians;
using glm::rotate;
using glm::round;
using glm::scale;
using glm::sin;
using glm::sqrt;
using glm::tan;
using glm::translate;
using glm::transpose;

// 获取数据
using glm::value_ptr;

// 生成左闭右开区间内的随机数, 默认生成浮点数
template <typename T = float>
class Random {
 public:
  // @param left: 左闭区间
  // @param right: 右开区间
  Random(const double& left, const double& right) : _generator(left, right) {
    std::random_device rd;
    _engine.seed(rd());
  }

  // 返回一个生成的随机数
  // @return T: 返回的随机数
  T operator()() { return static_cast<T>(_generator(_engine)); }

 private:
  std::uniform_real_distribution<> _generator;  // 随机数生成器
  std::default_random_engine       _engine;     // 默认随机引擎
};

// 向量比较仿函数
//
// @param lhs: 操作数
// @param rhs: 操作数
// @return bool: 如果lhs < rhs
template <typename T>
struct VecLess {
  bool operator()(T& lhs, T& rhs) const;
};

// 二维向量比较函数
template <>
struct VecLess<vec2> {
  bool operator()(const vec2& lhs, const vec2& rhs) const {
    if (lhs.x < rhs.x) {
      return true;
    } else if (lhs.x == rhs.x && lhs.y < rhs.y) {
      return true;
    } else {
      return false;
    }
  }
};

// 平行于坐标轴矩形(AABB)
struct Rect {
  Rect() {}
  // @param top_left: 矩形左上角位置
  // @param size: 矩形的宽和高
  Rect(const vec2 top_left, const vec2 size)
      : Pos(top_left), Size(size), EndPos(Pos.x + Size.x, Pos.y + Size.y) {}

  // 判断两个矩形是否相交
  //
  // @param other: 另一个矩形
  // @return bool: 是否包含
  bool Intersect(const Rect& other) const {
    bool x = (Pos.x <= other.EndPos.x) && (other.Pos.x <= EndPos.x);
    bool y = (Pos.y <= other.EndPos.y) && (other.Pos.y <= EndPos.y);
    return x && y;
  }

  // 判断此矩形是否包含另一个矩形
  //
  // @param other: 另一个矩形
  // @return bool: 是否包含
  bool Contain(const Rect& other) const {
    bool x = (Pos.x <= other.Pos.x) && (other.EndPos.x <= EndPos.x);
    bool y = (Pos.y <= other.Pos.y) && (other.EndPos.y <= EndPos.y);
    return x && y;
  }

  vec2 Pos;     // 矩形左上角位置
  vec2 Size;    // 矩形大小
  vec2 EndPos;  // 矩形右上角位置
};

// 矩形比较函数
struct RectLess {
  bool operator()(const Rect& lhs, const Rect& rhs) {
    if (VecLess<vec2>()(lhs.Pos, rhs.Pos)) {
      return true;
    } else if (lhs.Pos == rhs.Pos && VecLess<vec2>()(lhs.EndPos, rhs.EndPos)) {
      return true;
    } else {
      return false;
    }
  }
};

// 计算二元向量的叉乘
// @param lhs: 向量
// @param rhs: 向量
// @return float: 叉乘结果
inline float cross(const vec2& lhs, const vec2& rhs) {
  return lhs.x * rhs.y - rhs.x * lhs.y;
}

// 计算二元向量旋转
//
// @param v: 被旋转向量
// @param angle: 旋转角度(弧度)
// @param o: 旋转中心
// @return vec2: 旋转结果
inline vec2 rotate(const vec2& v, float angle, const vec2& o) {
  vec2 src(v - o);
  return vec2(src.x * cos(angle) - src.y * sin(angle),
              src.x * sin(angle) + src.y * cos(angle)) +
         o;
}

}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_COMMON_MATH_H__
