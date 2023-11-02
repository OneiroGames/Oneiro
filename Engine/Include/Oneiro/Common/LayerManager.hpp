//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Common.hpp"
#include "Oneiro/Common/Layer.hpp"

#include <algorithm>
#include <vector>

namespace oe
{
	class LayerManager
	{
	public:
		static LayerManager* Get() noexcept;

		template <class T>
		T* PushLayer(const std::string& name)
		{
			auto layer = mLayers.emplace_back(std::make_unique<T>(name)).get();
			layer->OnCreate();
			return dynamic_cast<T*>(layer);
		}

		template <class T>
		T* GetLayer(const std::string& name)
		{
			const auto& result = std::find_if(mLayers.begin(), mLayers.end(), [&](const auto& item) {
				return static_cast<bool>(item->GetName() == name);
			});
			if (result != mLayers.end())
				return dynamic_cast<T*>(result->get());
			OE_CORE_ERROR("Failed to find layer with '{}' name!", name);
			return {};
		}

		void PopLayer(Layer* layer);

		void PopLayer(const std::string& name);

		bool HasLayer(const std::string& name);

		std::vector<std::unique_ptr<Layer>>& GetLayers() noexcept;

	private:
		std::vector<std::unique_ptr<Layer>> mLayers{};
	};
} // namespace oe
