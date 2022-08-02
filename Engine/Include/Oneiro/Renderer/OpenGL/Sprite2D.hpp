//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <memory>

#include "Buffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace oe::Renderer::GL
{
    class Sprite2D
    {
      public:
        Sprite2D() = default;
        Sprite2D(const Sprite2D&) = delete;

        void Load(const std::string& path, bool keepAspectRatio = true);
        void Draw() const;
        void SetAlpha(float alpha);
        void SetUsingTextureAlpha(bool useTextureAlpha);

        [[nodiscard]] const Texture<gl::TEXTURE_2D>* GetTexture() const;
        [[nodiscard]] float GetAlpha() const;

        [[nodiscard]] bool IsKeepAR() const;
        [[nodiscard]] bool IsUseTextureAlpha() const;

      private:
        Texture<gl::TEXTURE_2D>* mTexture{};
        Buffer<gl::ARRAY_BUFFER, gl::STATIC_DRAW> mVBO{};
        VertexArray mVAO;
        float mAlpha{1.0f};
        bool mKeepAR{};
        bool mUseTextureAlpha{};
    };
} // namespace oe::Renderer::GL
