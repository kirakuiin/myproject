// bricks.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2021 Feb 26
// License: GPL.v3

#ifndef __MINIGAMES_INCLUDE_SNAKE_BRICKS_H__
#define __MINIGAMES_INCLUDE_SNAKE_BRICKS_H__

#include <physics/collision.h>

#include "include/gameobj.h"

namespace easy_engine {
namespace opengl {
class Texture2D;
}  // namespace opengl
}  // namespace easy_engine

namespace minigames {
namespace snake {

using easy_engine::vec2;
using easy_engine::opengl::Texture2D;
using easy_engine::physics::PolygonBox;

// 蛋基类
class Brick : public GameObj {
 public:
  // @param pos: 砖位置
  // @param size: 砖大小
  // @param row: 需要贴图的行数
  // @param column: 需要贴图的列数
  Brick(const vec2& pos, const vec2& size, int row = 1, int column = 1);

  virtual ~Brick() {}

  void Draw(std::shared_ptr<SpriteRender> render) override;

  HitBox& GetHitBox() override { return _hitbox; }

  void SetTexture(std::shared_ptr<Texture2D> brick) { _sprite = brick; }

 private:
  vec2       _pos;     // 墙位置
  vec2       _size;    // 大小
  PolygonBox _hitbox;  // 碰撞盒
  int        _row;     // 贴图行数
  int        _column;  // 贴图列数

  std::shared_ptr<Texture2D> _sprite;
};

}  // namespace snake
}  // namespace minigames

#endif  // __MINIGAMES_INCLUDE_SNAKE_BRICKS_H__
