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
std::map<TextureType, std::string> Texture::_m_type_name{
    {TextureType::DIFFUSE, "texture_diffuse"},
    {TextureType::SPECULAR, "texture_specular"},
};

Texture::Texture() {
    glGenTextures(1, &texture);
}

Texture::Texture(unsigned int texture) {
    this->texture = texture;
}

Texture::~Texture() noexcept {
    try {
        glDeleteTextures(1, &texture);
        std::cout << "Release texture object." << std::endl;
    } catch (...) {
        std::cout << "Release texture failed." << std::endl;
    }
}

void
Texture::LoadImage(const string& path) {
    this->path = path;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &height, &width,
                                    &nr_channels, 0);
    switch (nr_channels) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
    }
    if (data) {
        BindImage(data);
        stbi_image_free(data);
    } else {
        string msg(string("Load image: ") + path + string(" failed."));
        throw TextureException(msg);
    }
}

void
Texture::LoadImage(const char* path) {
    LoadImage(string(path));
}

void
Texture::SetParam(int dimen, int type, int value) {
    glTexParameteri(dimen, type, value);
}

// Texture2D implement
Texture2D::Texture2D()
    : Texture() {
    dimension = GL_TEXTURE_2D;
    glBindTexture(GL_TEXTURE_2D, this->texture);
    Texture::SetParam(dimension, GL_TEXTURE_WRAP_S, GL_REPEAT);
    Texture::SetParam(dimension, GL_TEXTURE_WRAP_T, GL_REPEAT);
    Texture::SetParam(dimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    Texture::SetParam(dimension, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture2D::Texture2D(unsigned int texture)
    : Texture(texture) {
    glBindTexture(dimension, this->texture);
}

void
Texture2D::BindImage(unsigned char* data) {
    glBindTexture(dimension, texture);
    glTexImage2D(dimension, 0, format, width,
                 height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(dimension);
}

// Texture3D implement
Texture3D::Texture3D()
    : Texture() {
    dimension = GL_TEXTURE_3D;
    glBindTexture(dimension, this->texture);
    Texture::SetParam(dimension, GL_TEXTURE_WRAP_S, GL_REPEAT);
    Texture::SetParam(dimension, GL_TEXTURE_WRAP_T, GL_REPEAT);
    Texture::SetParam(dimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    Texture::SetParam(dimension, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture3D::Texture3D(unsigned int texture)
    : Texture(texture) {
    glBindTexture(dimension, this->texture);
}

void
Texture3D::BindImage(unsigned char* data) {
    throw TextureException(string("Not implement."));
}

} // namespace gl

