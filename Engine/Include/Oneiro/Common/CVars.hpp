//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Common.hpp"

#include "Oneiro/Common/FileSystem/ConfigFile.hpp"

namespace oe
{
	struct SVars
	{
		std::unordered_map<std::string, bool> boolVars{};
		std::unordered_map<std::string, std::string> stringVars{};
		std::unordered_map<std::string, double> numVars{};
	};

	class CVars
	{
	public:
		CVars()
		{
			m_Variables.push_back({"Temp", {}});
		}

		void Load(const FileSystem::Path& path)
		{
			FileSystem::ConfigFile configFile{};
			configFile.Open(path);
			SVars vars{};
			auto bools = configFile.GetAllValues<bool>();
			for (const auto& item : bools)
			{
				vars.boolVars[item.first] = item.second;
			}
			auto strs = configFile.GetAllValues<const char*>();
			for (const auto& item : strs)
			{
				vars.stringVars[item.first] = item.second;
			}
			auto nums = configFile.GetAllValues<double>();
			for (const auto& item : nums)
			{
				vars.numVars[item.first] = item.second;
			}
			m_Variables.emplace_back(path, vars);
		}

		void Save()
		{
			for (const auto& item : m_Variables)
			{
				const auto& vars = item.second;
				FileSystem::ConfigFile configFile{};
				configFile.Open(item.first);

				for (const auto& [key, val] : vars.boolVars)
				{
					configFile.SetValue<bool>(configFile.FindSectionByKey(key), key, val);
				}

				for (const auto& [key, val] : vars.stringVars)
				{
					configFile.SetValue<const char*>(configFile.FindSectionByKey(key), key, val.c_str());
				}

				for (const auto& [key, val] : vars.numVars)
				{
					configFile.SetValue<double>(configFile.FindSectionByKey(key), key, val);
				}
				configFile.Save();
			}
		}

		bool GetBool(const std::string& key, bool defaultValue = {})
		{
			for (const auto& item : m_Variables)
			{
				const auto& iter = item.second.boolVars.find(key);
				if (iter != item.second.boolVars.end())
				{
					return iter->second;
				}
			}
			return defaultValue;
		}

		std::string GetString(const std::string& key, const std::string& defaultValue = {})
		{
			for (const auto& item : m_Variables)
			{
				const auto& iter = item.second.stringVars.find(key);
				if (iter != item.second.stringVars.end())
				{
					return iter->second;
				}
			}
			return defaultValue;
		}

		double GetNumber(const std::string& key, double defaultValue = {})
		{
			for (const auto& item : m_Variables)
			{
				const auto& iter = item.second.numVars.find(key);
				if (iter != item.second.numVars.end())
				{
					return iter->second;
				}
			}
			return defaultValue;
		}

		void SetBool(const std::string& key, bool val)
		{
			for (auto& item : m_Variables)
			{
				const auto& iter = item.second.boolVars.find(key);
				if (iter != item.second.boolVars.end())
				{
					iter->second = val;
				}
			}
		}

		void SetString(const std::string& key, const std::string& val)
		{
			for (auto& item : m_Variables)
			{
				const auto& iter = item.second.stringVars.find(key);
				if (iter != item.second.stringVars.end())
				{
					iter->second = val;
				}
			}
		}

		void SetNumber(const std::string& key, double val)
		{
			for (auto& item : m_Variables)
			{
				const auto& iter = item.second.numVars.find(key);
				if (iter != item.second.numVars.end())
				{
					iter->second = val;
				}
			}
		}

	private:
		std::vector<std::pair<FileSystem::Path, SVars>> m_Variables{};
	};
} // namespace oe