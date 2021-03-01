// egg.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 16
// License: GPL.v3

#include "include/snake/egg.h"

#include <common/math.h>
#include <graphics/sprite.h>

#include "include/common.h"
#include "include/snake/snake.h"

namespace minigames {
namespace snake {

NormalEgg::NormalEgg(const vec2& left_top, const vec2& right_bottom,
                     const vec2& size, Snake* p_snake)
    : _left_top(left_top),
      _right_bottom(right_bottom),
      _size(size),
      _hitbox{_left_top, _left_top + vec2(size.x, 0), _left_top + size,
              _left_top + vec2(0, size.y)} {
  RespawnEgg(p_snake);
}

void NormalEgg::Draw(std::shared_ptr<SpriteRender> render) {
  render->DrawSprite(_sprite, _pos, _size);
}

void NormalEgg::Eaten(Snake* p_snake) {
  p_snake->Grow();
  RespawnEgg(p_snake);
}

void NormalEgg::RespawnEgg(Snake* p_snake) {
  vec2 range = _right_bottom - _left_top - _size;

  easy_engine::Random<int> rand(0, easy_engine::max(range.x, range.y));

  while (true) {
    vec2 pos(_left_top.x + rand() % (int)(range.x),
             _left_top.y + rand() % (int)(range.y));
    _hitbox = PolygonBox{pos, pos + vec2(_size.x, 0), pos + _size,
                         pos + vec2(0, _size.y)};
    if (p_snake->CheckEggPos(this)) {
      _pos = pos;
      break;
    }
  }
}

}  // namespace snake
}  // namespace minigames
