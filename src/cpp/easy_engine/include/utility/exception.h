// exception.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 28
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_UTILITY_EXCEPTION_H__
#define __EASY_ENGINE_INCLUDE_UTILITY_EXCEPTION_H__

#include <string>

#include "../common/exception.h"
#include "log.h"

namespace easy_engine {
namespace utility {

const std::string ModuleName("Utility");
const std::string ErrorFile("EasyEngineUtility.err");
const std::string ErrorFmt("Utility Error: %s.");

class UtilityException : public Exception {
 public:
  explicit UtilityException(const std::string& msg = "") noexcept
      : Exception(msg) {
    ConfigLog();
    Logging::GetLogger(ModuleName)->Error(msg);
  }

 private:
  static void ConfigLog() {
    static bool is_first = true;
    if (is_first) {
      Logging::GetLogger(ModuleName)
          ->Config(LoggingLevel::ERROR, true, ErrorFile, ErrorFmt);
      is_first = false;
    }
  };
};

}  // namespace utility
}  // namespace easy_engine

#endif // __EASY_ENGINE_INCLUDE_UTILITY_EXCEPTION_H__
