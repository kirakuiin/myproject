// main.cc - minigames主函数
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 07
// License: GPL.v3

#define __EASY_ENGINE_ALL__
#include "easy_engine.h"

namespace {
const int Width  = 1024;
const int Height = 768;
}  // namespace

int main(void) {
  easy_engine::widget::Window window(Width, Height, "minigames");
  return 0;
}
