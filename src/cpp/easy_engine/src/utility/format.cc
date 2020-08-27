// format.cc - 格式化实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 27
// License: GPL.v3

#include "include/utility/format.h"

namespace easy_engine {
namespace utility {

std::string Format(const char* format) {
  std::stringstream ss;
  while (*format) {
    if (*format == '%') {
      if (*(format + 1) == '%') {
        ++format;
      }
    }
    ss << *format++;
  }
  return ss.str();
}

void Format(const char* format, std::stringstream& ss) {
  while (*format) {
    if (*format == '%') {
      if (*(format + 1) == '%') {
        ++format;
      } else {
        throw std::runtime_error("invalid format string: missing arguments");
      }
    }
    ss << *format++;
  }
}

}  // namespace utility
}  // namespace easy_engine
