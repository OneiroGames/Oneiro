//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Engine.hpp"

#include "Oneiro/Common/FileSystem/FileSystem.hpp"
#include "Oneiro/Common/IApplication.hpp"
#include "Oneiro/Common/JobSystem.hpp"
#include "Oneiro/Common/LayerManager.hpp"
#include "Oneiro/Common/ModuleManager.hpp"
#include "Oneiro/Common/Profiler.hpp"
#include "Oneiro/Renderer/ImGui/ImGui.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

void oe::Engine::Init(IApplication* application)
{
	m_EngineApi = CreateRef<EngineApi>();

	m_EngineApi->application = application;

	JobSystem::Init();

	FileSystem::Init();
	FileSystem::Mount(".", "/");

	m_EngineApi->cVars->Load("/Configs/Engine.ini");

	m_WMModule = m_EngineApi->moduleManager->LoadModule(m_EngineApi->cVars->GetString("WM"));

	const auto& props = application->GetProperties().windowProperties;
	const auto& window = m_EngineApi->windowManager->CreatePlatformWindow(props);
	window->Create();

	application->OnPreInit();
	application->OnInit();
	JobSystem::Wait();
}

void oe::Engine::Run()
{
	mIsRuntime = true;

	auto* window = m_EngineApi->windowManager->GetPlatformWindow(0);

	float lastFrame{};
	float currentFrame{};

	while (window->IsActive())
	{
		window->PollEvents();
		currentFrame = m_EngineApi->windowManager->GetTime();
		mDeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		m_EngineApi->application->OnUpdate(mDeltaTime);

		JobSystem::Wait();

		window->Update();
	}

	mIsRuntime = false;
}

void oe::Engine::Shutdown()
{
	JobSystem::Wait();

	m_EngineApi->application->OnShutdown();

	m_EngineApi->cVars->Save();

	m_EngineApi->windowManager->GetPlatformWindow(0)->Destroy();

	m_EngineApi->windowManager->Shutdown();
	m_EngineApi->moduleManager->UnLoadModule(m_WMModule);

	JobSystem::Shutdown();
	FileSystem::Shutdown();
}

float oe::Engine::GetDeltaTime() noexcept
{
	return mDeltaTime;
}

bool oe::Engine::IsRuntime() noexcept
{
	return mIsRuntime;
}
