// snake_game.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Oct 09
// License: GPL.v3

#include "include/snake/snake_game.h"

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

namespace minigames {
namespace snake {

SnakeGame::SnakeGame(
    std::shared_ptr<easy_engine::widget::Window>         p_window,
    std::shared_ptr<easy_engine::graphics::Camera2D>     p_camera,
    std::shared_ptr<easy_engine::graphics::SpriteRender> p_render)
    : _is_running(false),
      _is_alive(true),
      _p_window(p_window),
      _p_camera(p_camera),
      _p_render(p_render) {}

SnakeGame::~SnakeGame() {}

void SnakeGame::Init() {
  // initial font
  _p_font.reset(new easy_engine::graphics::Font(_p_camera));
  _p_font->Load("resource/fonts/newyork.ttf");
  // get window size
  int width, height;
  _p_window->GetWindowSize(&width, &height);
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
      "resource/images/cursor.png", "cursor");
  // initial game object
  auto GetImage = [](const std::string& name) {
    return easy_engine::utility::ResourceLocator::Image()->GetImage(name);
  };
  _p_snake.reset(new Snake(vec2(600, 700), vec2(20, 20), vec2(-1, 0)));
  _p_snake->SetTexture(GetImage("snake_head"), GetImage("snake_body"),
                       GetImage("snake_tail"));
  _p_egg.reset(
      new NormalEgg(vec2(0), vec2(width, height), vec2(30), _p_snake.get()));
  _p_egg->SetTexture(GetImage("egg"));
  _is_running = true;
}

void SnakeGame::ProcessInput() {
  if (_p_window->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
    _p_snake->SetVelocity(vec2(0, -1));
  } else if (_p_window->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
    _p_snake->SetVelocity(vec2(0, 1));
  } else if (_p_window->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
    _p_snake->SetVelocity(vec2(-1, 0));
  } else if (_p_window->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
    _p_snake->SetVelocity(vec2(1, 0));
  } else if (_p_window->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    _is_running = false;
  } else if (_p_window->GetKey(GLFW_KEY_R) == GLFW_PRESS) {
    auto GetImage = [](const std::string& name) {
      return easy_engine::utility::ResourceLocator::Image()->GetImage(name);
    };
    _p_snake.reset(new Snake(vec2(600, 700), vec2(20, 20), vec2(-1, 0)));
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
  if (CheckCollision()) {
    _is_alive = false;
  }
}

void SnakeGame::Draw() {
  if (_is_alive) {
    _p_snake->Draw(_p_render);
    _p_egg->Draw(_p_render);
  } else {
    _p_font->Draw("Game Over", vec2(400, 300), easy_engine::vec3(1, 0, 0));
  }
}

bool SnakeGame::IsRunning() const { return _is_running; }

bool SnakeGame::CheckCollision() const {
  return _p_snake->CheckSelfCollision();
}

}  // namespace snake
}  // namespace minigames
