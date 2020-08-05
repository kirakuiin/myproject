// game.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 27
// License: GPL.v3

#ifndef __PRACTICE_BREAKOUT_INCLUDE_GAME_H__
#define __PRACTICE_BREAKOUT_INCLUDE_GAME_H__

#include <irrKlang.h>
#include "include/font.h"

#include "render.h"
#include "gameobj.h"
#include "particle.h"
#include "post.h"

enum class GameState {
    GAME_ACTIVE = 0,
    GAME_MENU,
    GAME_WIN,
    GAME_EXIT,
};

class Game
{
  public:
    // 以屏幕大小为参数初始化
    Game(unsigned int width, unsigned int height);
    ~Game();

    // 初始化
    void Init();
    // 处理键盘输入
    void ProcessInput(float delta);
    // 更新对象信息
    void Update(float delta);
    // 处理碰撞
    void DoCollisions();
    // 渲染
    void Render();

    GameState       state;
    bool            keys[1024];
    bool            keys_just_rel[1024];
    unsigned int    width;
    unsigned int    height;

  private:
    // 重置游戏
    void ResetGame();
    // 重置场景
    void ResetLevel();
    // 重置挡板
    void ResetPaddle();
    // 重置道具
    void ResetPowerUps();
    // 重置变量
    void ResetVar();
    // 刷新道具
    void SpawnPowerUps(GameObject& block);
    // 激活道具
    void ActivatePowerUp(PowerUp& powerup);
    // 更新道具
    void UpdatePowerUps(float delta);
    // 检查其他道具状态
    bool IsOtherPowerUpActivate(PowerUp& current);
    // 是否胜利
    bool IsWin();

    shared_ptr<SpriteRender>    _srender;
    shared_ptr<irrklang::ISoundEngine> _sound_engine;
    shared_ptr<gl::Font>        _text;
    std::vector<GameLevel>      _levels;
    std::vector<PowerUp>        _powerups;
    unsigned int                _level;
    shared_ptr<GameObject>      _paddle;
    shared_ptr<BallObject>      _ball;
    shared_ptr<ParticleGenerator> _gene;
    shared_ptr<PostProcessor>   _effect;
    float                       _shake_time = 0.0f;
    int                         _lives = 3;
    int                         _score = 0;
    int                         _hit_num = 0;
    int                         _win_num = 0;
};

#endif // __PRACTICE_BREAKOUT_INCLUDE_GAME_H__
