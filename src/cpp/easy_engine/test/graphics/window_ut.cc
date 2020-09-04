// opengl_ut.cc - 所有的opengl相关ut
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#include <gtest/gtest.h>

#include <boost/format.hpp>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

#include "audio/soundengine.h"
#include "opengl/framebuffer.h"
#include "physics/collision.h"
#include "widget/button.h"
#include "widget/container.h"
#include "widget/widget.h"

#define __EASY_ENGINE_ALL__
#include "easy_engine.h"

using boost::format;
using namespace easy_engine;
using namespace easy_engine::utility;
using namespace easy_engine::opengl;
using namespace easy_engine::graphics;
using namespace easy_engine::widget;
using namespace easy_engine::audio;
using std::cout;
using std::endl;

namespace {
const int    SCREEN_WIDTH    = 800;
const int    SCREEN_HEIGHT   = 600;
const int    PARTICLE_AMOUNT = 500;
const double FRAME_RATE      = 0.025;

void Pos(double x, double y) { cout << format("x=%f, y=%f") % x % y << endl; }
void Btn(int k, int a, int m) {
  cout << format("key=%d, action=%d, mods=%d") % k % a % m << endl;
}
void Key(int k, int s, int a, int m) {
  cout << format("key=%d, scancode=%d, action=%d, mods=%d") % k % s % a % m
       << endl;
}
void Winsize(int w, int h) {
  cout << format("width=%d, height=%d") % w % h << endl;
}

// 摄像机
std::shared_ptr<Camera2D> g_camera(
    new Camera2D(vec2(0, SCREEN_HEIGHT), vec2(SCREEN_WIDTH, SCREEN_HEIGHT)));

// 处理摄像机移动的回调函数
void ProcessInputCam(Window* w) {
  static vec2 start(450, 350);
  if (w->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
    g_camera->MoveCentral(start += vec2(0, -10));
  }
  if (w->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
    g_camera->MoveCentral(start += vec2(0, 10));
  }
  if (w->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
    g_camera->MoveCentral(start += vec2(10, 0));
  }
  if (w->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
    g_camera->MoveCentral(start += vec2(-10, 0));
  }
}

// 模拟对象
Particle dummy_obj;

// 处理对象移动的回掉函数
void ProcessInputObj(Window* w) {
  if (w->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
    dummy_obj.Position += vec2(0, -10);
  }
  if (w->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
    dummy_obj.Position += vec2(0, 10);
  }
  if (w->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
    dummy_obj.Position += vec2(10, 0);
  }
  if (w->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
    dummy_obj.Position += vec2(-10, 0);
  }
}
}  // namespace

class WINUT : public testing::Test {
 public:
  static void SetUpTestCase() { config.LoadConfig("winut.json"); }
  static void TearDownTestCase() {}
  void        SetUp() override {}
  void        TearDown() override {}

  static FlatConfigurator config;  // 配置读取
};
FlatConfigurator WINUT::config;

TEST_F(WINUT, WindowTest) {
  Window w(SCREEN_WIDTH, SCREEN_HEIGHT, "windowtest");
  w.SetCursorInvisble(true);
  w.SetCursorPosCallback(Pos);
  w.SetMouseBtnCallback(Btn);
  w.SetScrollCallback(Pos);
  w.SetKeyboardCallback(Key);
  w.SetWindowSizeCallback(Winsize);
  DefaultFramebufferColor(0, 0, 0, 0);
  while (!w.ShouldClose()) {
    w.Update();
    ClearFramebuffer();
  }
  int wd, hi;
  w.GetWindowSize(&wd, &hi);
  cout << format("width=%d, height=%d") % wd % hi << endl;
}

TEST_F(WINUT, FrameBufferTest) {
  Window w(800, 600, "framebuffertest");

  // 创建一个新的帧缓冲
  auto color_attachment = std::shared_ptr<ColorAttachment>(
      new TextureColorAttachment(SCREEN_WIDTH, SCREEN_HEIGHT));
  Framebuffer fb;
  fb.Attach(color_attachment);
  fb.Bind();

  DefaultFramebufferColor(0, 0, 0, 0);
  while (!w.ShouldClose()) {
    ClearFramebuffer();
    w.Update();
  }
  UnbindFramebuffer();
}

TEST_F(WINUT, SpriteTest) {
  Window w(SCREEN_WIDTH, SCREEN_HEIGHT, "spritetest");
  DefaultFramebufferColor(0, 0, 0, 0);
  SpriteRender render(g_camera);

  std::shared_ptr<Texture2D> sprite(new Texture2D());
  sprite->LoadImage("texture/weapons.png");
  g_camera->MoveCentral(vec2(450, 350));
  render.SetTextureCoordAbs(vec2(512, 1024), vec2(136, 676), vec2(18, 36));
  while (!w.ShouldClose()) {
    ClearFramebuffer();
    render.DrawSprite(sprite, vec2(400, 300), vec2(180, 360));
    w.Update();
    ProcessInputCam(&w);
  }
}

TEST_F(WINUT, FontTest) {
  Window w(SCREEN_WIDTH, SCREEN_HEIGHT, "fonttest");
  DefaultFramebufferColor(0, 0, 0, 0);
  Font f(g_camera);
  f.Load(config.GetValue<std::string>("font_path"));
  g_camera->MoveCentral(vec2(450, 350));
  while (!w.ShouldClose()) {
    w.Update();
    ProcessInputCam(&w);
    ClearFramebuffer();
    f.Draw("hello world", vec2(400, 300), vec3(1, 0, 0));
  }
}

TEST_F(WINUT, ParticleTest) {
  Window w(SCREEN_WIDTH, SCREEN_HEIGHT, "particletest");
  DefaultFramebufferColor(0, 0, 0, 0);
  std::shared_ptr<SpriteRender> render(new SpriteRender(g_camera));
  std::shared_ptr<Texture2D>    sprite(new Texture2D());
  sprite->LoadImage("texture/weapons.png");
  SimpleParticleRender part(render, sprite, PARTICLE_AMOUNT);
  part.SetDrawRange(vec2(136, 676), vec2(18, 36));
  DefaultParticleGenerator gene(vec2(10, 20), 8.0f, vec2(5));
  dummy_obj.Position = vec2(395, 290);

  double prev       = Now<>();
  double curr       = 0;
  double delta_time = 0;
  while (!w.ShouldClose()) {
    curr = Now<>();
    delta_time += (curr - prev);
    prev = curr;
    ClearFramebuffer();
    while (delta_time >= FRAME_RATE) {
      part.Update(gene(dummy_obj.Position, vec2(0, 200)),
                  config.GetValue<int>("particle_gene_num"));
      delta_time -= FRAME_RATE;
    }
    part.Draw();
    w.Update();
    ProcessInputObj(&w);
  }
}

TEST_F(WINUT, AnimationTest) {
  Window w(SCREEN_WIDTH, SCREEN_HEIGHT, "animationtest");
  DefaultFramebufferColor(0, 0, 0, 0);
  std::shared_ptr<Texture2D>    background(new Texture2D());
  std::shared_ptr<SpriteRender> render(new SpriteRender(g_camera));
  std::shared_ptr<Animation>    anime(new Animation());
  background->LoadImage(config.GetValue<std::string>("animation_bg"));
  anime->AddAnimation(config.GetValue<std::string>("animation_path"));
  Font f_render(g_camera);
  f_render.Load(config.GetValue<std::string>("font_path"), 30);

  AnimationRender anime_render(render);
  anime_render.SetAnimation(anime);
  anime_render.SetPlayMode(
      AnimationPlayMode::PLAY_CYCLE | AnimationPlayMode::PLAY_BACKWARD, 22.5);
  anime_render.Playing();

  double prev       = Now<>();
  double curr       = 0;
  double delta_time = 0;
  int    count      = 0;
  int    fps        = 60;
  float  time       = 0;
  while (!w.ShouldClose()) {
    ++count;
    curr       = Now<>();
    delta_time = (curr - prev);
    time += delta_time;
    prev = curr;
    ClearFramebuffer();
    render->DrawSprite(background, vec2(0, 0), vec2(800, 600));
    anime_render.DrawAnimation(vec2(0, 75), vec2(800, 450));
    if (count % 60 == 0) {
      fps = (int)(1.0 / delta_time);
    }
    if ((int)time % 10 == 0) {
      anime_render.Pause();
    }
    if ((int)time % 3 == 0) {
      anime_render.Continue();
    }
    if (time >= 20 && time < 22) {
      if (anime_render.IsPlaying()) anime_render.Stop();
    } else if (time >= 22 and time < 24) {
      anime_render.Playing();
    }
    f_render.Draw("fps: " + std::to_string(fps), vec2(0));
    f_render.Draw("time: " + std::to_string((int)(time * 1000)) + "ms",
                  vec2(600, 0));
    w.Update();
  }
}

TEST_F(WINUT, ButtonTest) {
  std::shared_ptr<Window> w(
      new Window(SCREEN_WIDTH, SCREEN_HEIGHT, "buttontest"));
  std::shared_ptr<Texture2D>    background(new Texture2D());
  std::shared_ptr<Texture2D>    anobak(new Texture2D());
  std::shared_ptr<Texture2D>    curbak(background);
  std::shared_ptr<Texture2D>    button(new Texture2D());
  std::shared_ptr<Texture2D>    icon(new Texture2D());
  std::shared_ptr<SpriteRender> render(new SpriteRender(g_camera));
  background->LoadImage(config.GetValue<std::string>("animation_bg"));
  anobak->LoadImage(config.GetValue<std::string>("button_bg"));
  button->LoadImage(config.GetValue<std::string>("button_img"));
  icon->LoadImage(config.GetValue<std::string>("cursor_icon"));
  DefaultFramebufferColor(0, 0, 0, 0);
  std::shared_ptr<Button> btn(
      new Button(w, vec2(300, 300), vec2(100, 20), button));
  std::shared_ptr<Cursor> cursor(
      new Cursor(w, vec2(300, 300), vec2(20, 20), icon));
  WidgetContainer container;
  ButtonCallBack  callback([&](int btn) {
    if (btn == GLFW_MOUSE_BUTTON_LEFT) {
      curbak = anobak;
    } else {
      curbak = background;
    }
  });
  btn->SetCallback(callback);
  // std::shared_ptr<physics::CircleBox> hitbox(
  // new physics::CircleBox(vec2(350, 310), 100));
  // btn->SetHitBox(hitbox);
  // cursor->Disable();
  container.Add("switch_bg", btn);

  while (!w->ShouldClose()) {
    ClearFramebuffer();
    render->DrawSprite(curbak, vec2(0, 0), vec2(800, 600));
    container.Update();
    cursor->Update();
    container.Draw(render);
    cursor->Draw(render);
    w->Update();
  }
}

TEST_F(WINUT, SoundTest) {
  SoundEngine engine(50);
  vec3        lis(0, 0, 0);
  engine.SetListener(lis, vec3(0, 0, 1));
  engine.Play(MusicInfo(config.GetValue<std::string>("music_03"), 1, true,
                        false, true, 0.5, 0.5));
  engine.Play(MusicInfo(config.GetValue<std::string>("music_02"), 2, true, true,
                        true, 1, 1, vec3(0, 0, 0), 1));
  while (true) {
    char c;
    std::cin >> c;
    if (c == 'e') {
      break;
    }
    switch (c) {
      case '+':
        engine.Play(MusicInfo(config.GetValue<std::string>("music_01"), 3, true,
                              false, true));
        break;
      case 'w':
        lis.z += 1;
        break;
      case 's':
        lis.z -= 1;
        break;
      case 'a':
        lis.x -= 1;
        break;
      case 'd':
        lis.x += 1;
        break;
      case 'q':
        engine.StopMusic(1);
        break;
    }
    engine.SetListener(lis, vec3(0, 0, 1));
    cout << Format("%", lis) << endl;
  }
  engine.Stop();
}
