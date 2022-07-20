//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Text.hpp"
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "ft2build.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <spdlog/spdlog.h>

#include FT_FREETYPE_H

namespace oe::Renderer::GL
{
    void Text::Init()
    {
        mVAO.Generate();
        mVBO.Generate();
        mVAO.Bind();
        mVBO.Bind();
        mVBO.BufferData<float>(6 * 4, nullptr);
        VertexAttribPointer(0, 4, 4);

        const auto vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec4 aPos;
                out vec2 UV;

                uniform mat4 projection;

                void main()
                {
                    gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);
                    UV = aPos.zw;
                }
            )";

        const auto fragmentShaderSrc = R"(
                #version 330 core
                in vec2 UV;
                out vec4 color;

                uniform sampler2D text;
                uniform vec3 textColor;

                void main()
                {
                    color = vec4(textColor, 1.0) * vec4(1.0, 1.0, 1.0, texture(text, UV).r);
                }
            )";

        mShader.LoadShaderSrc<gl::VERTEX_SHADER>(vertexShaderSrc);
        mShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(fragmentShaderSrc);
        mShader.CreateProgram();

        FT_Library ftLibrary{};

        if (FT_Init_FreeType(&ftLibrary) != 0)
            throw std::runtime_error("Failed to init freetype!");

        FT_Face face{};

        if (FT_New_Face(ftLibrary, "Assets/Fonts/font.ttf", 0, &face) != 0) {}
        else
        {
            FT_Set_Pixel_Sizes(face, 0, 26);

            gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);

            for (uint32_t c = 0; c < 256; c++)
            {
                if ((c >= 0xC0) && (c <= 0xDF))
                {
                    if (FT_Load_Char(face, 0x0410 + (c - 0xC0), FT_LOAD_RENDER))
                    {
                        spdlog::get("log")->warn("FreeType: failed to load glyph!");
                        continue;
                    }
                }
                else if ((c >= 0xE0) && (c <= 0xFF))
                {
                    if (FT_Load_Char(face, 0x0430 + (c - 0xE0), FT_LOAD_RENDER))
                    {
                        spdlog::get("log")->warn("FreeType: failed to load glyph!");
                        continue;
                    }
                }
                else if (c == 0xA8)
                {
                    if (FT_Load_Char(face, 0x0401, FT_LOAD_RENDER))
                    {
                        spdlog::get("log")->warn("FreeType: failed to load glyph!");
                        continue;
                    }
                }
                else if (c == 0xB8)
                {
                    if (FT_Load_Char(face, 0x0451, FT_LOAD_RENDER))
                    {
                        spdlog::get("log")->warn("FreeType: failed to load glyph!");
                        continue;
                    }
                }
                else if (c == 0x85)
                {
                    if (FT_Load_Char(face, 0x2026, FT_LOAD_RENDER))
                    {
                        spdlog::get("log")->warn("FreeType: failed to load glyph!");
                        continue;
                    }
                }
                else
                {
                    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                    {
                        spdlog::get("log")->warn("FreeType: failed to load glyph!");
                        continue;
                    }
                }

                Character character1{};
                character1.Size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
                character1.Bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
                character1.Advance = static_cast<uint32_t>(face->glyph->advance.x);
                character1.Texture->Generate();
                character1.Texture->Bind();
                character1.Texture->TexImage2D(gl::RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, gl::RED, gl::UNSIGNED_BYTE,
                                               face->glyph->bitmap.buffer);
                character1.Texture->TexParameter(gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER);
                character1.Texture->TexParameter(gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER);
                character1.Texture->TexParameter(gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
                character1.Texture->TexParameter(gl::TEXTURE_MAG_FILTER, gl::NEAREST_MIPMAP_LINEAR);
                character1.Texture->GenerateMipmap();
                mCharacters.insert(std::pair<char, Character>(c, character1));
            }
            gl::BindTexture(gl::TEXTURE_2D, 0);
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ftLibrary);
    }

    void Text::SetString(const std::string& str)
    {
        mString = str;
    }

    void Text::Draw(const glm::vec2& pos, const glm::vec3& color)
    {
        float x = pos.x;
        float y = pos.y;
        mShader.Use();
        mShader.SetUniform("projection", glm::ortho(0.0f, static_cast<float>(Core::Root::GetWindow()->GetWidth()), 0.0f,
                                                    static_cast<float>(Core::Root::GetWindow()->GetHeight())));
        mShader.SetUniform("textColor", glm::vec3(1.0f, 0.0f, 0.0f));

        mVAO.Bind();

        for (char stringIt : mString)
        {
            const auto [Texture, Size, Bearing, Advance] = mCharacters[stringIt];

            if (stringIt == '\n')
            {
                x = pos.x;
                y -= 25.0f;
                continue;
            }

            const float xPos = x + Bearing.x;
            const float yPos = y - Size.y - Bearing.y;

            const float width = Size.x;
            const float height = Size.y;
            const float vertices[6][4] = {{xPos, yPos + height, 0.0, 0.0}, {xPos, yPos, 0.0, 1.0},
                                          {xPos + width, yPos, 1.0, 1.0},  {xPos, yPos + height, 0.0, 0.0},
                                          {xPos + width, yPos, 1.0, 1.0},  {xPos + width, yPos + height, 1.0, 0.0}};
            Texture->Bind();
            mVBO.Bind();
            mVBO.BufferSubData(sizeof(vertices), 0, vertices);
            DrawArrays(TRIANGLES, 6);
            mVBO.UnBind();

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (Advance >> 6) * 1.0F; // Bitshift by 6 to get value in pixels (2^6 = 64)
        }
        gl::BindVertexArray(0);
        gl::BindTexture(gl::TEXTURE_2D, 0);
    }
} // namespace oe::Renderer::GL
