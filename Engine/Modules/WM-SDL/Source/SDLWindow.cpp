//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "SDLWindow.hpp"

namespace oe
{
	void SDLWindow::Create()
	{
		auto flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
		switch (EngineApi::GetRHI()->GetRHIType())
		{
			case RHI::ERHIType::GL:
				flags |= SDL_WINDOW_OPENGL;
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
				break;
		}
		m_Window = SDL_CreateWindow(m_Properties.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Properties.width,
									m_Properties.height, flags);
	}

	void SDLWindow::CreateContext()
	{
		switch (EngineApi::GetRHI()->GetRHIType())
		{
			case RHI::ERHIType::GL: {
				SDL_GL_CreateContext(m_Window);
				m_ProcAddress = SDL_GL_GetProcAddress;
				break;
			}
		}
	}

	void SDLWindow::Update()
	{
		switch (EngineApi::GetRHI()->GetRHIType())
		{
			case RHI::ERHIType::GL: SDL_GL_SwapWindow(m_Window); break;
		}
	}

	void SDLWindow::PollEvents()
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

	void SDLWindow::Close()
	{
		m_IsActive = false;
	}

	void SDLWindow::Destroy()
	{
		SDL_DestroyWindow(m_Window);
	}

	SWindowHandleData SDLWindow::GetWindowHandleData()
	{
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(m_Window, &wmInfo);
		return {.wnd = wmInfo.info.win.window, .inst = wmInfo.info.win.hinstance, .procAddress = m_ProcAddress};
	}

	glm::i32vec2 SDLWindow::GetSize()
	{
		int32_t width{};
		int32_t height{};
		SDL_GetWindowSize(m_Window, &width, &height);
		return {width, height};
	}

	SDL_Window* SDLWindow::GetSDLWindow()
	{
		return m_Window;
	}

	bool SDLWindowManager::Initialize()
	{
		if (SDL_Init(SDL_INIT_VIDEO))
		{
			OE_CORE_ERROR("Failed to initialize SDL2!");
			return false;
		}
		return true;
	}

	bool SDLWindowManager::Shutdown()
	{
		SDL_Quit();
		return true;
	}

	IWindow* SDLWindowManager::CreatePlatformWindow(const SWindowProperties& windowProperties)
	{
		return m_Windows.emplace_back(CreateRef<SDLWindow>(windowProperties)).get();
	}

	float SDLWindowManager::GetTime()
	{
		return static_cast<float>(SDL_GetTicks64()) / 1000.0f;
	}

	bool SDLWindowManager::InitializeImGui(void* ctx)
	{
		m_ImGuiContext = reinterpret_cast<ImGuiContext*>(ctx);
		ImGui::SetCurrentContext(m_ImGuiContext);
		const auto& window = dynamic_cast<SDLWindow*>(m_Windows[0].get());
		ImGui_ImplSDL2_InitForOther(window->GetSDLWindow());
		return true;
	}

	bool SDLWindowManager::BeginImGuiFrame()
	{
		ImGui_ImplSDL2_NewFrame();
		return true;
	}

	bool SDLWindowManager::EndImGuiFrame()
	{
		return true;
	}

	bool SDLWindowManager::ShutdownImGui()
	{
		ImGui_ImplSDL2_Shutdown();
		return true;
	}

	bool SDLWindowEngineModule::Initialize(EngineApi* api)
	{
		m_SDLWindowManager = CreateRef<SDLWindowManager>();
		EngineApi::GetInstance()->windowManager = m_SDLWindowManager.get();
		return true;
	}

	bool SDLWindowEngineModule::Shutdown()
	{
		EngineApi::GetInstance()->windowManager = nullptr;
		m_SDLWindowManager.reset();
		return true;
	}
} // namespace oe