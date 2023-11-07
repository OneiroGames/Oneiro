//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/IModule.hpp"
#include "Oneiro/Modules/Renderer/Backend/GL/Types.hpp"

#include "glad/gl.h"

#include "imgui_impl_opengl3.h"

namespace oe
{
	fwog::Rect2D GetFwogRect2D(const Renderer::Rect2D& rect)
	{
		return {.offset = {.x = rect.offset.x, .y = rect.offset.y}, .extent = {.width = rect.extent.width, .height = rect.extent.height}};
	}

	fwog::Viewport GetFwogViewport(const Renderer::Viewport& viewport)
	{
		fwog::Viewport var{};
		var.minDepth = viewport.minDepth;
		var.maxDepth = viewport.maxDepth;
		var.drawRect = GetFwogRect2D(viewport.drawRect);
		var.depthRange = GetClipDepthRange(viewport.depthRange);
		return var;
	}

	class Shader : public Renderer::IShader
	{
	public:
		using Renderer::IShader::IShader;

		bool Load(Renderer::EShaderStage stage, const std::string& source) override
		{
			switch (stage)
			{
				case Renderer::EShaderStage::VERTEX: pipelineStage = fwog::PipelineStage::VERTEX_SHADER; break;
				case Renderer::EShaderStage::FRAGMENT: pipelineStage = fwog::PipelineStage::FRAGMENT_SHADER; break;
			}
			shader = CreateRef<fwog::Shader>(pipelineStage, source);
			return true;
		}

		Ref<fwog::Shader> shader{};
		fwog::PipelineStage pipelineStage{};
	};

	class Buffer : public Renderer::IBuffer
	{
	public:
		using Renderer::IBuffer::IBuffer;

		void Create(const void* data, size_t size, Renderer::BufferStorageFlags storageFlags = Renderer::BufferStorageFlag::NONE,
					std::string_view name = "") override
		{
			fwog::BufferStorageFlags bufferStorageFlags{};
			if (storageFlags & Renderer::BufferStorageFlag::NONE)
				bufferStorageFlags = fwog::BufferStorageFlag::NONE;
			if (storageFlags & Renderer::BufferStorageFlag::CLIENT_STORAGE)
				bufferStorageFlags = fwog::BufferStorageFlag::CLIENT_STORAGE;
			if (storageFlags & Renderer::BufferStorageFlag::DYNAMIC_STORAGE)
				bufferStorageFlags = fwog::BufferStorageFlag::DYNAMIC_STORAGE;
			if (storageFlags & Renderer::BufferStorageFlag::MAP_MEMORY)
				bufferStorageFlags = fwog::BufferStorageFlag::MAP_MEMORY;
			buffer = CreateRef<fwog::Buffer>(data, size, bufferStorageFlags, name);
		}

		void UpdateData(const void* data, size_t size, size_t destOffsetBytes = 0) override
		{
			buffer->UpdateData(data, size, destOffsetBytes);
		}

		void FillData(const Renderer::BufferFillInfo& clear = {}) override
		{
			fwog::BufferFillInfo bufferFillInfo{};
			bufferFillInfo.data = clear.data;
			bufferFillInfo.size = clear.size;
			bufferFillInfo.offset = clear.offset;

			buffer->FillData(bufferFillInfo);
		}

		void Invalidate() override
		{
			buffer->Invalidate();
		}

		Ref<fwog::Buffer> buffer{};
	};

	class GraphicsPipeline : public Renderer::IGraphicsPipeline
	{
	public:
		using Renderer::IGraphicsPipeline::IGraphicsPipeline;

		void Create(const Renderer::GraphicsPipelineInfo& info) override
		{
			fwog::GraphicsPipelineInfo graphicsPipelineInfo{.name = info.name,
															.vertexShader = dynamic_cast<const Shader*>(info.vertexShader)->shader.get(),
															.fragmentShader = dynamic_cast<const Shader*>(info.fragmentShader)->shader.get(),
															.tessellationControlShader = nullptr,
															.tessellationEvaluationShader = nullptr};
			graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = info.inputAssemblyState.primitiveRestartEnable;
			switch (info.inputAssemblyState.topology)
			{
				case Renderer::PrimitiveTopology::POINT_LIST:
					graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::POINT_LIST;
					break;
				case Renderer::PrimitiveTopology::LINE_LIST:
					graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::LINE_LIST;
					break;
				case Renderer::PrimitiveTopology::LINE_STRIP:
					graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::LINE_STRIP;
					break;
				case Renderer::PrimitiveTopology::TRIANGLE_LIST:
					graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::TRIANGLE_LIST;
					break;
				case Renderer::PrimitiveTopology::TRIANGLE_STRIP:
					graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::TRIANGLE_STRIP;
					break;
				case Renderer::PrimitiveTopology::TRIANGLE_FAN:
					graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::TRIANGLE_FAN;
					break;
				case Renderer::PrimitiveTopology::PATCH_LIST:
					graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::PATCH_LIST;
					break;
			}
			std::vector<fwog::VertexInputBindingDescription> vDesc;
			for (const auto& desc : info.vertexInputState.vertexBindingDescriptions)
			{
				fwog::VertexInputBindingDescription description{.location = desc.location, .binding = desc.binding, .offset = desc.offset};
				description.format = FormatToFwog(desc.format);
				vDesc.push_back(description);
			}
			graphicsPipelineInfo.vertexInputState.vertexBindingDescriptions = vDesc;
			graphicsPipelineInfo.tessellationState.patchControlPoints = info.tessellationState.patchControlPoints;
			graphicsPipelineInfo.rasterizationState = {.depthClampEnable = info.rasterizationState.depthClampEnable,
													   .polygonMode = PolygonModeToFwog(info.rasterizationState.polygonMode),
													   .cullMode = CullModeToFwog(info.rasterizationState.cullMode),
													   .frontFace = FrontFaceToFwog(info.rasterizationState.frontFace),
													   .depthBiasEnable = info.rasterizationState.depthBiasEnable,
													   .depthBiasConstantFactor = info.rasterizationState.depthBiasConstantFactor,
													   .depthBiasSlopeFactor = info.rasterizationState.depthBiasSlopeFactor,
													   .lineWidth = info.rasterizationState.lineWidth,
													   .pointSize = info.rasterizationState.pointSize};
			graphicsPipelineInfo.multisampleState = {.sampleShadingEnable = info.multisampleState.sampleShadingEnable,
													 .minSampleShading = info.multisampleState.minSampleShading,
													 .sampleMask = info.multisampleState.sampleMask,
													 .alphaToCoverageEnable = info.multisampleState.alphaToCoverageEnable,
													 .alphaToOneEnable = info.multisampleState.alphaToOneEnable};
			graphicsPipelineInfo.depthState = {.depthTestEnable = info.depthState.depthTestEnable,
											   .depthWriteEnable = info.depthState.depthWriteEnable,
											   .depthCompareOp = CompareOpToFwog(info.depthState.depthCompareOp)};
			graphicsPipelineInfo.stencilState = {.stencilTestEnable = info.stencilState.stencilTestEnable};
			pipeline = CreateRef<fwog::GraphicsPipeline>(graphicsPipelineInfo);
		}

		Ref<fwog::GraphicsPipeline> pipeline{};
	};

	/// @brief An object that encapsulates the state needed to issue dispatches
	class ComputePipeline : public Renderer::IComputePipeline
	{
	public:
		using Renderer::IComputePipeline::IComputePipeline;

		void Create(const Renderer::ComputePipelineInfo& info) override
		{
			//			pipeline = CreateRef<fwog::ComputePipeline>({});
		}

		Ref<fwog::ComputePipeline> pipeline{};
	};

	class CommandBuffer : public Renderer::ICommandBuffer
	{
	public:
		using Renderer::ICommandBuffer::ICommandBuffer;

		void BindGraphicsPipeline(const Ref<Renderer::IGraphicsPipeline>& pipeline) override
		{
			const auto& var = dynamic_cast<GraphicsPipeline*>(pipeline.get())->pipeline.get();
			fwog::Cmd::BindGraphicsPipeline(*var);
		}
		void BindComputePipeline(const Ref<Renderer::IComputePipeline>& pipeline) override
		{
			const auto& var = dynamic_cast<ComputePipeline*>(pipeline.get())->pipeline.get();
			fwog::Cmd::BindComputePipeline(*var);
		}
		void SetViewport(const Renderer::Viewport& viewport) override
		{
			fwog::Cmd::SetViewport(GetFwogViewport(viewport));
		}
		void SetScissor(const Renderer::Rect2D& scissor) override
		{
			fwog::Cmd::SetScissor(GetFwogRect2D(scissor));
		}
		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override
		{
			fwog::Cmd::Draw(vertexCount, instanceCount, firstVertex, firstInstance);
		}
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override
		{
			fwog::Cmd::Draw(indexCount, instanceCount, firstIndex, firstInstance);
		}
		void DrawIndirect(const Ref<Renderer::IBuffer>& commandBuffer, uint64_t commandBufferOffset, uint32_t drawCount, uint32_t stride) override
		{
			const auto& var = dynamic_cast<Buffer*>(commandBuffer.get())->buffer.get();
			fwog::Cmd::DrawIndirect(*var, commandBufferOffset, drawCount, stride);
		}
		void DrawIndirectCount(const Ref<Renderer::IBuffer>& commandBuffer, uint64_t commandBufferOffset, const Ref<Renderer::IBuffer>& countBuffer,
							   uint64_t countBufferOffset, uint32_t maxDrawCount, uint32_t stride) override
		{
			const auto& var1 = dynamic_cast<Buffer*>(commandBuffer.get())->buffer.get();
			const auto& var2 = dynamic_cast<Buffer*>(countBuffer.get())->buffer.get();
			fwog::Cmd::DrawIndirectCount(*var1, commandBufferOffset, *var2, countBufferOffset, maxDrawCount, stride);
		}
		void DrawIndexedIndirect(const Ref<Renderer::IBuffer>& commandBuffer, uint64_t commandBufferOffset, uint32_t drawCount,
								 uint32_t stride) override
		{
		}
		void DrawIndexedIndirectCount(const Ref<Renderer::IBuffer>& commandBuffer, uint64_t commandBufferOffset,
									  const Ref<Renderer::IBuffer>& countBuffer, uint64_t countBufferOffset, uint32_t maxDrawCount,
									  uint32_t stride) override
		{
		}
		void BindVertexBuffer(uint32_t bindingIndex, const Ref<Renderer::IBuffer>& buffer, uint64_t offset, uint64_t stride) override
		{
			const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
			fwog::Cmd::BindVertexBuffer(bindingIndex, *var, offset, stride);
		}
		void BindIndexBuffer(const Ref<Renderer::IBuffer>& buffer, Renderer::IndexType indexType) override
		{
			const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
			fwog::IndexType type{};
			switch (indexType)
			{
				case Renderer::IndexType::UNSIGNED_BYTE: type = fwog::IndexType::UNSIGNED_BYTE; break;
				case Renderer::IndexType::UNSIGNED_SHORT: type = fwog::IndexType::UNSIGNED_SHORT; break;
				case Renderer::IndexType::UNSIGNED_INT: type = fwog::IndexType::UNSIGNED_INT; break;
			}
			fwog::Cmd::BindIndexBuffer(*var, type);
		}
		void BindUniformBuffer(uint32_t index, const Ref<Renderer::IBuffer>& buffer, uint64_t offset = 0,
							   uint64_t size = Renderer::WHOLE_BUFFER) override
		{
			const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
			fwog::Cmd::BindUniformBuffer(index, *var, offset, size);
		}
		void BindUniformBuffer(std::string_view block, const Ref<Renderer::IBuffer>& buffer, uint64_t offset = 0,
							   uint64_t size = Renderer::WHOLE_BUFFER) override
		{
			const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
			fwog::Cmd::BindUniformBuffer(block, *var, offset, size);
		}
		void BindStorageBuffer(uint32_t index, const Ref<Renderer::IBuffer>& buffer, uint64_t offset = 0,
							   uint64_t size = Renderer::WHOLE_BUFFER) override
		{
			const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
			fwog::Cmd::BindStorageBuffer(index, *var, offset, size);
		}
		void BindStorageBuffer(std::string_view block, const Ref<Renderer::IBuffer>& buffer, uint64_t offset = 0,
							   uint64_t size = Renderer::WHOLE_BUFFER) override
		{
			const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
			fwog::Cmd::BindStorageBuffer(block, *var, offset, size);
		}
		void BindImage(uint32_t index, const Ref<Renderer::ITexture>& texture, uint32_t level) override {}
		void BindImage(std::string_view uniform, const Ref<Renderer::ITexture>& texture, uint32_t level) override {}
		void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override {}
		void Dispatch(Renderer::Extent3D groupCount) override {}
		void DispatchInvocations(uint32_t invocationCountX, uint32_t invocationCountY, uint32_t invocationCountZ) override {}
		void DispatchInvocations(Renderer::Extent3D invocationCount) override {}
		void DispatchIndirect(const Ref<Renderer::IBuffer>& commandBuffer, uint64_t commandBufferOffset) override {}
	};

	class RenderPass : public Renderer::IRenderPass
	{
	public:
		using Renderer::IRenderPass::IRenderPass;

		void Begin(const Renderer::RenderPassBeginInfo& beginInfo, const std::function<void(Renderer::ICommandBuffer*)>& func) override
		{
			commandBuffer = CreateRef<CommandBuffer>();
			renderFunc = func;
			renderInfo = {.name = beginInfo.name,
						  .viewport = GetFwogViewport(beginInfo.viewport),
						  .colorLoadOp = GetAttachmentLoadOp(beginInfo.colorLoadOp),
						  .clearColorValue = beginInfo.clearColorValue,
						  .depthLoadOp = GetAttachmentLoadOp(beginInfo.depthLoadOp),
						  .clearDepthValue = beginInfo.clearDepthValue,
						  .stencilLoadOp = GetAttachmentLoadOp(beginInfo.stencilLoadOp),
						  .clearStencilValue = beginInfo.clearStencilValue,
						  .enableSrgb = beginInfo.enableSrgb};
		}
		void End() override
		{
			Fwog::RenderToSwapchain(renderInfo, [&] {
				renderFunc(commandBuffer.get());
			});
		}
		void AddGraphicsPass(const std::function<void(Renderer::ICommandBuffer*)>& func, const Renderer::RenderPassInfo& info) override {}
		void AddComputePass(const std::function<void(Renderer::ICommandBuffer*)>& func, const Renderer::RenderPassInfo& info) override {}

		Ref<CommandBuffer> commandBuffer{};
		fwog::SwapchainRenderInfo renderInfo{};

		std::function<void(Renderer::ICommandBuffer*)> renderFunc{};
	};

	class RendererBackend : public Renderer::IRendererBackend
	{
	public:
		using IRendererBackend::IRendererBackend;

		bool PreInitialize() override
		{
			m_RendererBackendType = Renderer::ERendererBackendType::GL;
			return true;
		}

		bool Initialize(IWindow* window) override
		{
			gladLoadGL(static_cast<GLADloadfunc>(window->GetWindowHandleData().procAddress));
			Fwog::Initialize();
			return true;
		}

		Ref<Renderer::IShader> CreateShader(Renderer::EShaderStage shaderStage, const std::string& source) override
		{
			auto shader = CreateRef<Shader>();
			shader->Load(shaderStage, source);
			return shader;
		}

		Ref<Renderer::IBuffer> CreateBuffer(const void* data, size_t size,
											Renderer::BufferStorageFlags storageFlags = Renderer::BufferStorageFlag::NONE,
											std::string_view name = "") override
		{
			auto buffer = CreateRef<Buffer>();
			buffer->Create(data, size, storageFlags, name);
			return buffer;
		}

		Ref<Renderer::IGraphicsPipeline> CreateGraphicsPipeline(const Renderer::GraphicsPipelineInfo& info) override
		{
			auto pipeline = CreateRef<GraphicsPipeline>();
			pipeline->Create(info);
			return pipeline;
		}

		Ref<Renderer::IComputePipeline> CreateComputePipeline(const Renderer::ComputePipelineInfo& info) override
		{
			auto pipeline = CreateRef<ComputePipeline>();
			pipeline->Create(info);
			return pipeline;
		}

		Ref<Renderer::IRenderPass> CreateRenderPass() override
		{
			auto renderPass = CreateRef<RenderPass>();
			return renderPass;
		}

		bool Shutdown() override
		{
			fwog::Terminate();
			return true;
		}

		bool InitializeImGui(void* ctx) override
		{
			ImGui::SetCurrentContext(static_cast<ImGuiContext*>(ctx));
			ImGui_ImplOpenGL3_Init("#version 460");
			return true;
		}

		bool BeginImGuiFrame() override
		{
			ImGui_ImplOpenGL3_NewFrame();
			return true;
		}

		bool EndImGuiFrame() override
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			return true;
		}

		bool ShutdownImGui() override
		{
			ImGui_ImplOpenGL3_Shutdown();
			return true;
		}
	};

	class RendererEngineModule : public IModule
	{
	public:
		using IModule::IModule;

		bool Initialize() override
		{
			m_Renderer = CreateRef<RendererBackend>();
			GetEngineApi()->rendererBackend = m_Renderer.get();
			return true;
		}

		bool Shutdown() override
		{
			GetEngineApi()->rendererBackend = nullptr;
			m_Renderer.reset();
			return true;
		}

	private:
		Ref<RendererBackend> m_Renderer{};
	};

	extern "C" OE_MODULE_API IModule* CreateModule(EngineApi* engineApi)
	{
		return new RendererEngineModule(engineApi);
	}

	extern "C" OE_MODULE_API void DestroyModule(IModule* engineModule)
	{
		delete engineModule;
	}
} // namespace oe