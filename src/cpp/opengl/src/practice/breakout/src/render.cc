// render.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 28
// License: GPL.v3

#include "render.h"

#include <iostream>
#include <glad/glad.h>
#include "include/glm/gtc/matrix_transform.hpp"
#include "manager.h"

using glm::mat4;
using glm::translate;
using glm::scale;
using glm::ortho;

SpriteRender::~SpriteRender() noexcept {
    glDeleteVertexArrays(1, &_vao);
}

void
SpriteRender::DrawSprite(shared_ptr<Texture2D> texture, vec2 position,
                         float rotate, vec2 size, vec3 color) {
    _shader->Use();
    mat4 model(1.0);
    model = translate(model, vec3(position, 0.0f));

    model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate, vec3(0.0f, 0.0f, 1.0f));
    model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = scale(model, vec3(size, 1.0f));

    _shader->SetUniform("model", model);
    _shader->SetUniform("sprite_color", color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->texture);

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void
SpriteRender::Init() {
    unsigned int vbo;
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
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

