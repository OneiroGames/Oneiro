//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Common.AssetsProviders.WorldAsset;

import Oneiro.Common.AssetsProvider;
import Oneiro.Common.ECS.World;
import Oneiro.Common.EngineApi;
// import Oneiro.Common.Logger;

import <execution>;

bool oe::WorldAsset::IsLoaded() const noexcept
{
	return nativePtr != nullptr;
}

oe::Ref<oe::IAsset> oe::WorldAssetsProvider::CreateAsset(const Ref<AssetInfo>& assetInfo)
{
	return CreateRef<WorldAsset>(assetInfo, nullptr, typeid(World).hash_code());
}

void oe::WorldAssetsProvider::LoadAsset(const Ref<IAsset>& asset, bool async)
{
	if (async)
	{
		mAssets2Load.emplace_back(asset.get());
		return;
	}
	const auto& assetInfo = asset->GetAssetInfo();
	const auto& assetData = assetInfo->template GetData<FileSystem::Path>();
	const auto& path = get<0>(*assetData);
	asset->nativePtr = EngineApi::GetWorldManager()->LoadWorld(path);
	// if (!asset->nativePtr)
		// OE_CORE_WARN("Failed to load world from '{}' asset hash!", assetInfo->GetHash());
}

void oe::WorldAssetsProvider::LoadAssetsAsync()
{
	if (mAssets2Load.empty())
		return;

	std::vector<std::future<void>> futures{};

	std::for_each(std::execution::par, mAssets2Load.begin(), mAssets2Load.end(), [&](const auto& item) {
		futures.emplace_back(std::async(std::launch::async, [&]() {
			const auto& assetInfo = item->GetAssetInfo();
			const auto& assetData = assetInfo->template GetData<FileSystem::Path>();
			const auto& path = get<0>(*assetData);
			item->nativePtr = EngineApi::GetWorldManager()->LoadWorld(path);
			// if (!item->nativePtr)
				// OE_CORE_WARN("Failed to load world from '{}' asset hash!", assetInfo->GetHash());
		}));
	});

	for (size_t i{}; i < futures.size(); i++)
	{
		futures[i].wait();
	}

	mAssets2Load.clear();
}
