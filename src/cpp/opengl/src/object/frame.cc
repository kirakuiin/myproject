// frame.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 17
// License: GPL.v3

#include "include/frame.h"

#include <string>
#include <iostream>

#include "glad/glad.h"

namespace gl {

// Framebuffer implement
Framebuffer::Framebuffer(int width, int height, FrameBufferType type)
    : _width(width), _height(height), _type(type) {
    Init();
}

Framebuffer::~Framebuffer() noexcept {
    try {
        Release();
        std::cout << "Release Framebuffer object." << std::endl;
    } catch (...) {
        std::cout << "Release Framebuffer failed." << std::endl;
    }
}

void
Framebuffer::Init() {
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    // bind color buffer
    color_id = GenerateTextureAttachment(FrameBufferType::TEXTURE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, color_id, 0);

    // bind depth and stencil buffer
    switch (_type) {
        case FrameBufferType::TEXTURE:
            _dc_id = GenerateTextureAttachment(FrameBufferType::RENDER);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                   GL_TEXTURE_2D, _dc_id, 0);
            break;
        case FrameBufferType::RENDER:
            _dc_id = GenerateRenderAttachment();
            glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                      GL_DEPTH_STENCIL_ATTACHMENT,
                                      GL_RENDERBUFFER, _dc_id);
            break;
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::string msg("Framebuffer is not complete!");
        throw FramebufferException(msg);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
Framebuffer::Release() {
    glDeleteFramebuffers(1, &id);
    glDeleteTextures(1, &color_id);
    switch (_type) {
        case FrameBufferType::TEXTURE:
            glDeleteTextures(1, &_dc_id);
            break;
        case FrameBufferType::RENDER:
            glDeleteRenderbuffers(1, &_dc_id);
    }
}

unsigned int
Framebuffer::GenerateTextureAttachment(FrameBufferType type) {
    unsigned int texbuffer;
    glGenTextures(1, &texbuffer);
    glBindTexture(GL_TEXTURE_2D, texbuffer);
    switch (type) {
        case FrameBufferType::TEXTURE:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, NULL);
            break;
        case FrameBufferType::RENDER:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _width, _height,
                         0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
            break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texbuffer;
}

unsigned int
Framebuffer::GenerateRenderAttachment() {
    unsigned int rendbuffer;
    glGenRenderbuffers(1, &rendbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, rendbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return rendbuffer;
}

} // namespace gl
