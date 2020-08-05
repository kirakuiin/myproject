// manager.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 28
// License: GPL.v3

#include "manager.h"

std::map<string, shared_ptr<ShaderProgram>> ResourceManager::_shaders;
std::map<string, shared_ptr<Texture2D>> ResourceManager::_textures;

shared_ptr<ShaderProgram>
ResourceManager::LoadShader(const string& name,
                            const string& vert,
                            const string& frag,
                            const string& geom) {
    if (!_shaders.count(name)) {
        std::vector<gl::Shader*> shaders;
        gl::VertexShader v_shader(vert);
        shaders.push_back(&v_shader);
        gl::FragmentShader f_shader(frag);
        shaders.push_back(&f_shader);
        if (!geom.empty()) {
            gl::GeometryShader g_shader(geom);
            shaders.push_back(&g_shader);
        }
        _shaders[name] = shared_ptr<ShaderProgram>(new ShaderProgram(shaders));
        for (auto& i: shaders) {
            i->Delete();
        }
        shaders.clear();
    }
    return _shaders[name];
}

shared_ptr<ShaderProgram>
ResourceManager::GetShader(const string& name) {
    assert(_shaders.count(name) == 1);
    return _shaders[name];
}

shared_ptr<Texture2D>
ResourceManager::LoadTexture(const string& name,
                             const string& filepath) {
    if (!_textures.count(name)) {
        _textures[name] = shared_ptr<Texture2D>(new Texture2D());
        _textures[name]->LoadImage(filepath);
    }
    return _textures[name];
}

shared_ptr<Texture2D>
ResourceManager::GetTexture(const string& name) {
    assert(_textures.count(name) == 1);
    return _textures[name];
}

void
ResourceManager::Clear() {
    _shaders.clear();
    _textures.clear();
}
