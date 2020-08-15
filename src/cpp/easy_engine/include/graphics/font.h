// font.h - 文字
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 14
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_GRAPHICS_FONT_H__
#define __EASY_ENGINE_INCLUDE_GRAPHICS_FONT_H__

#include <map>
#include <memory>
#include <string>

#include "../common/math.h"
#include "../opengl/shader.h"

namespace easy_engine {
namespace graphics {

// 字体类, 用来在图形界面上显示文字
class Font {
 public:
 private:
  // 字形属性
  struct Character {
    unsigned int texture_id;  // 字形纹理id
    long         advance;     // 每个字形占用的总宽度
    vec2 bearing;  // 字形左侧距离y轴距离和字形上侧距离x轴距离
    vec2 size;     // 字形的宽和高度
  };

  // 存储所有需要显示的字符的字形
  std::map<int, Character>               _m_characters;
  std::shared_ptr<opengl::ShaderProgram> _p_program;  // 显示字形所需要的着色器
};
}  // namespace graphics
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_GRAPHICS_FONT_H__
