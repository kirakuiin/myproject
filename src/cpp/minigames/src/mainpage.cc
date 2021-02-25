// mainpage.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 20
// License: GPL.v3

#include "include/mainpage.h"

#include <utility/loader.h>
#include <utility/log.h>

#include "include/common.h"
#include "include/config.h"
#include "include/gameobj.h"
#include "include/snake/snake_game.h"

namespace {
const std::string loading_path("resource/animations/loading/loading_001.png");
const std::string menu_path("resource/animations/menu/menu_001.png");

const std::string start_page("start_page");
const std::string game_page("game_page");
const std::string start_label("start");
const std::string config_label("label");
const std::string exit_label("exit");
const std::string return_label("return");
}  // namespace

namespace minigames {

using easy_engine::graphics::AnimationPlayMode;
using easy_engine::utility::ResourceLocator;

MainPage::MainPage(std::shared_ptr<Window> window)
    : _p_window(window),
      _p_widgets(new WidgetContainer()),
      _p_loading(new Animation()),
      _p_menu(new Animation()),
      _p_current_page(_p_widgets.get()) {
  Init();
}

void MainPage::Update() {
  if (_p_game) {
    if (_p_game->IsRunning()) {
      _p_game->ProcessInput();
      _p_game->Update();
    } else {
      _p_game = nullptr;
      _p_current_page->Enable();
    }
  } else {
    _p_current_page->Update();
  }
}

void MainPage::Draw() {
  if (_p_game && _p_game->IsRunning()) {
    _p_game->Draw();
  } else {
    if (!_p_anime_render->IsPlaying()) {
      _p_anime_render->SetAnimation(_p_menu);
      _p_anime_render->SetPlayMode(
          AnimationPlayMode::PLAY_FORWARD | AnimationPlayMode::PLAY_CYCLE, 10);
      _p_anime_render->Playing();
      _p_loading->Clear();
      _p_current_page->Enable();
    }
    _p_anime_render->DrawAnimation(
        vec2(0, 0), vec2(_p_config->GetWidth(), _p_config->GetHeight()));
    _p_current_page->Draw(_p_render);
  }
}

void MainPage::Init() {
  _p_config.reset(new MiniCfg());
  _p_camera.reset(
      new Camera2D(vec2(0, _p_config->GetHeight()),
                   vec2(_p_config->GetWidth(), _p_config->GetHeight())));
  _p_render.reset(new SpriteRender(_p_camera));
  _p_anime_render.reset(new AnimationRender(_p_render));

  InitAnimation();
  InitWidget();
}

void MainPage::InitAnimation() {
  _p_loading->AddAnimation(loading_path);
  _p_loading->SetDuration(_p_loading->Size() - 1, 1);
  _p_menu->AddAnimation(menu_path);
  _p_anime_render->SetAnimation(_p_loading);
  _p_anime_render->SetPlayMode(
      AnimationPlayMode::PLAY_FORWARD | AnimationPlayMode::PLAY_ONCE, 10);
  _p_anime_render->Playing();
}

void MainPage::InitWidget() {
  InitStartPage();
  InitGamePage();
}

void MainPage::InitStartPage() {
  ResourceLocator::Image()->LoadImage("resource/images/button_001.png",
                                      "start_button");
  ResourceLocator::Image()->LoadImage("resource/images/button_002.png",
                                      "config_button");
  ResourceLocator::Image()->LoadImage("resource/images/button_003.png",
                                      "exit_button");
  ResourceLocator::Image()->LoadImage("resource/images/button_004.png",
                                      "return_button");
  std::shared_ptr<WidgetContainer> w_start_page(new WidgetContainer());
  _p_widgets->Add(start_page, w_start_page);

  std::shared_ptr<Button> start_btn(
      new Button(_p_window, vec2(25, 50), vec2(240, 100),
                 ResourceLocator::Image()->GetImage("start_button")));
  w_start_page->Add(start_label, start_btn);
  start_btn->SetCallback([&](int button) {
    _p_current_page->Disable();
    _p_current_page = _p_widgets->Get(game_page);
    _p_current_page->Enable();
  });

  std::shared_ptr<Button> config_btn(
      new Button(_p_window, vec2(25, 175), vec2(240, 100),
                 ResourceLocator::Image()->GetImage("config_button")));
  w_start_page->Add(config_label, config_btn);

  std::shared_ptr<Button> exit_btn(
      new Button(_p_window, vec2(25, 300), vec2(240, 100),
                 ResourceLocator::Image()->GetImage("exit_button")));
  w_start_page->Add(exit_label, exit_btn);
  exit_btn->SetCallback([&](int button) { _p_window->Close(); });

  _p_current_page = w_start_page.get();
}

void MainPage::InitGamePage() {
  ResourceLocator::Image()->LoadImage("resource/images/button_game_001.png",
                                      "snake_button");
  std::shared_ptr<WidgetContainer> w_game_page(new WidgetContainer());
  _p_widgets->Add(game_page, w_game_page);

  std::shared_ptr<Button> snake_btn(
      new Button(_p_window, vec2(25, 25), vec2(240, 240),
                 ResourceLocator::Image()->GetImage("snake_button")));
  w_game_page->Add("snake_btn", snake_btn);
  snake_btn->SetCallback([&](int button) {
    _p_current_page->Disable();
    _p_game.reset(new snake::SnakeGame(_p_window, _p_camera, _p_render));
    _p_game->Init();
  });

  std::shared_ptr<Button> return_btn(
      new Button(_p_window, vec2(759, 643), vec2(240, 100),
                 ResourceLocator::Image()->GetImage("return_button")));
  w_game_page->Add(return_label, return_btn);
  return_btn->SetCallback([&](int button) {
    _p_current_page->Disable();
    _p_current_page = _p_widgets->Get(start_page);
    _p_current_page->Enable();
  });
}

}  // namespace minigames
