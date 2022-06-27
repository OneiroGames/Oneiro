//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Engine.hpp"

#include <memory>
#include <stdexcept>
#include <string>

#include "Oneiro/Core/Core.hpp"
#include "Oneiro/Core/Event.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Renderer/Gui/GuiLayer.hpp"
#include "HazelAudio/HazelAudio.h"
#include "Oneiro/Core/ResourceManager.hpp"

namespace oe::Runtime
{
	void Engine::Init()
	{
		Core::Init();
		Renderer::PreInit();
		Renderer::GuiLayer::PreInit();
		Hazel::Audio::Init();
	}

	void Engine::Run(const std::shared_ptr<Application>& app)
	{
		using namespace Core;

		const auto window = app->GetWindow();
		Root::mApplicationInstance = app.get();
		Root::mWindowInstance = window;

		Event::Dispatcher::Subscribe<Event::ErrorEvent>([](const Event::Base& e)
		{
			const auto& errorEvent =
				dynamic_cast<const Event::ErrorEvent&>(e);
			log::get("log")->error("GLFW ERROR[" +
				std::to_string(
					errorEvent
					.Error) +
				"]: " +
				errorEvent
				.Description);
		});

		if (!window->Create())
			throw std::runtime_error("Failed to create window!");

		SetupEvents();

		glfwSwapInterval(1);

		if (!app->OnInit())
			throw std::runtime_error("Failed to initialize application!");

		Renderer::Init();
		Renderer::GuiLayer::Init();

		LoadResources();

		auto lastFrame = static_cast<float>(glfwGetTime());
		while (!window->IsClosed())
		{
			const auto currentFrame = static_cast<float>(glfwGetTime());
			mDeltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (!UpdateGame(app, mDeltaTime, window)) break;
		}

		app->OnShutdown();
	}

	bool Engine::UpdateGame(const std::shared_ptr<Application>& app, float deltaTime, const Core::Window* window)
	{
		if (app->IsStopped())
			return false;

		Core::Window::PollEvents();

#ifdef OE_RENDERER_VULKAN
        Renderer::Vulkan::BeginScene();
#else
		Renderer::GL::ClearColor(0.1f, 0.1f, 0.1f);
		Renderer::GL::Clear(Renderer::GL::ClearType::COLOR_BUFFER | Renderer::GL::ClearType::DEPTH_BUFFER);
#endif
		Renderer::GuiLayer::NewFrame();
		if (!app->OnUpdate(deltaTime))
			return false;
		Renderer::GuiLayer::Draw();
#ifdef OE_RENDERER_VULKAN
        Renderer::Vulkan::EndScene();
#else
		window->SwapBuffers();
#endif
		return true;
	}

	void Engine::Shutdown()
	{
		Hazel::Audio::Shutdown();
		Renderer::GuiLayer::Shutdown();
		Renderer::Shutdown();
		Core::Shutdown();
	}

    float Engine::GetDeltaTime()
	{
	    return mDeltaTime;
	}

    void Engine::SetupEvents()
	{
		using namespace Core;
		Event::Dispatcher::Subscribe<Event::FrameBufferSizeEvent>([](const Event::Base& e)
		{
			const auto& resizeEvent =
				dynamic_cast<const Event::FrameBufferSizeEvent&>(e);
			if (resizeEvent.Width > 0 ||
				resizeEvent.Height > 0)
			{
				Root::GetWindow()->UpdateSize(
					resizeEvent.Width,
					resizeEvent.Height);
				Renderer::GL::Viewport(
					resizeEvent.Width,
					resizeEvent.Height);
			}

			Root::GetApplication()->OnEvent(e);
		});

		Event::Dispatcher::Subscribe<Event::KeyInputEvent>([](const Event::Base& e)
		{
			Root::GetApplication()->OnEvent(e);
		});

		Event::Dispatcher::Subscribe<Event::MouseButtonEvent>([](const Event::Base& e)
		{
			Root::GetApplication()->OnEvent(e);
		});

		Event::Dispatcher::Subscribe<Event::CursorPosEvent>([](const Event::Base& e)
		{
			Root::GetApplication()->OnEvent(e);
		});
	}
}
