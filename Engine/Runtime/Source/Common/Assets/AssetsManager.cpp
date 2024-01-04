//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/Assets/AssetsManager.hpp"

void oe::AssetsManager::LoadAssetsAsync()
{
	for (const auto& assetsProvider : m_AssetsProviders)
	{
		assetsProvider.second->LoadAssetsAsync();
	}
}

void oe::AssetsManager::CollectGarbage()
{
	for (const auto& assetsProvider : m_AssetsProviders)
	{
		assetsProvider.second->CollectGarbage();
	}
}