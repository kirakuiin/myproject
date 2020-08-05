// manager.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 28
// License: GPL.v3

#ifndef __PRACTICE_BREAKOUT_INCLUDE_MANAGER_H__
#define __PRACTICE_BREAKOUT_INCLUDE_MANAGER_H__

#include <map>
#include <string>
#include <memory>

#include "include/texture.h"
#include "include/shader.h"

using gl::ShaderProgram;
using gl::Texture2D;
using std::string;
using std::shared_ptr;

class ResourceManager {
  public:
    static shared_ptr<ShaderProgram> LoadShader(const string& name,
                                                const string& vert,
                                                const string& frag,
                                                const string& geom = "");
    static shared_ptr<ShaderProgram> GetShader(const string& name);

    static shared_ptr<Texture2D> LoadTexture(const string& name,
                                             const string& filepath);
    static shared_ptr<Texture2D> GetTexture(const string& name);

    static void Clear();

  private:
    ResourceManager() {}

    static std::map<string, shared_ptr<ShaderProgram>> _shaders;
    static std::map<string, shared_ptr<Texture2D>>     _textures;
};

#endif // __PRACTICE_BREAKOUT_INCLUDE_MANAGER_H__
