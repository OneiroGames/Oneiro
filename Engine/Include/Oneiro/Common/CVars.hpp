//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Common.hpp"
#include "Oneiro/Common/FileSystem/ConfigFile.hpp"

namespace oe
{
	class CVars
	{
	public:
		void Load(const std::string& name, const FileSystem::Path& path)
		{
			m_Configs.emplace(name, CreateRef<FileSystem::ConfigFile>(path));
		}

		void Save()
		{
			for (const auto& item : m_Configs)
			{
				item.second->Save();
			}
		}

		std::string GetString(const std::string& name, const std::string& key, const std::string& defaultValue = {})
		{
			const auto& it = m_Configs.find(name);
			if (it != m_Configs.end())
			{
				if (it->second->Has(key))
					return it->second->operator[](key).GetString();
				else
					return defaultValue;
			}
			return defaultValue;
		}

	private:
		std::unordered_map<std::string, Ref<FileSystem::ConfigFile>> m_Configs{};
	};
} // namespace oe