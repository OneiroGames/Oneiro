//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/EngineApi.hpp"

#include "Oneiro/Common/ModuleManager.hpp"
#include "Oneiro/Common/World/World.hpp"

namespace oe
{
	EngineApi::~EngineApi()
	{
		moduleManager.reset();
		cVars.reset();
		ecs.reset();
		worldManager.reset();
		assetsManager.reset();
	}

	bool EngineApi::Initialize(IApplication* application)
	{
		m_Instance = new EngineApi();
		m_Instance->application = application;
		m_Instance->moduleManager = CreateRef<ModuleManager>();
		m_Instance->cVars = CreateRef<CVars>();
		m_Instance->ecs = CreateRef<flecs::world>();
		m_Instance->worldManager = CreateRef<WorldManager>();
		m_Instance->assetsManager = CreateRef<AssetsManager>();
		return true;
	}

	bool EngineApi::Shutdown()
	{
		delete m_Instance;
		return true;
	}

	EngineApi* EngineApi::GetInstance()
	{
		if (!m_Instance)
		{
			FileSystem::DynamicLibrary dynamicLibrary{};
			dynamicLibrary.Load("Oneiro-Common.dll");
			m_Instance = dynamicLibrary.GetFunction<CGetInstanceFunc>("CGetInstance")();
		}
		return m_Instance;
	}
} // namespace oe