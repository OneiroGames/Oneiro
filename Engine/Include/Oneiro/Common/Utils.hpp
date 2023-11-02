//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <memory>

namespace oe
{
	template <class T>
	using Ref = std::shared_ptr<T>;

	template <class T, class... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
} // namespace oe
