// snake_game.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Oct 09
// License: GPL.v3

#ifndef __MINIGAMES_INCLUDE_SNAKE_SNAKE_GAME_H__
#define __MINIGAMES_INCLUDE_SNAKE_SNAKE_GAME_H__

#include <memory>

#include "include/game.h"
#include "include/snake/bricks.h"

namespace easy_engine {
namespace graphics {
class Camera2D;
class SpriteRender;
class Font;
}  // namespace graphics
namespace widget {
class Window;
}  // namespace widget
}  // namespace easy_engine

namespace minigames {
namespace snake {

class Snake;
class NormalEgg;

// 贪吃蛇游戏
class SnakeGame : public GameInterface {
 public:
  explicit SnakeGame(std::shared_ptr<easy_engine::widget::Window>,
                     std::shared_ptr<easy_engine::graphics::Camera2D>,
                     std::shared_ptr<easy_engine::graphics::SpriteRender>);
  ~SnakeGame();

  void Init() override;

  void ProcessInput() override;

  void Update() override;

  void Draw() override;

  bool IsRunning() const override;

 private:
  bool CheckCollision() const;  // 检测蛇头是否发生碰撞

  void InitBricks();  // 初始化墙体设置

  bool _is_running;   // 游戏是否正在运行
  bool _is_alive;     // 蛇是否存活
  int  _scr_height;   //屏幕高度
  int  _scr_width;    // 屏幕宽度
  int  _snake_speed;  // 蛇的绝对速度

  std::shared_ptr<easy_engine::widget::Window>     _p_window;  // 窗口指针
  std::shared_ptr<easy_engine::graphics::Camera2D> _p_camera;  // 摄像机
  std::shared_ptr<easy_engine::graphics::SpriteRender> _p_render;  // 渲染器
  std::shared_ptr<easy_engine::graphics::Font>         _p_font;  // 字体指针

  std::shared_ptr<Snake>              _p_snake;    // 蛇指针
  std::shared_ptr<NormalEgg>          _p_egg;      // 蛋指针
  std::vector<std::shared_ptr<Brick>> _vp_bricks;  // 墙指针数组
};

}  // namespace snake
}  // namespace minigames

#endif  // __MINIGAMES_INCLUDE_SNAKE_SNAKE_GAME_H__
