//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/IApplication.hpp"
#include "Oneiro/Common/IRenderer.hpp"
#include "Oneiro/Common/WM/IWindowManager.hpp"

#include "tracy/Tracy.hpp"

namespace oe
{
	class ModuleManager;

	class EngineApi
	{
	public:
		EngineApi();

		static IApplication* GetApplication()
		{
			return instance->application;
		}

		static IWindowManager* GetWindowManager()
		{
			return instance->windowManager;
		}

		static IRenderer* GetRendererBackend()
		{
			return instance->rendererBackend;
		}

		static tracy::Profiler* GetProfiler()
		{
			return instance->profiler;
		}

		static ModuleManager* GetModuleManager()
		{
			return instance->moduleManager.get();
		}

		IApplication* application{};
		IWindowManager* windowManager{};
		IRenderer* rendererBackend{};
		tracy::Profiler* profiler{};

		Ref<ModuleManager> moduleManager{};

		inline static EngineApi* instance{};
	};
} // namespace oe