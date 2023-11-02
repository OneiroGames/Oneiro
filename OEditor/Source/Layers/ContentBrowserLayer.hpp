//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Layer.hpp"
#include "Oneiro/Core/Assets/IAsset.hpp"
#include "Oneiro/Renderer/Renderer2D.hpp"

namespace OEditor
{
	class ContentBrowserLayer final : public oe::Layer
	{
	public:
		using oe::Layer::Layer;

		void OnCreate() override;
		void OnDestroy() override;

		void OnBegin() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(const oe::Event::Base& baseEvent) override;
		void OnEnd() override;

		void LoadAssets() noexcept;
		void UnLoadAssets() noexcept;

	private:
		oe::Ref<oe::IAsset<oe::Renderer2D::Texture>> mPathTexture{};
		oe::Ref<oe::IAsset<oe::Renderer2D::Texture>> mFileTexture{};
		oe::FileSystem::Path mBasePath{};
		oe::FileSystem::Path mCurrentPath{};
	};
} // namespace OEditor
