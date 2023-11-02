//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/IApplication.hpp"

namespace SandBox
{
	class SandBoxApp final : public oe::IApplication
	{
	public:
		using oe::IApplication::IApplication;

		bool OnInit() override;
		bool OnUpdate(float deltaTime) override;
		void OnShutdown() override;
	};
} // namespace SandBox
