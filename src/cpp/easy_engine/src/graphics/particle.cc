// particle.cc - 粒子实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 21
// License: GPL.v3

#include "include/graphics/particle.h"

#include "include/common/time.h"

namespace {
// alpha分量变化率
const float AlphaChangeRate = 1.5f;
};  // namespace

namespace easy_engine {
namespace graphics {

void SimpleParticleRender::Update(const Particle& particle, int gene_num) {
  // 计算时间
  _curr_time = Now();
  if (_prev_time < 0) {
    _prev_time = _curr_time;
  }
  float delta_time = _curr_time - _prev_time;
  _prev_time       = _curr_time;

  for (int i = 0; i < gene_num; ++i) {
    int unused_index           = FindFirstUnusedIndex();
    _v_particles[unused_index] = particle;
  }
  for (int i = 0; i < _amount; ++i) {
    auto& p = _v_particles[i];
    p.TimeToLive -= delta_time;
    if (p.TimeToLive >= 0.0f) {
      p.Position += p.Velocity * delta_time;
      // 随着时间变化, 透明度逐渐变高
      p.Color.a -= delta_time * AlphaChangeRate;
    }
  }
}

void SimpleParticleRender::SetDrawRange(const vec2& pos, const vec2& size) {
  vec2 texsize(_p_sprite->Width(), _p_sprite->Height());
  _p_render->SetTextureCoordAbs(texsize, pos, size);
}

void SimpleParticleRender::Draw() {
  // 将混合设置为叠加状态
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  for (auto& iter : _v_particles) {
    if (iter.TimeToLive > 0) {
      _p_render->DrawSprite(_p_sprite, iter.Position, iter.Size, 0.0f,
                            iter.Color);
    }
  }
  // 重置为默认状态
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SimpleParticleRender::Init() {
  for (unsigned int i = 0; i < _amount; ++i) {
    _v_particles.push_back(Particle());
  }
}

unsigned int SimpleParticleRender::FindFirstUnusedIndex() const {
  static unsigned int last_pos = 0;
  for (int i = last_pos; i < _amount; ++i) {
    if (_v_particles[i].TimeToLive <= 0.0f) {
      last_pos = i;
      return last_pos;
    }
  }
  for (int i = 0; i < last_pos; ++i) {
    if (_v_particles[i].TimeToLive <= 0.0f) {
      last_pos = i;
      return last_pos;
    }
  }
  last_pos = (last_pos + 1) % _amount;
  return last_pos;
}

Particle DefaultParticleGenerator::operator()(const vec2& pos,
                                              const vec2& velocity) {
  float    rx = static_cast<int>(_rand() - 500) % static_cast<int>(_interval.x);
  float    ry = static_cast<int>(_rand() - 500) % static_cast<int>(_interval.y);
  float    rcolor = min(0.5 + _rand() / 1000.0f, 1.0);
  Particle p;
  p.Position   = pos + vec2(rx, ry) + _offset;
  p.Color      = vec4(rcolor, rcolor, rcolor, 1.0f);
  p.Size       = _size;
  p.TimeToLive = _ttl;
  p.Velocity   = velocity * 0.1f;
  return p;
}

}  // namespace graphics
}  // namespace easy_engine
