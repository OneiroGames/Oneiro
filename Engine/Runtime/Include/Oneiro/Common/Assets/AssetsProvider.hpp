//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Assets/Asset.hpp"
#include "Oneiro/Common/Assets/AssetInfo.hpp"

#include <execution>
#include <future>

namespace oe
{
	class IAssetsProvider
	{
	public:
		virtual Ref<IAsset> CreateAsset(const Ref<AssetInfo>& assetInfo) = 0;

		virtual void LoadAsset(const Ref<IAsset>& asset, bool async = false) = 0;

		virtual void LoadAssetsAsync() = 0;

		void CacheAsset(const Ref<IAsset>& asset) noexcept
		{
			if (!IsAssetCached(asset))
			{
				m_Cache.emplace_back(asset);
				return;
			}
			OE_CORE_WARN("Asset with '{}' hash already in cache!", asset->GetAssetInfo()->GetHash());
		}

		[[nodiscard]] bool IsAssetCached(const Ref<IAsset>& asset) const noexcept
		{
			const auto hash = asset->GetAssetInfo()->GetHash();
			const auto& found = std::find_if(m_Cache.begin(), m_Cache.end(), [&hash](const auto& item) {
				return *item == hash;
			});
			return (found != m_Cache.end() && found->get());
		}

		[[nodiscard]] bool IsAssetCached(size_t hash) const noexcept
		{
			const auto& found = std::find_if(m_Cache.begin(), m_Cache.end(), [&hash](const auto& item) {
				return item->GetAssetInfo()->GetHash() == hash;
			});
			return (found != m_Cache.end() && found->get());
		}

		Ref<IAsset> GetAsset(size_t hash) const noexcept
		{
			const auto& found = std::find_if(m_Cache.begin(), m_Cache.end(), [&hash](const auto& item) {
				return *item == hash;
			});
			if (found != m_Cache.end())
			{
				return found.operator*();
			}
			OE_CORE_ERROR("Failed to find asset with '{}' hash!", hash);
			return nullptr;
		}

		void CollectGarbage() noexcept
		{
			for (auto iter = m_Cache.begin(); iter != m_Cache.end();)
			{
				iter = iter->use_count() <= 1 || iter->get() == nullptr ? m_Cache.erase(iter) : ++iter;
			}
		}

		const std::vector<Ref<IAsset>>& GetCache() const noexcept
		{
			return m_Cache;
		}

	private:
		std::vector<Ref<IAsset>> m_Cache;
	};
} // namespace oe
