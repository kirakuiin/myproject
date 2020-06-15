// model.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  6 14
// License: GPL.v3

#ifndef __CPP_OPENGL_INCLUDE_MODEL_H__
#define __CPP_OPENGL_INCLUDE_MODEL_H__

#include <vector>
#include <map>
#include <memory>

#include <assimp/scene.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "texture.h"

namespace gl {
// 记录每个顶点需要的位置, 法线向量以及纹理坐标
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;
};

// 管理加载的网格数据
class Mesh {
  public:
    Mesh(const std::vector<Vertex> vertices,
         const std::vector<unsigned int>& indices,
         const std::vector<std::shared_ptr<Texture2D>>& textures);
    ~Mesh() noexcept;

    void Draw(ShaderProgram& shader);

    // 网格数据
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture2D>> textures; // TODO: 暂不考虑3d纹理

  private:
    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _ebo;

    void Init();
};

// 管理模型对象
class Model {
  public:
    explicit Model(const std::string& path);
    explicit Model(const char* path);

    void Draw(ShaderProgram& shader);

  private:
    void LoadModel(const std::string& path);
    std::vector<std::shared_ptr<Texture2D>> LoadMaterialTextures(
            aiMaterial* mat, aiTextureType type, TextureType type_enum);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> ProcessMesh(aiMesh* node, const aiScene* scene);

    // 模型数据
    std::vector<std::shared_ptr<Mesh>> _meshes;
    std::string _directory;

    // 防止重复加载
    std::map<std::string, std::shared_ptr<Texture2D>> _texture_loaded;
};

class ModelException: public GlException {
  public:
    explicit ModelException(const std::string& error_message)
        : GlException(error_message) {}
};

} // namespace gl

#endif // __CPP_OPENGL_INCLUDE_MODEL_H__
