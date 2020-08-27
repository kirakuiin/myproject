// format.h - 格式化字符串
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 26
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_UTILITY_FORMAT_H__
#define __EASY_ENGINE_INCLUDE_UTILITY_FORMAT_H__

#include <sstream>
#include <string>

#include "../common/math.h"

namespace easy_engine {
namespace utility {

/// 格式化函数, 将格式化字符和参数合成为字符串
///
/// @param format: 格式化字符串
/// @param : 变长参数
template <typename First, typename... Other>
std::string Format(const char* format, const First& first,
                   const Other&... other);
template <typename... Other>
std::string Format(const char* format, const vec2& first,
                   const Other&... other);
template <typename First, typename... Other>
std::string Format(const std::string& format, const First& first,
                   const Other&... other);
template <typename... Other>
std::string Format(const std::string& format, const vec2& first,
                   const Other&... other);
template <typename... Other>
void Format(const char* format, std::stringstream& ss, const vec2& first,
            const Other&... other);
template <typename First, typename... Other>
void Format(const char* format, std::stringstream& ss, const First& first,
            const Other&... other);
// 裸字符串情形
void Format(const char* format, std::stringstream& ss);
// 递归终止函数
std::string Format(const char* format);

template <typename... Other>
void Format(const char* format, std::stringstream& ss, const vec2& first,
            const Other&... other) {
  while (*format) {
    if (*format == '%') {
      if (*(format + 1) == '%') {
        ++format;
      } else {
        ss << '(' << first.x << ", " << first.y << ')';
        Format(format + 1, ss, other...);
        return;
      }
    }
    ss << *format++;
  }
  throw std::runtime_error("extra arguments provided to format");
}

template <typename First, typename... Other>
void Format(const char* format, std::stringstream& ss, const First& first,
            const Other&... other) {
  while (*format) {
    if (*format == '%') {
      if (*(format + 1) == '%') {
        ++format;
      } else {
        ss << first;
        Format(format + 1, ss, other...);
        return;
      }
    }
    ss << *format++;
  }
  throw std::runtime_error("extra arguments provided to format");
}

template <typename First, typename... Other>
std::string Format(const char* format, const First& first,
                   const Other&... other) {
  std::stringstream ss;
  while (*format) {
    if (*format == '%') {
      if (*(format + 1) == '%') {
        ++format;
      } else {
        ss << first;
        Format(format + 1, ss, other...);
        return ss.str();
      }
    }
    ss << *format++;
  }
  throw std::runtime_error("extra arguments provided to format");
}

template <typename First, typename... Other>
std::string Format(const std::string& format, const First& first,
                   const Other&... other) {
  return Format(format.c_str(), first, other...);
}

template <typename... Other>
std::string Format(const char* format, const vec2& first,
                   const Other&... other) {
  std::stringstream ss;
  while (*format) {
    if (*format == '%') {
      if (*(format + 1) == '%') {
        ++format;
      } else {
        ss << '(' << first.x << ", " << first.y << ')';
        Format(format + 1, ss, other...);
        return ss.str();
      }
    }
    ss << *format++;
  }
  throw std::runtime_error("extra arguments provided to format");
}

template <typename... Other>
std::string Format(const std::string& format, const vec2& first,
                   const Other&... other) {
  return Format(format.c_str(), first, other...);
}

}  // namespace utility
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_UTILITY_FORMAT_H__
