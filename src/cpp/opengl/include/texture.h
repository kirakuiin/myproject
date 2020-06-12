// texture.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 06
// License: GPL.v3

#ifndef __OPENGL_INCLUDE_TEXTURE_H__
#define __OPENGL_INCLUDE_TEXTURE_H__

#include "exception.h"

namespace gl {

// openGL纹理加载类
class Texture {
  public:
    Texture();
    explicit Texture(unsigned int texture);
    virtual ~Texture() noexcept;

    static void SetParam(int dimen, int type, int value);

    void LoadImage(const std::string& path);
    void LoadImage(const char* path);

    unsigned int texture = 0;       // 纹理地址
    int format = 0;                 // 纹理格式GL_RGB
    int width = 0;                  // 图像宽度
    int height = 0;                 // 图像高度
    int nr_channels = 0;            // 图像通道数
  private:
    virtual void BindImage(unsigned char* data) = 0;
};

class Texture2D: public Texture {
  public:
    Texture2D();
    explicit Texture2D(unsigned int texture);
  private:
    void BindImage(unsigned char* data) override;
};

class Texture3D: public Texture {
  public:
    Texture3D();
    explicit Texture3D(unsigned int texture);
  private:
    void BindImage(unsigned char* data) override;
};

class TextureException: public GlException {
  public:
    explicit TextureException(const std::string& error_message)
        : GlException(error_message) {}
};

} // namespace gl
#endif // __OPENGL_INCLUDE_TEXTURE_H__
