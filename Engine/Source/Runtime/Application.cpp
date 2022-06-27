//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Runtime/Application.hpp"

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"

namespace oe::Runtime
{
	Application::Application(const char* name, int width, int height)
    {
		mWindow = std::make_shared<Core::Window>(name, width, height);
	}

	Application::~Application() = default;

	bool Application::OnInit()
	{
		return true;
	}

	Core::Window* Application::GetWindow() const
    {
		return mWindow.get();
	}

	bool Application::OnUpdate(float deltaTime)
	{
		return true;
	}

	void Application::OnShutdown()
	{
	}

	void Application::OnEvent(const Core::Event::Base& e)
	{
	}

	bool Application::IsStopped() const
	{
		return mIsStopped;
	}

	void Application::Stop()
	{
		mIsStopped = true;
	}
}
