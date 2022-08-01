//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/Renderer.hpp"

#ifndef OE_RENDERER_VULKAN

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace
{
    oe::Renderer::Statistic stats{};
    constexpr oe::Renderer::Limits limits{};

    oe::Renderer::GL::Shader sprite2DShader{};
    oe::Renderer::GL::Shader mainShader{};
    oe::Renderer::GL::Shader pointShader{};
    oe::Renderer::GL::Shader circleShader{};
    oe::Renderer::GL::Shader lineShader{};
    oe::Renderer::GL::Shader quadShader{};

    oe::Renderer::GL::VertexArray lineVAO{};
    oe::Renderer::GL::Buffer<gl::ARRAY_BUFFER, gl::DYNAMIC_DRAW> lineVBO{};

    oe::Renderer::GL::VertexArray circleVAO{};
    oe::Renderer::GL::Buffer<gl::ARRAY_BUFFER, gl::DYNAMIC_DRAW> circleVBO{};

    oe::Renderer::GL::VertexArray pointVAO{};
    oe::Renderer::GL::Buffer<gl::ARRAY_BUFFER, gl::DYNAMIC_DRAW> pointVBO{};

    oe::Renderer::GL::VertexArray quadVAO{};
    oe::Renderer::GL::Buffer<gl::ARRAY_BUFFER, gl::DYNAMIC_DRAW> quadVBO{};
    oe::Renderer::GL::Buffer<gl::ELEMENT_ARRAY_BUFFER, gl::STATIC_DRAW> quadEBO{};

    constexpr glm::vec4 quadVertices[4] = {
        {-1.0f, -1.0f, 0.0f, 1.0f}, {1.0f, -1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {-1.0f, 1.0f, 0.0f, 1.0f}};

    uint32_t pointVertexCount{};
    oe::Renderer::PointVertex* pointVertexBase{};
    oe::Renderer::PointVertex* pointVertexPtr{};

    uint32_t lineVertexCount{};
    oe::Renderer::LineVertex* lineVertexBase{};
    oe::Renderer::LineVertex* lineVertexPtr{};

    uint32_t circleIndicesCount{};
    oe::Renderer::CircleVertex* circleVertexBase{};
    oe::Renderer::CircleVertex* circleVertexPtr{};

    uint32_t quadIndicesCount{};
    oe::Renderer::QuadVertex* quadVertexBase{};
    oe::Renderer::QuadVertex* quadVertexPtr{};

    uint32_t textureSlotIndex{1};
    std::array<const oe::Renderer::GL::Texture<gl::TEXTURE_2D>*, limits.MaxTextures> textureSlots{};

    void SetupBuffers();
    constexpr std::array<uint32_t, limits.MaxIndices> CreateQuadIndices();
}; // namespace

namespace oe::Renderer
{
    void PreInit()
    {
        stbi_set_flip_vertically_on_load(1);
    }

    void Init()
    {
        gl::Enable(gl::BLEND);
        gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);
        gl::Enable(gl::DEPTH_TEST);
        gl::Enable(gl::LINE_SMOOTH);
        gl::Enable(gl::PROGRAM_POINT_SIZE);

        constexpr auto spriteVertShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                uniform mat4 uTransform;
                uniform mat4 uView;
                uniform mat4 uProjection;
                out vec2 TexCoords;
                uniform float uAR;
                uniform bool uKeepAspectRatio;
                void main()
                {
                    vec2 scale = uKeepAspectRatio ? vec2(uAR > 1 ? 1 / uAR : 1, uAR < 1 ? uAR : 1) : vec2(1.0);
                    TexCoords = aPos.xy;
                    gl_Position = uProjection * uView * uTransform * vec4(aPos.xy * scale, 0.0, 1.0);
                }
            )";

        constexpr auto spriteFragShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
                uniform sampler2D uTexture;
                uniform bool uUseTextureAlpha;
                uniform float uTextureAlpha;
                in vec2 TexCoords;
                void main()
                {
                    vec4 Texture = texture2D(uTexture, TexCoords);
                    if (Texture.a < 0.35)
                            discard;
                    if (uTextureAlpha <= Texture.a)
                            Texture.a = uTextureAlpha;
                        FragColor = pow(vec4(Texture.rgba), vec4(1.0/2.2));
                }
            )";

        constexpr auto quadVertShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec4 aPos;
                layout (location = 1) in vec4 aColor;
                layout (location = 2) in int aTexIndex;
                layout (location = 3) in float aAlpha;
                layout (location = 4) in float aAr;
                layout (location = 5) in int aKeepAr;
                uniform mat4 uProjection;
                out vec4 Color;
                out vec2 TexCoords;
                flat out int TexIndex;
                out float Alpha;
                void main()
                {
                    vec2 scale = (aKeepAr == 1) ? vec2(aAr > 1 ? 1 / aAr : 1, aAr < 1 ? aAr : 1) : vec2(1.0);
                    gl_Position = uProjection * vec4(aPos.xy * scale, 0.0, 1.0);
                    Color = aColor;
                    TexCoords = aPos.xy;
                    TexIndex = aTexIndex;
                    Alpha = aAlpha;
                }
            )";

        constexpr auto quadFragShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
                uniform sampler2D uTextures[32];
                in vec4 Color;
                in vec2 TexCoords;
                flat in int TexIndex;
                in float Alpha;
                void main()
                {
                    vec4 Texture;

                    switch(TexIndex)
                    {
                        case  0: Texture = texture(uTextures[0], TexCoords); break;
                        case  1: Texture = texture(uTextures[1], TexCoords); break;
                        case  2: Texture = texture(uTextures[2], TexCoords); break;
                        case  3: Texture = texture(uTextures[3], TexCoords); break;
                        case  4: Texture = texture(uTextures[4], TexCoords); break;
                        case  5: Texture = texture(uTextures[5], TexCoords); break;
                        case  6: Texture = texture(uTextures[6], TexCoords); break;
                        case  7: Texture = texture(uTextures[7], TexCoords); break;
                        case  8: Texture = texture(uTextures[8], TexCoords); break;
                        case  9: Texture = texture(uTextures[9], TexCoords); break;
                        case 10: Texture = texture(uTextures[10], TexCoords); break;
                        case 11: Texture = texture(uTextures[11], TexCoords); break;
                        case 12: Texture = texture(uTextures[12], TexCoords); break;
                        case 13: Texture = texture(uTextures[13], TexCoords); break;
                        case 14: Texture = texture(uTextures[14], TexCoords); break;
                        case 15: Texture = texture(uTextures[15], TexCoords); break;
                        case 16: Texture = texture(uTextures[16], TexCoords); break;
                        case 17: Texture = texture(uTextures[17], TexCoords); break;
                        case 18: Texture = texture(uTextures[18], TexCoords); break;
                        case 19: Texture = texture(uTextures[19], TexCoords); break;
                        case 20: Texture = texture(uTextures[20], TexCoords); break;
                        case 21: Texture = texture(uTextures[21], TexCoords); break;
                        case 22: Texture = texture(uTextures[22], TexCoords); break;
                        case 23: Texture = texture(uTextures[23], TexCoords); break;
                        case 24: Texture = texture(uTextures[24], TexCoords); break;
                        case 25: Texture = texture(uTextures[25], TexCoords); break;
                        case 26: Texture = texture(uTextures[26], TexCoords); break;
                        case 27: Texture = texture(uTextures[27], TexCoords); break;
                        case 28: Texture = texture(uTextures[28], TexCoords); break;
                        case 29: Texture = texture(uTextures[29], TexCoords); break;
                        case 30: Texture = texture(uTextures[30], TexCoords); break;
                        case 31: Texture = texture(uTextures[31], TexCoords); break;
                        default: Texture = Color; break;
                    }

                    FragColor = Texture;//pow(vec4(Texture.rgba), vec4(1.0/2.2));
                }
            )";

        constexpr auto vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec4 aColor;
                layout (location = 1) in vec3 aPos;
                uniform mat4 uProjection;
                uniform mat4 uTransform;
                uniform mat4 uView;
                void main()
                {
                    gl_Position = uProjection * uTransform * uView * vec4(aPos, 1.0);
                }
            )";

        constexpr auto fragmentShaderSrc = R"(
                #version 330 core
                layout(location = 0) out vec4 FragColor;
                uniform sampler2D uTexture;
                in vec2 TexCoords;
                void main()
                {
                    FragColor = vec4(1.0);
                }
            )";

        constexpr auto lineVertShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec4 aColor;
                layout (location = 1) in vec4 aPos;
                out vec4 Color;
                void main()
                {
                    gl_Position = vec4(aPos.xyz, 1.0);
                    Color = aColor;
                }
            )";

        constexpr auto lineFragShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
                in vec4 Color;
                void main()
                {
                    FragColor = Color;
                }
            )";

        constexpr auto circleVertShaderSrc = R"(
                #version 330 core
                layout(location = 0) in vec4 aColor;
                layout(location = 1) in vec4 aWorldPosition;
                layout(location = 2) in vec4 aLocalPosition;
                layout(location = 3) in float aThickness;
                layout(location = 4) in float aFade;
                uniform mat4 uProjection;
                uniform vec2 uResolution;
                out vec4 Color;
                out vec2 UV;
                out float Thickness;
                out float Fade;
                void main()
                {
                    gl_Position = uProjection * vec4(aWorldPosition.xyz, 1.0);
                    Color = aColor;
                    UV = aLocalPosition.xy * 2.0 - 1.0; UV.x *= (uResolution.x / uResolution.y);
                    Thickness = aThickness;
                    Fade = aFade;
                }
            )";

        constexpr auto circleFragShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
                in vec4 Color;
                in vec2 UV;
                in float Thickness;
                in float Fade;
                void main()
                {
                    float distance = 1.0 - length(UV);
                    float circle = smoothstep(0.0, Fade, distance);
                    circle *= smoothstep(Thickness + Fade, Thickness, distance);

                    if (circle == 0.0)
                        discard;

                    FragColor = Color;
                    FragColor.a *= circle;
                }
            )";

        constexpr auto pointVertShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec4 aColor;
                layout (location = 1) in vec4 aPos;
                layout (location = 2) in float aSize;
                uniform mat4 uProjection;
                out vec4 Color;
                void main()
                {
                    gl_PointSize = aSize;
                    gl_Position = uProjection * vec4(aPos.xyz, 1.0);;
                    Color = aColor;
                }
            )";

        constexpr auto pointFragShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
                in vec4 Color;
                void main()
                {
                    FragColor = Color;
                }
            )";

        sprite2DShader.LoadShaderSrc<gl::VERTEX_SHADER>(spriteVertShaderSrc);
        sprite2DShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(spriteFragShaderSrc);
        sprite2DShader.CreateProgram();

        mainShader.LoadShaderSrc<gl::VERTEX_SHADER>(vertexShaderSrc);
        mainShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(fragmentShaderSrc);
        mainShader.CreateProgram();

        lineShader.LoadShaderSrc<gl::VERTEX_SHADER>(lineVertShaderSrc);
        lineShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(lineFragShaderSrc);
        lineShader.CreateProgram();

        circleShader.LoadShaderSrc<gl::VERTEX_SHADER>(circleVertShaderSrc);
        circleShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(circleFragShaderSrc);
        circleShader.CreateProgram();

        pointShader.LoadShaderSrc<gl::VERTEX_SHADER>(pointVertShaderSrc);
        pointShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(pointFragShaderSrc);
        pointShader.CreateProgram();

        quadShader.LoadShaderSrc<gl::VERTEX_SHADER>(quadVertShaderSrc);
        quadShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(quadFragShaderSrc);
        quadShader.CreateProgram();

        SetupBuffers();

        pointVertexBase = new PointVertex[limits.MaxVertices];
        lineVertexBase = new LineVertex[limits.MaxVertices];
        circleVertexBase = new CircleVertex[limits.MaxVertices];
        quadVertexBase = new QuadVertex[limits.MaxVertices];

        quadShader.Use();
        for (int i = 0; i < limits.MaxTextures; ++i)
            quadShader.SetUniform(("uTextures[" + std::to_string(i) + "]").c_str(), i);
    }

    void Begin(const MainCameraComponent& mainCamera)
    {
        const auto& window = Core::Root::GetWindow();

        sprite2DShader.Use();
        sprite2DShader.SetUniform("uProjection", mainCamera.GetOrthoProjection());

        quadShader.Use();
        quadShader.SetUniform("uProjection", mainCamera.GetOrthoProjection());

        pointShader.Use();
        pointShader.SetUniform("uProjection", mainCamera.GetOrthoProjection());

        circleShader.Use();
        circleShader.SetUniform("uProjection", mainCamera.GetOrthoProjection());
        circleShader.SetUniform("uResolution", glm::vec2(window->GetWidth(), window->GetHeight()));

        mainShader.Use();
        mainShader.SetUniform("uView", mainCamera.GetViewMatrix());
        mainShader.SetUniform("uProjection", mainCamera.GetPerspectiveProjection());

        StartBatch();
    }

    void RenderSprite2D(const glm::mat4& transform, const GL::Sprite2D& sprite2D)
    {
        if (sprite2D.IsKeepAR())
        {
            const auto& window = Core::Root::GetWindow();
            const auto& texture = sprite2D.GetTexture();
            RenderQuad(transform, *texture, sprite2D.GetAlpha(), sprite2D.IsKeepAR(),
                       window->GetAr() / (static_cast<float>(texture->GetData()->Width) / static_cast<float>(texture->GetData()->Height)));
            return;
        }
        else
            RenderQuad(transform, *sprite2D.GetTexture(), sprite2D.GetAlpha());
    }

    void RenderModel(const glm::mat4& transform, const GL::Model& model)
    {
        mainShader.Use();
        mainShader.SetUniform("uTransform", transform);
        model.Draw();

        stats.Indices += model.GetIndices().size();
        stats.Vertices += model.GetVertices().size();
        stats.DrawCalls++;
    }

    void RenderLine(const glm::mat4& transform, const glm::vec3& fromPos, const glm::vec3& toPos,
                    const glm::vec4& color /*= glm::vec4(1.0f)*/)
    {
        if (lineVertexCount >= limits.MaxVertices)
            StartBatch();

        lineVertexPtr->Color = color;
        lineVertexPtr->Position = transform * glm::vec4(fromPos.x, fromPos.y, fromPos.z, 1.0f);
        lineVertexPtr++;

        lineVertexPtr->Color = color;
        lineVertexPtr->Position = transform * glm::vec4(toPos.x, toPos.y, toPos.z, 1.0f);
        lineVertexPtr++;

        lineVertexCount += 2;

        stats.Vertices += 2;
    }

    void RenderCircle(const glm::mat4& transform, const glm::vec4& color /*= glm::vec4(1.0f)*/, float thickness /*= 1.0f*/,
                      float fade /*= 0.005f*/)
    {
        if (circleIndicesCount >= limits.MaxIndices)
            NextBatch();

        for (auto& vertex : quadVertices)
        {
            circleVertexPtr->Color = color;
            circleVertexPtr->WorldPosition = transform * vertex;
            circleVertexPtr->LocalPosition = vertex;
            circleVertexPtr->Thickness = thickness;
            circleVertexPtr->Fade = fade;
            circleVertexPtr++;
        }
        circleIndicesCount += 6;
        stats.Indices += 6;
    }

    void RenderPoint(const glm::mat4& transform, const glm::vec4& color, float size /*= 1.0f*/)
    {
        if (pointVertexCount >= limits.MaxVertices)
            NextBatch();

        pointVertexPtr->Color = color;
        pointVertexPtr->Position = transform * glm::vec4(1.0f);
        pointVertexPtr->Size = size;
        pointVertexPtr++;
        pointVertexCount++;
        stats.Vertices++;
    }

    void RenderQuad(const glm::mat4& transform, const GL::Texture<gl::TEXTURE_2D>& texture, float alpha /*= 1.0f*/, bool keepAr /*= false*/,
                    float ar /*= 0.0f*/)
    {
        if (quadIndicesCount >= limits.MaxIndices)
            NextBatch();

        uint32_t textureIndex{};
        const auto textureSlotsCount = textureSlots.size();

        for (uint32_t i{}; i < textureSlotsCount; ++i)
        {
            if (textureSlots[i] == &texture)
            {
                textureIndex = i;
                break;
            }
        }

        if (!textureIndex)
        {
            if (textureSlotIndex >= limits.MaxTextures)
                NextBatch();

            textureIndex = textureSlotIndex;
            textureSlots[textureIndex] = &texture;
            textureSlotIndex++;
        }

        for (const auto& vertex : quadVertices)
        {
            quadVertexPtr->Position = transform * vertex;
            quadVertexPtr->TexIndex = textureIndex;
            quadVertexPtr->Alpha = alpha;
            quadVertexPtr->KeepAr = keepAr;
            quadVertexPtr->Ar = ar;
            quadVertexPtr++;
        }
        quadIndicesCount += 6;
        stats.Indices += 6;
        stats.Textures++;
    }

    void RenderQuad(const glm::mat4& transform, const glm::vec4& color /*= glm::vec4(1.0f)*/)
    {
        if (quadIndicesCount >= limits.MaxIndices)
            NextBatch();

        for (const auto& vertex : quadVertices)
        {
            quadVertexPtr->Position = transform * vertex;
            quadVertexPtr->Color = color;
            quadVertexPtr++;
        }
        quadIndicesCount += 6;
        stats.Indices += 6;
    }

    void Flush()
    {
        if (lineVertexCount)
        {
            auto dataSize = (uint32_t)((uint8_t*)lineVertexPtr - (uint8_t*)lineVertexBase);
            gl::Disable(gl::DEPTH_TEST);
            lineShader.Use();
            lineVAO.Bind();
            lineVBO.Bind();
            lineVBO.BufferSubData(dataSize, 0, lineVertexBase);
            GL::DrawArrays(GL::LINES, lineVertexCount);
            gl::Enable(gl::DEPTH_TEST);

            stats.DrawCalls++;
        }

        if (pointVertexCount)
        {
            auto dataSize = (uint32_t)((uint8_t*)pointVertexPtr - (uint8_t*)pointVertexBase);
            gl::Disable(gl::DEPTH_TEST);
            pointShader.Use();
            pointVAO.Bind();
            pointVBO.Bind();
            pointVBO.BufferSubData(dataSize, 0, pointVertexBase);
            GL::DrawArrays(GL::POINTS, pointVertexCount);
            gl::Enable(gl::DEPTH_TEST);

            stats.DrawCalls++;
        }

        if (circleIndicesCount)
        {
            auto dataSize = (uint32_t)((uint8_t*)circleVertexPtr - (uint8_t*)circleVertexBase);
            gl::Disable(gl::DEPTH_TEST);
            circleShader.Use();
            circleVAO.Bind();
            circleVBO.Bind();
            circleVBO.BufferSubData(dataSize, 0, circleVertexBase);
            quadEBO.Bind();
            GL::DrawElements(GL::TRIANGLES, circleIndicesCount, GL::UNSIGNED_INT);
            gl::Enable(gl::DEPTH_TEST);

            stats.DrawCalls++;
        }

        if (quadIndicesCount)
        {
            auto dataSize = (uint32_t)((uint8_t*)quadVertexPtr - (uint8_t*)quadVertexBase);
            quadShader.Use();
            quadVAO.Bind();
            quadVBO.Bind();
            quadVBO.BufferSubData(dataSize, 0, quadVertexBase);
            quadEBO.Bind();
            const auto textureSlotsCount = textureSlots.size();
            for (uint32_t i{}; i < textureSlotsCount; ++i)
                if (textureSlots[i])
                    textureSlots[i]->Bind(i);
            GL::DrawElements(GL::TRIANGLES, quadIndicesCount, GL::UNSIGNED_INT);

            stats.DrawCalls++;
        }

        stats.FlushesCount++;
    }

    void StartBatch()
    {
        lineVertexCount = 0;
        lineVertexPtr = lineVertexBase;

        pointVertexCount = 0;
        pointVertexPtr = pointVertexBase;

        circleIndicesCount = 0;
        circleVertexPtr = circleVertexBase;

        quadIndicesCount = 0;
        quadVertexPtr = quadVertexBase;

        textureSlotIndex = 1;
    }

    void NextBatch()
    {
        Flush();
        StartBatch();
    }

    void End()
    {
        Flush();
    }

    void Shutdown() {}

    void SetLineWidth(float width)
    {
        gl::LineWidth(width);
    }

    void SetPointSize(float size)
    {
        gl::PointSize(size);
    }

    const Statistic& GetStats()
    {
        return stats;
    }

    void ResetStats()
    {
        stats.FlushesCount = 0;
        stats.Vertices = 0;
        stats.Indices = 0;
        stats.Textures = 0;
        stats.DrawCalls = 0;
    }

    const Limits& GetLimits()
    {
        return limits;
    }

    void GL::DrawArrays(DrawMode mode, int first, int count)
    {
        gl::DrawArrays(mode, first, count);
    }

    void GL::DrawArrays(DrawMode mode, int count)
    {
        gl::DrawArrays(mode, 0, count);
    }

    void GL::ClearColor(float r, float g, float b, float a)
    {
        gl::ClearColor(r, g, b, a);
    }

    void GL::DrawElements(DrawMode mode, int count, DataType type, const void* indices)
    {
        gl::DrawElements(mode, count, type, indices);
    }

    void GL::Clear(uint32_t mask)
    {
        gl::Clear(mask);
    }

    void GL::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        gl::Viewport(x, y, width, height);
    }

    void GL::Viewport(GLsizei width, GLsizei height)
    {
        Viewport(0, 0, width, height);
    }
} // namespace oe::Renderer

namespace
{
    void SetupBuffers()
    {
        using namespace oe::Renderer;
        // Begin Line
        lineVAO.Generate();
        lineVBO.Generate();
        lineVAO.Bind();
        lineVBO.Bind();
        lineVBO.BufferData<LineVertex>(limits.MaxVertices, nullptr);
        GL::VertexAttribPointer(0, 4, 8);
        GL::VertexAttribPointer(1, 4, 8, 4);
        // End Line

        // Begin Circle
        circleVAO.Generate();
        circleVBO.Generate();
        circleVAO.Bind();
        circleVBO.Bind();
        circleVBO.BufferData<CircleVertex>(limits.MaxVertices, nullptr);
        GL::VertexAttribPointer(0, 4, 14);
        GL::VertexAttribPointer(1, 4, 14, 4);
        GL::VertexAttribPointer(2, 4, 14, 8);
        GL::VertexAttribPointer(3, 1, 14, 12);
        GL::VertexAttribPointer(4, 1, 14, 13);
        // End Circle

        // Begin Point
        pointVAO.Generate();
        pointVBO.Generate();
        pointVAO.Bind();
        pointVBO.Bind();
        pointVBO.BufferData<PointVertex>(limits.MaxVertices, nullptr);
        GL::VertexAttribPointer(0, 4, 9);
        GL::VertexAttribPointer(1, 4, 9, 4);
        GL::VertexAttribPointer(2, 1, 9, 8);
        // End Point

        // Begin Quad
        quadVAO.Generate();
        quadVBO.Generate();
        quadEBO.Generate();
        quadVAO.Bind();
        quadVBO.Bind();
        quadVBO.BufferData<QuadVertex>(limits.MaxVertices, nullptr);
        quadEBO.Bind();
        quadEBO.BufferData(limits.MaxIndices, CreateQuadIndices().data());
        GL::VertexAttribPointer(0, 4, 12);
        GL::VertexAttribPointer(1, 4, 12, 4);
        GL::VertexAttribPointer(2, 1, 12, 8);
        GL::VertexAttribPointer(3, 1, 12, 9);
        GL::VertexAttribPointer(4, 1, 12, 10);
        GL::VertexAttribPointer(5, 1, 12, 11);
        // End Quad
    }

    constexpr std::array<uint32_t, limits.MaxIndices> CreateQuadIndices()
    {
        std::array<uint32_t, limits.MaxIndices> indices{};

        uint32_t offset{};
        for (uint32_t i{}; i < limits.MaxIndices; i += 6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        return indices;
    }
} // namespace

#else
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"

#include "Oneiro/Renderer/Gui/GuiLayer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/CommandPool.hpp"
#include "Oneiro/Renderer/Vulkan/DepthBuffer.hpp"
#include "Oneiro/Renderer/Vulkan/DescriptorSetLayout.hpp"
#include "Oneiro/Renderer/Vulkan/Framebuffer.hpp"
#include "Oneiro/Renderer/Vulkan/ImageViews.hpp"
#include "Oneiro/Renderer/Vulkan/Intance.hpp"
#include "Oneiro/Renderer/Vulkan/LogicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/MSAA.hpp"
#include "Oneiro/Renderer/Vulkan/PhysicalDevice.hpp"
#include "Oneiro/Renderer/Vulkan/Pipeline.hpp"
#include "Oneiro/Renderer/Vulkan/RenderPass.hpp"
#include "Oneiro/Renderer/Vulkan/Shader.hpp"
#include "Oneiro/Renderer/Vulkan/SwapChain.hpp"
#include "Oneiro/Renderer/Vulkan/UniformBuffer.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <array>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

namespace
{
    std::vector<VkShaderModule> _vertShaders{};
    std::vector<VkShaderModule> _fragShaders{};
    std::vector<VkDescriptorSetLayout> _descriptorSetLayouts{};
    std::vector<VkDescriptorSetLayout> _descriptorSetLayouts3D{};
    std::vector<VkVertexInputBindingDescription> _vertexInputBindingDescriptions{};
    std::vector<VkVertexInputAttributeDescription> _vertexInputAttributeDescriptions{};

    VkQueue _graphicsQueue{};
    VkQueue _presentQueue{};
    VkSemaphore _imageAvailableSemaphores{};
    VkSemaphore _renderFinishedSemaphores{};
    VkFence _inFlightFence{};

    oe::Renderer::Vulkan::Instance _instance;
    oe::Renderer::Vulkan::PhysicalDevice _physicalDevice;
    oe::Renderer::Vulkan::LogicalDevice _logicalDevice;
    oe::Renderer::Vulkan::WindowSurface _windowSurface;
    oe::Renderer::Vulkan::SwapChain _swapChain;
    oe::Renderer::Vulkan::ImageViews _imageViews;
    oe::Renderer::Vulkan::RenderPass _renderPass;
    std::vector<oe::Renderer::Vulkan::Framebuffer> _framebuffers;
    oe::Renderer::Vulkan::CommandPool _commandPool;
    oe::Renderer::Vulkan::CommandBuffer _commandBuffer;
    oe::Renderer::Vulkan::DescriptorSet _descriptorSet;
    oe::Renderer::Vulkan::DepthBuffer _depthBuffer;
    oe::Renderer::Vulkan::MSAA _msaa;

    oe::Renderer::Vulkan::DescriptorPool _guiDescriptorPool;

    oe::Renderer::Vulkan::Pipeline _2dPipeline;
    oe::Renderer::Vulkan::Pipeline _3dPipeline;

    oe::Renderer::Vulkan::Pipeline* _currentPipeline;
    std::vector<oe::Renderer::Vulkan::Pipeline*> _pipelines;

    const std::vector _validationLayers = {"VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor"};
    uint32_t _currentImageIndex{};
    uint32_t _guiCurrentImageIndex{};

    void CreateFramebuffers();
    void DestroyFramebuffers();
    void CreateAsyncObjects();
    void DesrtoyAsyncObjects();
    void DestroySwapChain(bool destroyShaders = false);
} // namespace

namespace oe::Renderer
{
    void PreInit()
    {
        _instance.Create();
        _windowSurface.Setup(Core::Root::GetWindow()->GetGLFW());
        _physicalDevice.Create();
        _logicalDevice.Create();
        _swapChain.Create();
        _renderPass.Create();
        _imageViews.Create(&_swapChain);
        _commandPool.Create();
        _msaa.Create();
        _depthBuffer.Create();
        _guiDescriptorPool.Create();

        GuiLayer::PreInit();
    }

    void Init()
    {
        //_2dPipeline.Create(VK_FRONT_FACE_CLOCKWISE,);
        //_3dPipeline.Create(VK_FRONT_FACE_COUNTER_CLOCKWISE);
        CreateFramebuffers();
        _commandBuffer.Create();
        CreateAsyncObjects();

        GuiLayer::Init();
    }

    void Shutdown()
    {
        _logicalDevice.WaitIdle();
        const auto device = _logicalDevice.Get();
        GuiLayer::Shutdown();
        _guiDescriptorPool.Destroy();
        DesrtoyAsyncObjects();
        DestroyDescriptorSetLayout();
        _commandPool.Destroy();
        DestroySwapChain(true);
        _logicalDevice.Destroy();
        _windowSurface.Destroy();
        _instance.Destroy();
    }
} // namespace oe::Renderer

namespace oe::Renderer::Vulkan
{
    void DestroyShaders(VkDevice device) {}

    void DestroyDescriptorSetLayout()
    {
        const auto device = _logicalDevice.Get();
        for (const auto& descriptorSetLayout : _descriptorSetLayouts)
            vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

        for (const auto& descriptorSetLayout : _descriptorSetLayouts3D)
            vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    }

    void ReCreateSwapChain()
    {
        int width{}, height{};
        glfwGetFramebufferSize(Core::Root::GetWindow()->GetGLFW(), &width, &height);
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(Core::Root::GetWindow()->GetGLFW(), &width, &height);
            glfwWaitEvents();
        }
        DestroySwapChain();

        _swapChain.Create();
        _imageViews.Create(&_swapChain);
        _renderPass.Create();
        _msaa.Create();
        _depthBuffer.Create();
        for (auto* pipeline : _pipelines)
        {
            pipeline->Create(VK_FRONT_FACE_CLOCKWISE, true);
        }
        CreateFramebuffers();
    }

    uint32_t UpdateCurrentImageIndex()
    {
        const VkResult result = vkAcquireNextImageKHR(_logicalDevice.Get(), _swapChain.Get(), UINT64_MAX, _imageAvailableSemaphores,
                                                      VK_NULL_HANDLE, &_currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
            ReCreateSwapChain();
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            OE_THROW_ERROR("Renderer::Vulkan", "Failed to acquire swapchain image!");

        return _currentImageIndex;
    }

    VkDescriptorPool GetGuiDescriptorPool()
    {
        return _guiDescriptorPool.Get();
    }

    const Instance* GetInstance()
    {
        return &_instance;
    }

    const PhysicalDevice* GetPhysDevice()
    {
        return &_physicalDevice;
    }

    const std::vector<const char*>& GetValidationLayers()
    {
        return _validationLayers;
    }

    const WindowSurface* GetWindowSurface()
    {
        return &_windowSurface;
    }

    const LogicalDevice* GetLogicalDevice()
    {
        return &_logicalDevice;
    }

    const SwapChain* GetSwapChain()
    {
        return &_swapChain;
    }

    const ImageViews* GetImageViews()
    {
        return &_imageViews;
    }

    const RenderPass* GetRenderPass()
    {
        return &_renderPass;
    }

    const std::vector<Framebuffer>& GetFramebuffers()
    {
        return _framebuffers;
    }

    const CommandPool* GetCommandPool()
    {
        return &_commandPool;
    }

    const CommandBuffer* GetCommandBuffer()
    {
        return &_commandBuffer;
    }

    const DescriptorSet* GetDescriptorSet()
    {
        return &_descriptorSet;
    }

    VkQueue* GetGraphicsQueuePtr()
    {
        return &_graphicsQueue;
    }

    VkQueue GetGraphicsQueue()
    {
        return _graphicsQueue;
    }

    VkQueue GetPresentQueue()
    {
        return _presentQueue;
    }

    VkQueue* GetPresentQueuePtr()
    {
        return &_presentQueue;
    }

    VkSemaphore GetImageAvailableSemaphores()
    {
        return _imageAvailableSemaphores;
    }

    VkSemaphore GetRenderFinishedSemaphores()
    {
        return _renderFinishedSemaphores;
    }

    VkFence GetInFlightFence()
    {
        return _inFlightFence;
    }

    uint32_t GetCurrentImageIndex()
    {
        return _currentImageIndex;
    }
    uint32_t GetGuiCurrentImageIndex()
    {
        return _currentImageIndex;
    }

    Pipeline* GetCurrentPipeline()
    {
        return _currentPipeline;
    }

    std::vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions()
    {
        return _vertexInputBindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions()
    {
        return _vertexInputAttributeDescriptions;
    }

    std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts()
    {
        return _descriptorSetLayouts;
    }

    std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts3D()
    {
        return _descriptorSetLayouts3D;
    }

    void Bind2DPipeline()
    {
        _2dPipeline.Bind();
        _currentPipeline = &_2dPipeline;
    }

    void Bind3DPipeline()
    {
        _3dPipeline.Bind();
        _currentPipeline = &_3dPipeline;
    }

    void BeginScene()
    {
        const auto device = GetLogicalDevice()->Get();
        vkWaitForFences(device, 1, &_inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &_inFlightFence);

        _commandBuffer.Reset();
        _commandBuffer.Begin();
        RenderPass::Begin(_renderPass.Get());
    }

    void QueueSubmit(const VkSemaphore signalSemaphores[]);
    void QueuePresent(const VkSemaphore signalSemaphores[]);

    void EndScene()
    {
        _renderPass.End();

        _commandBuffer.End();
        QueueSubmit(&_renderFinishedSemaphores);
        QueuePresent(&_renderFinishedSemaphores);
    }

    void QueueSubmit(const VkSemaphore signalSemaphores[])
    {
        VkSubmitInfo submitInfo{};
        const VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores};
        constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = _commandBuffer.GetPtr();
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VK_CHECK_RESULT(vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFence), "Failed to submit draw command bufffer!")
    }

    void QueuePresent(const VkSemaphore signalSemaphores[])
    {
        const VkSwapchainKHR swapChains[] = {_swapChain.Get()};
        const uint32_t imageIndex = GetCurrentImageIndex();
        VkPresentInfoKHR presentInfo{};

        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        if (const VkResult result = vkQueuePresentKHR(GetPresentQueue(), &presentInfo);
            result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            ReCreateSwapChain();
        else if (result != VK_SUCCESS)
            OE_THROW_ERROR("Renderer::Vulkan", "Failed to present swapchain image!")
    }

    void Draw(uint32_t verticesCount)
    {
        vkCmdDraw(_commandBuffer.Get(), verticesCount, 1, 0, 0);
    }

    void DrawIndexed(uint32_t indexCount)
    {
        vkCmdDrawIndexed(_commandBuffer.Get(), indexCount, 1, 0, 0, 0);
    }
    void SetCurrentPipeline(Pipeline* pipeline)
    {
        _currentPipeline = pipeline;
    }
    void AddPipeline(Pipeline* pipeline)
    {
        _pipelines.push_back(pipeline);
    }
} // namespace oe::Renderer::Vulkan

namespace
{
    void CreateFramebuffers()
    {
        _framebuffers.resize(_imageViews.Get().size());
        const size_t size = _framebuffers.size();
        for (size_t i{}; i < size; ++i)
            _framebuffers[i].Create({_msaa.GetView(), _depthBuffer.GetView(), _imageViews.Get()[i]});
    }

    void DestroyFramebuffers()
    {
        for (auto& framebuffer : _framebuffers)
            framebuffer.Destroy();
    }

    void CreateAsyncObjects()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (const auto device = _logicalDevice.Get();
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_imageAvailableSemaphores) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_renderFinishedSemaphores) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &_inFlightFence) != VK_SUCCESS)
            OE_THROW_ERROR("Renderer::Vulkan", "Failed to create sync objects for a frame!")
    }

    void DesrtoyAsyncObjects()
    {
        const auto device = _logicalDevice.Get();
        vkDestroySemaphore(device, _imageAvailableSemaphores, nullptr);
        vkDestroySemaphore(device, _renderFinishedSemaphores, nullptr);
        vkDestroyFence(device, _inFlightFence, nullptr);
    }

    void DestroySwapChain(bool destroyShaders)
    {
        vkDeviceWaitIdle(_logicalDevice.Get());
        DestroyFramebuffers();
        for (auto* pipeline : _pipelines)
        {
            pipeline->Destroy(destroyShaders);
        }
        _renderPass.Destroy();
        _msaa.Destroy();
        _depthBuffer.Destroy();
        _imageViews.Destroy();
        _swapChain.Destroy();
    }
} // namespace
#endif
