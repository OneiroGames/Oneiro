//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/World/Components.hpp"
#include "OpenGL/Model.hpp"
#include "glm/mat4x4.hpp"

namespace oe::Renderer
{
    struct PointVertex
    {
        glm::vec4 Color{};
        glm::vec4 Position{};
        float Size{};
    };

    struct LineVertex
    {
        glm::vec4 Color{};
        glm::vec4 Position{};
    };

    struct CircleVertex
    {
        glm::vec4 Color{};
        glm::vec4 WorldPosition{};
        glm::vec4 LocalPosition{};
        float Thickness{};
        float Fade{};
    };

    struct QuadVertex
    {
        glm::vec4 Position{};
        glm::vec4 Color{};
        glm::vec2 TexCoords{};
        int TexIndex{-1};
        float Alpha{};
        float Ar{};
        bool KeepAr{};
    };

    struct Limits
    {
        uint32_t MaxQuads = 20000;
        uint32_t MaxVertices = MaxQuads * 4;
        uint32_t MaxIndices = MaxQuads * 6;
        uint32_t MaxTextures = 32;
    };

    struct Statistic
    {
        uint32_t Vertices{};
        uint32_t Indices{};
        uint32_t Textures{};
        uint32_t DrawCalls{};
        uint32_t FlushesCount{};
    };

    void PreInit();
    void Init();

    void Begin(const MainCameraComponent& mainCamera);
    void RenderSprite2D(const glm::mat4& transform, const GL::Sprite2D& sprite2D);
    void RenderModel(const glm::mat4& transform, const GL::Model& model);
    void RenderLine(const glm::mat4& transform, const glm::vec3& fromPos, const glm::vec3& toPos, const glm::vec4& color = glm::vec4(1.0f));
    void RenderCircle(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f), float thickness = 1.0f, float fade = 0.005f);
    void RenderPoint(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f), float size = 1.0f);
    void RenderQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
    void RenderQuad(const glm::mat4& transform, const GL::Texture<gl::TEXTURE_2D>& texture, float alpha = 1.0f, bool keepAr = false,
                    float ar = 0.0f);

    void StartBatch();
    void NextBatch();
    void Flush();

    void End();

    void SetLineWidth(float width);
    void SetPointSize(float size);
    const Statistic& GetStats();
    const Limits& GetLimits();
    void ResetStats();

    void Shutdown();
} // namespace oe::Renderer

#ifndef OE_VULKAN_API

#include "OpenGL/gl_core_4_5.hpp"
#include <cstdint>

namespace oe::Renderer::GL
{
    enum DrawMode
    {
        TRIANGLES = gl::TRIANGLES,
        LINES = gl::LINES,
        POINTS = gl::POINTS
    };

    enum DataType
    {
        UNSIGNED_BYTE = gl::UNSIGNED_BYTE,
        UNSIGNED_INT = gl::UNSIGNED_INT,
        FLOAT = gl::FLOAT,
        INT = gl::INT
    };

    enum ClearType
    {
        COLOR_BUFFER = gl::COLOR_BUFFER_BIT,
        DEPTH_BUFFER = gl::DEPTH_BUFFER_BIT,
        STENCIL_BUFFER = gl::STENCIL_BUFFER_BIT
    };

    void DrawArrays(DrawMode mode, int count);
    void DrawArrays(DrawMode mode, int first, int count);
    void DrawElements(DrawMode mode, int count, DataType type, const void* indices = nullptr);

    void Clear(uint32_t mask);
    void ClearColor(float r, float g, float b, float a = 1.0f);

    void Viewport(GLsizei width, GLsizei height);
    void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
} // namespace oe::Renderer::GL
#else
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "vulkan/vulkan_core.h"

#include "Oneiro/Core/Oneiro.hpp"

#include <vector>

namespace oe::Renderer::Vulkan
{
    class Instance;
    class PhysicalDevice;
    class LogicalDevice;
    class WindowSurface;
    class SwapChain;
    class ImageViews;
    class Pipeline;
    class RenderPass;
    class Framebuffer;
    class CommandPool;
    class CommandBuffer;
    class Shader;
    class DescriptorSetLayout;
    class DescriptorSet;

    void ReCreateSwapChain();

    void BeginScene();
    void BeginGuiScene();
    void EndGuiScene();
    void EndScene();

    void Bind2DPipeline();
    void Bind3DPipeline();

    void Draw(uint32_t verticesCount);
    void DrawIndexed(uint32_t indexCount);

    void AddVertexShader(VkShaderModule shader);
    void AddFragmentShader(VkShaderModule shader);

    uint32_t UpdateCurrentImageIndex();
    VkDescriptorPool GetGuiDescriptorPool();

    const Instance* GetInstance();
    const PhysicalDevice* GetPhysDevice();
    const std::vector<const char*>& GetValidationLayers();
    const WindowSurface* GetWindowSurface();
    const SwapChain* GetGuiSwapChain();
    const LogicalDevice* GetLogicalDevice();
    const SwapChain* GetSwapChain();
    const ImageViews* GetImageViews();
    const RenderPass* GetRenderPass();
    const std::vector<Framebuffer>& GetFramebuffers();
    const CommandPool* GetCommandPool();
    const CommandBuffer* GetCommandBuffer();
    const DescriptorSet* GetDescriptorSet();
    VkQueue* GetGraphicsQueuePtr();
    VkQueue GetGraphicsQueue();
    VkQueue GetPresentQueue();
    VkQueue* GetPresentQueuePtr();
    VkSemaphore GetImageAvailableSemaphores();
    VkSemaphore GetRenderFinishedSemaphores();
    VkFence GetInFlightFence();
    uint32_t GetCurrentImageIndex();
    Pipeline* GetCurrentPipeline();

    void SetCurrentPipeline(Pipeline* pipeline);
    void AddPipeline(Pipeline* pipeline);

    std::vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions();
    std::vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions();
    std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts();
    std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts3D();
} // namespace oe::Renderer::Vulkan
#endif
