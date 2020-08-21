// sprite.h - 精灵
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 15
// License: GPL.v3

#include <memory>

#include "../opengl/shader.h"
#include "../opengl/texture.h"
#include "camera.h"

#ifndef __EASY_ENGINE_INCLUDE_GRAPHICS_SPRITE_H__
#define __EASY_ENGINE_INCLUDE_GRAPHICS_SPRITE_H__

namespace easy_engine {
namespace graphics {

// 渲染精灵类
class SpriteRender final {
 public:
  // @param camera: 摄像机
  // @param shader: 着色器, 默认为空会使用默认着色器
  // 如果要自行配置, 着色器代码必须包含两个uniform变量(sprite_color, model,
  // projection)
  explicit SpriteRender(std::shared_ptr<Camera2D>              camera,
                        std::shared_ptr<opengl::ShaderProgram> shader = nullptr)
      : _p_camera(camera), _p_shader(shader), _vao(0), _vbo(0) {
    InitVertex();
    InitShader();
  }
  ~SpriteRender();

  // 设置待渲染纹理的纹理坐标(决定渲染纹理的那一部分, 默认全部渲染)
  // 输入值代表百分比, 对于宽度来说, 0.5代表宽度一半的像素
  // @param pos: 待渲染区域的左上角坐标
  // @param size: 待渲染区域的宽度和高度
  void SetTextureCoordPer(const vec2& pos, const vec2& size);

  // 设置待渲染纹理的纹理坐标(决定渲染纹理的那一部分, 默认全部渲染)
  // 输入值代表绝对值, 对于宽度来说, 50代表50像素宽
  // @param texsize: 带渲染纹理的宽度和高度
  // @param pos: 待渲染区域的左上角坐标
  // @param size: 待渲染区域的宽度和高度
  void SetTextureCoordAbs(const vec2& texsize, const vec2& pos,
                          const vec2& size);

  // 渲染精灵
  // @param sprite: 精灵资源
  // @param position: 精灵左上角位置
  // @param size: 精灵大小
  // @param rotate: 精灵旋转程度
  // @param color: 精灵颜色
  void DrawSprite(std::shared_ptr<opengl::Texture2D> sprite,
                  const vec2& position, const vec2& size = vec2(10, 10),
                  float rotate = 0.0f, const vec3& color = vec3(1.0f));

 private:
  // 初始化顶点数据
  void InitVertex();
  // 初始化着色器
  void InitShader();

  std::shared_ptr<Camera2D>              _p_camera;  // 摄像机
  std::shared_ptr<opengl::ShaderProgram> _p_shader;  // 着色器

  unsigned int _vao;  // 顶点数据
  unsigned int _vbo;  // 对象数组
};

}  // namespace graphics
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_GRAPHICS_SPRITE_H__
