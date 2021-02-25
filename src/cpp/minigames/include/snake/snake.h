// snake.h - 蛇对象
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 08
// License: GPL.v3

#ifndef __MINIGAMES_INCLUDE_SNAKE_SNAKE_H__
#define __MINIGAMES_INCLUDE_SNAKE_SNAKE_H__

#include <physics/collision.h>

#include <queue>

#include "../gameobj.h"

namespace easy_engine {
namespace opengl {
class Texture2D;
}  // namespace opengl
}  // namespace easy_engine

namespace minigames {
namespace snake {

class Egg;
using easy_engine::vec2;
using easy_engine::opengl::Texture2D;
using easy_engine::physics::PolygonBox;

// 蛇的身体组件
class SnakeComponent : public GameObj {
 public:
  SnakeComponent(const vec2& pos, const vec2& size, const vec2& velocity);
  ~SnakeComponent() {}

  void Update() override;

  void Draw(std::shared_ptr<SpriteRender> render) override;

  HitBox& GetHitBox() override { return _hitbox; }

  // 设置速度向量
  //
  // @param velocity: 速度向量
  void SetVelocity(const vec2& velocity);

  // 获得速度向量
  //
  // @return vec2: 速度向量
  vec2 GetVelocity() const { return _velocity; }

  // 获得位置
  //
  // @return vec2: 位置
  vec2 GetPos() const { return _pos; }

  // 设置部件精灵
  //
  // @param sprite: 部件贴图
  void SetTexture(std::shared_ptr<Texture2D> sprite) { _sprite = sprite; }

 private:
  vec2       _pos;       // 位置
  vec2       _size;      // 大小
  vec2       _velocity;  // 速度向量
  float      _rot;       // 旋转角度
  PolygonBox _hitbox;    // 碰撞盒

  std::shared_ptr<Texture2D> _sprite;  // 精灵
};

class Snake : public GameObj {
 public:
  Snake(const vec2& pos, const vec2& size, const vec2& velocity);
  ~Snake() {}

  void Draw(std::shared_ptr<SpriteRender> render) override;

  HitBox& GetHitBox() override { return _v_body.back().GetHitBox(); }

  // 检查自身碰撞
  //
  // @return bool: 是否发生自碰撞
  bool CheckSelfCollision();

  // 设置蛇精灵
  //
  // @param head: 头贴图
  // @param body: 躯体贴图
  // @param tail: 尾巴贴图
  void SetTexture(std::shared_ptr<Texture2D> head,
                  std::shared_ptr<Texture2D> body,
                  std::shared_ptr<Texture2D> tail) {
    _body_sprite = body;
    _v_body[2].SetTexture(head);
    _v_body[1].SetTexture(body);
    _v_body[0].SetTexture(tail);
  }

  // 设置速度向量
  //
  // @param velocity: 速度向量
  void SetVelocity(const vec2& velocity);

  // 蛇吃蛋
  //
  // @param egg: 被吃掉的蛋
  void Eat(Egg* egg);

  // 增长身体
  //
  void Grow();

  // 检测是否碰撞到身体的任一部分
  bool CheckEggPos(Egg* egg);

 private:
  void IntervalUpdate() override;

  // 更新速度队列
  //
  // @param velocity: 新躯体的速度
  void UpdateVecQueue(const vec2& velocity);

  // 蛇头位于末尾, 蛇尾位于开始
  std::vector<SnakeComponent> _v_body;

  std::deque<vec2> _q_velocity;  // 速度队列
  std::deque<int>  _q_index;     // 蛇身在队列中索引

  std::shared_ptr<Texture2D> _body_sprite;  // 蛇身精灵
  vec2                       _body_size;    // 蛇身大小
};

}  // namespace snake
}  // namespace minigames

#endif  // __MINIGAMES_INCLUDE_SNAKE_SNAKE_H__
