// shader.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 04
// License: GPL.v3

#include "include/shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "include/glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include "include/commondef.h"

namespace gl {

using std::string;

// Shader implement
Shader::~Shader() noexcept {
    try {
        int result;
        glGetShaderiv(shader, GL_DELETE_STATUS, &result);
        if (!result) {
            glDeleteShader(shader);
        }
        std::cout << "Release shader object." << std::endl;
    }
    catch (...) {
        std::cout<<"Release shader failed."<<std::endl;
    }
}

void Shader::Init(const string& shader_path, int type) {
    std::ifstream path(shader_path);
    if (!path.is_open()) {
        string msg("Open: " + shader_path + " failed!");
        throw ShaderException(msg);
    }
    stringstream source;
    source << path.rdbuf();
    path.close();
    string shader_str = string(source.str());
    const char* shader_code = shader_str.c_str();

    int success;
    char info_log[gldef::INFO_LEN];
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, gldef::INFO_LEN, NULL, info_log);
        string msg("Type: " + ShaderException::GetErrorType(type) + " Msg: "
                + string(info_log));
        throw ShaderException(msg);
    }
}

void Shader::Delete() {
    glDeleteShader(shader);
}

// VertexShader implement
VertexShader::VertexShader(const string& shader_path) {
    Init(shader_path, GL_VERTEX_SHADER);
}

VertexShader::VertexShader(const char* shader_path)
    : VertexShader(string(shader_path)) {
}

void VertexShader::Init(const string& shader_path, int type) {
    Shader::Init(shader_path, type);
}

// FragmentShader implement
FragmentShader::FragmentShader(const string& shader_path) {
    Init(shader_path, GL_FRAGMENT_SHADER);
}

FragmentShader::FragmentShader(const char* shader_path)
    : FragmentShader(string(shader_path)) {
}

void FragmentShader::Init(const string& shader_path, int type) {
    Shader::Init(shader_path, type);
}

// GeometryShader implement
GeometryShader::GeometryShader(const string& shader_path) {
    Init(shader_path, GL_GEOMETRY_SHADER);
}

GeometryShader::GeometryShader(const char* shader_path)
    : GeometryShader(string(shader_path)) {
}

void GeometryShader::Init(const string& shader_path, int type) {
    Shader::Init(shader_path, type);
}

// ShaderProgram
ShaderProgram::~ShaderProgram() noexcept {
    try {
        int result;
        glGetProgramiv(program_id, GL_DELETE_STATUS, &result);
        if (GL_FALSE == result) {
            glDeleteProgram(program_id);
        }
        std::cout<<"Release shader program object."<<endl;
    }
    catch (...) {
        std::cout<<"Release shader program failed."<<endl;
    }
}

ShaderProgram::ShaderProgram(const std::vector<Shader*>& shaders) {
    program_id = glCreateProgram();
    for (auto shader: shaders) {
        glAttachShader(program_id, shader->shader);
    }
    glLinkProgram(program_id);
    int success;
    char info_log[gldef::INFO_LEN];
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, gldef::INFO_LEN, NULL, info_log);
        string msg(string("Link error,") + string("Msg: ") + string(info_log));
        throw ShaderProgramException(msg);
    }
}

void ShaderProgram::Use() {
    glUseProgram(program_id);
}

void ShaderProgram::SetUniform(const string& name, bool value) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
}

void ShaderProgram::SetUniform(const string& name, int value) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void ShaderProgram::SetUniform(const string& name, float value) const {
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void ShaderProgram::SetUniform(const std::string& name,
                               const glm::vec2& value) const {
    auto pos = glGetUniformLocation(program_id, name.c_str());
    glUniform2f(pos, value.x, value.y);
}

void ShaderProgram::SetUniform(const string& name,
                               const glm::vec3& value) const {
    auto pos = glGetUniformLocation(program_id, name.c_str());
    glUniform3f(pos, value.x, value.y, value.z);
}

void ShaderProgram::SetUniform(const string& name,
                               const glm::vec4& value) const {
    auto pos = glGetUniformLocation(program_id, name.c_str());
    glUniform4f(pos, value.x, value.y, value.z, value.w);
}

void ShaderProgram::SetUniform(const string& name,
                               const glm::mat3& value) const {
    auto pos = glGetUniformLocation(program_id, name.c_str());
    glUniformMatrix3fv(pos, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const string& name,
                               const glm::mat4& value) const {
    auto pos = glGetUniformLocation(program_id, name.c_str());
    glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int ShaderProgram::GetPos(const string& name) const {
    return glGetUniformLocation(program_id, name.c_str());
}

// ShaderException implement
std::string
ShaderException::GetErrorType(int type) {
    std::map<int, std::string> msg_tab {
        {GL_FRAGMENT_SHADER, "fragment shader"},
        {GL_VERTEX_SHADER, "vertex shader"},
        {GL_GEOMETRY_SHADER, "geometry shader"},
    };
    return msg_tab[type];
}

void BindProgramBlockToIndex(const ShaderProgram& program,
                         const std::string& name, int index) {
    unsigned int uniform_block_index = glGetUniformBlockIndex(
        program.program_id, name.c_str()
            );
    glUniformBlockBinding(program.program_id, uniform_block_index, index);
}

unsigned BindUniformBufferToIndex(size_t size, int index) {
    unsigned int id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, index, id, 0, size);
    return id;
}

} // namespace gl
