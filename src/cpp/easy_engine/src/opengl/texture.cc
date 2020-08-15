// texture.cc - 纹理实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 14
// License: GPL.v3

#include "include/opengl/texture.h"

#include <stb_image.h>

#include "include/opengl/exception.h"

namespace {
// 通道数类型
enum class ChannelType {
  RED  = 1,  // 灰度
  RGB  = 3,  // 标准颜色
  RGBA = 4,  // 带有透明度的RGB
};
}  // namespace

namespace easy_engine {
namespace opengl {

Texture2D::~Texture2D() { glDeleteTextures(1, &_id); }

void Texture2D::LoadImage(const std::string& image_path) {
  int            nr_channels;  // 文件通道数
  int            width;
  int            height;
  unsigned char* data =
      stbi_load(image_path.c_str(), &width, &height, &nr_channels, 0);
  _width  = width;
  _height = height;
  IdentifyFormat(nr_channels);

  if (data) {
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, _text_format, width, height, 0,
                 _image_format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter_mag);
    glGenerateMipmap(GL_TEXTURE_2D);
    UnbindTexture();
    stbi_image_free(data);
  } else {
    std::string msg("Load image: " + image_path + " failed");
    throw GlException(msg);
  }
}

void Texture2D::IdentifyFormat(int channel_nums) {
  switch (static_cast<ChannelType>(channel_nums)) {
    case ChannelType::RED:
      _image_format = GL_RED;
      _text_format  = GL_RED;
      break;
    case ChannelType::RGB:
      _image_format = GL_RGB;
      if (_is_in_srgb) {
        _text_format = GL_SRGB;
      } else {
        _text_format = _image_format;
      }
      break;
    case ChannelType::RGBA:
      _image_format = GL_RGBA;
      if (_is_in_srgb) {
        _text_format = GL_SRGB_ALPHA;
      } else {
        _text_format = _image_format;
      }
      break;
    default:
      break;
  }
}

void Texture2D::Bind() { glBindTexture(GL_TEXTURE_2D, _id); }

void UnbindTexture() { glad_glBindTexture(GL_TEXTURE_2D, 0); }

}  // namespace opengl
}  // namespace easy_engine
