//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <string>
#include <utility>

namespace oe
{
	class Layer
	{
	public:
		Layer() = delete;
		explicit Layer(std::string name);
		virtual ~Layer() = default;

		virtual void OnCreate();
		virtual void OnDestroy();

		virtual void OnBegin();
		virtual void OnUpdate(float deltaTime);
		virtual void OnEnd();

		[[nodiscard]] std::string GetName() const noexcept
		{
			return mName;
		}

	private:
		std::string mName{};
	};
} // namespace oe
