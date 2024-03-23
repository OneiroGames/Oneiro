//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Common.AssetsManager;

import Oneiro.Common.AssetsProvider;

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