// particle.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 30
// License: GPL.v3

#ifndef __PRACTICE_BREAKOUT_INCLUDE_PARTICLE_H__
#define __PRACTICE_BREAKOUT_INCLUDE_PARTICLE_H__


#include <memory>
#include <vector>

#include "include/glm/glm.hpp"
#include "include/texture.h"
#include "include/shader.h"
#include "gameobj.h"

using glm::vec2;
using glm::vec4;
using std::shared_ptr;
using gl::ShaderProgram;
using gl::Texture2D;

struct Particle {
    vec2 Position;
    vec2 Velocity;
    vec4 Color;
    float TimeToLife;

    Particle(): Position(0.0f), Velocity(0.0f), Color(1.0f), TimeToLife(0.0f) {}
};

class ParticleGenerator {
  public:
    ParticleGenerator(shared_ptr<ShaderProgram> shader,
                      shared_ptr<Texture2D> texture, int amount);

    void Update(float delta, GameObject& obj, int new_particles = 2,
                vec2 offset = vec2(0.0f));

    void Draw();

  private:
    void Init();

    int FirstUnusedParticle();

    void RespawnParticle(Particle& particle, GameObject& obj,
                         vec2 offset = vec2(0.0f, 0.0f));

    std::vector<Particle>       _particles;
    shared_ptr<ShaderProgram>   _shader;
    shared_ptr<Texture2D>       _sprite;
    int                         _amount;
    unsigned int                _vao;
};

#endif // __PRACTICE_BREAKOUT_INCLUDE_PARTICLE_H__
