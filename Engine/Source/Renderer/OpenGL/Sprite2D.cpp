//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Runtime/Engine.hpp"
#include <Oneiro/Core/ResourceManager.hpp>
#include <iostream>

namespace oe::Renderer::GL
{
    void Sprite2D::Load(const std::string& path, bool keepAspectRatio)
    {
        mKeepAR = keepAspectRatio;
        constexpr float vertices[] = {1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f};

        mVAO.Generate();
        mVBO.Generate();
        mVAO.Bind();
        mVBO.Bind();
        mVBO.BufferData(sizeof(vertices), vertices);
        VertexAttribPointer(0, 3, 3);
        mVAO.UnBind();
        mVBO.UnBind();
        mTexture = Core::GetTexturesManager().Add(std::make_shared<Texture<gl::TEXTURE_2D>>(path));
    }

    void Sprite2D::Draw() const
    {
        if (!mTexture)
            return;

        mVAO.Bind();
        mTexture->Bind();
        DrawArrays(TRIANGLES, 6);
    }

    void Sprite2D::SetAlpha(float alpha)
    {
        mAlpha = alpha;
    }

    void Sprite2D::SetUsingTextureAlpha(bool useTextureAlpha)
    {
        mUseTextureAlpha = useTextureAlpha;
    }

    const Texture<gl::TEXTURE_2D>* Sprite2D::GetTexture() const
    {
        return mTexture;
    }

    float Sprite2D::GetAlpha() const
    {
        return mAlpha;
    }

    bool Sprite2D::IsKeepAR() const
    {
        return mKeepAR;
    }

    bool Sprite2D::IsUseTextureAlpha() const
    {
        return mUseTextureAlpha;
    }
} // namespace oe::Renderer::GL
