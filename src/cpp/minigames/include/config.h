// config.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 20
// License: GPL.v3

#ifndef __CPP_MINIGAMES_INCLUDE_CONFIG_H__
#define __CPP_MINIGAMES_INCLUDE_CONFIG_H__

#include <utility/config.h>

#include <algorithm>

namespace minigames {

using boost::property_tree::ptree;

// minigames 游戏配置器
class MiniCfg : public easy_engine::utility::Configurator {
 public:
  MiniCfg();
  explicit MiniCfg(const std::string& cfgpath);
  // 音量相关
  void SetVolume(int volume);
  int  GetVolume();

  // 屏幕宽度
  void SetWidth(int width);
  int  GetWidth();

  // 屏幕高度
  void SetHeight(int height);
  int  GetHeight();

  // 设置上
  void        SetUp(const std::string& key);
  std::string GetUp();

  // 设置下
  void        SetDown(const std::string& key);
  std::string GetDown();

  // 设置左
  void        SetLeft(const std::string& key);
  std::string GetLeft();

  // 设置右
  void        SetRight(const std::string& key);
  std::string GetRight();

 private:
  ptree GetAudioCfg();
  ptree GetGraphicCfg();
  ptree GetControlCfg();
};

}  // namespace minigames

#endif  // __CPP_MINIGAMES_INCLUDE_CONFIG_H__
