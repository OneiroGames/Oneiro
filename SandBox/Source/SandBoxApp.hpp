//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/IApplication.hpp"

#include "Oneiro/Common/World/World.hpp"

namespace SandBox
{
	class SandBoxApp final : public oe::IApplication
	{
	public:
		using oe::IApplication::IApplication;

		bool OnInitialize() override;
		bool OnLogicUpdate(float deltaTime) override;
		void OnShutdown() override;

		oe::World* world{};
	};
} // namespace SandBox
