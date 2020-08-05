// font.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 25
// License: GPL.v3

#include "include/font.h"

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "include/glm/gtc/matrix_transform.hpp"

namespace gl {

using std::string;
using std::vector;
using glm::ivec2;

// ascii 字符集

Font::Font(float width, float height)
    : _pshader(nullptr),
      _width(width), _height(height) {
    InitShader();
    InitMemory();
}

Font::~Font() noexcept {
    try {
        glDeleteBuffers(1, &_vbo);
        std::cout << "Release font object." << std::endl;
    }
    catch (...) {
        std::cout<<"Release font failed."<<std::endl;
    }
}

void
Font::Load(const string& ttf_path, int height) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        string err_msg("Error: Could not init freetype library.");
        throw FontException(err_msg);
    }
    FT_Face face;
    if (FT_New_Face(ft, ttf_path.c_str(), 0, &face)) {
        string err_msg("Error: Failed to load font.");
        throw FontException(err_msg);
    }
    FT_Set_Pixel_Sizes(face, 0, height);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, char(c), FT_LOAD_RENDER)) {
            string err_msg("Error: Failed to load glyph[" + std::to_string(c) + "].");
            throw FontException(err_msg);
        }
        unsigned int id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character chara{
            id, ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        _characters[c] = chara;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void
Font::Draw(const std::string& text, const glm::vec2& pos,
           const glm::vec3& color, float scale) {
    // save content
    GLboolean value(false);
    glGetBooleanv(GL_BLEND, &value);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float x = pos.x;
    float y = pos.y;
    _pshader->Use();
    _pshader->SetUniform("texcolor", color);
    glm::mat4 projection = glm::ortho(0.0f, _width, _height, 0.0f, -1.0f, 1.0f);
    _pshader->SetUniform("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);

    for (const auto& c: text) {
        const Character& ch = _characters[c];

        float x_pos = x + ch.bearing.x * scale;
        float y_pos = y + (_characters['H'].bearing.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            {x_pos, y_pos + h, 0, 1},
            {x_pos + w, y_pos, 1, 0},
            {x_pos, y_pos, 0, 0},
            {x_pos, y_pos + h, 0, 1},
            {x_pos + w, y_pos + h, 1, 1},
            {x_pos + w, y_pos, 1, 0},
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

    // reset content
    if (!value) {
        glDisable(GL_BLEND);
    }
}

void
Font::InitShader() {
    VertexShader v_shader("shaders/font/font.vert");
    FragmentShader f_shader("shaders/font/font.frag");
    vector<Shader*> shaders;
    shaders.push_back(&v_shader);
    shaders.push_back(&f_shader);
    _pshader.reset(new ShaderProgram(shaders));
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();
}

void
Font::InitMemory() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float),
                          (void*)(2*sizeof(float)));
    glBindVertexArray(0);
}

} // namespace gl
