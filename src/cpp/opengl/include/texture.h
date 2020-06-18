// texture.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 06
// License: GPL.v3

#ifndef __OPENGL_INCLUDE_TEXTURE_H__
#define __OPENGL_INCLUDE_TEXTURE_H__

#include <map>
#include <vector>
#include "exception.h"

namespace gl {

enum class TextureType {
    NONE = 0,
    DIFFUSE,
    SPECULAR,
};

// openGL纹理加载类
class Texture {
  public:
    Texture();
    explicit Texture(unsigned int texture);
    virtual ~Texture() noexcept;

    static void SetParam(int dimen, int type, int value);
    static std::string GetName(TextureType type) { return _m_type_name[type]; }

    // 加载外部图像
    void LoadImage(const std::string& path);

    unsigned int texture = 0;                   // 纹理地址
    TextureType type = TextureType::NONE;       // 纹理类型

    int dimension = 0;                          // 图片纬度
    int format = 0;                             // 纹理格式RGBA
    int width = 0;                              // 图像宽度
    int height = 0;                             // 图像高度
    int nr_channels = 0;                        // 图像通道数
    std::string path = "";                      // 图片路径
  private:
    virtual void BindImage(unsigned char* data) = 0;

    static std::map<TextureType, std::string> _m_type_name; // 保存类型到名字的映射
};

class Texture2D: public Texture {
  public:
    Texture2D();
    explicit Texture2D(unsigned int texture);
  private:
    void BindImage(unsigned char* data) override;
};

class Texture3D: public Texture {
  public:
    Texture3D();
    explicit Texture3D(unsigned int texture);
  private:
    void BindImage(unsigned char* data) override;
};

class TextureCube: public Texture {
    public:
        TextureCube();
        explicit TextureCube(unsigned int texture);

        void LoadImages(const std::vector<std::string>& paths);
    private:
        void BindImage(unsigned char* data) override;

        int     _idx; // 记录图片下标
};

class TextureException: public GlException {
  public:
    explicit TextureException(const std::string& error_message)
        : GlException(error_message) {}
};

} // namespace gl
#endif // __OPENGL_INCLUDE_TEXTURE_H__
