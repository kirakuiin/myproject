// framebuffer.cc - 帧缓冲实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 13
// License: GPL.v3

#include "opengl/framebuffer.h"

#include <string>

#include "opengl/exception.h"
#include "opengl/glad/glad.h"

namespace easy_engine {
namespace opengl {

TextureColorAttachment::TextureColorAttachment(unsigned int width,
                                               unsigned int height,
                                               unsigned int index)
    : ColorAttachment(width, height, index) {
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _frame_width, _frame_height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

TextureColorAttachment::~TextureColorAttachment() { glDeleteTextures(1, &_id); }

void TextureColorAttachment::Attach() {
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index,
                         GL_TEXTURE_2D, _id, 0);
}

FloatColorAttachment::FloatColorAttachment(unsigned int width,
                                           unsigned int height,
                                           unsigned int index)
    : ColorAttachment(width, height, index) {
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _frame_width, _frame_width, 0,
               GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
}

FloatColorAttachment::~FloatColorAttachment() { glDeleteTextures(1, &_id); }

void FloatColorAttachment::Attach() {
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index,
                         GL_TEXTURE_2D, _id, 0);
}

MultiSampleColorAttachment::MultiSampleColorAttachment(unsigned int width,
                                                       unsigned int height,
                                                       unsigned int samples,
                                                       unsigned int index)
    : ColorAttachment(width, height, index), _samples(samples) {
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _id);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, GL_RGB,
                          _frame_width, _frame_height, GL_TRUE);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

MultiSampleColorAttachment::~MultiSampleColorAttachment() {
  glDeleteTextures(1, &_id);
}

void MultiSampleColorAttachment::Attach() {
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index,
                         GL_TEXTURE_2D_MULTISAMPLE, _id, 0);
}

RenderDepthAttachment::RenderDepthAttachment(unsigned int width,
                                             unsigned int height)
    : DepthAttachment(width, height) {
  glGenRenderbuffers(1, &_id);
  glBindRenderbuffer(GL_RENDERBUFFER, _id);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _frame_width,
                        _frame_height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderDepthAttachment::~RenderDepthAttachment() {
  glDeleteRenderbuffers(1, &_id);
}

void RenderDepthAttachment::Attach() {
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, _id);
}

// Framebuffer implement
Framebuffer::Framebuffer() : _id(0) { glGenFramebuffers(1, &_id); }

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &_id); }

const unsigned int Framebuffer::Width() {
  if (_color_attachments.empty()) {
    return 0;
  } else {
    return _color_attachments[0]->Width();
  }
}

const unsigned int Framebuffer::Height() {
  if (_color_attachments.empty()) {
    return 0;
  } else {
    return _color_attachments[0]->Height();
  }
}

void Framebuffer::Attach(std::shared_ptr<ColorAttachment> attach) {
  _color_attachments.push_back(attach);
  glBindFramebuffer(GL_FRAMEBUFFER, _id);
  if (attach) {
    attach->Attach();
  }
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::string msg("Framebuffer is not complete!");
    throw GlException(msg);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Attach(std::shared_ptr<DepthAttachment> attach) {
  _depth_attachment = attach;
  glBindFramebuffer(GL_FRAMEBUFFER, _id);
  if (attach) {
    attach->Attach();
  }
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::string msg("Framebuffer is not complete!");
    throw GlException(msg);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, _id); }

void UnbindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void DefaultFramebufferColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void ClearFramebuffer() { glClear(GL_COLOR_BUFFER_BIT); }

void BlitFramebuffer(Framebuffer* from, Framebuffer* to) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, from->Id());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, to->Id());
  glBlitFramebuffer(0, 0, from->Width(), from->Height(), 0, 0, to->Width(),
                    to->Height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

}  // namespace opengl
}  // namespace easy_engine
