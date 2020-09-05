// loader.h - 资源加载器
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  9 05
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_UTILITY_LOADER_H__
#define __EASY_ENGINE_INCLUDE_UTILITY_LOADER_H__

#include <memory>
#include <string>

#include "../audio/soundengine.h"
#include "../opengl/shader.h"
#include "../opengl/texture.h"

namespace easy_engine {
namespace utility {

using audio::MusicInfo;
using opengl::ShaderProgram;
using opengl::Texture2D;
using std::shared_ptr;
using std::string;

// 图像资源加载器基类
class ImageLoader {
 public:
  virtual ~ImageLoader() {}

  // 加载图像资源并赋予名称
  //
  // @param path: 图像位置
  // @param name: 资源名称
  virtual void LoadImage(const string& path, const string& name) = 0;

  // 根据名称得到图像资源
  //
  // @param name: 资源名称
  // @param Texture2D*: 纹理指针
  virtual shared_ptr<Texture2D> GetImage(const string& name) = 0;
};

// 默认图像加载器实现
class DefaultImageLoader : public ImageLoader {
 public:
  void LoadImage(const string& path, const string& name) override;
  shared_ptr<Texture2D> GetImage(const string& name) override;

 private:
  std::map<string, shared_ptr<Texture2D>> _m_images;
};

// 着色器资源加载基类
class ShaderLoader {
 public:
  virtual ~ShaderLoader() {}

  // 加载着色器资源并赋予名称
  //
  // @param path: 着色器位置
  // @param name: 资源名称
  virtual void LoadShader(const string& vert, const string& geom,
                          const string& frag, const string& name) = 0;

  // 根据名称得到着色器资源
  //
  // @param name: 资源名称
  // @param ShaderProgram*: 着色器程序指针
  virtual shared_ptr<ShaderProgram> GetShader(const string& name) = 0;
};

// 默认着色器加载器实现
class DefaultShaderLoader : public ShaderLoader {
 public:
  ~DefaultShaderLoader() {}
  void LoadShader(const string& vert, const string& geom, const string& frag,
                  const string& name) override;
  shared_ptr<ShaderProgram> GetShader(const string& name) override;

 private:
  std::map<string, shared_ptr<ShaderProgram>> _m_shaders;
};

// 声音资源加载基类
class AudioLoader {
 public:
  virtual ~AudioLoader() {}

  // 加载音频资源并赋予名称
  //
  // @param path: 音频位置
  // @param name: 资源名称
  virtual void LoadAudio(const string& path, const string& name) = 0;

  // 根据名称得到音频资源
  //
  // @param name: 资源名称
  // @param MusicInfo: 音乐信息
  virtual MusicInfo GetAudio(const string& name) = 0;
};

// 默认音频加载器实现
class DefaultAudioLoader : public AudioLoader {
 public:
  ~DefaultAudioLoader() {}
  void      LoadAudio(const string& path, const string& name) override;
  MusicInfo GetAudio(const string& name) override;

 private:
  std::map<string, MusicInfo> _m_audios;
};

// 资源定位器
// 注意: 着色器和图像加载必须位于opengl加载之后
class ResourceLocator final {
 public:
  // 提供一个图像加载器的实现
  //
  // @param loader: 图像加载器指针
  static void ProvideImageLoader(shared_ptr<ImageLoader> loader) {
    _p_image = loader;
  }

  // 提供一个着色器加载器的实现
  //
  // @param loader: 着色器加载器指针
  static void ProvideShaderLoader(shared_ptr<ShaderLoader> loader) {
    _p_shader = loader;
  }

  // 提供一个音频加载器的实现
  //
  // @param loader: 音频加载器指针
  static void ProvideAudioLoader(shared_ptr<AudioLoader> loader) {
    _p_audio = loader;
  }

  // 得到图像加载器
  //
  // @return ImageLoader*: 图像加载器指针
  static shared_ptr<ImageLoader> Image() {
    if (_p_image == nullptr) {
      _p_image.reset(new DefaultImageLoader());
    }
    return _p_image;
  }

  // 得到着色器加载器
  //
  // @return ShaderLoader*: 着色器加载器指针
  static shared_ptr<ShaderLoader> Shader() {
    if (_p_shader == nullptr) {
      _p_shader.reset(new DefaultShaderLoader());
    }
    return _p_shader;
  }

  // 得到音频加载器
  //
  // @return AudioLoader*: 音频加载器指针
  static shared_ptr<AudioLoader> Audio() {
    if (_p_audio == nullptr) {
      _p_audio.reset(new DefaultAudioLoader());
    }
    return _p_audio;
  }

 private:
  static shared_ptr<ImageLoader>  _p_image;
  static shared_ptr<ShaderLoader> _p_shader;
  static shared_ptr<AudioLoader>  _p_audio;
};

}  // namespace utility
}  // namespace easy_engine
#endif  // __EASY_ENGINE_INCLUDE_UTILITY_LOADER_H__
