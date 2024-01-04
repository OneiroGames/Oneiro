//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "AssetsProvider.hpp"
#include "Oneiro/Common/World/World.hpp"

namespace oe
{
	class WorldAsset : public IAsset
	{
	public:
		using IAsset::IAsset;

		[[nodiscard]] bool IsLoaded() const noexcept override;
	};

	class WorldAssetsProvider : public IAssetsProvider
	{
	public:
		Ref<IAsset> CreateAsset(const Ref<AssetInfo>& assetInfo) override;

		void LoadAsset(const Ref<IAsset>& asset, bool async = false) override;

		void LoadAssetsAsync() override;

	private:
		std::vector<IAsset*> mAssets2Load{};
	};
} // namespace oe
