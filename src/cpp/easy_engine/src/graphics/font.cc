// font.cc - 文字实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 14
// License: GPL.v3

#include "include/graphics/font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "include/graphics/exception.h"
#include "include/graphics/font.h"
#include "include/opengl/shader.h"
#include "include/opengl/texture.h"

namespace easy_engine {
namespace graphics {

Font::~Font() {
  glDeleteVertexArrays(1, &_vao);
  glDeleteBuffers(1, &_vbo);
}

void Font::Load(const std::string &ttf_path, int height) {
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::string err_msg("Error: Could not init freetype library.");
    throw GraphicsException(err_msg);
  }
  FT_Face face;
  if (FT_New_Face(ft, ttf_path.c_str(), 0, &face)) {
    std::string err_msg("Error: Failed to load font.");
    throw GraphicsException(err_msg);
  }
  FT_Set_Pixel_Sizes(face, 0, height);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (int c = 0; c < 128; ++c) {
    if (FT_Load_Char(face, char(c), FT_LOAD_RENDER)) {
      std::string err_msg("Error: Failed to load glyph[" + std::to_string(c) +
                          "].");
      throw GraphicsException(err_msg);
    }
    opengl::Texture2D text(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR,
                           GL_LINEAR, GL_RED, GL_RED);
    text.Generate(face->glyph->bitmap.width, face->glyph->bitmap.rows,
                  GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    Character chara{
        text.Id(),
        face->glyph->advance.x,
        ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
    };
    _m_characters[c] = chara;
  }
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void Font::Draw(const std::string &text, const glm::vec2 &pos,
                const glm::vec3 &color, float scale) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float x = pos.x;
  float y = pos.y;
  _p_program->Bind();
  _p_program->SetUniform("texcolor", color);
  _p_program->SetUniform("projection", _p_camera->GetProjectionMatrix());

  glBindVertexArray(_vao);
  for (const auto &c : text) {
    const Character &ch = _m_characters[c];

    float x_pos = x + ch.bearing.x * scale;
    float y_pos = y + (_m_characters['H'].bearing.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    float vertices[6][4] = {
        {x_pos, y_pos + h, 0, 1},     {x_pos + w, y_pos, 1, 0},
        {x_pos, y_pos, 0, 0},         {x_pos, y_pos + h, 0, 1},
        {x_pos + w, y_pos + h, 1, 1}, {x_pos + w, y_pos, 1, 0},
    };

    glBindTexture(GL_TEXTURE_2D, ch.texture_id);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    x += (ch.advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Font::InitVertex() {
  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);
  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glBindVertexArray(0);
}

void Font::InitShader() {
  std::shared_ptr<opengl::VertexShader> v_shader(
      new opengl::VertexShader("shaders/font/font.vert"));
  std::shared_ptr<opengl::FragmentShader> f_shader(
      new opengl::FragmentShader("shaders/font/font.frag"));
  _p_program.reset(new opengl::ShaderProgram());
  _p_program->Attach(v_shader);
  _p_program->Attach(f_shader);
  _p_program->Compile();
}

}  // namespace graphics
}  // namespace easy_engine
