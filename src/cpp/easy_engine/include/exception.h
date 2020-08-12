// exception.h - 异常
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#ifndef __CPP_EASY_ENGINE_INCLUDE_EXCEPTION_H__
#define __CPP_EASY_ENGINE_INCLUDE_EXCEPTION_H__

#include <exception>
#include <string>

namespace easy_engine {

class Exception : public std::exception {
 public:
  explicit Exception(const std::string& msg = "") noexcept
      : _error_message(msg) {}
  virtual ~Exception() noexcept {}
  const char* what() const noexcept override { return _error_message.c_str(); }

 private:
  std::string _error_message;
};

}  // namespace easy_engine

#endif  // __CPP_EASY_ENGINE_INCLUDE_EXCEPTION_H__
