// bricks.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2021 Mar 01
// License: GPL.v3

#include "include/snake/bricks.h"

#include <common/math.h>
#include <graphics/sprite.h>

#include "include/common.h"

namespace minigames {
namespace snake {

Brick::Brick(const vec2& pos, const vec2& size, int row, int column)
    : _pos(pos),
      _size(size),
      _hitbox{pos, pos + vec2(_size.x, 0), pos + _size, pos + vec2(0, _size.y)},
      _row(row),
      _column(column) {}

void Brick::Draw(std::shared_ptr<SpriteRender> render) {
  vec2 size(_size.x / _column, _size.y / _row);
  for (int i = 0; i < _row; ++i) {
    for (int j = 0; j < _column; ++j) {
      render->DrawSprite(
          _sprite, vec2(_pos.x + (j * size.x), _pos.y + i * size.y), size);
    }
  }
}

}  // namespace snake
}  // namespace minigames
