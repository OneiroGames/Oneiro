//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/LayerManager.hpp"

oe::LayerManager* oe::LayerManager::Get() noexcept
{
	static LayerManager layerManager;
	return &layerManager;
}

void oe::LayerManager::PopLayer(oe::Layer* layer)
{
	const auto& res = std::find_if(mLayers.begin(), mLayers.end(), [&layer](const auto& item) {
		return item.get() == layer;
	});
	if (res != mLayers.end())
	{
		layer->OnDestroy();
		mLayers.erase(res);
	}
}

void oe::LayerManager::PopLayer(const std::string& name)
{
	const auto& res = std::find_if(mLayers.begin(), mLayers.end(), [&](const auto& item) {
		return static_cast<bool>(item->GetName() == name);
	});
	if (res != mLayers.end())
	{
		res->get()->OnDestroy();
		mLayers.erase(res);
	}
}

bool oe::LayerManager::HasLayer(const std::string& name)
{
	return std::find_if(mLayers.begin(), mLayers.end(), [&](const auto& item) {
			   return item->GetName() == name;
		   }) != mLayers.end();
}

std::vector<std::unique_ptr<oe::Layer>>& oe::LayerManager::GetLayers() noexcept
{
	return mLayers;
}
