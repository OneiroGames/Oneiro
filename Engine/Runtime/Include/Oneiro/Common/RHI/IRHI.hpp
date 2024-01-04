//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/RHI/IBuffer.hpp"
#include "Oneiro/Common/RHI/ICommandBuffer.hpp"
#include "Oneiro/Common/RHI/IPipeline.hpp"
#include "Oneiro/Common/RHI/IRenderGraph.hpp"
#include "Oneiro/Common/RHI/IShader.hpp"
#include "Oneiro/Common/RHI/ITexture.hpp"
#include "Oneiro/Common/WM/IWindow.hpp"

namespace oe::RHI
{
	enum class ERHIType
	{
		UNDEFINED = 0,
		DX11,
		DX12,
		GL,
		VULKAN
	};

	class IRHI
	{
	public:
		virtual ~IRHI() = default;

		virtual bool PreInitialize() = 0;
		virtual bool Initialize(IWindow* window) = 0;
		virtual bool Shutdown() = 0;

		virtual bool InitializeImGui(void* ctx) = 0;
		virtual bool BeginImGuiFrame() = 0;
		virtual bool EndImGuiFrame() = 0;
		virtual bool ShutdownImGui() = 0;

		virtual Ref<IShader> CreateShader(EShaderStage shaderStage, const std::string& source) = 0;
		virtual Ref<IBuffer> CreateBuffer(const void* data, size_t size, BufferStorageFlags storageFlags = BufferStorageFlag::NONE,
										  std::string_view name = "") = 0;
		virtual Ref<IGraphicsPipeline> CreateGraphicsPipeline(const GraphicsPipelineInfo& info) = 0;
		virtual Ref<IComputePipeline> CreateComputePipeline(const ComputePipelineInfo& info) = 0;

		virtual Ref<IRenderGraph> CreateRenderGraph() = 0;

		virtual Ref<ITexture> CreateTexture(const TextureCreateInfo& createInfo, std::string_view name = "") = 0;
		virtual Ref<ITextureSampler> CreateTextureSampler(const SamplerState& samplerState) = 0;

		[[nodiscard]] ERHIType GetRHIType() const
		{
			return m_RHIType;
		}

	protected:
		ERHIType m_RHIType{};
	};
} // namespace oe::RHI