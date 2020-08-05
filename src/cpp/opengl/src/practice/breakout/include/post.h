// post.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 01
// License: GPL.v3

#ifndef __PRACTICE_BREAKOUT_INCLUDE_POST_H__
#define __PRACTICE_BREAKOUT_INCLUDE_POST_H__

#include <memory>
#include "include/shader.h"
#include "include/texture.h"
#include "include/frame.h"

using std::shared_ptr;
using gl::Framebuffer;
using gl::ShaderProgram;

class PostProcessor {
  public:
    PostProcessor(shared_ptr<ShaderProgram> shader, int width, int height);

    void BeginRender();

    void EndRender();

    void Render(float time);

    bool Confuse = false;
    bool Chaos = false;
    bool Shake = false;
    bool TimeStop = false;

  private:
    void InitData();

    int _width;
    int _height;
    unsigned int _vao = 0;
    Framebuffer _msfbo, _fbo;
    shared_ptr<ShaderProgram> _shader;
    bool _stopping = false;
};

#endif // __PRACTICE_BREAKOUT_INCLUDE_POST_H__
