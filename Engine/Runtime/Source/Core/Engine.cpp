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
import Oneiro.Common.RHI.ShaderCompiler;
import Oneiro.Common.CVars;
import Oneiro.RHI.OpenGL460;
import Oneiro.WM.SDL2;

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

		RHI::ShaderCompiler::Initialize();

		EngineApi::GetCVars()->Load("Engine", FileSystem::Path{"/Configs/Engine.json"});
		EngineApi::GetCVars()->Load("Game", FileSystem::Path{"/Configs/Game.json"});
	}

	void Engine::Init()
	{
		const auto& preferredWM = EngineApi::GetCVars()->GetString("Game", "WM_WIN64");
		const auto& preferredRHI = EngineApi::GetCVars()->GetString("Game", "RHI");

		if (preferredWM == "SDL2")
		{
			EngineApi::GetInstance()->windowManager = CreateRef<SDL2WM>();
		}

		if (preferredRHI == "OpenGL460")
		{
			EngineApi::GetInstance()->rhi = CreateRef<OpenGL460RHI>();
		}
		
		const auto& window = EngineApi::GetWindowManager()->CreatePlatformWindow(EngineApi::GetApplication()->GetProperties().windowProperties);

		EngineApi::GetRHI()->PreInitialize();

		window->Create();
		window->CreateContext();

		EngineApi::GetRHI()->Initialize(window);

		EngineApi::GetImGuiManager()->Initialize();

		EngineApi::GetModuleManager()->LoadModules();

		EngineApi::GetApplication()->OnPreInitialize();

		EngineApi::GetApplication()->OnInitialize();
	}

	void Engine::Run()
	{
		m_IsRuntime = true;

		const auto& window = EngineApi::GetWindowManager()->GetPlatformWindow(0);

		float lastFrame{};

		while (window->IsActive())
		{
			window->PollEvents();

			float currentFrame = EngineApi::GetWindowManager()->GetTime();
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
		EngineApi::GetModuleManager()->UnLoadModules();
		EngineApi::GetAssetsManager()->CollectGarbage();
		EngineApi::GetImGuiManager()->Shutdown();
		EngineApi::GetRHI()->Shutdown();
		EngineApi::GetWindowManager()->GetPlatformWindow(0)->Destroy();
		EngineApi::GetWindowManager()->Shutdown();
		RHI::ShaderCompiler::Shutdown();
		EngineApi::GetCVars()->Save();
		EngineApi::Shutdown();
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