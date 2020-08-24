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
  Random(const double& left, const double& right) : _generator(left, right) {}

  // 返回一个生成的随机数
  // @return T: 返回的随机数
  T operator()() { return static_cast<T>(_generator(_engine)); }

 private:
  std::uniform_real_distribution<> _generator;  // 随机数生成器
  std::default_random_engine       _engine;     // 默认随机引擎
};

}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_COMMON_MATH_H__
