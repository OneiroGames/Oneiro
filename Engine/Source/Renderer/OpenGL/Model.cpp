//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Model.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

namespace oe::Renderer::GL
{
    void Model::Load(const std::string& path)
    {
        mPath = path;
        mMesh = Core::GetMeshesManager().Add(std::make_shared<Mesh>(mVAO, mVBO, mEBO), path);
    }

    void Model::Load(const std::vector<Vertex>& vertices)
    {
        mMesh = new Mesh(mVAO, mVBO, mEBO);
        mMesh->Load(vertices);
        mMesh->Generate();
        mIsNeed2SaveVertices = true;
    }

    void Model::Draw() const
    {
        const auto& textures = mMesh->GetTextures();
        if (!textures.empty())
        {
            const auto texturesCount = static_cast<uint8_t>(textures.size());
            for (uint8_t i{}; i < texturesCount; ++i)
                textures[i]->Bind(i);
        }

        mVAO.Bind();

        if (mMesh->IsUseEBO())
        {
            mEBO.Bind();
            DrawElements(TRIANGLES, mMesh->GetIndicesCount(), UNSIGNED_INT);
            mEBO.UnBind();
        }
        else
        {
            DrawArrays(TRIANGLES, mMesh->GetVerticesCount());
        }

        mVAO.UnBind();
        mEBO.UnBind();

        gl::ActiveTexture(gl::TEXTURE0);
        gl::BindTexture(gl::TEXTURE_2D, 0);
    }

    std::string Model::GetPath()
    {
        return mPath;
    }

    bool Model::IsNeed2SaveVertices() const
    {
        return mIsNeed2SaveVertices;
    }
} // namespace oe::Renderer::GL
