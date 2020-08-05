// particle.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 30
// License: GPL.v3

#include "particle.h"
#include <random>
#include <iostream>
#include <glad/glad.h>

namespace {
    std::uniform_real_distribution<float> rand100(0, 100);
    std::default_random_engine generator;
}

ParticleGenerator::ParticleGenerator(shared_ptr<ShaderProgram> shader,
                                     shared_ptr<Texture2D> texture, int amount)
    : _shader(shader), _sprite(texture), _amount(amount)
{
    Init();
}

void
ParticleGenerator::Init() {
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

    for (int i = 0; i < _amount; ++i) {
        _particles.push_back(Particle());
    }
}

void
ParticleGenerator::Draw() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _shader->Use();
    for (auto& iter: _particles) {
        if (iter.TimeToLife > 0) {
            _shader->SetUniform("offset", iter.Position);
            _shader->SetUniform("color", iter.Color);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _sprite->texture);
            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
ParticleGenerator::Update(float delta, GameObject& obj, int new_particles,
                          vec2 offset) {
    for (int i = 0; i < new_particles; ++i) {
        int unused_particles = FirstUnusedParticle();
        RespawnParticle(_particles[unused_particles], obj, offset);
    }
    for (int i = 0; i < _amount; ++i) {
        auto& p = _particles[i];
        p.TimeToLife -= delta;
        if (p.TimeToLife >= 0.0f) {
            p.Position -= p.Velocity * delta;
            p.Color.a -= delta * 1.5;
        }
    }
}

int
ParticleGenerator::FirstUnusedParticle() {
    static int last_pos = 0;
    for (int i = last_pos; i < _amount; ++i) {
        if (_particles[i].TimeToLife <= 0.0f) {
            last_pos = i;
            return last_pos;
        }
    }
    for (int i = 0; i < last_pos; ++i) {
        if (_particles[i].TimeToLife <= 0.0f) {
            last_pos = i;
            return last_pos;
        }
    }
    last_pos = 0;
    return 0;
}

void
ParticleGenerator::RespawnParticle(Particle& particle, GameObject& obj,
                                   vec2 offset) {
    float rpos = (rand100(generator) - 50) / 10.0f;
    float rcolor = fmin(0.5 + (rand100(generator)) / 100.0f, 1.0);
    particle.Position = obj.Position + vec2(rpos) + vec2(offset);
    particle.Color = vec4(rcolor, rcolor, rcolor, 1.0f);
    particle.TimeToLife = 1.0f;
    particle.Velocity = obj.Velocity * 0.1f;
}

