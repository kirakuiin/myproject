// framebuffer.h - 帧缓冲
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 13
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_OPENGL_FRAMEBUFFER_H__
#define __EASY_ENGINE_INCLUDE_OPENGL_FRAMEBUFFER_H__

#include <memory>
#include <vector>

namespace easy_engine {
namespace opengl {

class Framebuffer;

// 帧缓冲附件, 分为颜色附件和深度附件
// 必须要附着到帧缓冲之中才能生效
class Attachment {
 public:
  // @param width: 缓冲宽度
  // @param height: 缓冲高度
  Attachment(unsigned int width, unsigned int height)
      : _id(0), _frame_width(width), _frame_height(height) {}
  virtual ~Attachment(){};

  // 返回附件id
  // @return unsigned int: 附件id
  const unsigned int Id() { return _id; }

  // 返回帧缓冲附件宽度
  // return unsigned int: 桢宽度
  const unsigned int Width() { return _frame_width; }

  // 返回帧缓冲附件高度
  // return unsigned int: 桢高度
  const unsigned int Height() { return _frame_height; }

 protected:
  // 执行附件的附加操作
  virtual void Attach() = 0;

  friend class Framebuffer;  // 帧缓冲为友元

  unsigned int _id;            // 附件id
  unsigned int _frame_width;   // 帧缓冲宽度
  unsigned int _frame_height;  // 帧缓冲高度
};

// 颜色缓冲附件
class ColorAttachment : public Attachment {
 public:
  ColorAttachment(unsigned int width, unsigned int height, unsigned int index)
      : Attachment(width, height), _index(index) {}

 protected:
  unsigned int _index;  // 颜色附件的下标, 用于同时输出多个颜色附件标记自身
};

// 纹理颜色缓冲附件
class TextureColorAttachment : public ColorAttachment {
 public:
  TextureColorAttachment(unsigned int width, unsigned int height,
                         unsigned int index = 0);
  ~TextureColorAttachment();

 private:
  void Attach() override;
};

// 浮点颜色缓冲附件(rgb值可以突破1.0, 一般用于hdr)
class FloatColorAttachment : public ColorAttachment {
 public:
  FloatColorAttachment(unsigned int width, unsigned int height,
                       unsigned int index = 0);
  ~FloatColorAttachment();

 private:
  void Attach() override;
};

// 多重采样颜色缓冲附件
class MultiSampleColorAttachment : public ColorAttachment {
 public:
  MultiSampleColorAttachment(unsigned int width, unsigned int height,
                             unsigned int samples = 4, unsigned int index = 0);
  ~MultiSampleColorAttachment();

 private:
  void Attach() override;

  unsigned int _samples;  // 多重采样数
};

// 深度模版缓冲附件
class DepthAttachment : public Attachment {
 public:
  DepthAttachment(unsigned int width, unsigned int height)
      : Attachment(width, height) {}
};

// 渲染器深度模板缓冲附件
class RenderDepthAttachment : public DepthAttachment {
 public:
  RenderDepthAttachment(unsigned int width, unsigned int height);
  ~RenderDepthAttachment();

 private:
  void Attach() override;
};

// 帧缓冲, 存储成像需要的所有像素
// 至少要附着一个颜色缓冲才能生效
class Framebuffer {
 public:
  Framebuffer();
  ~Framebuffer();

  // 向帧缓冲上附加附件
  void Attach(std::shared_ptr<ColorAttachment> attach);
  void Attach(std::shared_ptr<DepthAttachment> attach);

  // 激活帧缓冲, 使之作为着色器的输出接受者, 所有设置的接受者
  // @param id: 帧缓冲id, 如果为0则代表激活默认窗口缓冲
  void Bind();

  // 返回帧缓冲id
  // @return unsigned int: 帧缓冲id
  const unsigned int Id() { return _id; }

  // 返回颜色附件id
  // @param index: 颜色缓冲索引, 默认第一个
  // @return unsigned int: 颜色缓冲id
  const unsigned int ColorId(int index = 0) {
    return _color_attachments[index]->Id();
  }

  // 返回帧缓冲宽度
  // return unsigned int: 桢宽度
  const unsigned int Width();

  // 返回帧缓冲高度
  // return unsigned int: 桢高度
  const unsigned int Height();

 private:
  unsigned int _id;  // 帧缓冲id
  // 存储多个颜色缓冲附件
  std::vector<std::shared_ptr<ColorAttachment>> _color_attachments;
  // 存储深度附件
  std::shared_ptr<DepthAttachment> _depth_attachment;
};

// 将当前帧缓冲重置为屏幕缓冲
void UnbindFramebuffer();

// 设置帧缓冲的默认清空颜色
// @param r: 红色分量(0-1)
// @param g: 绿色分量(0-1)
// @param b: 蓝色分量(0-1)
// @param a: alpha分量(0-1)
void DefaultFramebufferColor(float r, float g, float b, float a);

// 清空激活帧缓冲的各种值(像素, 深度等等)
void ClearFramebuffer();

// 复制桢缓冲数据到另一个帧缓冲
void BlitFramebuffer(const Framebuffer& from, Framebuffer* to);

}  // namespace opengl
}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_OPENGL_FRAMEBUFFER_H__
