//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Common.EngineApi;

import Oneiro.Common.ModuleManager;
import Oneiro.Common.ECS.World;
import Oneiro.Common.AssetsManager;
import Oneiro.Common.CVars;
import Oneiro.Common.IApplication;
import Oneiro.Common.ImGuiManager;
import Oneiro.Common.JobManager;
import Oneiro.Common.RHI.Base;
import Oneiro.Common.WM.IWindowManager;
import Oneiro.Common.Logger;
import Oneiro.Common.FileSystem.DynamicLibrary;

namespace oe
{
	EngineApi* EngineApi::m_Instance = nullptr;

	EngineApi::~EngineApi()
	{
		moduleManager.reset();
		cVars.reset();
		ecs.reset();
		worldManager.reset();
		assetsManager.reset();
		imguiManager.reset();
        logger.reset();
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
		m_Instance->imguiManager = CreateRef<ImGuiManager>();
        m_Instance->logger = CreateRef<Logger>();
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
			dynamicLibrary.Load("Oneiro-Common");
			m_Instance = dynamicLibrary.GetFunction<CGetEngineApiInstanceFunc>("CGetEngineApiInstance")();
			dynamicLibrary.Close();
		}
		return m_Instance;
	}
} // namespace oe

// only export, not import
extern "C" __declspec(dllexport) oe::EngineApi* CGetEngineApiInstance()
{
	return oe::EngineApi::GetInstance();
}