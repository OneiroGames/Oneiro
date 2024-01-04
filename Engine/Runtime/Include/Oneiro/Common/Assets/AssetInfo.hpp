//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Common.hpp"

#include <any>
#include <tuple>

namespace oe
{
	class AssetInfo
	{
	public:
		AssetInfo() = delete;

		template <class... Args>
		explicit AssetInfo(size_t hash, Args... args) : m_Hash(hash), m_Data(std::make_any<std::tuple<Args...>>(std::make_tuple(args...)))
		{
		}

		template <class... Args>
		const std::tuple<Args...>* GetData() const
		{
			return std::any_cast<std::tuple<Args...>>(&m_Data);
		}

		[[nodiscard]] size_t GetHash() const noexcept;

		[[nodiscard]] bool operator==(size_t hash) const noexcept;

		[[nodiscard]] bool operator!=(size_t hash) const noexcept;

		[[nodiscard]] bool operator==(AssetInfo& other) const noexcept;

		[[nodiscard]] bool operator!=(AssetInfo& other) const noexcept;

	private:
		std::any m_Data{};
		size_t m_Hash{};
	};
} // namespace oe
