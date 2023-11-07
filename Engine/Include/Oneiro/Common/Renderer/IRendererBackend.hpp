//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Renderer/IBuffer.hpp"
#include "Oneiro/Common/Renderer/ICommandBuffer.hpp"
#include "Oneiro/Common/Renderer/IPipeline.hpp"
#include "Oneiro/Common/Renderer/IRenderPass.hpp"
#include "Oneiro/Common/Renderer/IShader.hpp"
#include "Oneiro/Common/Renderer/Renderer.hpp"
#include "Oneiro/Common/WM/IWindow.hpp"

namespace oe::Renderer
{
	enum class ERendererBackendType
	{
		UNDEFINED = 0,
		DX11,
		DX12,
		GL,
		VULKAN
	};

	class IRendererBackend
	{
	public:
		virtual ~IRendererBackend() = default;

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

		virtual Ref<IRenderPass> CreateRenderPass() = 0;

		[[nodiscard]] ERendererBackendType GetBackendType() const
		{
			return m_RendererBackendType;
		}

	protected:
		ERendererBackendType m_RendererBackendType{};
	};
} // namespace oe::Renderer