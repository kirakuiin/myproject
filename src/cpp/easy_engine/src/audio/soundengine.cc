// soundengine.cc - 音频引擎实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 29
// License: GPL.v3

#include "include/audio/soundengine.h"

#include <future>
#include <memory>
#include <mutex>
#include <stdexcept>

#include "include/audio/exception.h"
#include "irrKlang.h"

namespace easy_engine {
namespace audio {

namespace {

const int CleanSize = 10;  // 达到多少个音乐时清理资源

// irr资源删除器
template <typename T>
struct IrrDeleter {
  void operator()(T* t) { t->drop(); }
};

// 将vec3转换为vecdf
//
// @param vec: vec3变量
// @return vec3df: vec3df变量
irrklang::vec3df ConvertVec3(const vec3& vec) {
  return irrklang::vec3df(vec.x, vec.y, vec.z);
}

}  // namespace

SoundEngine::SoundEngine(int max_sound_request)
    : _is_running(true),
      _q_messages(max_sound_request),
      _clean_size(CleanSize) {
  Init();
}

SoundEngine::~SoundEngine() { Fini(); }

void SoundEngine::Play(const MusicInfo& music) {
  if (_is_running) {
    std::unique_lock<std::mutex> lock(_mutex);
    _q_messages.Enqueue(music);
    lock.unlock();
    _cond.notify_one();
  }
}

void SoundEngine::StopMusic(int id) {
  MusicInfo stop_info;
  stop_info.Id     = id;
  stop_info.IsPlay = false;
  Play(stop_info);
}

void SoundEngine::Stop() {
  Fini();
  // 获得异常
  return _result.get();
}

void SoundEngine::SetListener(const vec3& listen_pos, const vec3& look_dir) {
  _p_sound_engine->setListenerPosition(ConvertVec3(listen_pos),
                                       ConvertVec3(look_dir));
}

void SoundEngine::StopAllMusic() { _p_sound_engine->stopAllSounds(); }

void SoundEngine::Init() {
  _p_sound_engine.reset(irrklang::createIrrKlangDevice(),
                        IrrDeleter<irrklang::ISoundEngine>());
  if (!_p_sound_engine) {
    throw AudioException("create sound engine failed");
  }
  std::packaged_task<void()> task(std::bind(&SoundEngine::Run, this));
  _result = task.get_future();
  _p_thread.reset(new std::thread(std::move(task)));
}

void SoundEngine::Fini() {
  if (_is_running) {
    _is_running = false;
    _cond.notify_one();
    _p_thread->join();
    _p_sound_engine->stopAllSounds();
  }
}

void SoundEngine::Run() {
  try {
    while (_is_running) {
      std::unique_lock<std::mutex> lock(_mutex);
      _cond.wait(lock, [&]() { return !_q_messages.Empty() || !_is_running; });
      while (!_q_messages.Empty() && _is_running) {
        const MusicInfo& info = _q_messages.Top();
        if (info.IsPlay) {  // 播放音乐
          std::shared_ptr<irrklang::ISound> music;
          if (info.Is3d) {
            music.reset(_p_sound_engine->play3D(info.Path.c_str(),
                                                ConvertVec3(info.SoundPos),
                                                info.IsLoop, true, true),
                        IrrDeleter<irrklang::ISound>());
            music->setMinDistance(info.MinDistance);
          } else {
            music.reset(_p_sound_engine->play2D(info.Path.c_str(), info.IsLoop,
                                                true, true),
                        IrrDeleter<irrklang::ISound>());
          }
          if (_m_music.count(info.Id)) {
            _m_music[info.Id]->stop();
            _m_music.erase(info.Id);
          }
          _m_music[info.Id] = music;
          music->setPlaybackSpeed(info.Speed);
          music->setVolume(info.Volume);
          music->setIsPaused(false);
        } else {  // 停止音乐
          if (_m_music.count(info.Id)) {
            _m_music[info.Id]->stop();
            _m_music.erase(info.Id);
          }
        }
        _q_messages.Dequeue();
      }
      // 清理残留音乐资源
      if (_m_music.size() > _clean_size) {
        std::vector<int> delete_list;
        for (const auto& iter : _m_music) {
          if (iter.second->isFinished()) {
            delete_list.push_back(iter.first);
          }
        }
        for (const auto& iter : delete_list) {
          _m_music.erase(iter);
        }
        // 动态平衡
        _clean_size = _m_music.size() + CleanSize;
      }
    }
  } catch (std::exception& e) {
    throw AudioException(e.what());
  }
}

}  // namespace audio
}  // namespace easy_engine
