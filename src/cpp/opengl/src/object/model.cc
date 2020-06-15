// model.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  6 14
// License: GPL.v3

#include "include/model.h"

#include <string>
#include <map>
#include <iostream>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace gl {

// mesh implement
Mesh::Mesh(const std::vector<Vertex> vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<std::shared_ptr<Texture2D>>& textures)
    : vertices(vertices), indices(indices), textures(textures) {
    Init();
}

Mesh::~Mesh() noexcept {
    try {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);
        std::cout<<"Release mesh object."<<std::endl;
    } catch (...) {
        std::cout<<"Release mesh failed."<<std::endl;
    }
}

void Mesh::Init() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    // bind buffer
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // set vertex pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (void*)offsetof(Vertex, position));
    // set vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    // set texture coord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, texcoords));
    // unbind vao
    glBindVertexArray(0);
}

void Mesh::Draw(ShaderProgram& shader) {
    std::map<TextureType, int> mnum;
    // shaders必须遵守以下约定:
    //      1.命名均为texture_ax, 其中a为材质类型名称,x为0到sampler允许的最大数字.
    for (int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        if (!mnum.count(textures[i]->type)) {
            mnum[textures[i]->type] = 0;
        } else {
            mnum[textures[i]->type] += 1;
        }
        std::string name = Texture::GetName(textures[i]->type);
        std::string num = std::to_string(mnum[textures[i]->type]);
        std::string result = std::string("material.") + name + num;
        shader.SetUniform(result, i);
        glBindTexture(textures[i]->dimension, textures[i]->texture);
    }
    // 绘制网格
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

// Model implement
Model::Model(const std::string& path)
    : _meshes(), _directory(), _texture_loaded() {
    LoadModel(path);
}

Model::Model(const char* path)
    : Model(std::string(path)) {
}

void
Model::Draw(ShaderProgram& shader) {
    for (auto& mesh: _meshes) {
        mesh->Draw(shader);
    }
}

void
Model::LoadModel(const std::string& path) {
    Assimp::Importer import;
    // 图元三角化, y轴反转
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate |
                                                 aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::string message("Model error: " +
                            std::string(import.GetErrorString()));
        throw ModelException(message);
    }
    _directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void
Model::ProcessNode(aiNode* node, const aiScene* scene) {
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (int i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh>
Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture2D>> textures;

    for (int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        // set position
        glm::vec3 v;
        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;
        vertex.position = v;
        // set normal
        v.x = mesh->mNormals[i].x;
        v.y = mesh->mNormals[i].y;
        v.z = mesh->mNormals[i].z;
        vertex.normal = v;
        // set texcoord
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texcoords = vec;
        } else {
            vertex.texcoords = glm::vec2(0, 0);
        }
        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuse_maps = LoadMaterialTextures(
                material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
        auto specular_maps = LoadMaterialTextures(
                material, aiTextureType_SPECULAR, TextureType::SPECULAR);
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return std::shared_ptr<Mesh>(new Mesh(vertices, indices, textures));
}

std::vector<std::shared_ptr<Texture2D>>
Model::LoadMaterialTextures(
        aiMaterial* mat, aiTextureType type, TextureType type_enum) {
    std::vector<std::shared_ptr<Texture2D>> textures;
    for (int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string image_path = _directory + '/' + std::string(str.C_Str());
        if (!_texture_loaded.count(image_path)) {
            std::shared_ptr<Texture2D> texture(new Texture2D());
            texture->LoadImage(image_path);
            texture->type = type_enum;
            _texture_loaded[image_path] = texture;
        }
        textures.push_back(_texture_loaded[image_path]);
    }
    return textures;
}

} // namespace gl
