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
Framebuffer::Framebuffer()
    : id(0) {
    glGenFramebuffers(1, &id);
}

Framebuffer::~Framebuffer() noexcept {
    try {
        glDeleteFramebuffers(1, &id);
        std::cout << "Release Framebuffer object." << std::endl;
    } catch (...) {
        std::cout << "Release Framebuffer failed." << std::endl;
    }
}

void
Framebuffer::Attach(std::shared_ptr<ColorAttachment> ca,
                    std::shared_ptr<DepthStencilAttachment> da) {
    color = ca;
    depths = da;
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    color->Attach();
    depths->Attach();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::string msg("Framebuffer is not complete!");
        throw FramebufferException(msg);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextureColorAttachment::TextureColorAttachment(int width, int height)
    : ColorAttachment(width, height) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureColorAttachment::~TextureColorAttachment() noexcept {
    glDeleteTextures(1, &id);
}

void
TextureColorAttachment::Attach() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, id, 0);
}

MultisampleColorAttachment::MultisampleColorAttachment(int width, int height, int samples)
    : ColorAttachment(width, height), _samples(samples) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, GL_RGB, _width,
                            _height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

MultisampleColorAttachment::~MultisampleColorAttachment() noexcept {
    glDeleteTextures(1, &id);
}

void
MultisampleColorAttachment::Attach() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE, id, 0);
}

TextureDepthStencilAttachment::TextureDepthStencilAttachment(int width, int height)
    : DepthStencilAttachment(width, height) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _width, _height,
                 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureDepthStencilAttachment::~TextureDepthStencilAttachment() noexcept {
    glDeleteTextures(1, &id);
}

void
TextureDepthStencilAttachment::Attach() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D, id, 0);
}

RenderDepthStencilAttachment::RenderDepthStencilAttachment(int width, int height)
    : DepthStencilAttachment(width, height) {
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderDepthStencilAttachment::~RenderDepthStencilAttachment() noexcept {
    glDeleteRenderbuffers(1, &id);
}

void
RenderDepthStencilAttachment::Attach() {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, id);
}

MultisampleDepthStencilAttachment::MultisampleDepthStencilAttachment(int width, int height, int samples)
    : RenderDepthStencilAttachment(width, height), _samples(samples) {
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samples,
                                     GL_DEPTH24_STENCIL8, _width, _height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

MultisampleDepthStencilAttachment::~MultisampleDepthStencilAttachment() noexcept {
    glDeleteRenderbuffers(1, &id);
}

void
MultisampleDepthStencilAttachment::Attach() {
    RenderDepthStencilAttachment::Attach();
}

} // namespace gl
