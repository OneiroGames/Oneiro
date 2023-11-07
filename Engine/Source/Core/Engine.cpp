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
#include "Oneiro/Common/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/ImGui/ImGui.hpp"
#include "Oneiro/Renderer/Renderer2D.hpp"

void oe::Engine::Init(IApplication* application)
{
	m_EngineApi = CreateRef<EngineApi>();
	m_EngineApi->Initialize(application);

	JobSystem::Init();

	FileSystem::Init();
	FileSystem::Mount(".", "/");

	m_EngineApi->cVars->Load("/Configs/Engine.ini");

	m_WMModule = m_EngineApi->moduleManager->LoadModule(m_EngineApi->cVars->GetString("WM"));
	m_RendererModule = m_EngineApi->moduleManager->LoadModule(m_EngineApi->cVars->GetString("backend", "Oneiro-Module-Renderer-GL.dll"));

	const auto& props = application->GetProperties().windowProperties;
	const auto& window = m_EngineApi->windowManager->CreatePlatformWindow(props);
	m_EngineApi->rendererBackend->PreInitialize();
	window->Create();
	m_EngineApi->rendererBackend->Initialize(window);

	application->OnPreInit();

	InitializeImGui();
	Renderer2D::Initialize();

	application->OnInit();
	JobSystem::Wait();
}

void oe::Engine::Run()
{
	mIsRuntime = true;

	const auto& window = m_EngineApi->windowManager->GetPlatformWindow(0);

	float lastFrame{};
	float currentFrame{};

	while (window->IsActive())
	{
		window->PollEvents();

		currentFrame = m_EngineApi->windowManager->GetTime();
		mDeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		m_EngineApi->application->OnUpdate(mDeltaTime);

		Renderer2D::Draw();

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

	ShutdownImGui();

	m_EngineApi->windowManager->GetPlatformWindow(0)->Destroy();

	Renderer2D::Shutdown();

	m_EngineApi->rendererBackend->Shutdown();
	m_EngineApi->moduleManager->UnLoadModule(m_RendererModule);

	m_EngineApi->windowManager->Shutdown();
	m_EngineApi->moduleManager->UnLoadModule(m_WMModule);

	m_EngineApi->Shutdown();

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
