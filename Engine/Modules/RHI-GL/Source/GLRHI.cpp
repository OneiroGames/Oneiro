//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "GLRHI.hpp"

namespace oe
{
	fwog::Rect2D GetFwogRect2D(const RHI::Rect2D& rect)
	{
		return {.offset = {.x = rect.offset.x, .y = rect.offset.y}, .extent = {.width = rect.extent.width, .height = rect.extent.height}};
	}

	fwog::Viewport GetFwogViewport(const RHI::Viewport& viewport)
	{
		fwog::Viewport var{};
		var.minDepth = viewport.minDepth;
		var.maxDepth = viewport.maxDepth;
		var.drawRect = GetFwogRect2D(viewport.drawRect);
		var.depthRange = GetClipDepthRange(viewport.depthRange);
		return var;
	}

	bool Shader::Load(RHI::EShaderStage stage, const std::string& source)
	{
		switch (stage)
		{
			case RHI::EShaderStage::VERTEX: pipelineStage = fwog::PipelineStage::VERTEX_SHADER; break;
			case RHI::EShaderStage::FRAGMENT: pipelineStage = fwog::PipelineStage::FRAGMENT_SHADER; break;
		}
		shader = CreateRef<fwog::Shader>(pipelineStage, source);
		return true;
	}

	void Buffer::Create(const void* data, size_t size, RHI::BufferStorageFlags storageFlags, std::string_view name)
	{
		fwog::BufferStorageFlags bufferStorageFlags{};
		if (storageFlags & RHI::BufferStorageFlag::NONE)
			bufferStorageFlags = fwog::BufferStorageFlag::NONE;
		if (storageFlags & RHI::BufferStorageFlag::CLIENT_STORAGE)
			bufferStorageFlags = fwog::BufferStorageFlag::CLIENT_STORAGE;
		if (storageFlags & RHI::BufferStorageFlag::DYNAMIC_STORAGE)
			bufferStorageFlags = fwog::BufferStorageFlag::DYNAMIC_STORAGE;
		if (storageFlags & RHI::BufferStorageFlag::MAP_MEMORY)
			bufferStorageFlags = fwog::BufferStorageFlag::MAP_MEMORY;
		buffer = CreateRef<fwog::Buffer>(data, size, bufferStorageFlags, name);
	}

	void Buffer::UpdateData(const void* data, size_t size, size_t destOffsetBytes)
	{
		buffer->UpdateData(data, size, destOffsetBytes);
	}

	void Buffer::FillData(const RHI::BufferFillInfo& clear)
	{
		fwog::BufferFillInfo bufferFillInfo{};
		bufferFillInfo.data = clear.data;
		bufferFillInfo.size = clear.size;
		bufferFillInfo.offset = clear.offset;
		buffer->FillData(bufferFillInfo);
	}

	void Buffer::Invalidate()
	{
		buffer->Invalidate();
	}

	void GraphicsPipeline::Create(const RHI::GraphicsPipelineInfo& info)
	{
		fwog::GraphicsPipelineInfo graphicsPipelineInfo{.name = info.name,
														.vertexShader = dynamic_cast<const Shader*>(info.vertexShader)->shader.get(),
														.fragmentShader = dynamic_cast<const Shader*>(info.fragmentShader)->shader.get(),
														.tessellationControlShader = nullptr,
														.tessellationEvaluationShader = nullptr};
		graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = info.inputAssemblyState.primitiveRestartEnable;
		switch (info.inputAssemblyState.topology)
		{
			case RHI::PrimitiveTopology::POINT_LIST: graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::POINT_LIST; break;
			case RHI::PrimitiveTopology::LINE_LIST: graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::LINE_LIST; break;
			case RHI::PrimitiveTopology::LINE_STRIP: graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::LINE_STRIP; break;
			case RHI::PrimitiveTopology::TRIANGLE_LIST:
				graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::TRIANGLE_LIST;
				break;
			case RHI::PrimitiveTopology::TRIANGLE_STRIP:
				graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::TRIANGLE_STRIP;
				break;
			case RHI::PrimitiveTopology::TRIANGLE_FAN:
				graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::TRIANGLE_FAN;
				break;
			case RHI::PrimitiveTopology::PATCH_LIST: graphicsPipelineInfo.inputAssemblyState.topology = fwog::PrimitiveTopology::PATCH_LIST; break;
		}
		std::vector<fwog::VertexInputBindingDescription> vDesc;
		for (const auto& desc : info.vertexInputState.vertexBindingDescriptions)
		{
			fwog::VertexInputBindingDescription description{.location = desc.location, .binding = desc.binding, .offset = desc.offset};
			description.format = GetFormat(desc.format);
			vDesc.push_back(description);
		}
		graphicsPipelineInfo.vertexInputState.vertexBindingDescriptions = vDesc;
		graphicsPipelineInfo.tessellationState.patchControlPoints = info.tessellationState.patchControlPoints;
		graphicsPipelineInfo.rasterizationState = {.depthClampEnable = info.rasterizationState.depthClampEnable,
												   .polygonMode = GetPolygonModeTo(info.rasterizationState.polygonMode),
												   .cullMode = GetCullModeTo(info.rasterizationState.cullMode),
												   .frontFace = GetFrontFaceTo(info.rasterizationState.frontFace),
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
										   .depthCompareOp = GetCompareOpTo(info.depthState.depthCompareOp)};
		graphicsPipelineInfo.stencilState = {.stencilTestEnable = info.stencilState.stencilTestEnable};
		pipeline = CreateRef<fwog::GraphicsPipeline>(graphicsPipelineInfo);
	}

	void ComputePipeline::Create(const RHI::ComputePipelineInfo& info)
	{
		//            pipeline = CreateRef<fwog::ComputePipeline>({});
	}

	void Texture::Create(const RHI::TextureCreateInfo& createInfo, std::string_view name)
	{
		fwog::TextureCreateInfo info{
			.imageType = GetImageType(createInfo.imageType),
			.format = GetFormat(createInfo.format),
			.extent = {.width = createInfo.extent.width, .height = createInfo.extent.height, .depth = createInfo.extent.depth},
			.mipLevels = createInfo.mipLevels,
			.arrayLayers = createInfo.arrayLayers,
			.sampleCount = GetSampleCount(createInfo.sampleCount),
		};
		texture = CreateRef<fwog::Texture>(info, name);
	}

	void Texture::UpdateImage(const RHI::TextureUpdateInfo& info)
	{
		fwog::TextureUpdateInfo updateInfo{.level = info.level,
										   .offset = {.x = info.offset.x, .y = info.offset.y, .z = info.offset.z},
										   .extent = {.width = info.extent.width, .height = info.extent.height, .depth = info.extent.depth},
										   .format = GetUploadFormat(info.format),
										   .type = GetUploadType(info.type),
										   .pixels = info.pixels,
										   .rowLength = info.rowLength,
										   .imageHeight = info.imageHeight};
		texture->UpdateImage(updateInfo);
	}

	void Texture::UpdateCompressedImage(const RHI::CompressedTextureUpdateInfo& info) {}

	void Texture::ClearImage(const RHI::TextureClearInfo& info) {}

	void Texture::GenMipmaps()
	{
		texture->GenMipmaps();
	}

	Ref<RHI::ITextureView> Texture::CreateSingleMipView(uint32_t level)
	{
		return {};
	}

	Ref<RHI::ITextureView> Texture::CreateSingleLayerView(uint32_t layer)
	{
		return {};
	}

	Ref<RHI::ITextureView> Texture::CreateFormatView(RHI::Format newFormat)
	{
		return {};
	}

	Ref<RHI::ITextureView> Texture::CreateSwizzleView(RHI::ComponentMapping components)
	{
		return {};
	}

	void TextureView::Create(const RHI::TextureViewCreateInfo& viewInfo, RHI::ITexture& texture, std::string_view name) {}

	void TextureView::Create(const RHI::TextureViewCreateInfo& viewInfo, RHI::ITextureView& textureView, std::string_view name) {}

	void TextureView::Create(RHI::ITexture& texture, std::string_view name) {}

	void TextureSampler::Create(const RHI::SamplerState& samplerState) {}

	void CommandBuffer::BindGraphicsPipeline(const Ref<RHI::IGraphicsPipeline>& pipeline)
	{
		const auto& var = dynamic_cast<GraphicsPipeline*>(pipeline.get())->pipeline.get();
		fwog::Cmd::BindGraphicsPipeline(*var);
	}

	void CommandBuffer::BindComputePipeline(const Ref<RHI::IComputePipeline>& pipeline)
	{
		const auto& var = dynamic_cast<ComputePipeline*>(pipeline.get())->pipeline.get();
		fwog::Cmd::BindComputePipeline(*var);
	}

	void CommandBuffer::SetViewport(const RHI::Viewport& viewport)
	{
		fwog::Cmd::SetViewport(GetFwogViewport(viewport));
	}

	void CommandBuffer::SetScissor(const RHI::Rect2D& scissor)
	{
		fwog::Cmd::SetScissor(GetFwogRect2D(scissor));
	}

	void CommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		fwog::Cmd::Draw(vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		fwog::Cmd::Draw(indexCount, instanceCount, firstIndex, firstInstance);
	}

	void CommandBuffer::DrawIndirect(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset, uint32_t drawCount, uint32_t stride)
	{
		const auto& var = dynamic_cast<Buffer*>(commandBuffer.get())->buffer.get();
		fwog::Cmd::DrawIndirect(*var, commandBufferOffset, drawCount, stride);
	}

	void CommandBuffer::DrawIndirectCount(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset, const Ref<RHI::IBuffer>& countBuffer,
										  uint64_t countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
	{
		const auto& var1 = dynamic_cast<Buffer*>(commandBuffer.get())->buffer.get();
		const auto& var2 = dynamic_cast<Buffer*>(countBuffer.get())->buffer.get();
		fwog::Cmd::DrawIndirectCount(*var1, commandBufferOffset, *var2, countBufferOffset, maxDrawCount, stride);
	}

	void CommandBuffer::DrawIndexedIndirect(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset, uint32_t drawCount, uint32_t stride)
	{
	}

	void CommandBuffer::DrawIndexedIndirectCount(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset,
												 const Ref<RHI::IBuffer>& countBuffer, uint64_t countBufferOffset, uint32_t maxDrawCount,
												 uint32_t stride)
	{
	}

	void CommandBuffer::BindVertexBuffer(uint32_t bindingIndex, const Ref<RHI::IBuffer>& buffer, uint64_t offset, uint64_t stride)
	{
		const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
		fwog::Cmd::BindVertexBuffer(bindingIndex, *var, offset, stride);
	}

	void CommandBuffer::BindIndexBuffer(const Ref<RHI::IBuffer>& buffer, RHI::IndexType indexType)
	{
		const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
		fwog::IndexType type{};
		switch (indexType)
		{
			case RHI::IndexType::UNSIGNED_BYTE: type = fwog::IndexType::UNSIGNED_BYTE; break;
			case RHI::IndexType::UNSIGNED_SHORT: type = fwog::IndexType::UNSIGNED_SHORT; break;
			case RHI::IndexType::UNSIGNED_INT: type = fwog::IndexType::UNSIGNED_INT; break;
		}
		fwog::Cmd::BindIndexBuffer(*var, type);
	}

	void CommandBuffer::BindUniformBuffer(uint32_t index, const Ref<RHI::IBuffer>& buffer, uint64_t offset, uint64_t size)
	{
		const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
		fwog::Cmd::BindUniformBuffer(index, *var, offset, size);
	}

	void CommandBuffer::BindUniformBuffer(std::string_view block, const Ref<RHI::IBuffer>& buffer, uint64_t offset, uint64_t size)
	{
		const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
		fwog::Cmd::BindUniformBuffer(block, *var, offset, size);
	}

	void CommandBuffer::BindStorageBuffer(uint32_t index, const Ref<RHI::IBuffer>& buffer, uint64_t offset, uint64_t size)
	{
		const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
		fwog::Cmd::BindStorageBuffer(index, *var, offset, size);
	}

	void CommandBuffer::BindStorageBuffer(std::string_view block, const Ref<RHI::IBuffer>& buffer, uint64_t offset, uint64_t size)
	{
		const auto& var = dynamic_cast<Buffer*>(buffer.get())->buffer.get();
		fwog::Cmd::BindStorageBuffer(block, *var, offset, size);
	}

	void CommandBuffer::BindImage(uint32_t index, const Ref<RHI::ITexture>& texture, uint32_t level)
	{
		const auto& var = dynamic_cast<Texture*>(texture.get())->texture.get();
		fwog::Cmd::BindImage(index, *var, level);
	}

	void CommandBuffer::BindImage(std::string_view uniform, const Ref<RHI::ITexture>& texture, uint32_t level)
	{
		const auto& var = dynamic_cast<Texture*>(texture.get())->texture.get();
		fwog::Cmd::BindImage(uniform, *var, level);
	}

	void CommandBuffer::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {}

	void CommandBuffer::Dispatch(RHI::Extent3D groupCount) {}

	void CommandBuffer::DispatchInvocations(uint32_t invocationCountX, uint32_t invocationCountY, uint32_t invocationCountZ) {}

	void CommandBuffer::DispatchInvocations(RHI::Extent3D invocationCount) {}

	void CommandBuffer::DispatchIndirect(const Ref<RHI::IBuffer>& commandBuffer, uint64_t commandBufferOffset) {}

	void RenderGraph::Begin(const RHI::RenderGraphBeginInfo& beginInfo, const std::function<void(RHI::ICommandBuffer*)>& func)
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

	void RenderGraph::End()
	{
		Fwog::RenderToSwapchain(renderInfo, [&] {
			renderFunc(commandBuffer.get());
		});
	}

	void RenderGraph::AddGraphicsPass(const std::function<void(RHI::ICommandBuffer*)>& func, const RHI::PassInfo& info) {}

	void RenderGraph::AddComputePass(const std::function<void(RHI::ICommandBuffer*)>& func, const RHI::PassInfo& info) {}

	bool GLRHI::PreInitialize()
	{
		m_RHIType = RHI::ERHIType::GL;
		return true;
	}

	bool GLRHI::Initialize(IWindow* window)
	{
		gladLoadGL(static_cast<GLADloadfunc>(window->GetWindowHandleData().procAddress));
		Fwog::Initialize();
		return true;
	}

	bool GLRHI::Shutdown()
	{
		fwog::Terminate();
		return true;
	}

	Ref<RHI::IShader> GLRHI::CreateShader(RHI::EShaderStage shaderStage, const std::string& source)
	{
		auto shader = CreateRef<Shader>();
		shader->Load(shaderStage, source);
		return shader;
	}

	Ref<RHI::IBuffer> GLRHI::CreateBuffer(const void* data, size_t size, RHI::BufferStorageFlags storageFlags, std::string_view name)
	{
		auto buffer = CreateRef<Buffer>();
		buffer->Create(data, size, storageFlags, name);
		return buffer;
	}

	Ref<RHI::IGraphicsPipeline> GLRHI::CreateGraphicsPipeline(const RHI::GraphicsPipelineInfo& info)
	{
		auto pipeline = CreateRef<GraphicsPipeline>();
		pipeline->Create(info);
		return pipeline;
	}

	Ref<RHI::IComputePipeline> GLRHI::CreateComputePipeline(const RHI::ComputePipelineInfo& info)
	{
		auto pipeline = CreateRef<ComputePipeline>();
		pipeline->Create(info);
		return pipeline;
	}

	Ref<RHI::IRenderGraph> GLRHI::CreateRenderGraph()
	{
		auto renderPass = CreateRef<RenderGraph>();
		return renderPass;
	}

	Ref<RHI::ITexture> GLRHI::CreateTexture(const RHI::TextureCreateInfo& createInfo, std::string_view name)
	{
		auto texture = CreateRef<Texture>();
		texture->Create(createInfo, name);
		return texture;
	}

	Ref<RHI::ITextureSampler> GLRHI::CreateTextureSampler(const RHI::SamplerState& samplerState)
	{
		return {};
	}

	bool GLRHI::InitializeImGui(void* ctx)
	{
		ImGui::SetCurrentContext(static_cast<ImGuiContext*>(ctx));
		ImGui_ImplOpenGL3_Init("#version 460");
		return true;
	}

	bool GLRHI::BeginImGuiFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		return true;
	}

	bool GLRHI::EndImGuiFrame()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		return true;
	}

	bool GLRHI::ShutdownImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		return true;
	}

	bool GLRHIEngineModule::Initialize(EngineApi* api)
	{
		m_RHI = CreateRef<GLRHI>();
		EngineApi::GetInstance()->rhi = m_RHI.get();
		return true;
	}

	bool GLRHIEngineModule::Shutdown()
	{
		EngineApi::GetInstance()->rhi = nullptr;
		m_RHI.reset();
		return true;
	}
} // namespace oe