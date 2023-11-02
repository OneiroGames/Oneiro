//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/WM/IWindow.hpp"

namespace oe
{
	class IWindowManager
	{
	public:
		virtual ~IWindowManager() = default;

		virtual bool Initialize() = 0;
		virtual bool Shutdown() = 0;

		virtual IWindow* CreatePlatformWindow(const SWindowProperties& windowProperties) = 0;

		virtual float GetTime() = 0;

		virtual bool InitializeImGui(void* ctx) = 0;
		virtual bool BeginImGuiFrame() = 0;
		virtual bool EndImGuiFrame() = 0;
		virtual bool ShutdownImGui() = 0;

		IWindow* GetPlatformWindow(size_t pos)
		{
			return m_Windows[pos].get();
		}

	protected:
		std::vector<Ref<IWindow>> m_Windows{};
	};
} // namespace oe