// egg.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 16
// License: GPL.v3

#ifndef __MINIGAMES_INCLUDE_SNAKE_EGG_H__
#define __MINIGAMES_INCLUDE_SNAKE_EGG_H__

#include <physics/collision.h>

#include "include/gameobj.h"

namespace easy_engine {
namespace opengl {
class Texture2D;
}  // namespace opengl
}  // namespace easy_engine

namespace minigames {
namespace snake {

class Snake;
using easy_engine::vec2;
using easy_engine::opengl::Texture2D;
using easy_engine::physics::PolygonBox;

// 蛋基类
class Egg : public GameObj {
 public:
  virtual ~Egg() {}

  virtual void Eaten(Snake* snake) = 0;
};

// 普通蛋
class NormalEgg : public Egg {
 public:
  // @param left_top: 蛋生成区域左上角
  // @param right_bottom: 蛋生成区域右下角
  // @param size: 蛋大小
  // @param snake: 蛇对象
  NormalEgg(const vec2& left_top, const vec2& right_bottom, const vec2& size,
            Snake* snake);
  ~NormalEgg() {}

  void Draw(std::shared_ptr<SpriteRender> render) override;

  HitBox& GetHitBox() override { return _hitbox; }

  void Eaten(Snake* snake) override;

  void SetTexture(std::shared_ptr<Texture2D> egg) { _sprite = egg; }

 private:
  // 重新刷新一个蛋
  void RespawnEgg(Snake* snake);

  vec2       _left_top;      // 左上角位置
  vec2       _right_bottom;  // 右下角位置
  vec2       _pos;           // 蛋位置
  vec2       _size;          // 大小
  PolygonBox _hitbox;        // 碰撞盒

  std::shared_ptr<Texture2D> _sprite;
};

}  // namespace snake
}  // namespace minigames

#endif  // __MINIGAMES_INCLUDE_SNAKE_EGG_H__
