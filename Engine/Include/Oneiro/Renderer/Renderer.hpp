//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//
#pragma once

#include "Oneiro/Common/IRenderer.hpp"

namespace oe
{
	class Renderer : public IRenderer
	{
	public:
		using IRenderer::IRenderer;

		bool PreInitialize() override {}
		bool Initialize(IWindow* window) override {}
		bool Shutdown() override {}

		bool InitializeImGui(void* ctx) override {}
		bool BeginImGuiFrame() override {}
		bool EndImGuiFrame() override {}
		bool ShutdownImGui() override {}
	};
} // namespace oe