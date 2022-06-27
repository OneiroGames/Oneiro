//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Core/Input.hpp"
#include "Oneiro/Core/Event.hpp"
#include <memory>

namespace oe::Runtime
{
	class Application
	{
	public:
		Application(const char* name, int width, int height);
		virtual ~Application();

		virtual bool OnInit();
		virtual bool OnUpdate(float deltaTime);
		virtual void OnShutdown();
		virtual void OnEvent(const Core::Event::Base& e);

	    [[nodiscard]] Core::Window* GetWindow() const;
		[[nodiscard]] bool IsStopped() const;
	protected:
		void Stop();
	private:
		bool mIsStopped{};
		std::shared_ptr<Core::Window> mWindow;
	};

	std::shared_ptr<Application> CreateApplication(int argc, char* argv[]);
}
