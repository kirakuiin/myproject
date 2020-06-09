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
        string msg("Type: " + std::to_string(type) + " msg: "
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

// ShaderProgram
ShaderProgram::~ShaderProgram() noexcept {
    try {
        int result;
        glGetProgramiv(_program_id, GL_DELETE_STATUS, &result);
        if (GL_FALSE == result) {
            glDeleteProgram(_program_id);
        }
    }
    catch (...) {
        std::cout<<"Release shader program failed."<<endl;
    }
}

ShaderProgram::ShaderProgram(const std::vector<Shader*>& shaders) {
    _program_id = glCreateProgram();
    for (auto shader: shaders) {
        glAttachShader(_program_id, shader->shader);
    }
    glLinkProgram(_program_id);
    int success;
    char info_log[gldef::INFO_LEN];
    glGetProgramiv(_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_program_id, gldef::INFO_LEN, NULL, info_log);
        string msg(string("Link error,") + string("msg: ") + string(info_log));
        throw ShaderProgramException(msg);
    }
}

void ShaderProgram::Use() {
    glUseProgram(_program_id);
}

void ShaderProgram::SetUniform(const string& name, bool value) const {
    glUniform1i(glGetUniformLocation(_program_id, name.c_str()), (int)value);
}

void ShaderProgram::SetUniform(const string& name, int value) const {
    glUniform1i(glGetUniformLocation(_program_id, name.c_str()), value);
}

void ShaderProgram::SetUniform(const string& name, float value) const {
    glUniform1f(glGetUniformLocation(_program_id, name.c_str()), value);
}

void ShaderProgram::SetUniform(const string& name,
                               const std::vector<float>& values) const {
    size_t len = values.size();
    if (len < 1) {
        return;
    }
    auto pos = glGetUniformLocation(_program_id, name.c_str());
    switch (len) {
        case 1:
            glUniform1f(pos, values[0]);
        case 2:
            glUniform2f(pos, values[0], values[1]);
            break;
        case 3:
            glUniform3f(pos, values[0], values[1], values[2]);
            break;
        case 4:
        default:
            glUniform4f(pos, values[0], values[1], values[2], values[3]);
    }
}

void ShaderProgram::SetUniform(const std::string& name, UniformType type,
                               const float* value) {
    auto pos = glGetUniformLocation(_program_id, name.c_str());
    switch (type) {
        case UniformType::MATRIX4:
            glUniformMatrix4fv(pos, 1, GL_FALSE, value);
            break;
    }
}

unsigned int ShaderProgram::GetPos(const string& name) const {
    return glGetUniformLocation(_program_id, name.c_str());
}

} // namespace gl
