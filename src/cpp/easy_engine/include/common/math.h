// math.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 15
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_COMMON_MATH_H__
#define __EASY_ENGINE_INCLUDE_COMMON_MATH_H__

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace easy_engine {
// 常用数据类型
using glm::ivec2;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

// 常用的矩阵运算
using glm::abs;
using glm::clamp;
using glm::cos;
using glm::cross;
using glm::dot;
using glm::inverse;
using glm::length;
using glm::max;
using glm::min;
using glm::normalize;
using glm::ortho;
using glm::pow;
using glm::radians;
using glm::rotate;
using glm::scale;
using glm::sin;
using glm::sqrt;
using glm::tan;
using glm::translate;
using glm::transpose;

// 获取数据
using glm::value_ptr;
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_COMMON_MATH_H__
