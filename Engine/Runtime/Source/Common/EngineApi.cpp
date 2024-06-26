//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

#include "slang.h"
#include "slang-com-ptr.h"

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
		slangGlobalSession.~ComPtr();
		imguiManager.reset();
		rhi.reset();
		windowManager.reset();
		assetsManager.reset();
		worldManager.reset();
		moduleManager.reset();
		cVars.reset();
		logger.reset();
		ecs.reset();
	}

	bool EngineApi::Initialize(IApplication* application)
	{
		m_Instance = new EngineApi();
		m_Instance->application = application;
		m_Instance->ecs = CreateRef<flecs::world>();
		m_Instance->logger = CreateRef<Logger>();
		m_Instance->cVars = CreateRef<CVars>();
		m_Instance->moduleManager = CreateRef<ModuleManager>();
		m_Instance->worldManager = CreateRef<WorldManager>();
		m_Instance->assetsManager = CreateRef<AssetsManager>();
		m_Instance->imguiManager = CreateRef<ImGuiManager>();
		slang::createGlobalSession(m_Instance->slangGlobalSession.writeRef());
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