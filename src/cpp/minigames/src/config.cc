// config.cc - 配置器
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 20
// License: GPL.v3

#include <include/config.h>

namespace {
const std::string CfgPath("resource/config/config.json");
const std::string Audio("audios");
const std::string Graphic("graphics");
const std::string Control("controls");
const std::string Volume("volume");
const std::string Height("screen_height");
const std::string Width("screen_width");
const std::string Up("up");
const std::string Down("down");
const std::string Left("left");
const std::string Right("right");
}  // namespace

namespace minigames {

MiniCfg::MiniCfg() { LoadConfig(CfgPath); }
MiniCfg::MiniCfg(const std::string &cfgpath) { LoadConfig(cfgpath); }

void MiniCfg::SetVolume(int volume) {
  ptree audio = GetAudioCfg();
  Set(Volume, volume, &audio);
  SetObject(Audio, audio, &_elements);
}

int MiniCfg::GetVolume() { return Get<int>(GetAudioCfg(), Volume); }

void MiniCfg::SetWidth(int width) {
  ptree graphic = GetGraphicCfg();
  Set(Width, width, &graphic);
  SetObject(Graphic, graphic, &_elements);
}

int MiniCfg::GetWidth() { return Get<int>(GetGraphicCfg(), Width); }

void MiniCfg::SetHeight(int height) {
  ptree graphic = GetGraphicCfg();
  Set(Height, height, &graphic);
  SetObject(Graphic, graphic, &_elements);
}

int MiniCfg::GetHeight() { return Get<int>(GetGraphicCfg(), Height); }

void MiniCfg::SetUp(const std::string &key) {
  ptree control = GetControlCfg();
  Set(Up, key, &control);
  SetObject(Control, control, &_elements);
}

std::string MiniCfg::GetUp() { return Get<std::string>(GetControlCfg(), Up); }

void MiniCfg::SetDown(const std::string &key) {
  ptree control = GetControlCfg();
  Set(Down, key, &control);
  SetObject(Control, control, &_elements);
}

std::string MiniCfg::GetDown() {
  return Get<std::string>(GetControlCfg(), Down);
}

void MiniCfg::SetLeft(const std::string &key) {
  ptree control = GetControlCfg();
  Set(Left, key, &control);
  SetObject(Control, control, &_elements);
}

std::string MiniCfg::GetLeft() {
  return Get<std::string>(GetControlCfg(), Left);
}

void MiniCfg::SetRight(const std::string &key) {
  ptree control = GetControlCfg();
  Set(Right, key, &control);
  SetObject(Control, control, &_elements);
}

std::string MiniCfg::GetRight() {
  return Get<std::string>(GetControlCfg(), Right);
}

ptree MiniCfg::GetAudioCfg() { return GetObject(_elements, "audios"); }
ptree MiniCfg::GetGraphicCfg() { return GetObject(_elements, "graphics"); }
ptree MiniCfg::GetControlCfg() { return GetObject(_elements, "controls"); }

}  // namespace minigames
