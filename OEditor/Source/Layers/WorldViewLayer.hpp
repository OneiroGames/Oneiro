//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Layer.hpp"
#include "Oneiro/World/Components.hpp"
#include "Oneiro/World/Entity.hpp"
#include "Oneiro/World/WorldManager.hpp"

namespace OEditor
{
	class WorldViewLayer final : public oe::Layer
	{
	public:
		using oe::Layer::Layer;

		void OnCreate() override;
		void OnDestroy() override;

		void OnBegin() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(const oe::Event::Base& baseEvent) override;
		void OnEnd() override;

		static oe::World::Entity GetSelectedEntity() noexcept;
		static void SetSelectedEntity(oe::World::Entity entity) noexcept;

	private:
		inline static oe::World::Entity mSelectedEntity{};
		bool mIsEntityNameBeingEdited{};
	};
} // namespace OEditor
