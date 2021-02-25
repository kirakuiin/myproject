// game.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Oct 09
// License: GPL.v3

#ifndef __CPP_MINIGAMES_INCLUDE_GAME_H__
#define __CPP_MINIGAMES_INCLUDE_GAME_H__

namespace minigames {

// 游戏对象基类
class GameInterface {
 public:
  virtual ~GameInterface() {}

  // 初始化游戏内容
  virtual void Init() = 0;

  // 处理输入
  virtual void ProcessInput() = 0;

  // 更新游戏对象
  virtual void Update() = 0;

  // 渲染游戏对象
  virtual void Draw() = 0;

  // 判断游戏是否正在运行
  //
  // @return bool: 游戏是否正在运行
  virtual bool IsRunning() const = 0;
};

}  // namespace minigames
#endif  // __CPP_MINIGAMES_INCLUDE_GAME_H__
