//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/EngineApi.hpp"
#include "Oneiro/Common/IModule.hpp"
#include "Oneiro/Common/WM/IWindowManager.hpp"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

// clang-format off
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl2.h"
// clang-format on

namespace oe
{
	class SDLWindow : public IWindow
	{
	public:
		using IWindow::IWindow;

		void Create() override;

		void CreateContext() override;

		void Update() override;

		void PollEvents() override;

		void Close() override;

		void Destroy() override;

		SWindowHandleData GetWindowHandleData() override;

		glm::i32vec2 GetSize() override;

		SDL_Window* GetSDLWindow();

	private:
		SDL_Window* m_Window{};
		SDL_Event m_Event{};
		void* m_ProcAddress{};
	};

	class SDLWindowManager : public IWindowManager
	{
	public:
		using IWindowManager::IWindowManager;

		bool Initialize() override;

		bool Shutdown() override;

		IWindow* CreatePlatformWindow(const SWindowProperties& windowProperties) override;

		float GetTime() override;

		bool InitializeImGui(void* ctx) override;

		bool BeginImGuiFrame() override;

		bool EndImGuiFrame() override;

		bool ShutdownImGui() override;

	private:
		ImGuiContext* m_ImGuiContext{};
	};

	class SDLWindowEngineModule : public IModule
	{
	public:
		using IModule::IModule;

		bool Initialize(EngineApi* api) override;

		bool Shutdown() override;

	private:
		Ref<SDLWindowManager> m_SDLWindowManager{};
	};

	extern "C" OE_MODULE_API IModule* CreateModule()
	{
		return new SDLWindowEngineModule();
	}

	extern "C" OE_MODULE_API void DestroyModule(IModule* engineModule)
	{
		delete engineModule;
	}
} // namespace oe