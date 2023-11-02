//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Layer.hpp"

namespace OEditor
{
	class ProfilerLayer final : public oe::Layer
	{
	public:
		using oe::Layer::Layer;

		void OnCreate() override;
		void OnDestroy() override;

		void OnBegin() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(const oe::Event::Base& baseEvent) override;
		void OnEnd() override;

	private:
		std::array<float, 90> mValues{};
		double mRefreshTime{1.0 / 60.0};
		int mValuesOffset{};
	};
} // namespace OEditor
