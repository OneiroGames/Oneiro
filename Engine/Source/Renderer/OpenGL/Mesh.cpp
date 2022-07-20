//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Mesh.hpp"
#include "Oneiro/Core/Logger.hpp"

#include "Oneiro/Renderer/OpenGL/Texture.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

namespace oe::Renderer::GL
{
    Mesh::Mesh(VertexArray& vao, Buffer<gl::ARRAY_BUFFER, gl::STATIC_DRAW>& vbo, Buffer<gl::ELEMENT_ARRAY_BUFFER, gl::STATIC_DRAW>& ebo)
        : mVAO(vao), mVBO(vbo), mEBO(ebo)
    {
    }

    bool Mesh::Load(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_TransformUVCoords);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            log::get("log")->warn("Loading " + path + " mesh failed! Assimp error: " + importer.GetErrorString());
            return false;
        }

        mDirectory = path.substr(0, path.find_last_of('/'));
        ProcessAiNode(scene->mRootNode, scene);

        return true;
    }

    bool Mesh::Load(const std::vector<Vertex>& vertices)
    {
        mVertices = vertices;
        return true;
    }

    bool Mesh::Generate()
    {
        mVAO.Generate();
        mVBO.Generate();
        mVAO.Bind();
        mVBO.Bind();
        mVBO.BufferData(mVertices.size() * sizeof(Vertex), mVertices.data());
        if (mVertices.size() < mIndices.size())
        {
            mEBO.Generate();
            mEBO.Bind();
            mEBO.BufferData(mIndices.size() * sizeof(uint32_t), mIndices.data());
            mUseEBO = true;
            mIndicesCount = mIndices.size();
        }
        else
        {
            mVerticesCount = mVertices.size();
        }
        VertexAttribPointer(0, 4, 12);     // Color
        VertexAttribPointer(1, 3, 12, 4);  // Pos
        VertexAttribPointer(2, 3, 12, 7);  // Normal
        VertexAttribPointer(3, 2, 12, 10); // TexCoords
        mVAO.UnBind();
        mVBO.UnBind();
        mEBO.UnBind();

        return true;
    }

    const std::vector<Texture<gl::TEXTURE_2D>*>& Mesh::GetTextures()
    {
        return mTextures;
    }

    int Mesh::GetVerticesCount() const
    {
        return mVerticesCount;
    }

    int Mesh::GetIndicesCount() const
    {
        return mIndicesCount;
    }

    bool Mesh::IsUseEBO() const
    {
        return mUseEBO;
    }

    void Mesh::ProcessAiNode(const aiNode* node, const aiScene* scene)
    {
        for (uint32_t i{}; i < node->mNumMeshes; ++i)
        {
            const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessAiMesh(mesh, scene);
        }

        for (uint32_t i{}; i < node->mNumChildren; ++i)
        {
            ProcessAiNode(node->mChildren[i], scene);
        }
    }

    void Mesh::ProcessAiMesh(const aiMesh* mesh, const aiScene* scene)
    {
        uint8_t vertexColorIndex{};
        for (uint32_t i{}; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex{};

            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;

            if (mesh->HasNormals())
            {
                vertex.Normal.x = mesh->mNormals[i].x;
                vertex.Normal.y = mesh->mNormals[i].y;
                vertex.Normal.z = mesh->mNormals[i].z;
            }

            if (mesh->HasTextureCoords(0))
            {
                vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
            }

            if (mesh->HasVertexColors(vertexColorIndex))
            {
                vertex.Color.x = mesh->mColors[vertexColorIndex]->r;
                vertex.Color.y = mesh->mColors[vertexColorIndex]->g;
                vertex.Color.z = mesh->mColors[vertexColorIndex]->b;
                vertex.Color.w = mesh->mColors[vertexColorIndex]->a;
            }

            mVertices.emplace_back(vertex);
            vertexColorIndex++;
        }

        for (uint32_t i{}; i < mesh->mNumFaces; ++i)
        {
            const aiFace face = mesh->mFaces[i];
            for (uint32_t j{}; j < face.mNumIndices; ++j)
                mIndices.push_back(face.mIndices[j]);
        }

        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        LoadMaterialTextures(material, aiTextureType_DIFFUSE);
        LoadMaterialTextures(material, aiTextureType_SPECULAR);
    }

    void Mesh::LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type)
    {
        const int textureCount = static_cast<int>(mat->GetTextureCount(type));
        for (int i{}; i < textureCount; ++i)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            const auto texturePath = mDirectory + "/" + std::string(str.C_Str());
            for (const auto& texture : mTextures)
            {
                if (texture->GetData()->Path == texturePath)
                {
                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                Texture<gl::TEXTURE_2D>* texture = Core::GetTexturesManager().Add(std::make_shared<Texture<gl::TEXTURE_2D>>(texturePath));
                mTextures.push_back(texture);
            }
        }
    }
} // namespace oe::Renderer::GL
