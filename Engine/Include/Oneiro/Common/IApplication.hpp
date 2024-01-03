//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/FileSystem/Path.hpp"
#include "Oneiro/Common/WM/IWindow.hpp"

namespace oe
{
	struct ApplicationProperties
	{
		SWindowProperties windowProperties{};
		struct Engine
		{
			bool controlWorldState = true;
		} engine;
		FileSystem::Path projectFilePath{};
	};

	class IApplication
	{
	public:
		IApplication() = delete;

		IApplication(ApplicationProperties applicationProperties);

		virtual ~IApplication();

		virtual bool OnPreInitialize();
		virtual bool OnInitialize();
		virtual bool OnLogicUpdate(float deltaTime);
		virtual bool OnRender(float deltaTime);
		virtual void OnShutdown();

		[[nodiscard]] const ApplicationProperties& GetProperties() const noexcept;

	private:
		ApplicationProperties mProps{};
	};
} // namespace oe
