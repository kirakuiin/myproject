// gameobj.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 15
// License: GPL.v3

#include "include/gameobj.h"

#include <common/time.h>
#include <physics/collision.h>

namespace minigames {

void GameObj::Update() {
  _curr_time = easy_engine::Now<>();
  if (_prev_time == 0.0f) {
    _prev_time = _curr_time;
  }
  _delta_time += (_curr_time - _prev_time);
  _prev_time = _curr_time;
  while (_delta_time >= _frame_rate) {
    IntervalUpdate();
    _delta_time -= _frame_rate;
  }
}

CollisionResult GameObj::IsCollision(GameObj &rhs) {
  return easy_engine::physics::CheckCollision(GetHitBox(), rhs.GetHitBox());
}

}  // namespace minigames
