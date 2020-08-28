// exception.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 28
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_WIDGET_EXCEPTION_H__
#define __EASY_ENGINE_INCLUDE_WIDGET_EXCEPTION_H__

#include <string>

#include "../common/exception.h"
#include "../utility/log.h"

namespace easy_engine {
namespace widget {

const std::string ModuleName("Widget");
const std::string ErrorFile("EasyEngineWidget.err");
const std::string ErrorFmt("Widget Error: %s.");

class WidgetException : public Exception {
 public:
  explicit WidgetException(const std::string& msg = "") noexcept
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

}  // namespace widget
}  // namespace easy_engine

#endif // __EASY_ENGINE_INCLUDE_WIDGET_EXCEPTION_H__
