// shader.h - 着色器
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 14
// License: GPL.v3

#include <memory>
#include <string>
#include <vector>

#include "../common/math.h"

#ifndef __EASY_ENGINE_INCLUDE_OPENGL_SHADER_H__
#define __EASY_ENGINE_INCLUDE_OPENGL_SHADER_H__

namespace easy_engine {
namespace opengl {

using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

// 所有着色器的基类
// 着色器无法直接使用, 必须绑定到着色器程序中编译后才能生效
class Shader {
 public:
  virtual ~Shader();

  // 得到着色器的id
  // @return int: 着色器id
  unsigned int Id() const { return _id; }

  // 释放着色器
  void Delete();

 protected:
  // 初始化着色器
  void Init(const std::string& shader_path, int type);

  unsigned int _id;  // 着色器id
};

// 顶点着色器
class VertexShader final : public Shader {
 public:
  VertexShader(const std::string& shader_path);
};

// 几何着色器
class GeometryShader final : public Shader {
 public:
  GeometryShader(const std::string& shader_path);
};

// 片段着色器
class FragmentShader final : public Shader {
 public:
  FragmentShader(const std::string& shader_path);
};

// 着色器程序
// 至少要附着顶点和片段着色器编译后才能运行
// 程序一旦编译以后, 所有着色器资源都会被释放
class ShaderProgram final {
 public:
  ShaderProgram();
  ~ShaderProgram();

  // 返回着色器程序id
  // @return unsigned int: 着色器程序id
  unsigned int Id() const { return _id; }

  // 附加着色器到着色器程序
  // @param shader: 待附加的着色器程序
  void Attach(std::shared_ptr<Shader> shader);

  // 启用当前着色器
  void Bind();

  // 编译当前绑定的着色器
  void Compile();

  // 设置着色器uniform变量
  // @param name: uniform 变量名称
  // @param value: 变量值
  void SetUniform(const std::string& name, bool value) const;
  void SetUniform(const std::string& name, int value) const;
  void SetUniform(const std::string& name, unsigned int value) const;
  void SetUniform(const std::string& name, float value) const;
  void SetUniform(const std::string& name, const vec2& value) const;
  void SetUniform(const std::string& name, const vec3& value) const;
  void SetUniform(const std::string& name, const vec4& value) const;
  void SetUniform(const std::string& name, const mat3& value) const;
  void SetUniform(const std::string& name, const mat4& value) const;

 private:
  unsigned int                         _id;          // 着色器程序id
  bool                                 _is_compile;  // 是否已经编译
  std::vector<std::shared_ptr<Shader>> _v_shader_ids;  // 所有待编译着色器的id
};

}  // namespace opengl
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_OPENGL_SHADER_H__
