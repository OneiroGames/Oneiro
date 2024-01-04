//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/RHI/RHI_Defines.hpp"

namespace oe::RHI
{
	class IRenderGraph
	{
	public:
		virtual ~IRenderGraph() = default;

		virtual void Begin(const RenderGraphBeginInfo& beginInfo, const std::function<void(ICommandBuffer*)>& func) = 0;
		virtual void End() = 0;
		virtual void AddGraphicsPass(const std::function<void(ICommandBuffer*)>& func, const PassInfo& info) = 0;
		virtual void AddComputePass(const std::function<void(ICommandBuffer*)>& func, const PassInfo& info) = 0;
	};
} // namespace oe::RHI