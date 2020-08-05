// gameobj.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 28
// License: GPL.v3

#ifndef __PRACTICE_BREAKOUT_INCLUDE_GAMEOBJ_H__
#define __PRACTICE_BREAKOUT_INCLUDE_GAMEOBJ_H__

#include <tuple>
#include "render.h"

class GameObject {
  public:
    GameObject() {}
    GameObject(vec2 pos, vec2 size, shared_ptr<Texture2D> text,
               vec3 color = vec3(1.0f), vec2 velocity = vec2(0.0f));

    virtual ~GameObject() {}

    virtual void Draw(shared_ptr<SpriteRender> render);

    vec2 Position;
    vec2 Size;
    vec2 Velocity;
    vec3 Color;
    float Rotation = 0.0f;
    bool IsSolid = false;
    bool Destroyed = false;

    shared_ptr<Texture2D> Sprite;
};

class BallObject: public GameObject {
  public:
    BallObject(): GameObject() {}
    BallObject(vec2 pos, shared_ptr<Texture2D> text,
               float radius, vec2 velocity);

    vec2 Move(float delta, unsigned window_width);
    void Reset(vec2 position, vec2 velocity);

    float Radius = 0.0f;
    bool  IsStuck = true;
    bool  PassThrough = false;
    bool  Sticky = false;
    // 用来防止一帧内碰到两个对齐物体导致穿透的问题
    bool  XRevert = false;
    bool  YRevert = false;
    bool  Stop = false;
};

class PowerUp: public GameObject {
  public:
    PowerUp(vec2 position, shared_ptr<Texture2D> sprite,
            const std::string& type, vec3 color, float duration)
        : GameObject(position, vec2(60, 20), sprite, color, vec2(0.0f, 150.0f)),
          Type(type), Duration(duration), Activated(false) {
    }

    std::string Type;
    float Duration;
    bool Activated;
};

class GameLevel {
  public:
    GameLevel() {}

    // 从文件中加载关卡
    void Load(const std::string& filepath,
              unsigned int level_width, unsigned int level_height);

    // 绘制关卡
    void Draw(shared_ptr<SpriteRender> render);

    // 检查关卡是否完成
    bool IsComplete();

    std::vector<GameObject> Bricks;

  private:
    void Init(const std::vector<std::vector<int>>& tile_data,
              unsigned int level_width, unsigned int level_height);
};

enum class Direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT,
};

using Collision = std::tuple<bool, Direction, vec2>;

// 碰撞检测函数
bool CheckIn(const GameObject& one, const GameObject& two);
bool CheckCollision(const GameObject& one, const GameObject& two);
Collision CheckCollision(BallObject& one, const GameObject& two);
Direction VectorDirection(vec2 target);

#endif // __PRACTICE_BREAKOUT_INCLUDE_GAMEOBJ_H__
