// texture.h - 纹理
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#include <string>

#include "glad/glad.h"

#ifndef __EASY_ENGINE_INCLUDE_OPENGL_TEXTURE_H__
#define __EASY_ENGINE_INCLUDE_OPENGL_TEXTURE_H__

namespace easy_engine {
namespace opengl {

// 纹理基类
class Texture {
 public:
  virtual ~Texture(){};

  // 根据路径加载图像
  // @param image_path: 图像的路径
  virtual void LoadImage(const std::string& image_path) = 0;

  // 将此纹理设置为opengl着色器使用的纹理
  virtual void Bind() = 0;
};

// 2D纹理类
class Texture2D : public Texture {
  // 2D纹理构造函数
  // @param wrap_s: x轴环绕模式, 是指当x轴纹理坐标不在[0,
  // 1]内部时纹理的表现形式, 默认为GL_REPEAT, 可选(GL_MIRRORED_REPEAT,
  // GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER)
  // @param wrap_t: y轴环绕模式, 和wrap_s定义类似
  // @param filter_min: 当将纹理缩小时, 纹理坐标映射到纹理像素的方式,
  // 默认为GL_LINEAR, 可选(GL_NEAREST)
  // @param filter_mag: 当将纹理放大时, 和filter_min定义类似
  // @param image_format: 加载的图像格式, 一般无需设置, 默认为GL_RGB
  // @param text_format: 纹理的图像格式, 一般无需设置, 默认为GL_RGB,
  // 可选(GL_RGBA, GL_RED)
  // @param is_in_srgb: 设置图像颜色位于是否位于标准颜色空间, 会影响到gamma,
  // 默认为否
  Texture2D(unsigned int wrap_s = GL_REPEAT, unsigned int wrap_t = GL_REPEAT,
            unsigned int filter_min   = GL_LINEAR,
            unsigned int filter_mag   = GL_LINEAR,
            unsigned int image_format = GL_RGB,
            unsigned int text_format = GL_RGB, bool is_in_srgb = false)
      : _id(0),
        _width(0),
        _height(0),
        _wrap_s(wrap_s),
        _wrap_t(wrap_t),
        _filter_min(filter_min),
        _filter_mag(filter_mag),
        _image_format(image_format),
        _text_format(text_format),
        _is_in_srgb(is_in_srgb) {}
  ~Texture2D();

  void LoadImage(const std::string& image_path) override;

  void Bind() override;

  // 返回纹理id
  // @return int: 纹理id
  unsigned int Id() const { return _id; }

 private:
  // 根据通道数识别图片格式
  void IdentifyFormat(int channel_nums);

  unsigned int _id;            // 纹理id
  unsigned int _width;         // 纹理宽度
  unsigned int _height;        // 纹理高度
  unsigned int _wrap_s;        // s轴环绕模式
  unsigned int _wrap_t;        // t轴环绕模式
  unsigned int _filter_min;    // 被缩小时过滤模式
  unsigned int _filter_mag;    // 被放大时过滤模式
  unsigned int _image_format;  // 加载的图像格式
  unsigned int _text_format;   // 纹理格式
  bool         _is_in_srgb;    // 是否位于标准颜色空间
};

// 清除当前绑定的纹理
void UnbindTexture();

}  // namespace opengl
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_OPENGL_TEXTURE_H__
