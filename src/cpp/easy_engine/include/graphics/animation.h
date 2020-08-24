// animation.h - 动画
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 23
// License: GPL.v3

#include <memory>
#include <vector>

#include "../common/math.h"
#include "../opengl/texture.h"
#include "sprite.h"

#ifndef __EASY_ENGINE_INCLUDE_GRAPHICS_ANIMATION_H__
#define __EASY_ENGINE_INCLUDE_GRAPHICS_ANIMATION_H__

namespace easy_engine {
namespace graphics {

// 绘制一桢动画所需要的图像信息
struct AnimationDrawInfo {
  AnimationDrawInfo(std::shared_ptr<opengl::Texture2D> sprite, vec2 pos,
                    vec2 size)
      : Sprite(sprite), Pos(pos), Size(size) {}
  std::shared_ptr<opengl::Texture2D> Sprite;  // 纹理
  vec2 Pos;   // 纹理待绘制区域左上角坐标
  vec2 Size;  // 待绘制区域大小
};

// 播放模式
enum AnimationPlayMode {
  PLAY_ONCE     = 1,  // 仅播放一次
  PLAY_CYCLE    = 2,  // 循环播放
  PLAY_FORWARD  = 4,  // 正放
  PLAY_BACKWARD = 8,  // 倒放
};

// enum
const float PlayInfiniteTime = 0.0f;
const float DefaultFps       = 24.0f;

// 存储连续图像类
class Animation final {
 public:
  Animation() {}
  ~Animation() {}

  // 添加单个图片里的矩形区域作为动画. 加载顺序为从左到右, 从上到下
  // @param sprite_path: 精灵的路径
  // @param pos: 矩形区域的左上角坐标
  // @param size: 矩形区域的总大小
  // @param image_num: 加载区域的数量
  // @param row: 动画占据的行数, 默认一行
  void AddRectAnimation(const std::string& sprite_path, const vec2& pos,
                        const vec2& size, unsigned int image_num,
                        unsigned int row = 1);

  // 添加连续的图片
  // @param animation_start_path: 动画的起始文件路径, 所有的文件命名必须为
  // xxxx_001.xxx, 将会从起始文件开始按数字顺序加载指定数量的图片
  // @param image_num: 加载的图片数量, 默认加载全部
  void AddAnimation(const std::string& animation_start_path,
                    unsigned int       image_num = 0);

  // 添加连续的图片
  // @param animation_vec: 待加载图片的序列, 加载顺序为向量内图片路径的顺序
  void AddAnimation(const std::vector<std::string>& animation_vec);

  // 得到总图像数量
  // @return size_t: 图像数量
  size_t Size() const { return _v_images.size(); }

  // 是否已经加载图像
  // @return bool: 结果是否为空
  bool Empty() const { return _v_images.empty(); }

  // 清除所有存储
  void Clear() {
    _v_images.clear();
    _v_pos.clear();
    _v_size.clear();
  }

  // 得到渲染一帧动画所需要的图像信息
  // @param index: 图像下标
  AnimationDrawInfo GetDrawInfo(unsigned int index) {
    return AnimationDrawInfo(_v_images[index], _v_pos[index], _v_size[index]);
  }

 private:
  std::vector<std::shared_ptr<opengl::Texture2D>> _v_images;  // 图像序列
  std::vector<vec2>                               _v_pos;     // 位置序列
  std::vector<vec2>                               _v_size;    // 大小序列
};

// 动画渲染类, 渲染一些列连续的图像
class AnimationRender final {
 public:
  explicit AnimationRender(std::shared_ptr<SpriteRender> render)
      : _p_render(render) {}
  ~AnimationRender() {}

  void SetAnimation(std::shared_ptr<Animation> animation) {
    _p_animation = animation;
    SetPlayMode(AnimationPlayMode::PLAY_ONCE | AnimationPlayMode::PLAY_FORWARD);
  }

  // 设置动画播放信息
  // @param mode: 播放模式, 分为(一次性, 循环), (正放, 倒放)等等
  // 括号内的选项互斥, 可以以|串联同时设置多个选项
  // @param fps: 每秒钟播放多少桢
  // @param time: 播放时间
  void SetPlayMode(int mode, float fps = DefaultFps,
                   float time = PlayInfiniteTime);

  // 绘制一桢动画
  // @param position: 左上角位置
  // @param size: 大小
  // @param rotate: 旋转程度
  // @param color: 颜色
  void DrawAnimation(const vec2& pos, const vec2& size = vec2(10.0f),
                     float rot = 0.0f, const vec3& color = vec3(1.0f));

  // 是否正在播放
  // @return bool: 播放状态
  bool IsPlaying() const { return _is_playing; }

  // 开始播放
  void Playing();

  // 停止播放
  void Stop();

  // 暂停播放
  void Pause();

  // 继续播放
  void Continue();

 private:
  // 计算下一桢图像的下标
  void CalcNextFrameIndex();

  std::shared_ptr<SpriteRender> _p_render;     // 渲染器
  std::shared_ptr<Animation>    _p_animation;  // 动画

  // 管理播放状态变量
  int    _index;           // 当前播放下标
  double _prev_time;       // 上次播放时间
  double _curr_time;       // 当前时间
  float  _delta_time;      // 间隔时间
  double _already_play;    // 已播放时间
  double _total_time;      // 总播放时间
  int    _play_mode;       // 播放模式
  float  _fps;             // 每秒钟播放帧数
  float  _frame_interval;  // 两桢之间的时间间隔
  bool   _is_playing;      // 是否正在播放
  bool   _is_pause;        // 是否暂停
};

}  // namespace graphics
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_GRAPHICS_ANIMATION_H__
