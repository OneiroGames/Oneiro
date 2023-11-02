//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/WM/IWindow.hpp"

namespace oe
{
	enum class ERendererBackendType
	{
		UNDEFINED = 0,
		DX11,
		DX12,
		GL,
		VULKAN
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual bool PreInitialize() = 0;
		virtual bool Initialize(IWindow* window) = 0;
		virtual bool Shutdown() = 0;

		virtual bool InitializeImGui(void* ctx) = 0;
		virtual bool BeginImGuiFrame() = 0;
		virtual bool EndImGuiFrame() = 0;
		virtual bool ShutdownImGui() = 0;

		[[nodiscard]] ERendererBackendType GetBackendType() const
		{
			return m_RendererBackendType;
		}

	protected:
		ERendererBackendType m_RendererBackendType{};
	};
} // namespace oe