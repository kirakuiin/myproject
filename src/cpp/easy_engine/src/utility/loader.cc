// loader.cc - 加载器实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 05
// License: GPL.v3

#include "include/utility/loader.h"

namespace easy_engine {
namespace utility {

using opengl::Shader;

void DefaultImageLoader::LoadImage(const string &path, const string &name) {
  _m_images[name].reset(new Texture2D());
  _m_images[name]->LoadImage(path);
}

shared_ptr<Texture2D> DefaultImageLoader::GetImage(const string &name) {
  return _m_images[name];
}

void DefaultAudioLoader::LoadAudio(const string &path, const string &name) {
  _m_audios[name].Path = path;
}

MusicInfo DefaultAudioLoader::GetAudio(const string &name) {
  return _m_audios[name];
}

void DefaultShaderLoader::LoadShader(const string &vert, const string &geom,
                                     const string &frag, const string &name) {
  shared_ptr<Shader>        vertex(new opengl::VertexShader(vert));
  shared_ptr<Shader>        fragment(new opengl::FragmentShader(frag));
  shared_ptr<ShaderProgram> shader(new ShaderProgram());
  shader->Attach(vertex);
  shader->Attach(fragment);
  if (!geom.empty()) {
    shared_ptr<Shader> geometry(new opengl::GeometryShader(geom));
    shader->Attach(geometry);
  }
  shader->Compile();
  _m_shaders[name] = shader;
}

shared_ptr<ShaderProgram> DefaultShaderLoader::GetShader(const string &name) {
  return _m_shaders[name];
}

shared_ptr<ImageLoader>  ResourceLocator::_p_image(nullptr);
shared_ptr<AudioLoader>  ResourceLocator::_p_audio(nullptr);
shared_ptr<ShaderLoader> ResourceLocator::_p_shader(nullptr);

}  // namespace utility
}  // namespace easy_engine
