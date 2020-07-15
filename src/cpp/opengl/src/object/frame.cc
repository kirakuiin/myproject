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
    colors.push_back(ca);
    color = colors[0];
    depths = da;
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    if (ca.get()) {
        ca->Attach();
    }
    if (da.get()) {
        da->Attach();
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::string msg("Framebuffer is not complete!");
        throw FramebufferException(msg);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextureColorAttachment::TextureColorAttachment(int width, int height, int index)
    : ColorAttachment(width, height, index) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureColorAttachment::~TextureColorAttachment() noexcept {
    std::cout<<"Release texture attachment!"<<std::endl;
    glDeleteTextures(1, &id);
}

void
TextureColorAttachment::Attach() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index,
                           GL_TEXTURE_2D, id, 0);
}

FloatColorAttachment::FloatColorAttachment(int width, int height, int format, int index)
    : ColorAttachment(width, height, index) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, GL_RGB,
            GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

FloatColorAttachment::~FloatColorAttachment() noexcept {
    std::cout<<"Release texture attachment!"<<std::endl;
    glDeleteTextures(1, &id);
}

void
FloatColorAttachment::Attach() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index,
                           GL_TEXTURE_2D, id, 0);
}

MultisampleColorAttachment::MultisampleColorAttachment(int width, int height, int samples, int index)
    : ColorAttachment(width, height, index), _samples(samples) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, GL_RGB, _width,
                            _height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

MultisampleColorAttachment::~MultisampleColorAttachment() noexcept {
    std::cout<<"Release ms attachment!"<<std::endl;
    glDeleteTextures(1, &id);
}

void
MultisampleColorAttachment::Attach() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index,
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
    std::cout<<"Release texture attachment!"<<std::endl;
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
    std::cout<<"Release render attachment!"<<std::endl;
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
    std::cout<<"Release ms attachment!"<<std::endl;
    glDeleteRenderbuffers(1, &id);
}

void
MultisampleDepthStencilAttachment::Attach() {
    RenderDepthStencilAttachment::Attach();
}

ShadowDepthAttachment::ShadowDepthAttachment(int width, int height)
    : DepthStencilAttachment(width, height) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border[] = {1, 1, 1, 1};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glBindTexture(GL_TEXTURE_2D, 0);
}

ShadowDepthAttachment::~ShadowDepthAttachment() noexcept {
    std::cout<<"Release shadow attachment!"<<std::endl;
    glDeleteTextures(1, &id);
}

void
ShadowDepthAttachment::Attach() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, id, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

CubeShadowAttachment::CubeShadowAttachment(int width, int height)
    : DepthStencilAttachment(width, height) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    for (int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_DEPTH_COMPONENT, _width, _height,
                0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeShadowAttachment::~CubeShadowAttachment() noexcept {
    std::cout<<"Release cube attachment!"<<std::endl;
    glDeleteTextures(1, &id);
}

void
CubeShadowAttachment::Attach() {
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

} // namespace gl
