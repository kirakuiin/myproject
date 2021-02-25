// common.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 14
// License: GPL.v3

#include "include/common.h"

namespace minigames {

vec2 GetUnitVecFromDir(Direction dir) {
  switch (dir) {
    case Direction::UP:
      return vec2(0, -1);
    case Direction::LEFT:
      return vec2(-1, 0);
    case Direction::DOWN:
      return vec2(0, 1);
    case Direction::RIGHT:
      return vec2(1, 0);
  }
}

Direction GetDirFromVec(const vec2& vec) {
  if (vec.x > 0) {
    return Direction::RIGHT;
  } else if (vec.x < 0) {
    return Direction::LEFT;
  } else if (vec.y > 0) {
    return Direction::DOWN;
  } else {
    return Direction::UP;
  }
}

}  // namespace minigames
