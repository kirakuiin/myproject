// mainpage.h - 主页面
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 19
// License: GPL.v3

#ifndef __CPP_MINIGAMES_INCLUDE_MAINPAGE_H__
#define __CPP_MINIGAMES_INCLUDE_MAINPAGE_H__

#include <graphics/animation.h>
#define __EASY_ENGINE_WIDGET__
#include <easy_engine.h>

#include <memory>

#include "config.h"
#include "game.h"
#include "include/gameobj.h"

namespace minigames {

using easy_engine::graphics::Animation;
using easy_engine::graphics::AnimationRender;
using easy_engine::graphics::Camera2D;
using easy_engine::widget::Button;
using easy_engine::widget::Widget;
using easy_engine::widget::WidgetContainer;
using easy_engine::widget::Window;

class MainPage final {
 public:
  explicit MainPage(std::shared_ptr<Window> window);

  // 绘制主界面
  //
  void Draw();

  // 更新主界面
  //
  void Update();

 private:
  // 初始化界面
  void Init();

  // 初始化开场动画
  void InitAnimation();

  // 初始化控件
  void InitWidget();

  // 初始化初始界面
  void InitStartPage();

  // 初始化游戏界面
  void InitGamePage();

  std::shared_ptr<Window>          _p_window;        // 窗口
  std::shared_ptr<Camera2D>        _p_camera;        // 摄像机
  std::shared_ptr<SpriteRender>    _p_render;        // 渲染器
  std::shared_ptr<AnimationRender> _p_anime_render;  // 动画渲染器
  std::shared_ptr<MiniCfg>         _p_config;        // 配置器
  std::shared_ptr<WidgetContainer> _p_widgets;       // 组件容器
  std::shared_ptr<Animation>       _p_loading;       // 过场动画
  std::shared_ptr<Animation>       _p_menu;          // 菜单动画
  std::shared_ptr<GameInterface>   _p_game;          // 当前游戏
  WidgetContainer*                 _p_current_page;  // 当前界面
};
}  // namespace minigames

#endif  // __CPP_MINIGAMES_INCLUDE_MAINPAGE_H__
