//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <utility>

#include "SimpleIni.h"

#include "Oneiro/Common/FileSystem/FileSystem.hpp"

namespace oe::FileSystem
{
	class ConfigFile
	{
	public:
		ConfigFile() = default;

		ConfigFile(Path path) : m_Path(std::move(path))
		{
			Open(m_Path);
		}

		~ConfigFile()
		{
			Save();
		}

		bool Open(const Path& path)
		{
			m_Path = path;
			m_File.SetUnicode();
			auto data = Read(m_Path);
			return m_File.LoadData(data.c_str(), data.size());
		}

		bool Save()
		{
			std::string buffer{};
			m_File.Save(buffer);
			if (!buffer.empty() && !m_Path.empty())
				Write(m_Path, reinterpret_cast<const uint8_t*>(buffer.c_str()), buffer.size());
			return true;
		}

		bool Save(const Path& path)
		{
			std::string buffer{};
			m_File.Save(buffer);
			if (!buffer.empty() && !path.empty())
				Write(path, reinterpret_cast<const uint8_t*>(buffer.c_str()), buffer.size());
			return true;
		}

		template <class T>
		T ReadValue(const std::string& section, const std::string& key, T defaultValue = {})
		{
			if constexpr (std::is_same_v<T, const char*>)
			{
				return m_File.GetValue(section.c_str(), key.c_str(), defaultValue);
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				return static_cast<T>(m_File.GetDoubleValue(section.c_str(), key.c_str(), static_cast<double>(defaultValue)));
			}
			else if (std::is_same_v<T, bool>)
			{
				return m_File.GetBoolValue(section.c_str(), key.c_str(), defaultValue);
			}
		}

		std::vector<std::string> GetValues(const std::string& section, const std::string& key)
		{
			CSimpleIniA::TNamesDepend names{};
			m_File.GetAllValues(section.c_str(), key.c_str(), names);
			std::vector<std::string> result{};
			result.resize(names.size());
			for (const auto& name : names)
				result.emplace_back(name.pItem);
			return result;
		}

		template <class T>
		void SetValue(const std::string& section, const std::string& key, const T& value)
		{
			if constexpr (std::is_same_v<T, const char*>)
			{
				m_File.SetValue(section.c_str(), key.c_str(), value);
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				m_File.SetDoubleValue(section.c_str(), key.c_str(), static_cast<double>(value));
			}
			else if (std::is_same_v<T, bool>)
			{
				m_File.SetBoolValue(section.c_str(), key.c_str(), value);
			}
		}

	private:
		CSimpleIniA m_File{};
		Path m_Path{};
	};
} // namespace oe::FileSystem