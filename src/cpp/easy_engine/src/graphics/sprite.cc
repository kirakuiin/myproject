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

SpriteRender::~SpriteRender() {
  glDeleteVertexArrays(1, &_vao);
  glDeleteBuffers(1, &_vbo);
}

void SpriteRender::SetTextureCoordPer(const vec2 &pos, const vec2 &size) {
  fvec2 top_left(pos);
  fvec2 bottom_left(pos + vec2(0, size.y));
  fvec2 bottom_right(pos + size);
  fvec2 top_right(pos + vec2(size.x, 0));
  // 注意, 纹理坐标和opengl坐标在y轴是相反的
  float vertices[] = {
      0.0f,          1.0f,        bottom_left.x, bottom_left.y,  1.0f,
      0.0f,          top_right.x, top_right.y,   0.0f,           0.0f,
      top_left.x,    top_left.y,  0.0f,          1.0f,           bottom_left.x,
      bottom_left.y, 1.0f,        1.0f,          bottom_right.x, bottom_right.y,
      1.0f,          0.0f,        top_right.x,   top_right.y,
  };
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRender::SetTextureCoordAbs(const vec2 &texsize, const vec2 &pos,
                                      const vec2 &size) {
  fvec2 ts(texsize), p(pos), s(size);
  SetTextureCoordPer(p / ts, s / ts);
}

void SpriteRender::DrawSprite(std::shared_ptr<opengl::Texture2D> sprite,
                              const vec2 &position, const vec2 &size, float rot,
                              const vec3 &color) {
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
  float vertices[] = {
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  };

  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

}  // namespace graphics
}  // namespace easy_engine
