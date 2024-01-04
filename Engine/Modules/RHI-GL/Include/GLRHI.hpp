//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/IModule.hpp"
#include "Oneiro/Common/RHI/IRHI.hpp"

#include "Types.hpp"

#include "glad/gl.h"
#include "imgui_impl_opengl3.h"

namespace oe
{
	class Shader : public RHI::IShader
	{
	public:
		using RHI::IShader::IShader;

		bool Load(RHI::EShaderStage stage, const std::string& source) override;

		Ref<fwog::Shader> shader{};
		fwog::PipelineStage pipelineStage{};
	};

	class Buffer : public RHI::IBuffer
	{
	public:
		using RHI::IBuffer::IBuffer;

		void Create(const void* data, size_t size, RHI::BufferStorageFlags storageFlags = RHI::BufferStorageFlag::NONE,
					std::string_view name = "") override;

		void UpdateData(const void* data, size_t size, size_t destOffsetBytes = 0) override;

		void FillData(const RHI::BufferFillInfo& clear = {}) override;

		void Invalidate() override;

		Ref<fwog::Buffer> buffer{};
	};

	class GraphicsPipeline : public RHI::IGraphicsPipeline
	{
	public:
		using RHI::IGraphicsPipeline::IGraphicsPipeline;

		void Create(const RHI::GraphicsPipelineInfo& info) override;

		Ref<fwog::GraphicsPipeline> pipeline{};
	};

	/// @brief An object that encapsulates the state needed to issue dispatches
	class ComputePipeline : public RHI::IComputePipeline
	{
	public:
		using RHI::IComputePipeline::IComputePipeline;

		void Create(const RHI::ComputePipelineInfo& info) override;

		Ref<fwog::ComputePipeline> pipeline{};
	};

	class Texture : public RHI::ITexture
	{
	public:
		using RHI::ITexture::ITexture;

		/// @brief Constructs the texture
		/// @param createInfo Parameters to construct the texture
		/// @param name An optional name for viewing the resource in a graphics debugger
		void Create(const RHI::TextureCreateInfo& createInfo, std::string_view name = "") override;

		/// @brief Updates a subresource of the image
		/// @param info The subresource and data to upload
		/// @note info.format must not be a compressed image format
		void UpdateImage(const RHI::TextureUpdateInfo& info) override;

		/// @brief Updates a subresource of the image
		/// @param info The subresource and data to upload
		/// @note Image must be in a compressed image format
		/// @note info.data must be in a compatible compressed image format
		void UpdateCompressedImage(const RHI::CompressedTextureUpdateInfo& info) override;

		/// @brief Clears a subresource of the image to a specified value
		/// @param info The subresource and value to clear it with
		void ClearImage(const RHI::TextureClearInfo& info) override;

		/// @brief Automatically generates LoDs of the image. All mip levels beyond 0 are filled with the generated LoDs
		void GenMipmaps() override;

		/// @brief Creates a view of a single mip level of the image
		[[nodiscard]] Ref<RHI::ITextureView> CreateSingleMipView(uint32_t level) override;

		/// @brief Creates a view of a single array layer of the image
		[[nodiscard]] Ref<RHI::ITextureView> CreateSingleLayerView(uint32_t layer) override;

		/// @brief Reinterpret the data of this texture
		/// @param newFormat The format to reinterpret the data as
		/// @return A new texture view
		[[nodiscard]] Ref<RHI::ITextureView> CreateFormatView(RHI::Format newFormat) override;

		/// @brief Creates a view of the texture with a new component mapping
		/// @param components The swizzled components
		/// @return A new texture view
		[[nodiscard]] Ref<RHI::ITextureView> CreateSwizzleView(RHI::ComponentMapping components) override;

		Ref<fwog::Texture> texture{};
	};

	class TextureView : public RHI::ITextureView
	{
	public:
		using RHI::ITextureView::ITextureView;

		void Create(const RHI::TextureViewCreateInfo& viewInfo, ITexture& texture, std::string_view name = "") override;
		void Create(const RHI::TextureViewCreateInfo& viewInfo, ITextureView& textureView, std::string_view name = "") override;
		void Create(ITexture& texture, std::string_view name = "") override;
	};

	class TextureSampler : public RHI::ITextureSampler
	{
	public:
		using RHI::ITextureSampler::ITextureSampler;

		void Create(const RHI::SamplerState& samplerState);
	};

	class CommandBuffer : public RHI::ICommandBuffer
	{
	public:
		using RHI::ICommandBuffer::ICommandBuffer;

		void BindGraphicsPipeline(const Ref<RHI::IGraphicsPipeline>& pipeline) override;
		void BindComputePipeline(const Ref<RHI::IComputePipeline>& pipeline) override;
		void SetViewport(const RHI::Viewport& viewport) override;
		void SetScissor(const RHI::Rect2D& scissor) override;
		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;
		void DrawIndirect(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset, uint32_t drawCount, uint32_t stride) override;
		void DrawIndirectCount(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset, const Ref<RHI::IBuffer>& countBuffer,
							   uint64_t countBufferOffset, uint32_t maxDrawCount, uint32_t stride) override;
		void DrawIndexedIndirect(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset, uint32_t drawCount, uint32_t stride) override;
		void DrawIndexedIndirectCount(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset, const Ref<RHI::IBuffer>& countBuffer,
									  uint64_t countBufferOffset, uint32_t maxDrawCount, uint32_t stride) override;
		void BindVertexBuffer(uint32_t bindingIndex, const Ref<RHI::IBuffer>& buffer, uint64_t offset, uint64_t stride) override;
		void BindIndexBuffer(const Ref<RHI::IBuffer>& buffer, RHI::IndexType indexType) override;
		void BindUniformBuffer(uint32_t index, const Ref<RHI::IBuffer>& buffer, uint64_t offset = 0, uint64_t size = RHI::WHOLE_BUFFER) override;
		void BindUniformBuffer(std::string_view block, const Ref<RHI::IBuffer>& buffer, uint64_t offset = 0,
							   uint64_t size = RHI::WHOLE_BUFFER) override;
		void BindStorageBuffer(uint32_t index, const Ref<RHI::IBuffer>& buffer, uint64_t offset = 0, uint64_t size = RHI::WHOLE_BUFFER) override;
		void BindStorageBuffer(std::string_view block, const Ref<RHI::IBuffer>& buffer, uint64_t offset = 0,
							   uint64_t size = RHI::WHOLE_BUFFER) override;
		void BindImage(uint32_t index, const Ref<RHI::ITexture>& texture, uint32_t level) override;
		void BindImage(std::string_view uniform, const Ref<RHI::ITexture>& texture, uint32_t level) override;
		void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;
		void Dispatch(RHI::Extent3D groupCount) override;
		void DispatchInvocations(uint32_t invocationCountX, uint32_t invocationCountY, uint32_t invocationCountZ) override;
		void DispatchInvocations(RHI::Extent3D invocationCount) override;
		void DispatchIndirect(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset) override;
	};

	class RenderGraph : public RHI::IRenderGraph
	{
	public:
		using RHI::IRenderGraph::IRenderGraph;

		void Begin(const RHI::RenderGraphBeginInfo& beginInfo, const std::function<void(RHI::ICommandBuffer*)>& func) override;
		void End() override;
		void AddGraphicsPass(const std::function<void(RHI::ICommandBuffer*)>& func, const RHI::PassInfo& info) override;
		void AddComputePass(const std::function<void(RHI::ICommandBuffer*)>& func, const RHI::PassInfo& info) override;

		Ref<CommandBuffer> commandBuffer{};
		fwog::SwapchainRenderInfo renderInfo{};

		std::function<void(RHI::ICommandBuffer*)> renderFunc{};
	};

	class GLRHI : public RHI::IRHI
	{
	public:
		using IRHI::IRHI;

		bool PreInitialize() override;
		bool Initialize(IWindow* window) override;
		bool Shutdown() override;

		Ref<RHI::IShader> CreateShader(RHI::EShaderStage shaderStage, const std::string& source) override;
		Ref<RHI::IBuffer> CreateBuffer(const void* data, size_t size, RHI::BufferStorageFlags storageFlags = RHI::BufferStorageFlag::NONE,
									   std::string_view name = "") override;
		Ref<RHI::IGraphicsPipeline> CreateGraphicsPipeline(const RHI::GraphicsPipelineInfo& info) override;
		Ref<RHI::IComputePipeline> CreateComputePipeline(const RHI::ComputePipelineInfo& info) override;
		Ref<RHI::IRenderGraph> CreateRenderGraph() override;
		Ref<RHI::ITexture> CreateTexture(const RHI::TextureCreateInfo& createInfo, std::string_view name = "") override;
		Ref<RHI::ITextureSampler> CreateTextureSampler(const RHI::SamplerState& samplerState) override;

		bool InitializeImGui(void* ctx) override;
		bool BeginImGuiFrame() override;
		bool EndImGuiFrame() override;
		bool ShutdownImGui() override;
	};

	class GLRHIEngineModule : public IModule
	{
	public:
		using IModule::IModule;

		bool Initialize(EngineApi* api) override;

		bool Shutdown() override;

	private:
		Ref<GLRHI> m_RHI{};
	};

	extern "C" OE_MODULE_API IModule* CreateModule()
	{
		return new GLRHIEngineModule();
	}

	extern "C" OE_MODULE_API void DestroyModule(IModule* engineModule)
	{
		delete engineModule;
	}
} // namespace oe