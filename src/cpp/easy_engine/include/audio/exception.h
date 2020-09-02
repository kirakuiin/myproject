// exception.h - 图形模块公共异常
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 14
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_AUDIO_EXCEPTION_H__
#define __EASY_ENGINE_INCLUDE_AUDIO_EXCEPTION_H__

#include <string>

#include "../common/exception.h"
#include "../utility/log.h"

namespace easy_engine {
namespace audio {

const std::string ModuleName("Audio");
const std::string ErrorFile("EasyEngineAudio.err");
const std::string ErrorFmt("Audio Error: %s.");

class AudioException : public Exception {
 public:
  explicit AudioException(const std::string& msg = "") noexcept
      : Exception(msg) {
    ConfigLog();
    utility::Logging::GetLogger(ModuleName)->Error(msg);
  }

 private:
  static void ConfigLog() {
    static bool is_first = true;
    if (is_first) {
      utility::Logging::GetLogger(ModuleName)
          ->Config(utility::LoggingLevel::ERROR, true, ErrorFile, ErrorFmt);
      is_first = false;
    }
  };
};

}  // namespace audio
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_AUDIO_EXCEPTION_H__
