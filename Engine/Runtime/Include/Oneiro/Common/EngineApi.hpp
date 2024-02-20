//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Assets/AssetsManager.hpp"
#include "Oneiro/Common/CVars.hpp"
#include "Oneiro/Common/IApplication.hpp"
#include "Oneiro/Common/JobManager.hpp"
#include "Oneiro/Common/RHI/IRHI.hpp"
#include "Oneiro/Common/WM/IWindowManager.hpp"

#include "flecs.h"
#include "flecs/addons/cpp/flecs.hpp"

namespace oe
{
	class ModuleManager;
	class WorldManager;

	class EngineApi
	{
	public:
		~EngineApi();

		static bool Initialize(IApplication* application);
		static bool Shutdown();

		static IApplication* GetApplication()
		{
			return GetInstance()->application;
		}

		static IWindowManager* GetWindowManager()
		{
			return GetInstance()->windowManager;
		}

		static RHI::IRHI* GetRHI()
		{
			return GetInstance()->rhi;
		}

		static ModuleManager* GetModuleManager()
		{
			return GetInstance()->moduleManager.get();
		}

		static CVars* GetCVars()
		{
			return GetInstance()->cVars.get();
		}

		static WorldManager* GetWorldManager()
		{
			return GetInstance()->worldManager.get();
		}

		static AssetsManager* GetAssetsManager()
		{
			return GetInstance()->assetsManager.get();
		}

		static flecs::world* GetECS()
		{
			return GetInstance()->ecs.get();
		}

		static EngineApi* GetInstance();

		IApplication* application{};
		IWindowManager* windowManager{};
		RHI::IRHI* rhi{};

		Ref<ModuleManager> moduleManager{};
		Ref<CVars> cVars{};
		Ref<WorldManager> worldManager{};
		Ref<AssetsManager> assetsManager{};
		Ref<flecs::world> ecs{};

	private:
		inline static EngineApi* m_Instance{};
	};
} // namespace oe

using CGetInstanceFunc = ::oe::EngineApi* (*)();
inline __declspec(dllexport) ::oe::EngineApi* CGetEngineApi()
{
	return ::oe::EngineApi::GetInstance();
}
