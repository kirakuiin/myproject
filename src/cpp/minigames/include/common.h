// common.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 14
// License: GPL.v3

#ifndef __CPP_MINIGAMES_INCLUDE_COMMON_H__
#define __CPP_MINIGAMES_INCLUDE_COMMON_H__

#include <common/const.h>
#include <common/format.h>
#include <common/math.h>
#include <utility/log.h>

#include <string>

namespace minigames {

using easy_engine::Direction;
using easy_engine::Format;
using easy_engine::vec2;

// 根据方向计算单位向量
//
// @param dir: 方向
// @return vec2: 方向单位向量
vec2 GetUnitVecFromDir(Direction dir);

// 根据向量计算方向
//
// @param vec: 向量
// @param Direction: 方向向量
Direction GetDirFromVec(const vec2& vec);

}  // namespace minigames

#endif  // __CPP_MINIGAMES_INCLUDE_COMMON_H__
