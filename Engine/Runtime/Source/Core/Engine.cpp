//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Core.Engine;

import Oneiro.Common.FileSystem.Base;
import Oneiro.Common.IApplication;
import Oneiro.Common.JobManager;
import Oneiro.Common.ModuleManager;
import Oneiro.Common.ECS.World;
import Oneiro.Common.EngineApi;
import Oneiro.Common.IModule;
import Oneiro.Common.WM.IWindow;
import Oneiro.Common.IApplication;

namespace oe
{
	float Engine::m_DeltaTime = {};
	bool Engine::m_IsRuntime = {};

	void Engine::PreInit(IApplication* application)
	{
		// JobManager::Initialize();

		EngineApi::Initialize(application);

		m_EngineApi = EngineApi::GetInstance();

		FileSystem::Init();
		FileSystem::Mount(FileSystem::Path{"."}, "/");

		EngineApi::GetCVars()->Load("Engine", FileSystem::Path{"/Configs/Engine.ini"});
	}

	void Engine::Init()
	{
		EngineApi::GetModuleManager()->LoadModulesFromPath(FileSystem::Path{"Modules/"});

		const auto& window = EngineApi::GetWindowManager()->CreatePlatformWindow(EngineApi::GetApplication()->GetProperties().windowProperties);

		EngineApi::GetRHI()->PreInitialize();

		window->Create();
		window->CreateContext();

		EngineApi::GetRHI()->Initialize(window);

		EngineApi::GetImGuiManager()->Initialize();

		EngineApi::GetApplication()->OnPreInitialize();

		EngineApi::GetApplication()->OnInitialize();
	}

	void Engine::Run()
	{
		m_IsRuntime = true;

		const auto& window = EngineApi::GetWindowManager()->GetPlatformWindow(0);

		float lastFrame{};
		float currentFrame{};

		while (window->IsActive())
		{
			window->PollEvents();

			currentFrame = EngineApi::GetWindowManager()->GetTime();
			m_DeltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			EngineApi::GetApplication()->OnLogicUpdate(m_DeltaTime);

			EngineApi::GetImGuiManager()->BeginFrame();
			EngineApi::GetWorldManager()->GetWorld()->UpdateRuntime(m_DeltaTime);
			EngineApi::GetImGuiManager()->EndFrame();

			window->Update();
		}

		m_IsRuntime = false;
	}

	void Engine::Shutdown()
	{
		EngineApi::GetApplication()->OnShutdown();
		EngineApi::GetAssetsManager()->CollectGarbage();
		EngineApi::GetCVars()->Save();
		EngineApi::GetImGuiManager()->Shutdown();
		EngineApi::GetRHI()->Shutdown();
		EngineApi::GetWindowManager()->GetPlatformWindow(0)->Destroy();
		EngineApi::GetWindowManager()->Shutdown();
		EngineApi::Shutdown();
		// JobManager::Shutdown();
	}

	float Engine::GetDeltaTime() noexcept
	{
		return m_DeltaTime;
	}

	bool Engine::IsRuntime() noexcept
	{
		return m_IsRuntime;
	}
} // namespace oe