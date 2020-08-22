// timer.h - 计时器
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 22
// License: GPL.v3

#include <chrono>
#include <thread>

#ifndef __EASY_ENGINE_INCLUDE_COMMON_TIMER_H__
#define __EASY_ENGINE_INCLUDE_COMMON_TIMER_H__

namespace easy_engine {

using Sec = std::chrono::seconds;       // 秒
using Ms  = std::chrono::milliseconds;  // 毫秒
using Us  = std::chrono::microseconds;  // 微秒

// 获得当前微秒级的准确时间
// @template T: 返回的时间单位, 分为Sec(秒), Ms(毫秒), Us(微秒), 默认秒
// @return double: 当前时间距离1970-1-1 00:00:00的值
template <typename T = Sec>
double Now() {
  double value(std::chrono::duration_cast<Us>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count());
  return value * (static_cast<double>(T::period::den) / Us::period::den);
}

// 休眠指定秒数
// @param duration: 持续时间
void Sleep(double duration) {
  std::this_thread::sleep_for(Us(static_cast<int>(duration * Us::period::den)));
}
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_COMMON_TIMER_H__
