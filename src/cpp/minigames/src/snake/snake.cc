// snake.cc - 蛇类型实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 11
// License: GPL.v3

#include "include/snake/snake.h"

#include <common/math.h>
#include <common/time.h>
#include <graphics/sprite.h>

#include "include/common.h"
#include "include/snake/egg.h"

#define __EASY_ENGINE_UTILITY__
#include <easy_engine.h>

using easy_engine::utility::Logging;

namespace minigames {
namespace snake {

using easy_engine::Format;
using easy_engine::Rect;

SnakeComponent::SnakeComponent(const vec2& pos, const vec2& size,
                               const vec2& velocity)
    : _pos(pos),
      _size(size),
      _velocity(0),
      _rot(0),
      _hitbox{pos, pos + vec2(size.x, 0), pos + size, pos + vec2(0, size.y)} {
  SetVelocity(velocity);
}

void SnakeComponent::Update() {
  _pos += _velocity;
  _hitbox.Translate(_velocity);
}

void SnakeComponent::Draw(std::shared_ptr<SpriteRender> render) {
  render->DrawSprite(_sprite, _pos, _size, _rot);
}

void SnakeComponent::SetVelocity(const vec2& velocity) {
  if (easy_engine::dot(velocity, _velocity) < 0) {
    return;
  }
  if (GetDirFromVec(_velocity) != GetDirFromVec(velocity)) {
    float new_rot = ((int)GetDirFromVec(velocity) - (int)Direction::UP) * 90.0f;
    float angle   = easy_engine::radians(new_rot - _rot);
    _rot          = easy_engine::radians(new_rot);
    vec2 center   = _hitbox.GetCenter();
    for (auto& vert : _hitbox.Vertices) {
      vert = easy_engine::rotate(vert, angle, center);
    }
  }
  _velocity = velocity;
}

Snake::Snake(const vec2& pos, const vec2& size, const vec2& velocity)
    : _body_size(size), _eated_egg(0) {
  for (int i = 2; i >= 0; --i) {
    _v_body.emplace_back(
        pos - easy_engine::normalize(velocity) * size * (float)i, size,
        velocity);
  }
  UpdateVecQueue(velocity);  // update tail
  UpdateVecQueue(velocity);  // update body
}

void Snake::Draw(std::shared_ptr<SpriteRender> render) {
  for (auto& iter : _v_body) {
    iter.Draw(render);
  }
}

void Snake::SetVelocity(const vec2& velocity) {
  auto& head = _v_body.back();
  head.SetVelocity(velocity);
}

void Snake::Eat(Egg* egg) {
  egg->Eaten(this);
  ++_eated_egg;
}

bool Snake::CheckSelfCollision() {
  // check body collision
  for (int i = 0; i < _v_body.size() - 2; ++i) {
    if (easy_engine::physics::CheckCollision(GetHitBox(),
                                             _v_body[i].GetHitBox())
            .IsIntersect) {
      return true;
    }
  }
  // check second section
  SnakeComponent& head   = _v_body.back();
  SnakeComponent& second = _v_body[_v_body.size() - 2];
  if (easy_engine::physics::CheckCollision(head.GetHitBox(), second.GetHitBox())
          .IsIntersect &&
      (0 > easy_engine::dot(head.GetVelocity(), second.GetVelocity()))) {
    return true;
  }
  return false;
}

void Snake::Grow() {
  auto& head     = _v_body.back();
  vec2  pos      = head.GetPos();
  vec2  velocity = head.GetVelocity();
  // 将蛇头移动一节
  head.SetVelocity(_body_size * easy_engine::normalize(velocity));
  head.Update();
  head.SetVelocity(velocity);
  // 生成蛇身
  SnakeComponent new_body(pos, _body_size, velocity);
  new_body.SetTexture(_body_sprite);
  _v_body.insert(_v_body.end() - 1, new_body);
  UpdateVecQueue(velocity);
}

bool Snake::CheckEggPos(Egg* egg) {
  for (auto& iter : _v_body) {
    if (easy_engine::physics::CheckCollision(iter.GetHitBox(), egg->GetHitBox())
            .IsIntersect) {
      return false;
    }
  }
  return true;
}

void Snake::IntervalUpdate() {
  for (int i = 0; i < _v_body.size() - 1; ++i) {
    _v_body[i].SetVelocity(_q_velocity[_q_index[i]]);
    _v_body[i].Update();
  }
  auto& head = _v_body.back();
  head.Update();
  _q_velocity.push_front(head.GetVelocity());
  _q_velocity.pop_back();
}

void Snake::UpdateVecQueue(const vec2& velocity) {
  float len =
      easy_engine::length(_body_size * easy_engine::normalize(velocity));
  float vec = easy_engine::length(velocity);
  for (int i = 0; i < len / vec; ++i) {
    _q_velocity.push_front(velocity);
  }
  _q_index.push_front(_q_velocity.size() - 1);
}

}  // namespace snake
}  // namespace minigames
