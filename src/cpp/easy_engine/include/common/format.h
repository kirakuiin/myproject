// format.h - 格式化字符串
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 26
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_COMMON_FORMAT_H__
#define __EASY_ENGINE_INCLUDE_COMMON_FORMAT_H__

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "exception.h"
#include "math.h"

namespace easy_engine {

// 裸字符串情形
void Format(const char* format, std::stringstream& ss);

// 递归终止函数
std::string Format(const char* format);

/// 格式化函数, 将格式化字符和参数合成为字符串
///
/// @param format: 格式化字符串
/// @param : 变长参数
template <typename First, typename... Other>
std::string Format(const std::string& format, const First& first,
                   const Other&... other);

// vec2的输出重载函数
//
// @param out: 输出对象
// @param val: 输出向量
// @return ostream: 输出对象
std::ostream& operator<<(std::ostream& out, const vec2& val);

// vec3的输出重载函数
//
// @param out: 输出对象
// @param val: 输出向量
// @return ostream: 输出对象
std::ostream& operator<<(std::ostream& out, const vec3& val);

// vector的输出重载函数
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> val) {
  out << "vector{";
  for (const auto& iter : val) {
    out << iter << " ,";
  }
  out << "}";
  return out;
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
  throw Exception("extra arguments provided to format");
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
  throw Exception("extra arguments provided to format");
}

template <typename First, typename... Other>
std::string Format(const std::string& format, const First& first,
                   const Other&... other) {
  return Format(format.c_str(), first, other...);
}

}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_COMMON_FORMAT_H__
