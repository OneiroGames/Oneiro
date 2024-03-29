//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"

// clang-format off
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl2.h"
// clang-format on

#include "glm/glm.hpp"

module Oneiro.WM.SDL2;

import Oneiro.Common.EngineApi;
import Oneiro.Common.Logger;
import Oneiro.Common.WM.IWindowManager;

namespace oe
{
	void SDLWindow::Create()
	{
		auto flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
		switch (EngineApi::GetRHI()->GetRHIType())
		{
			case RHI::ERHIType::OPENGL460:
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
			case RHI::ERHIType::OPENGL460: {
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
			case RHI::ERHIType::OPENGL460: SDL_GL_SwapWindow(m_Window); break;
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

	bool SDL2WM::Initialize()
	{
		if (SDL_Init(SDL_INIT_VIDEO))
		{
			OE_CORE_ERROR("Failed to initialize SDL2!");
			return false;
		}
		return true;
	}

	bool SDL2WM::Shutdown()
	{
		SDL_Quit();
		return true;
	}

	IWindow* SDL2WM::CreatePlatformWindow(const SWindowProperties& windowProperties)
	{
		return m_Windows.emplace_back(CreateRef<SDLWindow>(windowProperties)).get();
	}

	float SDL2WM::GetTime()
	{
		return static_cast<float>(SDL_GetTicks64()) / 1000.0f;
	}

	bool SDL2WM::InitializeImGui()
	{
		ImGui::SetCurrentContext(EngineApi::GetImGuiManager()->GetContext());
		const auto& window = dynamic_cast<SDLWindow*>(m_Windows[0].get());
		ImGui_ImplSDL2_InitForOther(window->GetSDLWindow());
		return true;
	}

	bool SDL2WM::BeginImGuiFrame()
	{
		ImGui_ImplSDL2_NewFrame();
		return true;
	}

	bool SDL2WM::EndImGuiFrame()
	{
		return true;
	}

	bool SDL2WM::ShutdownImGui()
	{
		ImGui_ImplSDL2_Shutdown();
		return true;
	}
} // namespace oe