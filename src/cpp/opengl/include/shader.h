// shader.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 04
// License: GPL.v3

#ifndef __OPENGL_INCLUDE_SHADER_H__
#define __OPENGL_INCLUDE_SHADER_H__

#include "exception.h"
#include "include/glm/glm.hpp"

namespace gl {

// openGL着色器加载类
class Shader {
  public:
    virtual ~Shader() noexcept;

    virtual void Init(const std::string& path, int type) = 0;

    void Delete();

    unsigned int shader = 0; // 着色器id
};

// 顶点着色器
class VertexShader: public Shader {
  public:
    explicit VertexShader(const std::string& shader_path);
    explicit VertexShader(const char* shader_path);

    void Init(const std::string& path, int type) override;
};

// 片段着色器
class FragmentShader: public Shader {
  public:
    explicit FragmentShader(const std::string& shader_path);
    explicit FragmentShader(const char* shader_path);

    void Init(const std::string& path, int type) override;
};

// openGL着色器链接程序
class ShaderProgram {
  public:
    explicit ShaderProgram(const std::vector<Shader*>& shaders);
    ~ShaderProgram() noexcept;

    // 启用本程序
    void Use();

    // 获得位置
    unsigned int GetPos(const std::string& name) const;

    // 设置glsl中的uniform变量
    void SetUniform(const std::string& name, bool value) const;
    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, float value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;
    void SetUniform(const std::string& name, const glm::vec4& value) const;
    void SetUniform(const std::string& name, const glm::mat3& value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;

    unsigned int program_id;
};

class ShaderException: public GlException {
  public:
    explicit ShaderException(const std::string& error_message)
        : GlException(error_message) {}
    static std::string GetErrorType(int type);
};

class ShaderProgramException: public GlException {
  public:
    explicit ShaderProgramException(const std::string& error_message)
        : GlException(error_message) {}
};

// uniform buffer
// 将着色器中指定名称的块绑定到index上
void BindProgramBlockToIndex(const ShaderProgram& program,
                         const std::string& name, int index);

// 创建一个指定大小uniformbuffer并绑定到index上
unsigned int BindUniformBufferToIndex(size_t size, int index);

} // namespace gl
#endif // __OPENGL_INCLUDE_SHADER_H__
