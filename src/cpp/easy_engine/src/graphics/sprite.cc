// sprite.cc - 精灵实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 15
// License: GPL.v3

#include "include/graphics/sprite.h"

#include <vector>

namespace easy_engine {
namespace graphics {

SpriteRender::~SpriteRender() { glDeleteVertexArrays(1, &_vao); }

void SpriteRender::DrawSprite(std::shared_ptr<opengl::Texture2D> sprite,
                              const vec2& position, const vec2& size, float rot,
                              const vec3& color) {
  _p_shader->Bind();

  mat4 model(1.0f);
  // 位移
  model = translate(model, vec3(position, 0.0f));
  // 旋转
  model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
  model = rotate(model, rot, vec3(0.0f, 0.0f, 1.0f));
  model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
  // 缩放
  model = scale(model, vec3(size, 1.0f));

  _p_shader->SetUniform("model", model);
  _p_shader->SetUniform("sprite_color", color);
  _p_shader->SetUniform("projection", _p_camera->GetProjectionMatrix());

  sprite->Bind();

  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void SpriteRender::InitShader() {
  if (_p_shader) {
    return;
  }
  _p_shader.reset(new opengl::ShaderProgram());
  std::shared_ptr<opengl::VertexShader> vs(
      new opengl::VertexShader("shaders/sprite/sprite.vert"));
  _p_shader->Attach(vs);
  std::shared_ptr<opengl::FragmentShader> fs(
      new opengl::FragmentShader("shaders/sprite/sprite.frag"));
  _p_shader->Attach(fs);
  _p_shader->Compile();
}

void SpriteRender::InitVertex() {
  // 初始化顶点数据
  unsigned int vbo;
  float        vertices[] = {
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  };

  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

}  // namespace graphics
}  // namespace easy_engine
