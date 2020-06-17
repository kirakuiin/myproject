// frame.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 17
// License: GPL.v3

#ifndef __CPP_OPENGL_INCLUDE_FRAME_H__
#define __CPP_OPENGL_INCLUDE_FRAME_H__

#include "exception.h"

namespace gl {

enum class FrameBufferType {
    RENDER,
    TEXTURE,
};

// openGL帧缓冲类
class Framebuffer {
  public:
    Framebuffer(int width, int height,
                FrameBufferType type = FrameBufferType::RENDER);
    ~Framebuffer() noexcept;

    unsigned int id;        // 帧缓冲id
    unsigned int color_id;  // 颜色缓冲id
  private:
    void Init();
    void Release();

    unsigned int GenerateTextureAttachment(FrameBufferType type);
    unsigned int GenerateRenderAttachment();

    int _width;             // 屏幕宽度
    int _height;            // 屏幕高度
    FrameBufferType _type;  // 深度和模版缓冲类型
    unsigned int _dc_id;    // 深度和模版缓冲id
};

class FramebufferException: public GlException {
  public:
    explicit FramebufferException(const std::string& error_message)
        : GlException(error_message) {}
};

} // namespace gl

#endif // __CPP_OPENGL_INCLUDE_FRAME_H__
