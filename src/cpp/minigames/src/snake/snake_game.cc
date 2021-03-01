// snake_game.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Oct 09
// License: GPL.v3

#include "include/snake/snake_game.h"

#include "include/common.h"
#include "include/snake/egg.h"
#include "include/snake/snake.h"

#define __EASY_ENGINE_OPENGL__
#define __EASY_ENGINE_UTILITY__
#define __EASY_ENGINE_GRAPHICS__
#define __EASY_ENGINE_WIDGET__
#include <easy_engine.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using easy_engine::widget::Window;

const int SNAKE_SPEED = 4;
const int BRICK_W     = 500;
const int BRICK_H     = 20;

namespace minigames {
namespace snake {

SnakeGame::SnakeGame(
    std::shared_ptr<easy_engine::widget::Window>         p_window,
    std::shared_ptr<easy_engine::graphics::Camera2D>     p_camera,
    std::shared_ptr<easy_engine::graphics::SpriteRender> p_render)
    : _is_running(false),
      _is_alive(true),
      _snake_speed(SNAKE_SPEED),
      _p_window(p_window),
      _p_camera(p_camera),
      _p_render(p_render) {}

SnakeGame::~SnakeGame() {}

void SnakeGame::Init() {
  // initial font
  _p_font.reset(new easy_engine::graphics::Font(_p_camera));
  _p_font->Load("resource/fonts/newyork.ttf");
  // get window size
  _p_window->GetWindowSize(&_scr_width, &_scr_height);
  easy_engine::opengl::DefaultFramebufferColor(0, 0, 0, 1);
  // initial resource
  easy_engine::utility::ResourceLocator::Image()->LoadImage(
      "resource/images/snake/head.png", "snake_head");
  easy_engine::utility::ResourceLocator::Image()->LoadImage(
      "resource/images/snake/body.png", "snake_body");
  easy_engine::utility::ResourceLocator::Image()->LoadImage(
      "resource/images/snake/tail.png", "snake_tail");
  easy_engine::utility::ResourceLocator::Image()->LoadImage(
      "resource/images/snake/egg.png", "egg");
  easy_engine::utility::ResourceLocator::Image()->LoadImage(
      "resource/images/snake/brick.png", "brick");
  easy_engine::utility::ResourceLocator::Image()->LoadImage(
      "resource/images/cursor.png", "cursor");
  // initial game object
  auto GetImage = [](const std::string& name) {
    return easy_engine::utility::ResourceLocator::Image()->GetImage(name);
  };
  _p_snake.reset(new Snake(vec2(_scr_width / 2, _scr_height / 2), vec2(20, 20),
                           vec2(-SNAKE_SPEED, 0)));
  _p_snake->SetTexture(GetImage("snake_head"), GetImage("snake_body"),
                       GetImage("snake_tail"));
  int       w_mid    = (_scr_width - BRICK_W) / 2;
  int       h_mid    = (_scr_height - BRICK_W) / 2;
  const int EGG_SIZE = 30;
  _p_egg.reset(new NormalEgg(vec2(w_mid + BRICK_H, h_mid + BRICK_H),
                             vec2(w_mid + BRICK_W - BRICK_H - EGG_SIZE,
                                  h_mid + BRICK_W - BRICK_H - EGG_SIZE),
                             vec2(EGG_SIZE), _p_snake.get()));
  _p_egg->SetTexture(GetImage("egg"));
  InitBricks();
  _is_running = true;
  DEBUG("Init ok");
}

void SnakeGame::ProcessInput() {
  if (_p_window->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
    _p_snake->SetVelocity(vec2(0, -_snake_speed));
  } else if (_p_window->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
    _p_snake->SetVelocity(vec2(0, _snake_speed));
  } else if (_p_window->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
    _p_snake->SetVelocity(vec2(-_snake_speed, 0));
  } else if (_p_window->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
    _p_snake->SetVelocity(vec2(_snake_speed, 0));
  } else if (_p_window->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    _is_running = false;
  } else if (_p_window->GetKey(GLFW_KEY_R) == GLFW_PRESS) {
    auto GetImage = [](const std::string& name) {
      return easy_engine::utility::ResourceLocator::Image()->GetImage(name);
    };
    _p_snake.reset(new Snake(vec2(_scr_width / 2, _scr_height / 2),
                             vec2(20, 20), vec2(-SNAKE_SPEED, 0)));
    _p_snake->SetTexture(GetImage("snake_head"), GetImage("snake_body"),
                         GetImage("snake_tail"));
    _is_alive = true;
  }
}

void SnakeGame::Update() {
  _p_snake->Update();
  _p_egg->Update();
  if (_p_snake->IsCollision(*_p_egg.get()).IsIntersect) {
    _p_snake->Eat(_p_egg.get());
  }
  if (CheckCollision() || _p_snake->GetEggNumber() == 100) {
    _is_alive = false;
  }
}

void SnakeGame::Draw() {
  if (_is_alive) {
    for (auto& iter : _vp_bricks) {
      iter->Draw(_p_render);
    }
    _p_snake->Draw(_p_render);
    _p_egg->Draw(_p_render);
    _p_font->Draw(easy_engine::Format("Score: %", _p_snake->GetEggNumber()),
                  vec2(_scr_width - 200, 40));
  } else {
    _p_font->Draw("Game Over", vec2(_scr_width / 2 - 100, _scr_height / 2),
                  easy_engine::vec3(1, 0, 0));
  }
}

bool SnakeGame::IsRunning() const { return _is_running; }

bool SnakeGame::CheckCollision() const {
  bool result = false;
  for (auto& iter : _vp_bricks) {
    result |= easy_engine::physics::CheckCollision(iter->GetHitBox(),
                                                   _p_snake->GetHitBox())
                  .IsIntersect;
  }
  return result | _p_snake->CheckSelfCollision();
}

void SnakeGame::InitBricks() {
  int scr_w = _scr_width;
  int scr_h = _scr_height;
  int w_mid = (scr_w - BRICK_W) / 2;
  int h_mid = (scr_h - BRICK_W) / 2;

  std::vector<std::pair<vec2, vec2>> v_pos_size = {
      std::pair<vec2, vec2>(vec2(w_mid, h_mid), vec2(BRICK_W, BRICK_H)),
      std::pair<vec2, vec2>(vec2(w_mid, scr_h - h_mid - BRICK_H),
                            vec2(BRICK_W, BRICK_H)),
      std::pair<vec2, vec2>(vec2(w_mid, h_mid), vec2(BRICK_H, BRICK_W)),
      std::pair<vec2, vec2>(vec2(scr_w - w_mid - BRICK_H, h_mid),
                            vec2(BRICK_H, BRICK_W))};

  for (int i = 0; i < 4; ++i) {
    std::shared_ptr<Brick> brick;
    if (i < 2) {
      brick.reset(new Brick(v_pos_size[i].first, v_pos_size[i].second, 1, 22));
    } else {
      brick.reset(new Brick(v_pos_size[i].first, v_pos_size[i].second, 22, 1));
    }
    brick->SetTexture(
        easy_engine::utility::ResourceLocator::Image()->GetImage("brick"));
    _vp_bricks.push_back(brick);
  }
}

}  // namespace snake
}  // namespace minigames
