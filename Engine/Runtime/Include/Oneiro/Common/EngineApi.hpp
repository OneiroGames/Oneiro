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
		static bool Initialize(EngineApi* api);

		static bool Shutdown();

		static IApplication* GetApplication()
		{
			return m_Instance->application;
		}

		static IWindowManager* GetWindowManager()
		{
			return m_Instance->windowManager;
		}

		static RHI::IRHI* GetRHI()
		{
			return m_Instance->rhi;
		}

		static ModuleManager* GetModuleManager()
		{
			return m_Instance->moduleManager.get();
		}

		static CVars* GetCVars()
		{
			return m_Instance->cVars.get();
		}

		static WorldManager* GetWorldManager()
		{
			return m_Instance->worldManager.get();
		}

		static AssetsManager* GetAssetsManager()
		{
			return m_Instance->assetsManager.get();
		}

		static flecs::world* GetECS()
		{
			return m_Instance->ecs.get();
		}

		static EngineApi* GetInstance()
		{
			return m_Instance;
		}

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
		bool m_IsOwner{true};
	};
} // namespace oe
