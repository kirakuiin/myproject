// particle.h - 粒子
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 21
// License: GPL.v3

#include <memory>
#include <vector>

#include "../common/math.h"
#include "../opengl/texture.h"
#include "sprite.h"

#ifndef __EASY_ENGINE_INCLUDE_GRAPHICS_PARTICLE_H__
#define __EASY_ENGINE_INCLUDE_GRAPHICS_PARTICLE_H__

namespace easy_engine {
namespace graphics {
// 基本粒子的结构
struct Particle {
  vec2  Position;           // 位置
  vec2  Size;               // 大小
  vec2  Velocity;           // 速度
  vec4  Color;              // 颜色
  float TimeToLive = 0.0f;  // 生存时间
};

// 简单粒子渲染器, 根据一个纹理渲染粒子
class SimpleParticleRender {
 public:
  // @param render: 渲染器
  // @param amount: 粒子最大存在数量
  SimpleParticleRender(std::shared_ptr<SpriteRender>      render,
                       std::shared_ptr<opengl::Texture2D> sprite,
                       unsigned int                       amount)
      : _p_render(render), _p_sprite(sprite), _amount(amount), _prev_time(-1) {
    Init();
  }

  virtual ~SimpleParticleRender() {}

  // 更新所有粒子状态
  // @param particle: 需要被绘制的粒子数据
  // @param gene_num: 每次更新生成的新粒子数量, 默认两个
  void Update(const Particle& particle, int gene_num);

  // 设置纹理的被绘制区域, 默认绘制全部纹理
  // @param pos: 被绘制区域左上角坐标
  // @param size: 被绘制区域大小
  void SetDrawRange(const vec2& pos, const vec2& size);

  // 绘制粒子
  void Draw();

 private:
  // 初始化粒子生成器
  void Init();

  // 寻找第一个未使用粒子下标
  unsigned int FindFirstUnusedIndex() const;

  std::shared_ptr<SpriteRender>      _p_render;     // 渲染器
  std::shared_ptr<opengl::Texture2D> _p_sprite;     // 精灵
  std::vector<Particle>              _v_particles;  // 粒子容器
  unsigned int                       _amount;       // 粒子总数量
  double                             _prev_time;    // 之前的时间
  double                             _curr_time;    // 当前时间
};

// 默认粒子生成器, 以某点为中心生成均匀随机分布的粒子
class DefaultParticleGenerator {
 public:
  // @param size: 粒子大小
  // @param ttl: 粒子生存时间(秒)
  // @param interval: 生成粒子的最大间隔
  // @param offset: 整体位置偏移量
  DefaultParticleGenerator(const vec2& size, float ttl,
                           const vec2& interval = vec2(5.0),
                           const vec2& offset   = vec2(0.0))
      : _rand(0, 1000),
        _size(size),
        _interval(interval),
        _offset(offset),
        _ttl(ttl) {}

  // 生成粒子
  // @param pos: 粒子位置
  // @param velocity: 粒子速度
  // @return Particle: 生成的粒子
  Particle operator()(const vec2& pos, const vec2& velocity);

 private:
  Random<> _rand;      // 随机数生成器
  vec2     _size;      // 粒子大小
  vec2     _interval;  // 生成粒子最大间隔
  vec2     _offset;    // 粒子位置偏移
  float    _ttl;       // 粒子生存时间
};
}  // namespace graphics
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_GRAPHICS_PARTICLE_H__
