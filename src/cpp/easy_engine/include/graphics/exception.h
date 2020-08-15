// exception.h - 图形模块公共异常
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 14
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_GRAPHICS_EXCEPTION_H__
#define __EASY_ENGINE_INCLUDE_GRAPHICS_EXCEPTION_H__

#include <string>

#include "../common/exception.h"
#include "../utility/log.h"

const std::string ModuleName("Graphics");
const std::string ErrorFile("EasyEngineGraphics.err");
const std::string ErrorFmt("Graphics Error: %s.");

namespace easy_engine {
namespace graphics {

class GraphicsException : public Exception {
 public:
  explicit GraphicsException(const std::string& msg = "") noexcept
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

}  // namespace graphics
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_GRAPHICS_EXCEPTION_H__
