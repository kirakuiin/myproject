// exception.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 25
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_PHYSICS_EXCEPTION_H__
#define __EASY_ENGINE_INCLUDE_PHYSICS_EXCEPTION_H__

#include <string>

#include "../common/exception.h"
#include "../utility/log.h"

namespace easy_engine {
namespace physics {

const std::string ModuleName("Physics");
const std::string ErrorFile("EasyEnginePhysics.err");
const std::string ErrorFmt("Physics Error: %s.");

class PhysicsException : public Exception {
 public:
  explicit PhysicsException(const std::string& msg = "") noexcept
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

}  // namespace physics
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_PHYSICS_EXCEPTION_H__
