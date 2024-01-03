//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/EngineApi.hpp"

class ImGuiContext;

namespace oe
{
	class ImGuiManager
	{
	public:
		static bool Initialize(EngineApi* api);

		static bool BeginFrame();

		static bool EndFrame();

		static bool Shutdown();

	private:
		static ImGuiContext* m_Context;
	};
} // namespace oe