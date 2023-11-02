//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Layer.hpp"

namespace OEditor
{
	class ProjectSettingsLayer final : public oe::Layer
	{
	public:
		using oe::Layer::Layer;

		void OnCreate() override;
		void OnDestroy() override;

		void OnBegin() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(const oe::Event::Base& baseEvent) override;
		void OnEnd() override;
	};
} // namespace OEditor
