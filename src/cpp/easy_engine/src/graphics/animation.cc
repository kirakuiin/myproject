// animation.cc - 动画实现
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 23
// License: GPL.v3

#include "include/graphics/animation.h"

#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <memory>
#include <regex>
#include <string>

#include "include/common/timer.h"
#include "include/graphics/exception.h"
#include "include/utility/log.h"

using boost::lexical_cast;
using boost::filesystem::directory_iterator;
using boost::filesystem::path;

namespace {
// 默认的动画文件后缀匹配模式
const std::regex basic_reg("(.+_)(\\d+)");

// 比较两个文件名的大小, 以后缀数字为基准进行比较
bool CompareString(const std::string& lhs, const std::string& rhs) {
  std::smatch result;
  std::regex_match(path(lhs).stem().string(), result, basic_reg);
  int l = lexical_cast<int>(result[2]);
  std::regex_match(path(rhs).stem().string(), result, basic_reg);
  int r = lexical_cast<int>(result[2]);
  return l < r;
}
}  // namespace

namespace easy_engine {
namespace graphics {

void Animation::AddRectAnimation(const std::string& sprite_path,
                                 const vec2& pos, const vec2& size,
                                 unsigned int image_num, unsigned int row) {
  std::shared_ptr<opengl::Texture2D> text(new opengl::Texture2D());
  text->LoadImage(sprite_path);
  unsigned int column = ceil(lexical_cast<float>(image_num) / row);
  // 转换为百分比模式
  float x_interval = size.x / (column * text->Width());
  float y_interval = size.y / (column * text->Height());
  vec2  per_pos    = pos / vec2(text->Width(), text->Height());
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      if (i * column + j < image_num) {
        _v_images.push_back(text);
        _v_pos.push_back(
            vec2(per_pos.x + j * x_interval, per_pos.y + i * y_interval));
        _v_size.push_back(vec2(x_interval, y_interval));
      }
    }
  }
}

void Animation::AddAnimation(const std::string& animation_start_path,
                             unsigned int       image_num) {
  path        first_path(animation_start_path);
  std::string file_name = first_path.stem().string();
  std::smatch result;
  if (!std::regex_match(file_name, result, basic_reg)) {
    throw GraphicsException("Input animation file name is invalid");
  }
  std::vector<std::string> animation_vec;
  animation_vec.push_back(animation_start_path);
  std::string file_prefix = result[1];
  int         start_num   = lexical_cast<int>(result[2]);
  // 遍历当前路径下所有文件
  std::regex         other_reg(file_prefix + "(\\d+)");
  directory_iterator end;
  for (directory_iterator it(first_path.parent_path()); it != end; ++it) {
    if (std::regex_match(it->path().stem().string(), result, other_reg)) {
      int num = lexical_cast<int>(result[1]);
      if (num > start_num) {
        animation_vec.push_back(it->path().string());
      }
    }
  }
  if (0 == image_num) {
    image_num = animation_vec.size();
  }
  // 排序后去除多余图像
  std::sort(animation_vec.begin(), animation_vec.end(), CompareString);
  animation_vec.erase(animation_vec.begin() + image_num, animation_vec.end());
  AddAnimation(animation_vec);
}

void Animation::AddAnimation(const std::vector<std::string>& animation_vec) {
  for (const auto& path : animation_vec) {
    std::shared_ptr<opengl::Texture2D> text(new opengl::Texture2D());
    text->LoadImage(path);
    _v_images.push_back(text);
    _v_pos.push_back(vec2(0));
    _v_size.push_back(vec2(1));
  }
}

void AnimationRender::DrawAnimation(const vec2& pos, const vec2& size,
                                    float rot, const vec3& color) {
  if (!_is_playing || !_p_animation) {
    return;
  }
  // 控制显示速度
  _curr_time = Now<>();
  if (_prev_time == 0.0f) {
    _prev_time = _curr_time;
  }
  // 暂停时继续渲染当前帧
  if (_is_pause) {
    _prev_time = _curr_time;
  } else {
    _delta_time += (_curr_time - _prev_time);
    _already_play += (_curr_time - _prev_time);
    _prev_time = _curr_time;
    if (_total_time == PlayInfiniteTime) {
    } else if (_total_time <= _already_play) {
      _is_playing = false;
    }

    while (_delta_time >= _frame_interval) {
      CalcNextFrameIndex();
      // 单次播放, 在截止时间到来之前播放完毕
      if (_index < 0) {
        _is_playing = false;
        return;
      }
      _delta_time -= _frame_interval;
    }
  }
  AnimationDrawInfo info = _p_animation->GetDrawInfo(_index);
  _p_render->SetTextureCoordPer(info.Pos, info.Size);
  _p_render->DrawSprite(info.Sprite, pos, size, rot, color);
}

void AnimationRender::SetPlayMode(int mode, float fps, float time) {
  _is_playing     = false;
  _is_pause       = false;
  _fps            = fps;
  _frame_interval = 1.0 / _fps;
  _total_time     = time;
  _already_play   = 0.0f;
  _curr_time      = 0.0f;
  _prev_time      = 0.0f;
  _delta_time     = 0.0f;
  _play_mode      = mode;
  _index          = 0;
}

void AnimationRender::CalcNextFrameIndex() {
  size_t top = _p_animation->Size();
  if (_play_mode >> 2 & AnimationPlayMode::PLAY_FORWARD >> 2) {
    _index += 1;
  } else if (_play_mode >> 3 & AnimationPlayMode::PLAY_BACKWARD >> 3) {
    if (_index == -1) {
      _index = top;
    }
    _index -= 1;
  } else {
    _index += 1;
  }
  if (_play_mode & AnimationPlayMode::PLAY_ONCE) {
    if (_index < 0 || _index == top) {
      _index = -1;
    }
  } else if (_play_mode >> 1 & AnimationPlayMode::PLAY_CYCLE >> 1) {
    if (_index < 0) {
      _index = top - 1;
    } else if (_index == top) {
      _index = 0;
    }
  } else {
    if (_index < 0 || _index == top) {
      _index = -1;
    }
  }
}

void AnimationRender::Playing() {
  if (!_is_playing) {
    _is_playing = true;
  }
}

void AnimationRender::Stop() {
  if (_is_playing) {
    SetPlayMode(_play_mode, _fps, _total_time);
    _is_playing = false;
  }
}

void AnimationRender::Pause() {
  if (_is_playing && !_is_pause) {
    _is_pause = true;
  }
}

void AnimationRender::Continue() {
  if (_is_playing && _is_pause) {
    _is_pause = false;
  }
}

}  // namespace graphics
}  // namespace easy_engine
