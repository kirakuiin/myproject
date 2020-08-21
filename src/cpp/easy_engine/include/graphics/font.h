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
#include "camera.h"

namespace easy_engine {
namespace graphics {

// 字体类, 用来在图形界面上显示文字
class Font final {
 public:
  // @param camera: 摄像机
  Font(std::shared_ptr<Camera2D> camera) : _p_camera(camera), _vao(0), _vbo(0) {
    InitVertex();
    InitShader();
  }
  ~Font();

  // 加载字体
  // @param ttf_path: 字体路径
  // @param height: 字体最高高度, 默认48
  void Load(const std::string& ttf_path, int height = 48);

  // 输出字符串
  // @param text: 待输出的文本
  // @param pos: 待输出文本的左上角坐标
  // @param color: 字体颜色, 默认黑色
  // @param scale: 字体放大倍数, 默认一倍
  void Draw(const std::string& text, const glm::vec2& pos,
            const glm::vec3& color = glm::vec3(1.0), float scale = 1.0f);

 private:
  // 字形属性
  struct Character {
    unsigned int texture_id;  // 字形纹理id
    long         advance;     // 每个字形占用的总宽度
    ivec2 bearing;  // 字形左侧距离y轴距离和字形上侧距离x轴距离
    ivec2 size;     // 字形的宽和高度
  };

  // 初始化顶点数据
  void InitVertex();

  // 初始化着色器
  void InitShader();

  // 存储所有需要显示的字符的字形
  std::map<int, Character>               _m_characters;
  std::shared_ptr<Camera2D>              _p_camera;  // 摄像机
  std::shared_ptr<opengl::ShaderProgram> _p_program;  // 显示字形所需要的着色器

  // opengl顶点数据
  unsigned int _vao;
  unsigned int _vbo;
};
}  // namespace graphics
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_GRAPHICS_FONT_H__
