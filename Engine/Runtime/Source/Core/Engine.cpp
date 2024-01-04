//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Engine.hpp"

#include "Oneiro/Common/FileSystem/FileSystem.hpp"
#include "Oneiro/Common/IApplication.hpp"
#include "Oneiro/Common/JobManager.hpp"
#include "Oneiro/Common/ModuleManager.hpp"
#include "Oneiro/Common/World/World.hpp"
#include "Oneiro/Rendering/ImGui/ImGuiManager.hpp"
#include "Oneiro/Rendering/Renderer2D.hpp"

namespace oe
{
	void Engine::PreInit(IApplication* application)
	{
		JobManager::Initialize();

		EngineApi::Initialize(application);

		m_EngineApi = EngineApi::GetInstance();

		FileSystem::Init();
		FileSystem::Mount(".", "/");

		EngineApi::GetCVars()->Load("Engine", "/Configs/Engine.ini");
	}

	void Engine::Init()
	{
		EngineApi::GetModuleManager()->LoadModulesFromPath("Modules/");

		const auto& window = EngineApi::GetWindowManager()->CreatePlatformWindow(EngineApi::GetApplication()->GetProperties().windowProperties);

		EngineApi::GetRHI()->PreInitialize();

		window->Create();
		window->CreateContext();

		EngineApi::GetRHI()->Initialize(window);

		EngineApi::GetApplication()->OnPreInitialize();

		Renderer2D::Initialize();
		ImGuiManager::Initialize(EngineApi::GetInstance());

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

			EngineApi::GetWorldManager()->GetWorld()->UpdateRuntime(m_DeltaTime);

			Renderer2D::Draw();

			window->Update();
		}

		m_IsRuntime = false;
	}

	void Engine::Shutdown()
	{
		EngineApi::GetApplication()->OnShutdown();
		oe::EngineApi::GetAssetsManager()->CollectGarbage();
		Renderer2D::Shutdown();
		ImGuiManager::Shutdown();
		EngineApi::GetCVars()->Save();
		EngineApi::GetRHI()->Shutdown();
		EngineApi::GetWindowManager()->GetPlatformWindow(0)->Destroy();
		EngineApi::GetWindowManager()->Shutdown();
		EngineApi::Shutdown();
		JobManager::Shutdown();
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