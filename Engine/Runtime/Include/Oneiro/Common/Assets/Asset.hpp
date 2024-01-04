//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Assets/AssetInfo.hpp"

#define OE_MAKE_ASSET_HASH(x) std::hash<std::string>{}(x)

namespace oe
{
	class IAsset
	{
	public:
		IAsset() = delete;
		virtual ~IAsset() = default;

		explicit IAsset(const Ref<AssetInfo>& assetInfo, void* ptr, size_t typeHash) : m_Info(assetInfo), nativePtr(ptr), m_TypeHash(typeHash) {}

		[[nodiscard]] virtual bool IsLoaded() const noexcept = 0;

		template <class T>
		[[nodiscard]] T* Get() const noexcept
		{
			if (typeid(T).hash_code() == m_TypeHash)
			{
				return (T*)nativePtr;
			}
			return nullptr;
		}

		[[nodiscard]] const AssetInfo* GetAssetInfo() const noexcept
		{
			return m_Info.get();
		}

		bool operator==(size_t hash) const noexcept
		{
			return *m_Info == hash;
		}

		bool operator!=(size_t hash) const noexcept
		{
			return !operator==(hash);
		}

		bool operator==(IAsset& other) const noexcept
		{
			return m_Info == other.m_Info;
		}

		bool operator!=(IAsset& other) const noexcept
		{
			return !operator==(other);
		}

		void* nativePtr{};

	protected:
		Ref<AssetInfo> m_Info{};
		size_t m_TypeHash{};
	};
} // namespace oe
