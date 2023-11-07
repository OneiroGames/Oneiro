//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Renderer/Renderer.hpp"

namespace oe::Renderer
{
	class IRenderPass
	{
	public:
		virtual void Begin(const RenderPassBeginInfo& beginInfo, const std::function<void(Renderer::ICommandBuffer*)>& func) = 0;
		virtual void End() = 0;
		virtual void AddGraphicsPass(const std::function<void(ICommandBuffer*)>& func, const RenderPassInfo& info) = 0;
		virtual void AddComputePass(const std::function<void(ICommandBuffer*)>& func, const RenderPassInfo& info) = 0;
	};
} // namespace oe::Renderer