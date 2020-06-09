// exception.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 05
// License: GPL.v3

#ifndef __OPENGL_INCLUDE_EXCEPTION_H__
#define __OPENGL_INCLUDE_EXCEPTION_H__

#include <exception>
#include <string>

namespace gl {

class GlException: public std::exception {
  public:
    explicit GlException(const std::string& msg = "") noexcept
        : _error_message(msg) {
    }
    virtual ~GlException() noexcept {}
    const char* what() const noexcept {
        return _error_message.c_str();
    }
  private:
    std::string _error_message;
};

} // namespace gl

#endif // __OPENGL_INCLUDE_EXCEPTION_H__
