// post.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 01
// License: GPL.v3

#include "post.h"

#include <glad/glad.h>
#include "include/glm/gtc/matrix_transform.hpp"

using gl::ColorAttachment;
using glm::vec2;

PostProcessor::PostProcessor(shared_ptr<ShaderProgram> shader,
                             int width, int height)
    : _width(width), _height(height), _shader(shader) {
    std::shared_ptr<gl::ColorAttachment> ca(
        new gl::MultisampleColorAttachment(width, height, 4)
    );
    _msfbo.Attach(ca, nullptr);
    ca.reset(new gl::TextureColorAttachment(width, height));
    _fbo.Attach(ca, nullptr);

    _shader->Use();
    _shader->SetUniform("scene", 0);
    float offset = 1.0f/300.0f;
    vec2 offsets[9] {
        vec2(-offset, offset),
        vec2(0, offset),
        vec2(offset, offset),
        vec2(-offset, 0),
        vec2(0, 0),
        vec2(offset, 0),
        vec2(-offset, -offset),
        vec2(0, -offset),
        vec2(offset, -offset),
    };
    for (int i = 0; i < 9; ++i) {
        _shader->SetUniform("offsets[" + std::to_string(i) + "]", offsets[i]);
    }
    int edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1,
    };
    for (int i = 0; i < 9; ++i) {
        _shader->SetUniform("edge_kernel[" + std::to_string(i) + "]",
                            edge_kernel[i]);
    }
    float blur_kernel[9] = {
        1.0/16, 2.0/16, 1.0/16,
        2.0/16, 4.0/16, 2.0/16,
        1.0/16, 2.0/16, 1.0/16,
    };
    for (int i = 0; i < 9; ++i) {
        _shader->SetUniform("blur_kernel[" + std::to_string(i) + "]",
                            blur_kernel[i]);
    }
    InitData();
}

void
PostProcessor::BeginRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, _msfbo.id);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void
PostProcessor::EndRender() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _msfbo.id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo.id);
    glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void
PostProcessor::Render(float time) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _shader->Use();
    _shader->SetUniform("time", time);
    _shader->SetUniform("chaos", static_cast<int>(Chaos));
    _shader->SetUniform("confuse", static_cast<int>(Confuse));
    _shader->SetUniform("shake", static_cast<int>(Shake));
    _shader->SetUniform("timestop", static_cast<int>(TimeStop));
    if (TimeStop && !_stopping) {
        _stopping = true;
        _shader->SetUniform("begin_time", time);
    }
    if (!TimeStop && _stopping) {
        _stopping = false;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fbo.color->id);
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void
PostProcessor::InitData() {
    unsigned int vbo;
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float),
                          (void*)(2*sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


