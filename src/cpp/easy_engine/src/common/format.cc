// format.cc - 格式化实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 27
// License: GPL.v3

#include "include/common/format.h"

namespace easy_engine {

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
        throw Exception("invalid format string: missing arguments");
      }
    }
    ss << *format++;
  }
}

std::ostream& operator<<(std::ostream& out, const vec2& val) {
  out << "vec2(x=" << val.x << ", y=" << val.y << ")";
  return out;
}

std::ostream& operator<<(std::ostream& out, const vec3& val) {
  out << "vec2(x=" << val.x << ", y=" << val.y << ", z=" << val.z << ")";
  return out;
}

}  // namespace easy_engine
