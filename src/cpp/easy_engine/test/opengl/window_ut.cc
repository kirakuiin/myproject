// opengl_ut.cc - 所有的opengl相关ut
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <boost/format.hpp>
#include <ctime>
#include <iostream>
#include <memory>

#include "common/timer.h"
#include "graphics/animation.h"
#include "graphics/camera.h"
#include "graphics/font.h"
#include "graphics/particle.h"
#include "graphics/sprite.h"
#include "opengl/framebuffer.h"
#include "opengl/shader.h"
#include "opengl/window.h"
#include "utility/config.h"

using boost::format;
using namespace easy_engine;
using namespace easy_engine::utility;
using namespace easy_engine::opengl;
using namespace easy_engine::graphics;
using std::cout;
using std::endl;

namespace {
const int    SCREEN_WIDTH    = 800;
const int    SCREEN_HEIGHT   = 600;
const int    PARTICLE_AMOUNT = 500;
const double FRAME_RATE      = 0.025;

void Pos(GLFWwindow* w, double x, double y) {
  cout << format("x=%f, y=%f") % x % y << endl;
}
void Btn(GLFWwindow* w, int k, int a, int m) {
  cout << format("key=%d, action=%d, mods=%d") % k % a % m << endl;
}
void Key(GLFWwindow* w, int k, int s, int a, int m) {
  cout << format("key=%d, scancode=%d, action=%d, mods=%d") % k % s % a % m
       << endl;
}

// 摄像机
std::shared_ptr<Camera2D> g_camera(
    new Camera2D(vec2(0, SCREEN_HEIGHT), vec2(SCREEN_WIDTH, SCREEN_HEIGHT)));

// 处理摄像机移动的回调函数
void ProcessInputCam(GLFWwindow* w, int k, int s, int a, int m) {
  static vec2 start(450, 350);
  if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
    g_camera->MoveCentral(start += vec2(0, -10));
  }
  if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
    g_camera->MoveCentral(start += vec2(0, 10));
  }
  if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
    g_camera->MoveCentral(start += vec2(10, 0));
  }
  if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
    g_camera->MoveCentral(start += vec2(-10, 0));
  }
}

// 模拟对象
Particle dummy_obj;

// 处理对象移动的回掉函数
void ProcessInputObj(GLFWwindow* w, int k, int s, int a, int m) {
  if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
    dummy_obj.Position += vec2(0, -10);
  }
  if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
    dummy_obj.Position += vec2(0, 10);
  }
  if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
    dummy_obj.Position += vec2(10, 0);
  }
  if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
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
  while (!w.ShouldClose()) {
    w.Update();
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
  w.SetKeyboardCallback(ProcessInputCam);
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
  }
}

TEST_F(WINUT, FontTest) {
  Window w(SCREEN_WIDTH, SCREEN_HEIGHT, "fonttest");
  w.SetKeyboardCallback(ProcessInputCam);
  DefaultFramebufferColor(0, 0, 0, 0);
  Font f(g_camera);
  f.Load(config.GetValue<std::string>("font_path"));
  g_camera->MoveCentral(vec2(450, 350));
  while (!w.ShouldClose()) {
    ClearFramebuffer();
    f.Draw("hello world", vec2(400, 300), vec3(1, 0, 0));
    w.Update();
  }
}

TEST_F(WINUT, ParticleTest) {
  Window w(SCREEN_WIDTH, SCREEN_HEIGHT, "particletest");
  w.SetKeyboardCallback(ProcessInputObj);
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
