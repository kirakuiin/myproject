// gameobj.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 08
// License: GPL.v3

#ifndef __CPP_MINIGAMES_INCLUDE_GAMEOBJ_H__
#define __CPP_MINIGAMES_INCLUDE_GAMEOBJ_H__

#include <common/format.h>
#include <utility/log.h>

#include <memory>

namespace easy_engine {
namespace graphics {
class SpriteRender;
}  // namespace graphics
namespace physics {
struct HitBox;
struct CollisionResult;
}  // namespace physics
}  // namespace easy_engine

namespace minigames {

using easy_engine::Format;
using easy_engine::graphics::SpriteRender;
using easy_engine::physics::CollisionResult;
using easy_engine::physics::HitBox;

// 所有游戏对象的基类
class GameObj {
 public:
  explicit GameObj(double frame_rate = 1.0 / 60.0)
      : _prev_time(0), _curr_time(0), _delta_time(0), _frame_rate(frame_rate){};
  virtual ~GameObj() {}

  // 绘制游戏对象
  //
  // @param render: 精灵渲染对象
  virtual void Draw(std::shared_ptr<SpriteRender> render) = 0;

  // 获得对象的碰撞盒
  //
  // @return HitBox: 碰撞盒
  virtual HitBox& GetHitBox() = 0;

  // 更新游戏对象状态
  virtual void Update();

  // 更新游戏对象内部状态
  virtual void IntervalUpdate() {}

  // 检查两个对象是否碰撞
  //
  // @param rhs: 另一个对象
  // @return CollisionResult: 碰撞结果
  CollisionResult IsCollision(GameObj& rhs);

 private:
  double _prev_time;   // 上次播放时间
  double _curr_time;   // 当前时间
  double _delta_time;  // 间隔时间
  double _frame_rate;  // 桢率
};

}  // namespace minigames

#endif  // __CPP_MINIGAMES_INCLUDE_GAMEOBJ_H__
