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
Texture::LoadImage(const string& path, bool is_srgb) {
    this->path = path;
    unsigned char* data = stbi_load(path.c_str(), &width, &height,
                                    &nr_channels, 0);
    switch (nr_channels) {
        case 1:
            format = GL_RED;
            s_format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            if (is_srgb) {
                s_format = GL_SRGB;
            } else {
                s_format = format;
            }
            break;
        case 4:
            format = GL_RGBA;
            if (is_srgb) {
                s_format = GL_SRGB_ALPHA;
            } else {
                s_format = format;
            }
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
Texture::SetParam(int dimen, int type, int value) {
    glTexParameteri(dimen, type, value);
}

// Texture2D implement
Texture2D::Texture2D()
    : Texture() {
    dimension = GL_TEXTURE_2D;
    glBindTexture(dimension, this->texture);
}

Texture2D::Texture2D(unsigned int texture)
    : Texture(texture) {
    dimension = GL_TEXTURE_2D;
    glBindTexture(dimension, this->texture);
}

void
Texture2D::BindImage(unsigned char* data) {
    glBindTexture(dimension, texture);
    glTexImage2D(dimension, 0, s_format, width,
                 height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(dimension);
}

// Texture3D implement
Texture3D::Texture3D()
    : Texture() {
    dimension = GL_TEXTURE_3D;
    glBindTexture(dimension, this->texture);
}

Texture3D::Texture3D(unsigned int texture)
    : Texture(texture) {
    dimension = GL_TEXTURE_3D;
    glBindTexture(dimension, this->texture);
}

void
Texture3D::BindImage(unsigned char* data) {
    throw TextureException(string("Not implement."));
}

// TextureCube implement
TextureCube::TextureCube()
    : Texture(), _idx(0) {
    dimension = GL_TEXTURE_CUBE_MAP;
    glBindTexture(dimension, this->texture);
}

TextureCube::TextureCube(unsigned int texture)
    : Texture(texture), _idx(0) {
    dimension = GL_TEXTURE_CUBE_MAP;
    glBindTexture(dimension, this->texture);
}

void
TextureCube::LoadImages(const std::vector<std::string>& paths) {
    if (6 != paths.size()) {
        string msg(string("Only receive six path"));
        throw TextureException(msg);
    }
    for (const auto& path: paths) {
        LoadImage(path);
    }
    Texture::SetParam(dimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    Texture::SetParam(dimension, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Texture::SetParam(dimension, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    Texture::SetParam(dimension, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    Texture::SetParam(dimension, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    _idx = 0;
}

void
TextureCube::BindImage(unsigned char* data) {
    glBindTexture(dimension, texture);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+_idx, 0, s_format, width,
                 height, 0, format, GL_UNSIGNED_BYTE, data);
    ++_idx;
}

} // namespace gl

