// render.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 28
// License: GPL.v3

#ifndef __PRACTICE_BREAKOUT_INCLUDE_RENDER_H__
#define __PRACTICE_BREAKOUT_INCLUDE_RENDER_H__

#include <memory>
#include "include/glm/glm.hpp"

#include "include/texture.h"
#include "include/shader.h"

using glm::vec2;
using glm::vec3;
using std::shared_ptr;
using gl::Texture2D;
using gl::ShaderProgram;

class SpriteRender {
  public:
    explicit SpriteRender(shared_ptr<ShaderProgram> shader)
        : _shader(shader), _vao(0) { Init(); }
    ~SpriteRender() noexcept;

    void DrawSprite(shared_ptr<Texture2D> texture, vec2 position,
                    float rotate = 0.0f, vec2 size=vec2(10, 10),
                    vec3 color = vec3(1.0f));
  private:
    void Init();

    shared_ptr<ShaderProgram> _shader;
    unsigned                  _vao;
};

#endif // __PRACTICE_BREAKOUT_INCLUDE_RENDER_H__
