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

		void Create() override
		{
			auto flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
			switch (EngineApi::GetRendererBackend()->GetBackendType())
			{
				case Renderer::ERendererBackendType::GL:
					flags |= SDL_WINDOW_OPENGL;
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
					break;
			}
			m_Window = SDL_CreateWindow(m_Properties.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Properties.width,
										m_Properties.height, flags);
			switch (EngineApi::GetRendererBackend()->GetBackendType())
			{
				case Renderer::ERendererBackendType::GL: SDL_GL_CreateContext(m_Window); break;
			}
		}

		void Update() override
		{
			switch (EngineApi::GetRendererBackend()->GetBackendType())
			{
				case Renderer::ERendererBackendType::GL: SDL_GL_SwapWindow(m_Window); break;
			}
		}

		void PollEvents() override
		{
			while (SDL_PollEvent(&m_Event))
			{
				ImGui_ImplSDL2_ProcessEvent(&m_Event);
				switch (m_Event.type)
				{
					case SDL_EventType::SDL_QUIT: {
						Close();
						break;
					}
					case SDL_EventType::SDL_WINDOWEVENT: {
						switch (m_Event.window.event)
						{
							case SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE: {
								Close();
								break;
							}
							case SDL_WindowEventID::SDL_WINDOWEVENT_SIZE_CHANGED: {
								break;
							}
						}
					}
				}
			}
		}

		void Close() override
		{
			m_IsActive = false;
		}

		void Destroy() override
		{
			SDL_DestroyWindow(m_Window);
		}

		SWindowHandleData GetWindowHandleData() override
		{
			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo(m_Window, &wmInfo);
			void* procAddress{};
			switch (EngineApi::GetRendererBackend()->GetBackendType())
			{
				case Renderer::ERendererBackendType::GL: procAddress = SDL_GL_GetProcAddress; break;
			}
			return {.wnd = wmInfo.info.win.window, .inst = wmInfo.info.win.hinstance, .procAddress = procAddress};
		}

		glm::i32vec2 GetSize() override
		{
			int32_t width{};
			int32_t height{};
			SDL_GetWindowSize(m_Window, &width, &height);
			return {width, height};
		}

		SDL_Window* GetSDLWindow()
		{
			return m_Window;
		}

	private:
		SDL_Window* m_Window{};
		SDL_Event m_Event{};
	};

	class SDLWindowManager : public IWindowManager
	{
	public:
		using IWindowManager::IWindowManager;

		bool Initialize() override
		{

			if (SDL_Init(SDL_INIT_VIDEO))
			{
				OE_CORE_ERROR("Failed to initialize SDL2!");
				return false;
			}

			return true;
		}

		bool Shutdown() override
		{
			SDL_Quit();
			return true;
		}

		IWindow* CreatePlatformWindow(const SWindowProperties& windowProperties) override
		{
			return m_Windows.emplace_back(CreateRef<SDLWindow>(windowProperties)).get();
		}

		float GetTime() override
		{
			return static_cast<float>(SDL_GetTicks64()) / 1000.0f;
		}

		bool InitializeImGui(void* ctx) override
		{
			m_ImGuiContext = reinterpret_cast<ImGuiContext*>(ctx);

			ImGui::SetCurrentContext(m_ImGuiContext);
			const auto& window = dynamic_cast<SDLWindow*>(m_Windows[0].get());
			ImGui_ImplSDL2_InitForOther(window->GetSDLWindow());
			return true;
		}

		bool BeginImGuiFrame() override
		{
			ImGui_ImplSDL2_NewFrame();
			return true;
		}

		bool EndImGuiFrame() override
		{
			return true;
		}

		bool ShutdownImGui() override
		{
			ImGui_ImplSDL2_Shutdown();
			return true;
		}

	private:
		ImGuiContext* m_ImGuiContext{};
	};

	class SDLWindowEngineModule : public IModule
	{
	public:
		using IModule::IModule;

		bool Initialize() override
		{
			m_SDLWindowManager = CreateRef<SDLWindowManager>();
			GetEngineApi()->windowManager = m_SDLWindowManager.get();
			return true;
		}

		bool Shutdown() override
		{
			GetEngineApi()->windowManager = nullptr;
			m_SDLWindowManager.reset();
			return true;
		}

	private:
		Ref<SDLWindowManager> m_SDLWindowManager{};
	};

	extern "C" OE_MODULE_API IModule* CreateModule(EngineApi* engineApi)
	{
		return new SDLWindowEngineModule(engineApi);
	}

	extern "C" OE_MODULE_API void DestroyModule(IModule* engineModule)
	{
		delete engineModule;
	}
} // namespace oe