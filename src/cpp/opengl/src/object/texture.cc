// texture.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 06
// License: GPL.v3

#include "include/texture.h"

#include <string>
#include <iostream>
#include <stb_image.h>

#include "glad/glad.h"
#include "include/commondef.h"

namespace gl {

using std::string;

// Texture implement
Texture::Texture() {
    glGenTextures(1, &texture);
}

Texture::Texture(unsigned int texture) {
    this->texture = texture;
}

Texture::~Texture() noexcept {
    try {
        glDeleteTextures(1, &texture);
    } catch (...) {
        std::cout << "Release texture failed." << std::endl;
    }
}

void
Texture::LoadImage(const string& path, int format) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &height, &width,
                                    &nr_channels, 0);
    this->format = format;
    if (data) {
        BindImage(data);
        stbi_image_free(data);
    } else {
        string msg(string("Load image: ") + path + string(" failed."));
        throw TextureException(msg);
    }
}

void
Texture::LoadImage(const char* path, int format) {
    LoadImage(string(path), format);
}

// Texture2D implement
Texture2D::Texture2D()
    : Texture() {
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

Texture2D::Texture2D(unsigned int texture)
    : Texture(texture) {
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

void
Texture2D::BindImage(unsigned char* data) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width,
                 height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

// Texture3D implement
Texture3D::Texture3D()
    : Texture() {
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

Texture3D::Texture3D(unsigned int texture)
    : Texture(texture) {
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

void
Texture3D::BindImage(unsigned char* data) {
    throw TextureException(string("Not implement."));
}

} // namespace gl

