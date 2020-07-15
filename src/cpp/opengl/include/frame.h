// frame.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 17
// License: GPL.v3

#ifndef __CPP_OPENGL_INCLUDE_FRAME_H__
#define __CPP_OPENGL_INCLUDE_FRAME_H__

#include <memory>
#include <vector>

#include "exception.h"

namespace gl {

class ColorAttachment;
class DepthStencilAttachment;

// openGL帧缓冲类
class Framebuffer {
  public:
    Framebuffer();
    ~Framebuffer() noexcept;

    // 附加颜色缓冲附件和深度模板附件
    void Attach(std::shared_ptr<ColorAttachment> ca,
                std::shared_ptr<DepthStencilAttachment> da);

    unsigned int id;                                        // 帧缓冲id
    std::shared_ptr<ColorAttachment> color;
    std::vector<std::shared_ptr<ColorAttachment>> colors;
    std::shared_ptr<DepthStencilAttachment> depths;
  private:
};

// 附件类
class BufferAttachment {
  public:
    BufferAttachment(int width, int height)
        : id(0), _width(width), _height(height) {}
    virtual ~BufferAttachment() {}

    // 将附件附加到帧缓冲上
    virtual void Attach() = 0;

    unsigned int id;        // 附件id

  protected:
    int _width;             // 屏幕宽度
    int _height;            // 屏幕高度
};

class ColorAttachment: public BufferAttachment {
  public:
    ColorAttachment(int width, int height, int index=0)
        : BufferAttachment(width, height), _index(index) {}

    virtual void Attach() = 0;

  protected:
    int _index;             // 颜色缓冲索引
};

class TextureColorAttachment: public ColorAttachment {
  public:
    TextureColorAttachment(int width, int height, int index=0);
    ~TextureColorAttachment() noexcept;

    void Attach() override;
};

class FloatColorAttachment: public ColorAttachment {
  public:
    FloatColorAttachment(int width, int height, int format, int index=0);
    ~FloatColorAttachment() noexcept;

    void Attach() override;
};

class MultisampleColorAttachment: public ColorAttachment {
  public:
    MultisampleColorAttachment(int width, int height, int samples, int index=0);
    ~MultisampleColorAttachment() noexcept;

    void Attach() override;

  private:
    int _samples;        // 采样数
};

class DepthStencilAttachment: public BufferAttachment {
  public:
    DepthStencilAttachment(int width, int height)
        : BufferAttachment(width, height) {}

    virtual void Attach() = 0;
};

class TextureDepthStencilAttachment: public DepthStencilAttachment {
  public:
    TextureDepthStencilAttachment(int width, int height);
    ~TextureDepthStencilAttachment() noexcept;

    void Attach() override;
};

class RenderDepthStencilAttachment: public DepthStencilAttachment {
  public:
    RenderDepthStencilAttachment(int width, int height);
    ~RenderDepthStencilAttachment() noexcept;

    void Attach() override;
};

class MultisampleDepthStencilAttachment: public RenderDepthStencilAttachment {
  public:
    MultisampleDepthStencilAttachment(int width, int height, int samples);
    ~MultisampleDepthStencilAttachment() noexcept;

    void Attach() override;

  private:
    int _samples;        // 采样数
};

class ShadowDepthAttachment: public DepthStencilAttachment {
  public:
    ShadowDepthAttachment(int width, int height);
    ~ShadowDepthAttachment() noexcept;

    void Attach() override;
};

class CubeShadowAttachment: public DepthStencilAttachment {
  public:
    CubeShadowAttachment(int width, int height);
    ~CubeShadowAttachment() noexcept;

    void Attach() override;
};

class FramebufferException: public GlException {
  public:
    explicit FramebufferException(const std::string& error_message)
        : GlException(error_message) {}
};

} // namespace gl

#endif // __CPP_OPENGL_INCLUDE_FRAME_H__
