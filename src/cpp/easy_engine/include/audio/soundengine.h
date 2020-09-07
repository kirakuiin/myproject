// soundengine.h - 音频引擎
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 29
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_AUDIO_SOUNDENGINE_H__
#define __EASY_ENGINE_INCLUDE_AUDIO_SOUNDENGINE_H__

#include <condition_variable>
#include <future>
#include <map>
#include <memory>
#include <thread>

#include "../common/math.h"
#include "../common/queue.h"

namespace irrklang {
class ISound;
class ISoundEngine;
}  // namespace irrklang

namespace easy_engine {
namespace audio {

// 音乐资源
struct MusicInfo {
  MusicInfo(){};
  MusicInfo(const std::string& path, int id, bool isplay = true,
            bool is3d = false, bool isloop = true, float volume = float(1.0f),
            float speed = float(1.0f), vec3 sound_pos = vec3(0),
            float min_dis = float(1.0f))
      : Path(path),
        Id(id),
        IsPlay(isplay),
        Is3d(is3d),
        IsLoop(isloop),
        SoundPos(sound_pos),
        MinDistance(min_dis),
        Volume(volume),
        Speed(speed) {}

  std::string Path;         // 音乐路径
  int         Id;           // 音乐对象唯一标识
  bool        IsPlay;       // 是否播放音乐
  bool        Is3d;         // 是否为3d音效
  bool        IsLoop;       // 是否循环播放
  vec3        SoundPos;     // 声音位置
  float       MinDistance;  // 声音达到最大的距离
  float       Volume;       // 音量
  float       Speed;        // 播放速度
};

// 音频引擎
class SoundEngine final {
 public:
  // @param max_sound_request: 最大播放请求数量
  explicit SoundEngine(int max_sound_request);
  ~SoundEngine();

  // 关闭引擎
  //
  // @throw: 线程异常
  void Stop();

  // 设置听者位置信息
  //
  // @param listen_pos: 声音接受者所在位置
  // @param look_dir: 声音接受者的观察方向
  void SetListener(const vec3& listen_pos, const vec3& look_dir);

  // 播放音乐
  void Play(const MusicInfo& music);

  // 停止音乐
  //
  // @param id: 音乐唯一标识
  void StopMusic(int id);

  // 停止所有音乐
  //
  void StopAllMusic();

 private:
  // 初始化引擎
  void Init();

  // 结束引擎
  void Fini();

  // 运行线程
  void Run();

  std::map<int, std::shared_ptr<irrklang::ISound>> _m_music;  // 正在播放的音乐
  std::shared_ptr<std::thread>                     _p_thread;  // 执行线程
  std::shared_ptr<irrklang::ISoundEngine> _p_sound_engine;     // 声音引擎
  std::future<void>                       _result;             // 运行结果

  bool                    _is_running;  // 是否正在运行
  MessageQueue<MusicInfo> _q_messages;  // 消息队列
  int                     _clean_size;  // 清理界限

  std::mutex              _mutex;  // 锁
  std::condition_variable _cond;   // 条件变量
};

}  // namespace audio
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_AUDIO_SOUNDENGINE_H__
