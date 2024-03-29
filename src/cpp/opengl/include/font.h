// font.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 25
// License: GPL.v3


#ifndef __CPP_OPENGL_INCLUDE_FONT_H__
#define __CPP_OPENGL_INCLUDE_FONT_H__

#include <string>
#include <map>
#include <memory>
#include "include/glm/glm.hpp"

#include "exception.h"
#include "shader.h"

namespace gl {

struct Character {
    unsigned int texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    long advance;
};

class Font {
  public:
    // 字体路径和字体高度
    explicit Font(float width, float height);
    ~Font() noexcept;

    void Load(const std::string& ttf_path, int height = 48);

    void Draw(const std::string& text, const glm::vec2& pos,
              const glm::vec3& color = glm::vec3(1.0), float scale = 1.0f);

  private:

    void InitShader();
    void InitMemory();

    std::map<int, Character>        _characters;
    std::shared_ptr<ShaderProgram>  _pshader;
    unsigned int                    _vao;
    unsigned int                    _vbo;

    float                           _width;
    float                           _height;
};

class FontException: public GlException {
  public:
    explicit FontException(const std::string& error_message)
        : GlException(error_message) {}
};

} // namespace gl

#endif // __CPP_OPENGL_INCLUDE_FONT_H__
